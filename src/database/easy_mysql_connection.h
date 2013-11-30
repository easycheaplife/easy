/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   11:33
	filename: 	F:\easy\src\database\easy_mysql_connection.h
	file path:	F:\easy\src\database
	file base:	easy_mysql_connection
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_mysql_connection_h__
#define easy_mysql_connection_h__

#include <winsock.h>
#include <mysql.h>
#include <string>
#include <vector>
#include <map>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_util_h__
#include "easy_util.h"
#endif //easy_util_h__

#ifndef easy_activation_queue_h__
#include "easy_activation_queue.h"
#endif //easy_activation_queue_h__

#ifndef easy_query_result_h__
#include "easy_query_result.h"
#endif //easy_query_result_h__

#ifndef easy_prepared_statement_h__
#include "easy_prepared_statement.h"
#endif //easy_prepared_statement_h__

namespace easy
{
	class EasyPreparedStatement;
	class EasyMySQLPreparedStatement;

	enum ConnectionFlags
	{
		CONNECTION_ASYNC = 0x1,
		CONNECTION_SYNCH = 0x2,
	};

	struct EasyMySQLConnectionInfo
	{
		EasyMySQLConnectionInfo() {}
		EasyMySQLConnectionInfo(const std::string& info_string)
		{
			Tokens tokens(info_string, ';');

			if (tokens.size() != 5)
				return;

			easy_uint8 i = 0;

			host_.assign(tokens[i++]);
			port_or_socket_.assign(tokens[i++]);
			user_.assign(tokens[i++]);
			password_.assign(tokens[i++]);
			database_.assign(tokens[i++]);
		}

		std::string user_;
		std::string password_;
		std::string database_;
		std::string host_;
		std::string port_or_socket_;
	};

	typedef std::map<easy_uint32 /*index*/, std::pair<const char* /*query*/, ConnectionFlags /*sync/async*/> > PreparedStatementMap;

	#define PREPARE_STATEMENT(a, b, c) queries_[a] = std::make_pair(strdup(b), c);
	class EasyMySQLConnection
	{
		template <class T> friend class EasyDatabaseWorkerPool;
		friend class EasyPingOperation;
	public:
		EasyMySQLConnection(EasyMySQLConnectionInfo& conn_info);                              //! Constructor for synchronous connections.

		virtual ~EasyMySQLConnection();

		virtual easy_bool Open();

		void Close();

		easy_bool Execute(const char* sql);

		easy_bool Execute(EasyPreparedStatement* stmt);

		EasyResultSet* Query(const char* sql);

		EasyPreparedResultSet* Query(EasyPreparedStatement* stmt);

		void SetCharacterSet(std::string& character_set) { mysql_set_character_set(mysql_, character_set.c_str());}

		void Ping() { }

		void Unlock() { }
	
		protected:
		easy_bool LockIfReady(){ }

		MYSQL* GetHandle()  { return mysql_; }

		EasyMySQLPreparedStatement* GetPreparedStatement(easy_uint32 index);

		void PrepareStatement(easy_uint32 index, const char* sql, ConnectionFlags flags);

		easy_bool PrepareStatements();

		virtual void DoPrepareStatements() = 0;

		std::vector<EasyMySQLPreparedStatement*>	mysql_stmts_;			//! PreparedStatements storage
		PreparedStatementMap						queries_;				//! Query storage
		easy_bool									reconnecting_;			//! Are we reconnecting?
		easy_bool									prepare_error_;			//! Was there any error while preparing statements?

	private:
		easy_bool _Query(const char *sql, MYSQL_RES **pResult, MYSQL_FIELD **pFields,\
			easy_uint64* pRowCount, easy_uint32* pFieldCount);

		easy_bool _Query(EasyPreparedStatement* stmt, MYSQL_RES **pResult, easy_uint64* pRowCount, easy_uint32* pFieldCount);

		easy_bool _HandleMySQLErrno(easy_uint32 err_no);
	private:
		MYSQL *						mysql_;						//! MySQL Handle.

		EasyMySQLConnectionInfo&	connection_info_;			//! Connection info (used for logging)

		ConnectionFlags				connection_flags;			//! Connection flags (for preparing relevant statements)
	};
}
#endif // easy_mysql_connection_h__