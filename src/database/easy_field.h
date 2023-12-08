/********************************************************************
	created:	2011/10/22
	created:	22:10:2011   14:25
	filename: 	E:\lee-private-project\trunk\easy\src\database\easy_field.h
	file path:	E:\lee-private-project\trunk\easy\src\database
	file base:	easy_field
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_field_h__
#define easy_field_h__
#ifdef __WINDOWS
#include <WinSock.h>
#endif

#include <mysql.h>
#include <stdio.h>
#include <string>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_log_h__
#include "easy_log.h"
#endif // easy_log_h__

namespace easy {
class EasyField {
    friend class EasyResultSet;
    friend class EasyPreparedResultSet;
  public:
    bool GetBool() const { // Wrapper, actually gets integer
        return GetUInt8() == 1 ? true : false;
    }

    easy_uint8 GetUInt8() const {
        if (!data.value) {
            return 0;
        }

        if (data.raw) {
            return *reinterpret_cast<easy_uint8*>(data.value);
        }
        return static_cast<easy_uint8>(atol((char*)data.value));
    }

    easy_int8 GetInt8() const {
        if (!data.value)
            return 0;

        if (data.raw)
            return *reinterpret_cast<easy_int8*>(data.value);
        return static_cast<easy_int8>(atol((char*)data.value));
    }

    easy_uint16 GetUInt16() const {
        if (!data.value)
            return 0;
        if (data.raw)
            return *reinterpret_cast<easy_uint16*>(data.value);
        return static_cast<easy_uint16>(atol((char*)data.value));
    }

    easy_int16 GetInt16() const {
        if (!data.value)
            return 0;

        if (data.raw)
            return *reinterpret_cast<easy_int16*>(data.value);
        return static_cast<easy_int16>(atol((char*)data.value));
    }

    easy_uint32 GetUInt32() const {
        if (!data.value)
            return 0;
        if (data.raw)
            return *reinterpret_cast<easy_uint32*>(data.value);
        return static_cast<easy_uint32>(atol((char*)data.value));
    }

    easy_uint32 GetInt32() const {
        if (!data.value)
            return 0;
        if (data.raw)
            return *reinterpret_cast<easy_uint32*>(data.value);
        return static_cast<easy_uint32>(atol((char*)data.value));
    }

    easy_uint64 GetUInt64() const {
        if (!data.value)
            return 0;
        if (data.raw)
            return *reinterpret_cast<easy_uint64*>(data.value);
        return static_cast<easy_uint64>(atol((char*)data.value));
    }

    easy_int64 GetInt64() const {
        if (!data.value)
            return 0;
        if (data.raw)
            return *reinterpret_cast<easy_int64*>(data.value);
        return static_cast<easy_int64>(atol((char*)data.value));
    }

    float GetFloat() const {
        if (!data.value)
            return 0.0f;

        if (data.raw)
            return *reinterpret_cast<float*>(data.value);
        return static_cast<float>(atof((char*)data.value));
    }

    double GetDouble() const {
        if (!data.value)
            return 0.0f;

        if (data.raw)
            return *reinterpret_cast<double*>(data.value);
        return static_cast<double>(atof((char*)data.value));
    }

    const char* GetCString() const {
        if (!data.value)
            return NULL;

        return static_cast<const char*>(data.value);
    }

    std::string GetString() const {
        if (!data.value)
            return "";

        if (data.raw) {
            const char* string = GetCString();
            if (!string)
                string = "";
            return std::string(string, data.length);
        }
        return std::string((char*)data.value);
    }

    EasyField();

    ~EasyField();
  protected:

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
    struct {
        easy_uint32 length;     // Length (prepared strings only)
        void* value;            // Actual data in memory
        enum_field_types type;  // Field type
        bool raw;               // Raw bytes? (Prepared statement or ad hoc)
    } data;
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

    void SetByteValue(const void* newValue, const size_t newSize, enum_field_types newType, easy_uint32 length);

    void SetStructuredValue(char* newValue, enum_field_types newType);

    void CleanUp() {
        delete[] ((char*)data.value);
        data.value = NULL;
    }

    static size_t SizeForType(MYSQL_FIELD* field) {
        switch (field->type) {
        case MYSQL_TYPE_NULL:
            return 0;
        case MYSQL_TYPE_TINY:
            return 1;
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
            return 2;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        case MYSQL_TYPE_FLOAT:
            return 4;
        case MYSQL_TYPE_DOUBLE:
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_BIT:
            return 8;

        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
            return sizeof(MYSQL_TIME);

        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
            return field->max_length + 1;

        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
            return 64;

        case MYSQL_TYPE_GEOMETRY:
        default:
            s_log->SaveLog(LOG_FILE_DATABASE,kStandard,"SQL::SizeForType(): invalid field type %u", easy_uint32(field->type));
            return 0;
        }
    }

    bool IsNumeric() const {
        return (data.type == MYSQL_TYPE_TINY ||
                data.type == MYSQL_TYPE_SHORT ||
                data.type == MYSQL_TYPE_INT24 ||
                data.type == MYSQL_TYPE_LONG ||
                data.type == MYSQL_TYPE_FLOAT ||
                data.type == MYSQL_TYPE_DOUBLE ||
                data.type == MYSQL_TYPE_LONGLONG );
    }
};
}
#endif // easy_field_h__