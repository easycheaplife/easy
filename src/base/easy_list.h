/********************************************************************
created:	2011/11/23
created:	23:11:2011   16:10
file base:	easy_list
file ext:	h
author:		Lee

purpose:	design a list, thread safe, reference sgi
*********************************************************************/
#ifndef easy_list_h__
#define easy_list_h__

#if 1
#include "easy_assist.h"
#else
#ifndef easy_iterator_base_h__
#include "easy_iterator_base.h"
#endif //easy_iterator_base_h__

#ifndef easy_constructor_h__
#include "easy_constructor.h"
#endif //easy_constructor_h__

#ifndef easy_iterator_h__
#include "easy_iterator.h"
#endif //easy_iterator_h__

#ifndef easy_algorithm_h__
#include "easy_algorithm.h"
#endif //easy_algorithm_h__

#ifndef easy_type_traits_h__
#include "easy_type_traits.h"
#endif //easy_type_traits_h__
#endif

#ifndef easy_allocator_h__
#include "easy_allocator.h"
#endif //easy_allocator_h__

namespace easy
{

	struct _List_node_base 
	{
		_List_node_base* _M_next;
		_List_node_base* _M_prev;
	};

	template <class _Tp>
	struct _List_node : public _List_node_base 
	{
		_Tp _M_data;
	};

	struct _List_iterator_base 
	{
		typedef size_t                     size_type;
		typedef ptrdiff_t                  difference_type;
		typedef bidirectional_iterator_tag iterator_category;

		_List_node_base* _M_node;

		_List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
		_List_iterator_base() {}

		//	can not add lock, it will be some error.
		void _M_incr() { _M_node = _M_node->_M_next;}
		void _M_decr() { _M_node = _M_node->_M_prev;}

		bool operator==(const _List_iterator_base& __x) const 
		{
			return _M_node == __x._M_node;
		}
		bool operator!=(const _List_iterator_base& __x) const 
		{
			return _M_node != __x._M_node;
		}
	}; 

	template<class _Tp, class _Ref, class _Ptr>
	struct _List_iterator : public _List_iterator_base 
	{
		//	using partial specialization
		typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
		typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
		typedef _List_iterator<_Tp,_Ref,_Ptr>             _Self;

		typedef _Tp value_type;
		typedef _Ptr pointer;
		typedef _Ref reference;
		typedef _List_node<_Tp> _Node;

		_List_iterator(_Node* __x) : _List_iterator_base(__x) {}
		_List_iterator() {}
		_List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}

		reference operator*() const { return ((_Node*) _M_node)->_M_data; }

		pointer operator->() const { return &(operator*()); }


		_Self& operator++() 
		{ 
			this->_M_incr();
			return *this;
		}
		_Self operator++(int) 
		{ 
			_Self __tmp = *this;
			this->_M_incr();
			return __tmp;
		}
		_Self& operator--() 
		{ 
			this->_M_decr();
			return *this;
		}
		_Self operator--(int) 
		{ 
			_Self __tmp = *this;
			this->_M_decr();
			return __tmp;
		}
	};

	inline bidirectional_iterator_tag
		iterator_category(const _List_iterator_base&)
	{
		return bidirectional_iterator_tag();
	}

	template <class _Tp, class _Ref, class _Ptr>
	inline _Tp* value_type(const _List_iterator<_Tp, _Ref, _Ptr>&)
	{
		return 0;
	}

	inline ptrdiff_t* distance_type(const _List_iterator_base&)
	{
		return 0;
	}

	// Base for general standard-conforming allocators.
	template <class _Tp, class _Allocator, bool _IsStatic>
	class _List_alloc_base 
	{
	public:
		typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
			allocator_type;
		allocator_type get_allocator() const { return _Node_allocator; }

		_List_alloc_base(const allocator_type& __a) : _Node_allocator(__a) {}

	protected:
		_List_node<_Tp>* _M_get_node()
		{ return _Node_allocator.allocate(1); }
		void _M_put_node(_List_node<_Tp>* __p)
		{ _Node_allocator.deallocate(__p, 1); }

	protected:
		typename _Alloc_traits<_List_node<_Tp>, _Allocator>::allocator_type
			_Node_allocator;
		_List_node<_Tp>* _M_node;
	};

	// Specialization for instance less allocators.
	template <class _Tp, class _Allocator>
	class _List_alloc_base<_Tp, _Allocator, true> 
	{
	public:
		typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
			allocator_type;
		allocator_type get_allocator() const { return allocator_type(); }

		_List_alloc_base(const allocator_type&) {}

	protected:
		typedef typename _Alloc_traits<_List_node<_Tp>, _Allocator>::_Alloc_type
			_Alloc_type;
		_List_node<_Tp>* _M_get_node() { return _Alloc_type::allocate(1); }
		void _M_put_node(_List_node<_Tp>* __p) { _Alloc_type::deallocate(__p, 1); }

	protected:
		_List_node<_Tp>* _M_node;
	};

	template <class _Tp, class _Alloc>
	class _List_base 
		: public _List_alloc_base<_Tp, _Alloc,
		_Alloc_traits<_Tp, _Alloc>::_S_instanceless>
	{
	public:
		typedef _List_alloc_base<_Tp, _Alloc,
			_Alloc_traits<_Tp, _Alloc>::_S_instanceless>
			_Base; 
		typedef typename _Base::allocator_type allocator_type;

		_List_base(const allocator_type& __a) : _Base(__a) 
		{
			//	note: (if you use ¡®-fpermissive¡¯, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
			//	when you use inherit, you must this this to access the base class member or use using declare
			this->_M_node = this->_M_get_node();
			this->_M_node->_M_next = this->_M_node;
			this->_M_node->_M_prev = this->_M_node;
		}
		~_List_base() 
		{
			clear();
			_M_put_node(this->_M_node);
		}

		void clear();
	};

	template <class _Tp, class _Alloc>
	void  _List_base<_Tp,_Alloc>::clear() 
	{
		_List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node->_M_next;
		while (__cur != this->_M_node) 
		{
			_List_node<_Tp>* __tmp = __cur;
			__cur = (_List_node<_Tp>*) __cur->_M_next;
			_Destroy(&__tmp->_M_data);
			_M_put_node(__tmp);
		}
		this->_M_node->_M_next = this->_M_node;
		this->_M_node->_M_prev = this->_M_node;
	}

	template <class _Tp, class _Alloc = alloc >
	class list : protected _List_base<_Tp, _Alloc> 
	{
		typedef _List_base<_Tp, _Alloc> _Base;
	protected:
		typedef void* _Void_pointer;
	public:      
		typedef _Tp value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef _List_node<_Tp> _Node;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef typename _Base::allocator_type allocator_type;
		allocator_type get_allocator() const { return _Base::get_allocator(); }

	public:
		typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
		typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;

		//	partial specialization
		//	will be a error at cygwin,why?
#ifdef __WINDOWS
		typedef reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef reverse_iterator<iterator>			reverse_iterator;
#elif defined __LINUX

#endif

	protected:
		using _Base::_M_node;
		using _Base::_M_put_node;
		using _Base::_M_get_node;

	protected:
		_Node* _M_create_node(const _Tp& __x)
		{
			_Node* __p = _M_get_node();
			try 
			{
				_Construct(&__p->_M_data, __x);
			}
			catch(...) 
			{ 
				_M_put_node(__p); 
				throw; 
			}
			return __p;
		}

		_Node* _M_create_node()
		{
			_Node* __p = _M_get_node();
			try 
			{
				_Construct(&__p->_M_data);
			}
			catch(...) 
			{ 
				_M_put_node(__p); 
				throw; 
			}
			return __p;
		}

	public:
		explicit list(const allocator_type& __a = allocator_type()) : _Base(__a) {}

		//	create a list which includes __n * __val
		list(size_type __n,const _Tp& __val,const allocator_type& __a = allocator_type())
			: _Base(__a)
		{
			insert(begin(),__n,__val);
		}

		//	create a list which includes __n * 0
		explicit list(size_type __n) : _Base(allocator_type()) { insert(begin(),__n,_Tp());}

		// We don't need any dispatching tricks here, because insert does all of
		// that anyway.
		template<typename _InputIterator>
		list(_InputIterator __first,_InputIterator __last,const allocator_type& __a = allocator_type())
			:_Base(__a)
		{
			insert(begin(),__first,__last);
		}

		list(const list<_Tp,_Alloc>& __x) : _Base(__x.get_allocator()){ insert(begin(),__x.begin(),__x.end()); }

		list<_Tp,_Alloc>& operator= (const list<_Tp,_Alloc>& __x) 
		{ 
			if (this != &__x)
			{
				iterator __first1 = begin();
				iterator __last1 = end();
				const_iterator __first2 = __x.begin();
				const_iterator __last2 = __x.end();
				while (__first1 != __last1 && __first2 != __last2)
				{
					*__first1++ = *__first2++;
					if (__first2 == __last2)
					{
						erase(__first1,__last1);
					}
					else
					{
						insert(__first1,__first2,__last2);
					}
				}
			}
			return *this;
		}

		~list() { }
	public:
		iterator begin()             { return (_Node*)(_M_node->_M_next); }
		const_iterator begin() const { return (_Node*)(_M_node->_M_next); }

		iterator end()             { return _M_node; }
		const_iterator end() const { return _M_node; }
#ifdef __WINDOWS
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
#elif defined __LINUX

#endif

		bool empty() const { return _M_node->_M_next == _M_node; }
		size_type size() const 
		{
			size_type __result = 0;
			distance(begin(), end(), __result);
			return __result;
		}
		size_type max_size() const { return size_type(-1); }

		reference front() { return *begin(); }
		const_reference front() const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back() const { return *(--end()); }

		void swap(list<_Tp,_Alloc>& __x) { easy::swap(_M_node,__x._M_node); }

		iterator insert(iterator __position, const _Tp& __x) 
		{
			_Node* __tmp = _M_create_node(__x);
			__tmp->_M_next = __position._M_node;
			__tmp->_M_prev = __position._M_node->_M_prev;
			__position._M_node->_M_prev->_M_next = __tmp;
			__position._M_node->_M_prev = __tmp;
			return __tmp;
		}
		iterator insert(iterator __position) { return insert(__position,_Tp()); }

		//  [12/24/2011 update by Lee]
		//	it looks as some thing wrong
		template <class _InputIterator>
		void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
			typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
			_M_insert_dispatch(__pos, __first, __last, _Integral());
		}

		void insert(iterator __pos, size_type __n, const _Tp& __x)
		{ _M_fill_insert(__pos, __n, __x); }

		void push_front(const _Tp& __x) { insert(begin(), __x); }
		void push_front() {insert(begin());}
		void push_back(const _Tp& __x) { insert(end(), __x); }
		void push_back() {insert(end());}

		iterator erase(iterator __position)
		{
			_List_node_base* __next_node = __position._M_node->_M_next;
			_List_node_base* __prev_node = __position._M_node->_M_prev;
			_Node* __n = (_Node*)__position._M_node;
			__prev_node->_M_next = __next_node;
			__next_node->_M_prev = __prev_node;
			_Destroy(&__n->_M_data);
			_M_put_node(__n);
			return iterator((_Node*)__next_node);			
		}

		iterator erase(iterator __first,iterator __last)
		{
			while(__first != __last)
			{
				erase(__first++);
			}
			return __last;
		}

		void clear()
		{
			_Base::clear();
		}

		void resize(size_type __new_size,const _Tp& __x)
		{
			iterator __i = begin();
			size_type __len = 0;
			for (; __i != end() && __len < __new_size; ++__i, ++__len)
			{
				;
			}
			if (__len == __new_size)
			{
				erase(__i,end());
			}
			else
			{
				insert(end(),__new_size - __len,__x);
			}
		}

		void resize(size_type __new_size)
		{
			resize(__new_size,_Tp());
		}

		void pop_front() { erase(begin()); }

		void pop_back() { iterator __end = end(); erase(--__end); }

		// assign(), a generalized assignment member function.  Two
		// versions: one that takes a count, and one that takes a range.
		// The range version is a member template, so we dispatch on whether
		// or not the type is an integer.
		void assign(size_type __n,const _Tp& __val) { _M_fill_assign(__n,__val); }

		template<typename _InputIterator>
		void assign(_InputIterator __first,_InputIterator __last)
		{
			typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
			_M_assign_dispatch(__first,__last,_Integral());
		}

		void splice(iterator __position,list& __x)
		{
			if (!__x.empty())
			{
				this->transfer(__position,__x.begin(),__x.end());
			}
		}

		void splice(iterator __position,list&, iterator __i)
		{
			iterator __j = __i;
			++__j;
			if (__position == __i || __position == __j)
			{
				return;
			}
			this->transfer(__position,__i,__j);
		}

		void splice(iterator __position,list&,iterator __first,iterator __last)
		{
			if (__first != __last) 
				this->transfer(__position, __first, __last);
		}

		void remove(const _Tp& __val)
		{
			iterator __first = begin();
			iterator __last = end();
			while(__first != __last)
			{
				iterator __next = __first;
				++__next;
				if (*__first == __val)
				{
					erase(__first);
				}
				__first = __next;
			}
		}

		void unique()
		{
			iterator __first = begin();
			iterator __last = end();
			if (__first == __last)
			{
				return;
			}
			iterator __next = __first;
			while (++__next != __last)
			{
				if (*__first == *__next)
				{
					erase(__next);
				}
				else
				{
					__first = __next;
				}
				__next = __first;
			}
		}

		void merge(list& __x)
		{
			iterator __first1 = begin();
			iterator __last1 = end();
			iterator __first2 = __x.begin();
			iterator __last2 = __x.end();
			while (__first1 != __last1 && __first2 != __last2)
			{
				if (*__first2 < *__first1)
				{
					iterator __next = __first2;
					transfer(__first1,__first2,++__next);
					__first2 = __next;
				}
				else
				{
					++__first1;
				}
			}
			if (__first2 != __last2)
			{
				transfer(__last1,__first2,__last2);
			}
		}

		void reverse();

		void sort()
		{
			//	do nothing if the list has length 0 or 1
			if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node)
			{
				list<_Tp,_Alloc> __carry;
				list<_Tp,_Alloc> __counter[64];
				int __fill = 0;
				while (!empty())
				{
					__carry.splice(__carry.begin(),*this,begin());
					int __i = 0;
					while(__i < __fill && !__counter[__i].empty())
					{
						__counter[__i].merge(__carry);
						__carry.swap(__counter[__i++]);
					}
					__carry.swap(__counter[__i]);
					if (__i == __fill)
					{
						++__fill;
					}
				}
				for(int __i = 1; __i < __fill; ++__i)
				{
					__counter[__i].merge(__counter[__i-1]);
				}
				swap(__counter[__fill -1]);
			}
		}

		template<typename _Predicate> void remove_if(_Predicate __pred)
		{
			iterator __first = begin();
			iterator __last = end();
			while (__first != __last)
			{
				iterator __next = __first;
				++__next;
				if (__pred(*__first))
				{
					erase(__first);
				}
				__first = __next;
			}
		}

		template<typename _BinaryPredicate>
		void unique(_BinaryPredicate __binary_pred)
		{
			iterator __first = begin();
			iterator __last = end();
			if (__first == end())
			{
				return;
			}
			iterator __next = __first;
			while(++__next != ++__last)
			{
				if (__binary_pred(*__first,*__next))
				{
					erase(__next);
				}
				else
				{
					__first = __next;
				}
				__next = __first;
			}
		}

		template<typename _StrictWeakOrdering>
		void merge(list<_Tp,_Alloc>& __x,_StrictWeakOrdering __comp)
		{
			iterator __first1 = begin();
			iterator __last1 = end();
			iterator __first2 = __x.begin();
			iterator __last2 = __x.end();
			while (__first1 != __last1 && __first2 != __last2)
				if (__comp(*__first2, *__first1)) {
					iterator __next = __first2;
					transfer(__first1, __first2, ++__next);
					__first2 = __next;
				}
				else
					++__first1;
			if (__first2 != __last2) transfer(__last1, __first2, __last2);
		}

		template<typename _StrictWeakOrdering>
		void sort(_StrictWeakOrdering __comp)
		{
			//	do nothing if the last has length 0 or 1
			if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node)
			{
				list<_Tp,_Alloc>	__carry;
				list<_Tp,_Alloc>	__counter[64];
				int __fill = 0;
				while (!empty())
				{
					__carry.splice(__carry.begin(),*this,begin());
					int __i = 0;
					while (__i < __fill && !__counter[__i].empty())
					{
						__counter[__i].merge(__carry,__comp);
						__carry.swap(__counter[__i++]);
					}
					__carry.swap(__counter[__i]);
					if (__i == __fill)
					{
						++__fill;
					}
				}
				for(int __i = 1; __i < __fill; ++__i)
				{
					__counter[__i].merge(__counter[__i-1],__comp);
				}
				swap(__counter[__fill-1]);
			}
		}

	protected:
		void transfer(iterator __position, iterator __first, iterator __last) 
		{
			if (__position != __last)
			{
				//	remove [first,last) from its old position
				__last._M_node->_M_prev->_M_next = __position._M_node;
				__first._M_node->_M_prev->_M_next = __last._M_node;
				__position._M_node->_M_prev->_M_next = __first._M_node;
				//	splice [first,last) info its new position
				_List_node_base* __tmp = __position._M_node->_M_prev;
				__position._M_node->_M_prev = __last._M_node->_M_prev;
				__last._M_node->_M_prev = __first._M_node->_M_prev;
				__first._M_node->_M_prev = __tmp;
			}
		}
	private:
		void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
		{
			for ( ; __n > 0; --__n)
				insert(__pos, __x);
		}

		// Check whether it's an integral type.  If so, it's not an iterator.
		template<typename _Integer>
		void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
			__true_type) {
				_M_fill_insert(__pos, (size_type) __n, (_Tp) __x);
		}

		template <typename _InputIterator>
		void _M_insert_dispatch(iterator __pos,
			_InputIterator __first, _InputIterator __last,
			__false_type)
		{
			for ( ; __first != __last; ++__first)
				insert(__pos, *__first);
		}

		void _M_fill_assign(size_type __n, const _Tp& __val)
		{
			iterator __i = begin();
			for (; __i != end() && __n> 0; ++__i,--__n)
			{
				*__i = __val;
			}
			if (__n > 0)
			{
				insert(end(),__n,__val);
			}
			else
			{
				erase(__i,end());
			}
		}

		template<typename _Integer>
		void _M_assign_dispatch(_Integer __n,_Integer __val,__true_type)
		{
			_M_fill_assign((size_type)__n,(_Tp)__val);
		}

		template<typename _InputIterator>
		void _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
			__false_type)
		{
			iterator __first1 = begin();
			iterator __last1 = end();
			for (; __first1 != __last1 && __first != __last; ++__first1,++__first)
			{
				*__first1 = *__first;
				if (__first == __last)
				{
					erase(__first1,__last1);
				}
				else
				{
					insert(__last1,__first,__last);
				}
			}
		}
	};

	inline void __list_base_reverse(_List_node_base* __p)
	{
		_List_node_base* __tmp = __p;
		do 
		{
			easy::swap(__tmp->_M_next,__tmp->_M_prev);
			__tmp = __tmp->_M_next;
		} while (__tmp != __p);
	}

	template<typename _Tp,class _Alloc>
	inline void list<_Tp,_Alloc>::reverse() { __list_base_reverse(this->_M_node);}

	template<typename _Tp,typename _Alloc>
	inline bool operator == (const list<_Tp,_Alloc>& __x,const list<_Tp,_Alloc>& __y)
	{
		typedef typename list<_Tp,_Alloc>::const_iterator	const_iterator;
		const_iterator	__end1 = __x.end();
		const_iterator	__end2 = __y.end();
		const_iterator	__i1 = __x.begin();
		const_iterator	__i2 = __y.end();
		while(__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2)
		{
			++__i1;
			++__i2;
		}
		return __i1 == __end1 && __i2 == __end2;
	}

	template <typename _Tp, typename _Alloc>
	inline bool operator<(const list<_Tp,_Alloc>& __x,
		const list<_Tp,_Alloc>& __y)
	{
		return lexicographical_compare(__x.begin(), __x.end(),
			__y.begin(), __y.end());
	}

	template <typename _Tp, typename _Alloc>
	inline bool operator!=(const list<_Tp,_Alloc>& __x,
		const list<_Tp,_Alloc>& __y) 
	{
		return !(__x == __y);
	}

	template <typename _Tp, typename _Alloc>
	inline bool operator>(const list<_Tp,_Alloc>& __x,
		const list<_Tp,_Alloc>& __y) 
	{
		return __y < __x;
	}

	template <typename _Tp, typename _Alloc>
	inline bool operator<=(const list<_Tp,_Alloc>& __x,
		const list<_Tp,_Alloc>& __y) 
	{
		return !(__y < __x);
	}

	template <typename _Tp, typename _Alloc>
	inline bool operator>=(const list<_Tp,_Alloc>& __x,
		const list<_Tp,_Alloc>& __y) 
	{
		return !(__x < __y);
	}

	template <typename _Tp, typename _Alloc>
	inline void 
		swap(list<_Tp, _Alloc>& __x, list<_Tp, _Alloc>& __y)
	{
		__x.swap(__y);
	}
}
#endif // easy_list_h__