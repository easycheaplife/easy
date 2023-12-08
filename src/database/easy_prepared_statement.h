/********************************************************************
	created:	2011/10/22
	created:	22:10:2011   17:22
	filename: 	E:\lee-private-project\trunk\easy\src\database\easy_prepared_statement.h
	file path:	E:\lee-private-project\trunk\easy\src\database
	file base:	easy_prepared_statement
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_prepared_statement_h__
#define easy_prepared_statement_h__

#ifndef easy_sql_operation_h__
#include "easy_sql_operation.h"
#endif //easy_sql_operation_h__

namespace easy {
//- Union for data buffer (upper-level bind -> queue -> lower-level bind)
union PreparedStatementDataUnion {
    easy_bool boolean;
    easy_uint8 ui8;
    easy_int8 i8;
    easy_uint16 ui16;
    easy_int16 i16;
    easy_uint32 ui32;
    easy_int32 i32;
    easy_uint64 ui64;
    easy_int64 i64;
    float f;
    double d;
};

//- This enum helps us differ data held in above union
enum PreparedStatementValueType {
    TYPE_BOOL,
    TYPE_UI8,
    TYPE_UI16,
    TYPE_UI32,
    TYPE_UI64,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING
};

struct EasyPreparedStatementData {
    PreparedStatementDataUnion data;
    PreparedStatementValueType type;
    std::string str;
};

//- Forward declare
class EasyMySQLPreparedStatement;

//- Upper-level class that is used in code
class EasyPreparedStatement {
    friend class EasyPreparedStatementTask;
    friend class EasyMySQLPreparedStatement;
    friend class EasyMySQLConnection;

  public:
    explicit EasyPreparedStatement(easy_int32 index);
    ~EasyPreparedStatement();

    void setBool(const easy_uint8 index, const easy_bool value);
    void setUInt8(const easy_uint8 index, const easy_uint8 value);
    void setUInt16(const easy_uint8 index, const easy_uint16 value);
    void setUInt32(const easy_uint8 index, const easy_uint32 value);
    void setUInt64(const easy_uint8 index, const easy_uint64 value);
    void setInt8(const easy_uint8 index, const easy_int8 value);
    void setInt16(const easy_uint8 index, const easy_int16 value);
    void setInt32(const easy_uint8 index, const easy_int32 value);
    void setInt64(const easy_uint8 index, const easy_int64 value);
    void setFloat(const easy_uint8 index, const float value);
    void setDouble(const easy_uint8 index, const double value);
    void setString(const easy_uint8 index, const std::string& value);

  protected:
    void BindParameters();

  protected:
    EasyMySQLPreparedStatement* stmt_;
    easy_uint32 index_;
    std::vector<EasyPreparedStatementData> statement_data_;    //- Buffer of parameters, not tied to MySQL in any way yet
};

//- Class of which the instances are unique per MySQLConnection
//- access to these class objects is only done when a prepared statement task
//- is executed.
class EasyMySQLPreparedStatement {
    friend class EasyMySQLConnection;
    friend class EasyPreparedStatement;

  public:
    EasyMySQLPreparedStatement(MYSQL_STMT* stmt);
    ~EasyMySQLPreparedStatement();

    void setBool(const easy_uint8 index, const easy_bool value);
    void setUInt8(const easy_uint8 index, const easy_uint8 value);
    void setUInt16(const easy_uint8 index, const easy_uint16 value);
    void setUInt32(const easy_uint8 index, const easy_uint32 value);
    void setUInt64(const easy_uint8 index, const easy_uint64 value);
    void setInt8(const easy_uint8 index, const easy_int8 value);
    void setInt16(const easy_uint8 index, const easy_int16 value);
    void setInt32(const easy_uint8 index, const easy_int32 value);
    void setInt64(const easy_uint8 index, const easy_int64 value);
    void setFloat(const easy_uint8 index, const float value);
    void setDouble(const easy_uint8 index, const double value);
    void setString(const easy_uint8 index, const char* value);

  protected:
    MYSQL_STMT* GetSTMT() {
        return m_stmt_;
    }

    MYSQL_BIND* GetBind() {
        return bind_;
    }

    EasyPreparedStatement* stmt_;

    void ClearParameters();

    easy_bool CheckValidIndex(easy_uint8 index);

    std::string getQueryString(const char *query);


  private:
    void setValue_(MYSQL_BIND* param, enum_field_types type, const void* value, easy_uint32 len, easy_bool isUnsigned);

  private:
    MYSQL_STMT* m_stmt_;
    easy_uint32 param_count_;
    std::vector<easy_bool> params_set_;
    MYSQL_BIND* bind_;
};
}
#endif // easy_prepared_statement_h__