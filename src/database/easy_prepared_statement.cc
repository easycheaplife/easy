#include "easy_prepared_statement.h"
#include <iosfwd>
#include <sstream>
namespace easy {


void EasyPreparedStatement::setBool( const easy_uint8 index, const easy_bool value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.boolean = value;
    statement_data_[index].type = TYPE_BOOL;
}

void EasyPreparedStatement::setUInt8( const easy_uint8 index, const easy_uint8 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.ui32 = value;
    statement_data_[index].type = TYPE_UI8;
}

void EasyPreparedStatement::setUInt16( const easy_uint8 index, const easy_uint16 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.ui32 = value;
    statement_data_[index].type = TYPE_UI16;
}

void EasyPreparedStatement::setUInt32( const easy_uint8 index, const easy_uint32 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.ui32 = value;
    statement_data_[index].type = TYPE_UI32;
}

void EasyPreparedStatement::setUInt64( const easy_uint8 index, const easy_uint64 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.ui64 = value;
    statement_data_[index].type = TYPE_UI64;
}

void EasyPreparedStatement::setInt8( const easy_uint8 index, const easy_int8 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.i32 = value;
    statement_data_[index].type = TYPE_I8;
}

void EasyPreparedStatement::setInt16( const easy_uint8 index, const easy_int16 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.i32 = value;
    statement_data_[index].type = TYPE_I16;
}

void EasyPreparedStatement::setInt32( const easy_uint8 index, const easy_int32 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.i32 = value;
    statement_data_[index].type = TYPE_I32;
}

void EasyPreparedStatement::setInt64( const easy_uint8 index, const easy_int64 value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.i64 = value;
    statement_data_[index].type = TYPE_I64;
}

void EasyPreparedStatement::setFloat( const easy_uint8 index, const float value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.f = value;
    statement_data_[index].type = TYPE_FLOAT;
}

void EasyPreparedStatement::setDouble( const easy_uint8 index, const double value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].data.d = value;
    statement_data_[index].type = TYPE_DOUBLE;
}

void EasyPreparedStatement::setString( const easy_uint8 index, const std::string& value ) {
    if (index >= statement_data_.size())
        statement_data_.resize(index+1);

    statement_data_[index].str = value;
    statement_data_[index].type = TYPE_STRING;
}

void EasyPreparedStatement::BindParameters() {
    assert (stmt_);

    easy_uint32 i = 0;
    for (; i < statement_data_.size(); i++) {
        switch (statement_data_[i].type) {
        case TYPE_BOOL:
            stmt_->setBool(i, statement_data_[i].data.boolean);
            break;
        case TYPE_UI8:
        case TYPE_UI16:
        case TYPE_UI32:
            stmt_->setUInt32(i, statement_data_[i].data.ui32);
            break;
        case TYPE_I8:
        case TYPE_I16:
        case TYPE_I32:
            stmt_->setInt32(i, statement_data_[i].data.i32);
            break;
        case TYPE_UI64:
            stmt_->setUInt64(i, statement_data_[i].data.ui64);
            break;
        case TYPE_I64:
            stmt_->setInt64(i, statement_data_[i].data.i64);
            break;
        case TYPE_FLOAT:
            stmt_->setFloat(i, statement_data_[i].data.f);
            break;
        case TYPE_DOUBLE:
            stmt_->setDouble(i, statement_data_[i].data.d);
            break;
        case TYPE_STRING:
            stmt_->setString(i, statement_data_[i].str.c_str());
            break;
        }
    }
#ifdef _DEBUG
    if (i < stmt_->param_count_)
        s_log->SaveLog(LOG_FILE_DATABASE,kWarnings,"[WARNING]: BindParameters() for statement %u did not bind all allocated parameters", index_);
#endif
}

EasyPreparedStatement::EasyPreparedStatement( easy_int32 index )
    : stmt_(NULL),
      index_(index) {

}

EasyPreparedStatement::~EasyPreparedStatement() {

}


EasyMySQLPreparedStatement::EasyMySQLPreparedStatement( MYSQL_STMT* stmt )
    : m_stmt_(stmt)
    ,bind_(NULL) {
    /// Initialize variable parameters
    param_count_ = mysql_stmt_param_count(stmt);
    params_set_.assign(param_count_, false);
    bind_ = new MYSQL_BIND[param_count_];
    memset(bind_, 0, sizeof(MYSQL_BIND)*param_count_);

    /// "If set to 1, causes mysql_stmt_store_result() to update the metadata MYSQL_FIELD->max_length value."
    easy_my_bool bool_tmp = 1;
    mysql_stmt_attr_set(stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &bool_tmp);
}

EasyMySQLPreparedStatement::~EasyMySQLPreparedStatement() {
    ClearParameters();
    mysql_stmt_close(m_stmt_);
    delete[] bind_;
}

void EasyMySQLPreparedStatement::setBool( const easy_uint8 index, const easy_bool value ) {
    setUInt32(index, value);
}

void EasyMySQLPreparedStatement::setUInt8( const easy_uint8 index, const easy_uint8 value ) {
    setUInt32(index, value);
}

void EasyMySQLPreparedStatement::setUInt16( const easy_uint8 index, const easy_uint16 value ) {
    setUInt32(index, value);
}

void EasyMySQLPreparedStatement::setUInt32( const easy_uint8 index, const easy_uint32 value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_LONG, &value, sizeof(easy_uint32), true);
}

void EasyMySQLPreparedStatement::setUInt64( const easy_uint8 index, const easy_uint64 value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_LONGLONG, &value, sizeof(easy_uint64), true);
}

void EasyMySQLPreparedStatement::setInt8( const easy_uint8 index, const easy_int8 value ) {
    setInt32(index, value);
}

void EasyMySQLPreparedStatement::setInt16( const easy_uint8 index, const easy_int16 value ) {
    setInt32(index, value);
}

void EasyMySQLPreparedStatement::setInt32( const easy_uint8 index, const easy_int32 value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_LONG, &value, sizeof(easy_int32), false);
}

void EasyMySQLPreparedStatement::setInt64( const easy_uint8 index, const easy_int64 value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_LONGLONG, &value, sizeof(easy_int64), false);
}

void EasyMySQLPreparedStatement::setFloat( const easy_uint8 index, const float value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_FLOAT, &value, sizeof(float), (value > 0.0f));
}

void EasyMySQLPreparedStatement::setDouble( const easy_uint8 index, const double value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    setValue_(param, MYSQL_TYPE_DOUBLE, &value, sizeof(double), (value > 0.0f));
}

void EasyMySQLPreparedStatement::setString( const easy_uint8 index, const char* value ) {
    CheckValidIndex(index);
    params_set_[index] = true;
    MYSQL_BIND* param = &bind_[index];
    size_t len = strlen(value) + 1;
    param->buffer_type = MYSQL_TYPE_VAR_STRING;
    delete [] static_cast<char *>(param->buffer);
    param->buffer = new char[len];
    param->buffer_length = len;
    param->is_null_value = 0;
    param->length = new unsigned long(len-1);

    memcpy(param->buffer, value, len);
}

void EasyMySQLPreparedStatement::ClearParameters() {
    for (easy_uint32 i=0; i < param_count_; ++i) {
        delete bind_[i].length;
        bind_[i].length = NULL;
        delete[] (char*) bind_[i].buffer;
        bind_[i].buffer = NULL;
        params_set_[i] = false;
    }
}

easy_bool EasyMySQLPreparedStatement::CheckValidIndex( easy_uint8 index ) {
    if (index >= param_count_)
        return false;

    if (params_set_[index])
        s_log->SaveLog(LOG_FILE_DATABASE,kWarnings,"[WARNING] Prepared Statement (id: %u) trying to bind value on already bound index (%u).", stmt_->index_, index);
    return true;
}

std::string EasyMySQLPreparedStatement::getQueryString( const char *query ) {
    std::string queryString = query;
    easy_uint32 pos = 0;
    for (easy_uint32 i = 0; i < stmt_->statement_data_.size(); i++) {
        pos = queryString.find("?", pos);
        std::stringstream replace;
        replace << "'";
        switch (stmt_->statement_data_[i].type) {
        case TYPE_BOOL:
            replace << (stmt_->statement_data_[i].data.boolean ? "1" : "0");
            break;
        case TYPE_UI8:
        case TYPE_UI16:
        case TYPE_UI32:
            replace << stmt_->statement_data_[i].data.ui32;
            break;
        case TYPE_I8:
        case TYPE_I16:
        case TYPE_I32:
            replace << stmt_->statement_data_[i].data.i32;
            break;
        case TYPE_UI64:
            replace << stmt_->statement_data_[i].data.ui64;
            break;
        case TYPE_I64:
            replace << stmt_->statement_data_[i].data.i64;
            break;
        case TYPE_FLOAT:
            replace << stmt_->statement_data_[i].data.f;
            break;
        case TYPE_DOUBLE:
            replace << stmt_->statement_data_[i].data.d;
            break;
        case TYPE_STRING:
            replace << stmt_->statement_data_[i].str;
            break;
        }
        replace << "'";
        queryString.replace(pos, 1, replace.str());
    }
    return queryString;
}

void EasyMySQLPreparedStatement::setValue_( MYSQL_BIND* param, enum_field_types type, const void* value, easy_uint32 len, easy_bool isUnsigned ) {
    param->buffer_type = type;
    delete [] static_cast<char *>(param->buffer);
    param->buffer = new char[len];
    param->buffer_length = 0;
    param->is_null_value = 0;
    param->length = NULL;               // Only != NULL for strings
    param->is_unsigned = isUnsigned;

    memcpy(param->buffer, value, len);
}
}