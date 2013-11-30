/********************************************************************
	created:	2011/12/15
	created:	15:12:2011   13:07
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_obj_pool.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_obj_pool
	file ext:	h
	author:		Lee
	
	purpose:	a common template class to manage object alloca and release.
*********************************************************************/
#ifndef easy_obj_pool_h__
#define easy_obj_pool_h__
#include <list>
#include <algorithm>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

#ifndef easy_copy_disabled_h__
#include "easy_copy_disabled.h"
#endif //easy_copy_disabled_h__

#ifndef easy_stdandard_allocator_h__
#include "easy_stdandard_allocator.h"
#endif //easy_stdandard_allocator_h__

namespace easy
{
	template<typename _Type/*, typename _Lock*/>
	class EasyObjectPool : public EasyCopyDisabled, public easy_stdand_allocator
	{
	public:
		explicit EasyObjectPool(easy_int32 obj_size)
		{
			obj_size_ = obj_size;
		}

		~EasyObjectPool()
		{
			_clear();
		}

		_Type allocate()
		{
			void* obj = easy_null;
			if (free_list_.empty())
			{
				obj = static_cast<void*>(new easy_uint8[obj_size_]);
			}
			else
			{
				obj = free_list_.back();
				free_list_.pop_back();
			}
			return static_cast<_Type>(obj);
		}

		void destrory(_Type* obj)
		{
			if (!obj)
			{
				return;
			}
			free_list_.push_back(obj);
		}

		easy_int32 obj_size() const { return obj_size_; }

	private:
		void _clear() { for_each(free_list_.begin(),free_list_.end(),delete_objdect); free_list_.clear(); std::list <_Type>().swap(free_list_); }

		static 	void delete_objdect( void* p )
		{
			if (p)
			{
				delete [] p;
			}
		}

		typedef std::list <_Type>	TypeFreeList;
		TypeFreeList				free_list_;
		easy_int32					obj_size_;
	};
}
#endif // easy_obj_pool_h__