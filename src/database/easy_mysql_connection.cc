#include "easy_mysql_connection.h"
#include <assert.h>
namespace easy {


EasyMySQLConnection::EasyMySQLConnection( EasyMySQLConnectionInfo& conn_info ):
    connection_info_(conn_info),
    connection_flags(CONNECTION_SYNCH),
    mysql_(NULL),
    reconnecting_(false),
    prepare_error_(false) {

}

EasyMySQLConnection::~EasyMySQLConnection() {

}

easy_bool EasyMySQLConnection::Open() {
    MYSQL* mysql;
    mysql = mysql_init(NULL);
    if (!mysql) {
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"Could not initialize Mysql connection to database `%s`", connection_info_.database_.c_str());
        return false;
    }
    int port;
    char const* unix_socket;

    mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
#ifdef _WIN32
    if (connection_info_.host_ == ".") {                                         // named pipe use option (Windows)
        unsigned int opt = MYSQL_PROTOCOL_PIPE;
        mysql_options(mysql, MYSQL_OPT_PROTOCOL, (char const*)&opt);
        port = 0;
        unix_socket = 0;
    } else {																	// generic case
        port = atoi(connection_info_.port_or_socket_.c_str());
        unix_socket = 0;
    }
#else
    if (connection_info_.host_ == ".") {                                         // socket use option (Unix/Linux)
        unsigned int opt = MYSQL_PROTOCOL_SOCKET;
        mysql_options(mysql, MYSQL_OPT_PROTOCOL, (char const*)&opt);
        connection_info_.host = "localhost";
        port = 0;
        unix_socket = connection_info_.port_or_socket_.c_str();
    } else {																	// generic case
        port = atoi(connection_info_.port_or_socket_.c_str());
        unix_socket = 0;
    }
#endif
    mysql_ = mysql_real_connect(mysql, connection_info_.host_.c_str(), connection_info_.user_.c_str(),\
                                connection_info_.password_.c_str(), connection_info_.database_.c_str(), port, unix_socket, 0);
#if 0
    if(mysql_query(mysql,"select * from t_user")) {
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"mysql_query: Error: %s",mysql_error(mysql));
    }
#endif
    if (mysql_) {
        if (!reconnecting_) {
            s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"MySQL client library: %s", mysql_get_client_info());
            s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"MySQL server ver: %s ", mysql_get_server_info(mysql_));
            if (mysql_get_server_version(mysql_) != mysql_get_client_version()) {
                s_log->SaveLog(LOG_FILE_DATABASE,kWarnings,"[WARNING] MySQL client/server version mismatch; may conflict with behaviour of prepared statements.");
            }
        }

        s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"Connected to MySQL database at %s", connection_info_.host_.c_str());
        mysql_autocommit(mysql_, 1);

        // set connection properties to UTF8 to properly handle locales for different
        // server configs - core sends data in UTF8, so MySQL must expect UTF8 too
        mysql_set_character_set(mysql_, "utf8");
        return PrepareStatements();
    } else {
        s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"Could not connect to MySQL database at %s: %s", connection_info_.host_.c_str(), mysql_error(mysql));
        mysql_close(mysql);
        return false;
    }
    return true;
}

void EasyMySQLConnection::Close() {
    /// Only close us if we're not operating
    delete this;
}

easy_bool EasyMySQLConnection::PrepareStatements() {
    DoPrepareStatements();
    for (PreparedStatementMap::const_iterator itr = queries_.begin(); itr != queries_.end(); ++itr) {
        PrepareStatement(itr->first, itr->second.first, itr->second.second);
    }
    return !prepare_error_;
}

void EasyMySQLConnection::PrepareStatement( easy_uint32 index, const char* sql, ConnectionFlags flags ) {
    // For reconnection case
    if (reconnecting_)
        delete mysql_stmts_[index];
    // Check if specified query should be prepared on this connection
    // i.e. don't prepare async statements on synchronous connections
    // to save memory that will not be used.
    if (!(connection_flags & flags)) {
        mysql_stmts_[index] = NULL;
        return;
    }
    MYSQL_STMT * stmt = mysql_stmt_init(mysql_);
    if (!stmt) {
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"[ERROR]: In mysql_stmt_init() id: %u, sql: \"%s\" ", index, sql);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"[ERROR]: %s\n", mysql_error(mysql_));
        prepare_error_ = true;
    } else {
        if (mysql_stmt_prepare(stmt, sql, static_cast<unsigned long>(strlen(sql)))) {
            s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"[ERROR]: In mysql_stmt_prepare() id: %u, sql: \"%s\" ", index, sql);
            s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"[ERROR]: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            prepare_error_ = true;
        } else {
            EasyMySQLPreparedStatement* mStmt = new EasyMySQLPreparedStatement(stmt);
            mysql_stmts_[index] = mStmt;
        }
    }
}

easy_bool EasyMySQLConnection::Execute( const char* sql ) {
    if (!mysql_)
        return false;
    {
        if (mysql_query(mysql_, sql)) {
            easy_int32	l_errno = mysql_errno(mysql_);

            s_log->SaveLog(LOG_FILE_DATABASE,kInformative,"SQL: %s", sql);
            s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"ERROR: [%u] %s", l_errno, mysql_error(mysql_));

            if (_HandleMySQLErrno(l_errno)) {	// If it returns true, an error was handled successfully (i.e. reconnection)
                return Execute(sql);			// Try again
            }
            return false;
        }
    }
    return true;
}

easy_bool EasyMySQLConnection::Execute( EasyPreparedStatement* stmt ) {
    if (!mysql_)
        return false;
    easy_uint32 index = stmt->index_;
    EasyMySQLPreparedStatement* mysql_stmt = GetPreparedStatement(index);
    assert(mysql_stmt);            // Can only be null if preparation failed, server side error or bad query
    mysql_stmt->stmt_ = stmt;     // Cross reference them for debug output
    stmt->stmt_ = mysql_stmt;     // TODO: Cleaner way
    stmt->BindParameters();
    MYSQL_STMT* msql_STMT = mysql_stmt->GetSTMT();
    MYSQL_BIND* msql_BIND = mysql_stmt->GetBind();

    if (mysql_stmt_bind_param(msql_STMT, msql_BIND)) {
        easy_uint32 l_errno = mysql_errno(mysql_);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"SQL(p): %s\n [ERROR]: [%u] %s\n", mysql_stmt->getQueryString(queries_[index].first).c_str(), l_errno, mysql_stmt_error(msql_STMT));

        if (_HandleMySQLErrno(l_errno))  // If it returns true, an error was handled successfully (i.e. reconnection)
            return Execute(stmt);       // Try again

        mysql_stmt->ClearParameters();
        return false;
    }
    if (mysql_stmt_execute(msql_STMT)) {
        easy_uint32 l_errno = mysql_errno(mysql_);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"SQL(p): %s\n [ERROR]: [%u] %s", mysql_stmt->getQueryString(queries_[index].first).c_str(), l_errno, mysql_stmt_error(msql_STMT));

        if (_HandleMySQLErrno(l_errno))  // If it returns true, an error was handled successfully (i.e. reconnection)
            return Execute(stmt);       // Try again

        mysql_stmt->ClearParameters();
        return false;
    }
    mysql_stmt->ClearParameters();
    return true;
}

easy_bool EasyMySQLConnection::_HandleMySQLErrno( easy_uint32 err_no ) {
    switch (err_no) {
    case 2006:  // "MySQL server has gone away"
    case 2013:  // "Lost connection to MySQL server during query"
    case 2048:  // "Invalid connection handle"
    case 2055: { // "Lost connection to MySQL server at '%s', system error: %d"
        reconnecting_ = true;
        easy_uint64 oldThreadId = mysql_thread_id(GetHandle());
        mysql_close(GetHandle());
        if (this->Open()) {                         // Don't remove 'this' pointer unless you want to skip loading all prepared statements....
            s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"Connection to the MySQL server is active.");
            if (oldThreadId != mysql_thread_id(GetHandle()))
                s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"Successfully reconnected to %s @%s:%s (%s).",
                               connection_info_.database_.c_str(), connection_info_.host_.c_str(), connection_info_.port_or_socket_.c_str(),
                               (connection_flags & CONNECTION_ASYNC) ? "asynchronous" : "synchronous");

            reconnecting_ = false;
            return true;
        }

        easy_uint32 lErrno = mysql_errno(GetHandle());   // It's possible this attempted reconnect throws 2006 at us. To prevent crazy recursive calls, sleep here.
        return _HandleMySQLErrno(lErrno);           // Call self (recursive)
    }

    case 1213:      // "Deadlock found when trying to get lock; try restarting transaction"
        return false;    // Implemented in TransactionTask::Execute and DatabaseWorkerPool<T>::DirectCommitTransaction
    // Query related errors - skip query
    case 1058:      // "Column count doesn't match value count"
    case 1062:      // "Duplicate entry '%s' for key '%d'"
    case 1054:      // "Unknown column '%s' in 'order clause'"
        return false;

    default:
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"Unhandled MySQL errno %u. Unexpected behaviour possible.", err_no);
        return false;
    }
}

EasyResultSet* EasyMySQLConnection::Query( const char* sql ) {
    if (!sql)
        return NULL;

    MYSQL_RES *result = NULL;
    MYSQL_FIELD *fields = NULL;
    easy_uint64 rowCount = 0;
    easy_uint32 fieldCount = 0;

    if (!_Query(sql, &result, &fields, &rowCount, &fieldCount))
        return NULL;

    return new EasyResultSet(result, fields, rowCount, fieldCount);
}

EasyPreparedResultSet* EasyMySQLConnection::Query(EasyPreparedStatement* stmt) {
    MYSQL_RES *result = NULL;
    easy_uint64 row_count = 0;
    easy_uint32 field_count = 0;

    if (!_Query(stmt, &result, &row_count, &field_count))
        return NULL;

    if (mysql_more_results(mysql_)) {
        mysql_next_result(mysql_);
    }
    return new EasyPreparedResultSet(stmt->stmt_->GetSTMT(), result, row_count, field_count);
}

EasyMySQLPreparedStatement* EasyMySQLConnection::GetPreparedStatement( easy_uint32 index ) {
    assert(index < mysql_stmts_.size());
    EasyMySQLPreparedStatement* ret = mysql_stmts_[index];
    if (!ret)
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"ERROR: Could not fetch prepared statement %u on database `%s`, connection type: %s.",
                       index, connection_info_.database_.c_str(), (connection_flags & CONNECTION_ASYNC) ? "asynchronous" : "synchronous");

    return ret;
}

easy_bool EasyMySQLConnection::_Query( const char *sql, MYSQL_RES **pResult, MYSQL_FIELD **pFields, easy_uint64* pRowCount, easy_uint32* pFieldCount ) {
    if (!mysql_)
        return false;
    if (mysql_query(mysql_, sql)) {
        easy_uint32 l_error = mysql_errno(mysql_);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"SQL: %s", sql);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"ERROR: [%u] %s", l_error, mysql_error(mysql_));

        if (_HandleMySQLErrno(l_error))      // If it returns true, an error was handled successfully (i.e. reconnection)
            return _Query(sql, pResult, pFields, pRowCount, pFieldCount);    // We try again

        return false;
    }
    *pResult = mysql_store_result(mysql_);
    *pRowCount = mysql_affected_rows(mysql_);
    *pFieldCount = mysql_field_count(mysql_);

    if (!*pResult )
        return false;

    if (!*pRowCount) {
        mysql_free_result(*pResult);
        return false;
    }
    *pFields = mysql_fetch_fields(*pResult);
    return true;
}

easy_bool EasyMySQLConnection::_Query( EasyPreparedStatement* stmt, MYSQL_RES **pResult, easy_uint64* pRowCount, easy_uint32* pFieldCount ) {
    if (!mysql_)
        return false;
    easy_uint32 index = stmt->index_;

    EasyMySQLPreparedStatement* m_mStmt = GetPreparedStatement(index);
    assert(m_mStmt);					// Can only be null if preparation failed, server side error or bad query
    m_mStmt->stmt_ = stmt;				// Cross reference them for debug output
    stmt->stmt_ = m_mStmt;				// TODO: Cleaner way
    stmt->BindParameters();

    MYSQL_STMT* msql_STMT = m_mStmt->GetSTMT();
    MYSQL_BIND* msql_BIND = m_mStmt->GetBind();

    if (mysql_stmt_bind_param(msql_STMT, msql_BIND)) {
        easy_uint32 lErrno = mysql_errno(mysql_);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"SQL(p): %s\n [ERROR]: [%u] %s", m_mStmt->getQueryString(queries_[index].first).c_str(), lErrno, mysql_stmt_error(msql_STMT));

        if (_HandleMySQLErrno(lErrno))  // If it returns true, an error was handled successfully (i.e. reconnection)
            return _Query(stmt, pResult, pRowCount, pFieldCount);       // Try again

        m_mStmt->ClearParameters();
        return false;
    }

    if (mysql_stmt_execute(msql_STMT)) {
        easy_uint32 lErrno = mysql_errno(mysql_);
        s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"SQL(p): %s\n [ERROR]: [%u] %s",
                       m_mStmt->getQueryString(queries_[index].first).c_str(), lErrno, mysql_stmt_error(msql_STMT));

        if (_HandleMySQLErrno(lErrno))  // If it returns true, an error was handled successfully (i.e. reconnection)
            return _Query(stmt, pResult, pRowCount, pFieldCount);      // Try again

        m_mStmt->ClearParameters();
        return false;
    }
    m_mStmt->ClearParameters();

    *pResult = mysql_stmt_result_metadata(msql_STMT);
    *pRowCount = mysql_stmt_num_rows(msql_STMT);
    *pFieldCount = mysql_stmt_field_count(msql_STMT);
    return true;
}

}