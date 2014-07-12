/********************************************************************
	created:	2011/12/15
	created:	15:12:2011   13:07
	file base:	easy_lock_queue
	file ext:	h
	author:		Lee
	
	purpose:	 
*********************************************************************/
#ifndef easy_locked_queue_h__
#define easy_locked_queue_h__

namespace easy
{
	template<typename _Type, typename _Lock,typename _StorageType /*= std::list<_Type*>*/>
	class lock_queue 	
	{
		typedef typename _Type::_Key				_Key;

		static const size_t MAX_POOL_SIZE = _Type::MAX_POOL_SIZE;

	public:
		_Type* allocate(_Key __key)
		{
			_Type* __ret = 0;
			if (free_list_.empty())
			{
				__ret = new _Type(__key);
			}
			else
			{
				lock_.acquire_lock();
				__ret = free_list_.back();
				free_list_.pop_back();
				lock_.release_lock();
			}
			return __ret;
		}

		_Type* allocate(_Key __key1,_Key __key2)
		{
			_Type* __ret = 0;
			if (free_list_.empty())
			{
				__ret = new _Type(__key1,__key2);
			}
			else
			{
				lock_.acquire_lock();
				__ret = free_list_.back();
				__ret->init(__key1,__key2);
				free_list_.pop_back();
#ifdef _DEBUG
				printf("free_list_ size = %d\n",free_size());
#endif // _DEBUG
				lock_.release_lock();
			}
			return __ret;
		}

		void deallcate(_Type* __val)
		{
			if (!__val)
			{
				return;
			}
			if (MAX_POOL_SIZE < free_list_.size())
			{
				delete __val;
				return;
			}
			lock_.acquire_lock();
			free_list_.push_back(__val);
#ifdef _DEBUG
			printf("free_list_ size = %d\n",free_size());
#endif // _DEBUG
			lock_.release_lock();
		}

		size_t free_size() /*const*/ 
		{ 
			size_t __size = 0;
			lock_.acquire_lock(); 
			__size = free_list_.size(); 
			lock_.release_lock();
			return __size;
		}

		void clear() 
		{ 
			lock_.acquire_lock(); 
			for (typename _StorageType::iterator __it = free_list_.begin(); __it != free_list_.end(); ++__it)
			{
				if ((*__it))
				{
					(*__it)->clear();
					delete (*__it);
					(*__it)  = NULL;
				}
			}
			free_list_.clear(); 
			_StorageType().swap(free_list_); 
			lock_.release_lock();
		}

		~lock_queue()
		{
			clear();
		}
	private:
		_Lock						lock_;
		_StorageType				free_list_;
	};

#ifdef __EASY_USE_EXPLCIT_SPECIALIZATION
	//	explicit specialization
	template<typename _Type, typename _Lock>
	class lock_queue< _Type, _Lock,std::list<_Type*>>
	{
		typedef typename _Type::_Key				_Key;

		static const size_t MAX_POOL_SIZE = _Type::MAX_POOL_SIZE;
	public:
		_Type* allocate(_Key __key)
		{
			_Type* __ret = 0;
			if (free_list_.empty())
			{
				__ret = new _Type(__key);
			}
			else
			{
				lock_.acquire_lock();
				__ret = free_list_.back();
				free_list_.pop_back();
				lock_.release_lock();
			}
			return __ret;
		}

		void deallcate(_Type* __val)
		{
			if (!__val)
			{
				return;
			}
			if (MAX_POOL_SIZE < free_list_.size())
			{
				delete __val;
				return;
			}
			lock_.acquire_lock();
			free_list_.push_back(__val);
			lock_.release_lock();
		}

		size_t free_size() const 
		{ 
			size_t __size = 0;
			lock_.acquire_lock(); 
			__size = free_list_.size(); 
			lock_.release_lock();
			return __size;
		}

		void clear() 
		{ 
			lock_.acquire_lock(); 
			for (std::list<_Type*>::iterator __it = free_list_.begin(); __it != free_list_.end(); ++__it)
			{
				if ((*__it))
				{
					delete (*__it);
					(*__it)  = NULL;
				}
			}
			free_list_.clear(); 
			std::list<_Type*>().swap(free_list_); 
			lock_.release_lock();
		}

		~lock_queue()
		{
			clear();
		}
	private:
		_Lock						lock_;
		std::list<_Type*>			free_list_;
	};
#endif

	template<typename _Type, typename _Lock,typename _Key_Type,typename _StorageType /*= std::map<_Key_Type,_Type*>*/>
	class lock_container 	
	{
	public:
		typedef typename _StorageType::iterator				iterator;
		typedef typename _StorageType::const_iterator		const_iterator;

		void insert(_Key_Type __key,_Type* __val)
		{
			lock_.acquire_lock(); 
			container_[__key] = __val;
			lock_.release_lock();
		}

		void remove(_Key_Type __key)
		{
			lock_.acquire_lock();
			//	it will be a  error: need ¡®typename¡¯ before ¡®_Storage Type:: iterator¡¯ because ¡®_StorageType¡¯ is a dependent scope 
			//	if not use typename at cygwin
			iterator __find = container_.find(__key);
			if (__find != container_.end())
			{
				container_.erase(__find);
			}
			lock_.release_lock();
		}

		_Type* find(_Key_Type __key)
		{
			_Type* __type = NULL;
			lock_.acquire_lock();
			iterator __find = container_.find(__key);
			if (__find != container_.end())
			{
				__type = (*__find).second;
			}
			lock_.release_lock();
			return __type;
		}

		void clear()
		{
			lock_.acquire_lock();
			for (iterator __it = container_.begin(); __it != container_.end(); ++__it)
			{
				if ((*__it).second)
				{
					delete (*__it).second;
					(*__it).second  = NULL;
				}
			}
			container_.clear(); 
			_StorageType().swap(container_); 
			lock_.release_lock();
		}

		iterator begin() { return container_.begin(); }

		iterator end() { return container_.end();}

		~lock_container(){ clear();}

	private:
		_Lock						lock_;
		_StorageType				container_;
	};
}
#endif // easy_locked_queue_h__