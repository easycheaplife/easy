/********************************************************************
	created:	2011/12/13
	created:	13:12:2011   9:44
	file base:	corpus_database
	file ext:	h
	author:		Lee

	purpose:	some thing about corpus database operator
*********************************************************************/
#ifndef corpus_database_h__
#define corpus_database_h__

#include "easy_data_base_worker_pool.h"
#include "easy_log.h"

enum CorpusDatabaseStatements {
    /*  Naming standard for defines:
        {DB}_{SET/DEL/ADD/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */
    CORPUS_ADD_CHINESE,
    CORPUS_ADD_ENGLISH,
    CORPUS_ADD_JANPANESE,
    CORPUS_ADD_RUSSIAN,
    CORPUS_ADD_KOREAN,
    CORPUS_DEL_CHINESE,
    CORPUS_DEL_ENGLISH,
    CORPUS_DEL_JANPANESE,
    CORPUS_DEL_RUSSIAN,
    CORPUS_DEL_KOREAN,
    CORPUS_SEL_CHINESE,
    CORPUS_SEL_ENGLISH,
    CORPUS_SEL_JANPANESE,
    CORPUS_SEL_RUSSIAN,
    CORPUS_SEL_KOREAN,
    CORPUS_SEL_LIMIT_CHINESE,
    CORPUS_SEL_LIMIT_ENGLISH,
    CORPUS_SEL_LIMIT_JANPANESE,
    CORPUS_SEL_LIMIT_RUSSIAN,
    CORPUS_SEL_LIMIT_KOREAN,
    CORPUS_SEL_COUNT_CHINESE,
    CORPUS_SEL_COUNT_ENGLISH,
    CORPUS_SEL_COUNT_JANPANESE,
    CORPUS_SEL_COUNT_RUSSIAN,
    CORPUS_SEL_COUNT_KOREAN,
    CORPUS_SEL_CHARACTER_COUNT_CHINESE,
    CORPUS_SEL_CHARACTER_COUNT_ENGLISH,
    CORPUS_SEL_CHARACTER_COUNT_JANPANESE,
    CORPUS_SEL_CHARACTER_COUNT_RUSSIAN,
    CORPUS_SEL_CHARACTER_COUNT_KOREAN,
    CORPUS_ADD_USER,
    CORPUS_USER_CHECK,
    CORPUS_ADD_LOG,
    CORPUS_DATABASE_STATEMENTS_MAX
};

class CorpusDatabase : public easy::EasyMySQLConnection {
  public:
    //	Constructors for sync and async connections
    CorpusDatabase(easy::EasyMySQLConnectionInfo& conn_info) : easy::EasyMySQLConnection(conn_info) { }
  private:
    //	Loads database type specific prepared statements
    void DoPrepareStatements() {
        if (!reconnecting_) {
            mysql_stmts_.resize(CORPUS_DATABASE_STATEMENTS_MAX);
        }
        PREPARE_STATEMENT(CORPUS_ADD_CHINESE,  "INSERT INTO t_chinese(file_name,context,ftp_path,character_count) VALUES(?,?,?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_ADD_ENGLISH,  "INSERT INTO t_english(file_name,context,ftp_path,character_count) VALUES(?,?,?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_ADD_JANPANESE,  "INSERT INTO t_japanese(file_name,context,ftp_path,character_count) VALUES(?,?,?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_ADD_RUSSIAN,  "INSERT INTO t_russian(file_name,context,ftp_path,character_count) VALUES(?,?,?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_ADD_KOREAN,  "INSERT INTO t_korean(file_name,context,ftp_path,character_count) VALUES(?,?,?,?)" , easy::CONNECTION_SYNCH)

        PREPARE_STATEMENT(CORPUS_DEL_CHINESE,  "DELETE from t_chinese WHERE (file_name = ?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_DEL_ENGLISH,  "DELETE from t_english WHERE (file_name = ?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_DEL_JANPANESE,  "DELETE from t_japanese WHERE (file_name = ?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_DEL_RUSSIAN,  "DELETE from t_russian WHERE (file_name = ?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_DEL_KOREAN,  "DELETE from t_korean WHERE (file_name = ?)" , easy::CONNECTION_SYNCH)

        PREPARE_STATEMENT(CORPUS_SEL_CHINESE,  "SELECT file_name,ftp_path,context,character_count FROM t_chinese WHERE context like ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_ENGLISH,  "SELECT file_name,ftp_path,context,character_count FROM t_english WHERE context like ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_JANPANESE,  "SELECT file_name,ftp_path,context,character_count FROM t_japanese WHERE context like ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_RUSSIAN,  "SELECT file_name,ftp_path,context,character_count FROM t_russian WHERE context like ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_KOREAN,  "SELECT file_name,ftp_path,context,character_count FROM t_korean WHERE context like ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_LIMIT_CHINESE,  "SELECT file_name,ftp_path,context,character_count FROM t_chinese  WHERE context like ? LIMIT ?,?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_LIMIT_ENGLISH,  "SELECT file_name,ftp_path,context,character_count FROM t_english WHERE context like ? LIMIT ?,?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_LIMIT_JANPANESE,  "SELECT file_name,ftp_path,context,character_count FROM t_japanese WHERE context like ? LIMIT ?,?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_LIMIT_RUSSIAN,  "SELECT file_name,ftp_path,context,character_count FROM t_russian WHERE context like ? LIMIT ?,?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_LIMIT_KOREAN,  "SELECT file_name,ftp_path,context,character_count FROM t_korean WHERE context like ? LIMIT ?,?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_COUNT_CHINESE,  "SELECT COUNT(*) context FROM t_chinese  WHERE context like ? " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_COUNT_ENGLISH,  "SELECT COUNT(*) context FROM t_english WHERE context like ? " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_COUNT_JANPANESE,  "SELECT COUNT(*) context FROM t_japanese WHERE context like ? " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_COUNT_RUSSIAN,  "SELECT COUNT(*) context FROM t_russian WHERE context like ? " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_COUNT_KOREAN,  "SELECT COUNT(*) context FROM t_korean WHERE context like ? " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_CHARACTER_COUNT_CHINESE,  "SELECT SUM(character_count) FROM t_chinese " , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_CHARACTER_COUNT_ENGLISH,  "SELECT SUM(character_count) FROM t_english" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_CHARACTER_COUNT_JANPANESE,  "SELECT SUM(character_count) FROM t_japanese" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_CHARACTER_COUNT_RUSSIAN,  "SELECT SUM(character_count) FROM t_russian" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_SEL_CHARACTER_COUNT_KOREAN,  "SELECT SUM(character_count) FROM t_korean" , easy::CONNECTION_SYNCH)

        PREPARE_STATEMENT(CORPUS_ADD_USER,  "INSERT INTO t_user(username,password,permission) VALUES(?,?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_USER_CHECK,  "SELECT permission FROM t_user WHERE (password = ?) AND (password = ?)", easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(CORPUS_ADD_LOG,  "INSERT INTO t_log(username) VALUES(?)" , easy::CONNECTION_SYNCH)


    }
};

typedef easy::EasyDatabaseWorkerPool<CorpusDatabase> CorpusDatabaseWorkerPool;

#endif // corpus_database_h__