#include "easy_query_result.h"

namespace easy
{


	EasyResultSet::EasyResultSet( MYSQL_RES *result,MYSQL_FIELD *fields, easy_uint64 row_count, easy_uint32 field_count ):
			row_count_(row_count),
			field_count_(field_count),
			result_(result),
			fields_(fields)
	{
		current_row_ = new EasyField[field_count];
		assert(current_row_);
	}

	EasyResultSet::~EasyResultSet()
	{
		_CleanUp();
	}

	void EasyResultSet::_CleanUp()
	{
		if (current_row_)
		{
			delete [] current_row_;
			current_row_ = NULL;
		}

		if (result_)
		{
			mysql_free_result(result_);
			result_ = NULL;
		}
	}

	easy_bool EasyResultSet::NextRow()
	{
		MYSQL_ROW row;

		if (!result_)
			return false;

		row = mysql_fetch_row(result_);
		if (!row)
		{
			_CleanUp();
			return false;
		}

		for (easy_int32 i = 0; i < field_count_; i++)
			current_row_[i].SetStructuredValue(row[i], fields_[i].type);

		return true;
	}

	EasyPreparedResultSet::EasyPreparedResultSet( MYSQL_STMT* stmt, MYSQL_RES *result, easy_uint64 row_count, easy_uint32 field_count ) :
			row_count_(row_count),
				row_position_(0),
				field_count_(field_count),
				bind_(NULL),
				stmt_(stmt),
				res_(result),
				is_null_(NULL),
				length_(NULL)		
	{
		if (!res_)
			return;

		if (stmt_->bind_result_done)
		{
			delete[] stmt_->bind->length;
			delete[] stmt_->bind->is_null;
		}

		bind_		= new MYSQL_BIND[field_count_];
		is_null_	= new my_bool[field_count_];
		length_		= new unsigned long[field_count_];

		memset(is_null_, 0, sizeof(my_bool) * field_count_);
		memset(bind_, 0, sizeof(MYSQL_BIND) * field_count_);
		memset(length_, 0, sizeof(unsigned long) * field_count_);

		//- This is where we store the (entire) result set
		if (mysql_stmt_store_result(stmt_))
		{
			s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"%s:mysql_stmt_store_result, cannot bind result from MySQL server. Error: %s", __FUNCTION__, mysql_stmt_error(stmt_));
			return;
		}
		//- This is where we prepare the buffer based on metadata
		easy_uint32 i = 0;
		MYSQL_FIELD* field = mysql_fetch_field(res_);
		while (field)
		{
			size_t size = EasyField::SizeForType(field);

			bind_[i].buffer_type = field->type;
			bind_[i].buffer = malloc(size);
			memset(bind_[i].buffer, 0, size);
			bind_[i].buffer_length = size;
			bind_[i].length = &length_[i];
			bind_[i].is_null = &is_null_[i];
			bind_[i].error = NULL;
			bind_[i].is_unsigned = field->flags & UNSIGNED_FLAG;

			++i;
			field = mysql_fetch_field(res_);
		}

		//- This is where we bind the bind the buffer to the statement
		if (mysql_stmt_bind_result(stmt_, bind_))
		{
			s_log->SaveLog(LOG_FILE_DATABASE,kErrors,"%s:mysql_stmt_bind_result, cannot bind result from MySQL server. Error: %s", __FUNCTION__, mysql_stmt_error(stmt_));
			delete[] bind_;
			delete[] is_null_;
			delete[] length_;
			return;
		}

		row_count_ = mysql_stmt_num_rows(stmt_);

		rows_.resize(easy_uint32(row_count_));
		while (_NextRow())
		{
			rows_[easy_uint32(row_position_)] = new EasyField[field_count_];
			for (easy_uint64 fIndex = 0; fIndex < field_count_; ++fIndex)
			{
				if (!*bind_[fIndex].is_null)
					rows_[easy_uint32(row_position_)][fIndex].SetByteValue( bind_[fIndex].buffer,
					bind_[fIndex].buffer_length,
					bind_[fIndex].buffer_type,
					*bind_[fIndex].length );
				else
					switch (bind_[fIndex].buffer_type)
				{
					case MYSQL_TYPE_TINY_BLOB:
					case MYSQL_TYPE_MEDIUM_BLOB:
					case MYSQL_TYPE_LONG_BLOB:
					case MYSQL_TYPE_BLOB:
					case MYSQL_TYPE_STRING:
					case MYSQL_TYPE_VAR_STRING:
						rows_[easy_uint32(row_position_)][fIndex].SetByteValue( "",
							bind_[fIndex].buffer_length,
							bind_[fIndex].buffer_type,
							*bind_[fIndex].length );
						break;
					default:
						rows_[easy_uint32(row_position_)][fIndex].SetByteValue( 0,
							bind_[fIndex].buffer_length,
							bind_[fIndex].buffer_type,
							*bind_[fIndex].length );
				}
			}
			row_position_++;
		}
		row_position_ = 0;

		/// All data is buffered, let go of mysql c api structures
		_CleanUp();
	}

	EasyPreparedResultSet::~EasyPreparedResultSet()
	{

	}

	void EasyPreparedResultSet::_FreeBindBuffer()
	{
		for (easy_uint32 i = 0; i < field_count_; ++i)
			free (bind_[i].buffer);
	}

	void EasyPreparedResultSet::_CleanUp()
	{
		/// More of the in our code allocated sources are deallocated by the poorly documented mysql c api
		if (res_)
			mysql_free_result(res_);

		_FreeBindBuffer();
		mysql_stmt_free_result(stmt_);

		delete[] bind_;
	}

	easy_bool EasyPreparedResultSet::_NextRow()
	{
		/// Only called in low-level code, namely the constructor
		/// Will iterate over every row of data and buffer it
		if (row_position_ >= row_count_)
			return false;

		easy_int32 retval = mysql_stmt_fetch( stmt_ );

		if (!retval || retval == MYSQL_DATA_TRUNCATED)
			retval = true;

		if (retval == MYSQL_NO_DATA)
			retval = false;

		return retval;
	}

	easy_bool EasyPreparedResultSet::NextRow()
	{
		/// Only updates the m_rowPosition so upper level code knows in which element
		/// of the rows vector to look
		if (++row_position_ >= row_count_)
			return false;

		return true;
	}

}