/********************************************************************
	created:	2011/10/22
	created:	22:10:2011   14:22
	filename: 	E:\lee-private-project\trunk\easy\src\database\easy_query_result.h
	file path:	E:\lee-private-project\trunk\easy\src\database
	file base:	easy_query_result
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_query_result_h__
#define easy_query_result_h__
#ifdef __WINDOWS
#include <WinSock.h>
#endif
#include <mysql.h>
#include <assert.h>
#include <vector>

#ifndef easy_field_h__
#include "easy_field.h"
#endif //easy_field_h__

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_counted_auto_ptr_h__
#include "easy_counted_auto_ptr.h"
#endif //easy_counted_auto_ptr_h__

namespace easy {
class EasyResultSet {
  public:
    EasyResultSet(MYSQL_RES *result,MYSQL_FIELD *fields, easy_uint64 row_count, easy_uint32 field_count);

    ~EasyResultSet();

    easy_bool NextRow();

    easy_uint64 GetRowCount() const {
        return row_count_;
    }

    easy_uint32 GetFieldCount() const {
        return field_count_;
    }

    EasyField* Fetch() const {
        return current_row_;
    }

    const EasyField & operator [] (easy_uint32 index) const {
        assert(index < field_count_);
        return current_row_[index];
    }
  protected:
    EasyField*			current_row_;
    easy_uint64			row_count_;
    easy_uint32			field_count_;

  private:
    void				_CleanUp();
    MYSQL_RES*			result_;
    MYSQL_FIELD *		fields_;
};

typedef EasyRefcountedAutoPtr<EasyResultSet, EasyMutex> EasyQueryResult;

class EasyPreparedResultSet {
  public:
    EasyPreparedResultSet(MYSQL_STMT* stmt, MYSQL_RES *result, easy_uint64 row_count, easy_uint32 field_count);

    ~EasyPreparedResultSet();

    easy_bool NextRow();

    easy_uint64 GetRowCount() const {
        return row_count_;
    }
    easy_uint32 GetFieldCount() const {
        return field_count_;
    }

    EasyField* Fetch() const {
        assert(row_position_ < row_count_);
        return rows_[easy_uint32(row_position_)];
    }

    const EasyField & operator [] (easy_uint32 index) const {
        assert(row_position_ < row_count_);
        assert(index < field_count_);
        return rows_[easy_uint32(row_position_)][index];
    }
  protected:
    easy_uint64				row_count_;
    easy_uint64				row_position_;
    std::vector<EasyField*> rows_;
    easy_uint32				field_count_;
  private:
    MYSQL_BIND*				bind_;
    MYSQL_STMT*				stmt_;
    MYSQL_RES*				res_;

    easy_my_bool*			is_null_;

    unsigned long*			length_;

    void _FreeBindBuffer();

    void _CleanUp();

    easy_bool _NextRow();
};

typedef EasyRefcountedAutoPtr<EasyPreparedResultSet, EasyMutex> PreparedQueryResult;
}
#endif // easy_query_result_h__