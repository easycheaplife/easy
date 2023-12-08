/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   10:53
	filename: 	F:\easy\src\database\easy_sql_operation.h
	file path:	F:\easy\src\database
	file base:	easy_sql_operation
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_sql_operation_h__
#define easy_sql_operation_h__

#ifndef easy_priority_queue_h__
#include "easy_priority_queue.h"
#endif //easy_priority_queue_h__

#ifndef easy_mysql_connection_h__
#include "easy_mysql_connection.h"
#endif //easy_mysql_connection_h__

namespace easy {
class EasyMySQLConnection;

class EasySQLOperation : public EasyPriorityQueue {
  public:
    EasySQLOperation( )
        : connection_ ( easy_null ) {

    }

    virtual easy_int32 call (void) {
        Execute();
        return 0;
    }

    virtual easy_bool Execute() = 0;

    virtual void SetConnection( EasyMySQLConnection* connect ) {
        connection_ = connect ;
    }
  protected:
    EasyMySQLConnection*	connection_;
};
}

#endif // easy_sql_operation_h__