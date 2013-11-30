/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   10:28
	filename: 	F:\easy\src\database\easy_data_base_worker_pool.h
	file path:	F:\easy\src\database
	file base:	easy_data_base_worker_pool
	file ext:	h
	author:		Lee
	
	purpose:	manager multi thread to operator a database
*********************************************************************/
#ifndef easy_data_base_worker_pool_h__
#define easy_data_base_worker_pool_h__
#include <vector>

#ifndef easy_activation_queue_h__
#include "easy_activation_queue.h"
#endif //easy_activation_queue_h__

#ifndef easy_sql_operation_h__
#include "easy_sql_operation.h"
#endif //easy_sql_operation_h__

#ifndef easy_mysql_connection_h__
#include "easy_mysql_connection.h"
#endif // easy_mysql_connection_h__

#ifndef easy_query_result_h__
#include "easy_query_result.h"
#endif //easy_query_result_h__

#define MAX_QUERY_LEN 32*1024

namespace easy
{
	class EasyPingOperation : public EasySQLOperation
	{
	public:
		virtual easy_bool Execute()
		{
			if ( connection_->LockIfReady() )
			{
				connection_->Ping();
				connection_->Unlock();
				return true;
			}
			return false;
		}
	};

	template< class T >
	class EasyDatabaseWorkerPool 
	{
	public:
		EasyDatabaseWorkerPool() 
		{
			memset(connection_count_, 0, sizeof(connection_count_));
			connection_.resize(IDX_SIZE);
		}

		bool Open(const std::string& info_string, easy_uint8 async_threads, easy_uint8 synch_threads)
		{
			easy_bool res = true;
			connection_info_ = EasyMySQLConnectionInfo(info_string);

			/// Open synchronous connections (direct, blocking operations)
			connection_[IDX_SYNCH].resize(synch_threads);
			for (easy_uint8 i = 0; i < synch_threads; ++i)
			{
				T* t = new T(connection_info_);
				res &= t->Open();
				connection_[IDX_SYNCH][i] = t;
				++connection_count_[IDX_SYNCH];
			}
			return res;
		}

		void Close()
		{
			printf("Closing down database pool '%s'.", connection_info_.database_.c_str());

			/// Shut down the synchronous connections
			for (easy_uint8 i = 0; i < connection_count_[IDX_SYNCH]; ++i)
			{
				T* t = m_connections[IDX_SYNCH][i];
				t->Close();
				delete t;
				t = NULL;
			}

			printf("All connections on database pool %s closed.", connection_info_.database.c_str());
		}

		void SetCharacterSet(std::string& character_set) { connection_[IDX_SYNCH][0]->SetCharacterSet(character_set); }

		//! Enqueues a one-way SQL operation in string format that will be executed asynchronously.
		easy_bool Execute(const char* sql)
		{
			if (!sql)
				return false;

			return connection_[IDX_SYNCH][0]->Execute(sql);
		}

        //! Enqueues a one-way SQL operation in string format -with variable args- that will be executed asynchronously.
        easy_bool PExecute(const char* sql, ...)
        {
            if (!sql)
                return false;

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

           return Execute(szQuery);
        }

        //! Directly executes a one-way SQL operation in string format, that will block the calling thread until finished.
        easy_bool DirectExecute(const char* sql)
        {
            if (!sql)
                return false;

           return connection_[IDX_SYNCH][0]->Execute(sql);
        }

        //! Directly executes a one-way SQL operation in string format -with variable args-, that will block the calling thread until finished.
        easy_bool DirectPExecute(const char* sql, ...)
        {
            if (!sql)
                return false;

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, sql);
            vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
            va_end(ap);

            return DirectExecute(szQuery);
        }

		//! Directly executes a one-way SQL operation in prepared statement format, that will block the calling thread until finished.
		easy_bool DirectExecute(EasyPreparedStatement* stmt)
		{
			return connection_[IDX_SYNCH][0]->Execute(stmt);
		}

		//! Directly executes an SQL query in string format that will block the calling thread until finished.
		//! Returns reference counted auto pointer, no need for manual memory management in upper level code.
#if 0
		EasyResultSet* Query(const char* sql, EasyMySQLConnection* conn = NULL)
		{
			EasyResultSet* result = connection_[IDX_SYNCH][0]->Query(sql);
			if (!result || !result->GetRowCount())
				return result;

			result->NextRow();
			return result;
		}
#else
		EasyQueryResult Query(const char* sql, EasyMySQLConnection* conn = NULL)
		{
			EasyResultSet* result = connection_[IDX_SYNCH][0]->Query(sql);
			if (!result || !result->GetRowCount())
				return EasyQueryResult(NULL);

			result->NextRow();
			return EasyQueryResult(result);
		}
#endif

		//! Directly executes an SQL query in prepared format that will block the calling thread until finished.
		//! Returns reference counted auto pointer, no need for manual memory management in upper level code.
#if 0
		EasyPreparedResultSet* Query(EasyPreparedStatement* stmt)
		{
			EasyPreparedResultSet* ret = connection_[IDX_SYNCH][0]->Query(stmt);
			if (!ret || !ret->GetRowCount())
				return NULL;
			return ret;
		}
#else
		PreparedQueryResult Query(EasyPreparedStatement* stmt)
		{
			EasyPreparedResultSet* ret = connection_[IDX_SYNCH][0]->Query(stmt);
			if (!ret || !ret->GetRowCount())
				return PreparedQueryResult(NULL);
			return PreparedQueryResult(ret);
		}
#endif

		//! Automanaged (internally) pointer to a prepared statement object for usage in upper level code.
		//! This object is not tied to the prepared statement on the MySQL context yet until execution.
		EasyPreparedStatement* GetPreparedStatement(easy_uint32 index)
		{
			return new EasyPreparedStatement(index);
		}

		~EasyDatabaseWorkerPool() { }
	private:
		enum
		{
			IDX_ASYNC,
			IDX_SYNCH,
			IDX_SIZE,
		};
		//	a queue used to store sql operator, we also use other data struct, such as vector
		EasyActivationQueue*			sql_operator_queue_;

		std::vector< std::vector<T*> >	connection_;

		easy_uint32						connection_count_[2];

		EasyMySQLConnectionInfo			connection_info_;

	};
}
#endif // easy_data_base_worker_pool_h__


