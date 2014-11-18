
/********************************************************************
	created:	2012/02/03
	created:	3:2:2012   9:44
	file base:	easy_timer
	file ext:	h
	author:		Lee
	
	purpose:	design a timer using the theory of water meter
*********************************************************************/
#ifndef easy_timer_h__
#define easy_timer_h__
#include "easy_allocator.h"
#include "easy_time.h"

#define  ListProp(g, tv,n,s)\
	g[n-1] = tv##n##s

/// define m for timer
#define lnum		5
#define sbits		6
#define ebits		8
#define sbitsize	( 1 << sbits )	//	64	
#define ebitsize	( 1 << ebits )	//	256
#define smask		( sbitsize- 1)		
#define emask		( ebitsize -1)	

enum time_type
{
	kTimerTypeBegin,
	kTimerTypeTick,
	kTimerTypeEnd
};

namespace easy
{
	struct list_node
	{
		list_node* next_;
		list_node* prev_;
	};

	class link_list
	{
	public:
		link_list():list_(NULL),index_(0),size_(0){}

		explicit link_list(size_t __siez) : size_(__siez),index_(0)
		{
			list_ = new list_node[__siez];
		}

		void init() 
		{
			for (int __i = 0; __i < size_; ++__i)
			{
				_init_empty(__i);
			}
		}
		//	insert value __p between __prev and __next
		void insert(list_node* __node,list_node* __prev,list_node* __next)
		{
			__next->prev_ = __node;
			__node->next_ = __next;
			__node->prev_ = __prev;
			__prev->next_ = __node;
		}
		
		void insert_head(list_node* __node,list_node* __head)
		{
			insert(__node,__head,__head->next_);
		}

		void insert_tail(list_node* __node,list_node* __head)
		{
			insert(__node,__head->prev_,__head);
		}

		void erase(list_node* __node)
		{
			__node->next_->prev_ = __node->prev_;
			__node->prev_->next_ = __node->next_;
		}

		list_node* node(size_t __index) const 
		{ 
			if (__index < size_)
			{
				return &list_[__index];
			}
			return NULL;
		}

		size_t index() const { return index_; }

		void set_index(size_t __index) { index_ = __index; }

		size_t size() const { return size_; }

		~link_list() { delete list_; list_ = NULL; index_ = 0; index_ = 0;}

	private:
		void _init_empty(size_t __index) 
		{ 
			list_[__index].next_ = list_[__index].prev_ = &list_[__index]; 
		}
	private:
		list_node*		list_;
		size_t			index_;
		size_t			size_;
	};

	struct timer_node : public easy::my_alloc
	{
		list_node	node_;			//	timer node
		easy_ulong	expire_;		//	timer expire,current time + n millisecond
		time_type	timer_type_;	
		void*		fun_;
	};

	class base_event : public easy::my_alloc
	{
	public:
		virtual ~base_event() { }

		virtual easy_long time_out(timer_node* __timer_node) = 0;
	};

	class timer
	{
	public:
		timer() 
		{
			wheel1_ = new link_list( ebitsize );
			wheel2_ = new link_list( sbitsize );
			wheel3_ = new link_list( sbitsize );
			wheel4_ = new link_list( sbitsize );
			wheel5_ = new link_list( sbitsize );
		}

		explicit timer(easy_uint32 __millisecond) 
		{
			wheel1_ = new link_list( ebitsize );
			wheel2_ = new link_list( sbitsize );
			wheel3_ = new link_list( sbitsize );
			wheel4_ = new link_list( sbitsize );
			wheel5_ = new link_list( sbitsize );
			init(__millisecond);
		}

		~timer()
		{
			delete wheel1_;
			delete wheel2_;
			delete wheel3_;
			delete wheel4_;
			delete wheel5_;
			wheel1_ = NULL;
			wheel2_ = NULL;
			wheel3_ = NULL;
			wheel4_ = NULL;
			wheel5_ = NULL;
			delete [] vecs_;
		}

		void init(easy_uint32 __millisecond)
		{
			jeffies_ = EasyTime::get_cur_sys_time();
			if (__millisecond > 0)
			{
				millisecond_ = __millisecond;
			}
			wheel1_->init();
			wheel2_->init();
			wheel3_->init();
			wheel4_->init();
			wheel5_->init();
			vecs_ = new link_list* [lnum];

			ListProp(vecs_,wheel,1,_);
			ListProp(vecs_,wheel,2,_);
			ListProp(vecs_,wheel,3,_);
			ListProp(vecs_,wheel,4,_);
			ListProp(vecs_,wheel,5,_);
		}

		void add_timer(timer_node* __timer_node)
		{
			if (!_check_timer(__timer_node))
			{
				return;
			}
			//	the wheel will be inserted
			link_list* __wheel = NULL;
			list_node * __cur_node = NULL;
			easy_uint32 __index = 0;
			easy_ulong __expires = (__timer_node->expire_ - jeffies_) / millisecond_;
			//	ebitsize = 256 = 2^8
			if (__expires < ebitsize)
			{
				//	the first wheel size is 256, so & eMask will 
				__index = (__expires + wheel1_->index()) & emask;
				__cur_node = wheel1_->node( __index );
				__wheel = wheel1_;
			}
			//	2^14
			else if (__expires < (1 << (sbits + ebits)))
			{
				__index = ((__expires >> ebits) + wheel2_->index()) & smask;
				__cur_node = wheel2_->node(__index);
				__wheel = wheel2_;
			}
			//	2^18
			else if (__expires < (1 << (2 * sbits + ebits)))
			{
				__index = ((__expires >> (1 * sbits + ebits)) + wheel3_->index()) & smask;
				__cur_node = wheel3_->node(__index);
				__wheel = wheel3_;

			}
			//	2^26
			else if (__expires < (1 << (3 * sbits + ebits)))
			{
				__index = ((__expires >> (2 * sbits + ebits)) + wheel4_->index()) & smask;
				__cur_node = wheel4_->node(__index);
				__wheel = wheel4_;
			}
			//	2^31
			else if (__expires < (0xffffffffUL >> 1) )
			{
				__index = ((__expires >> (3 * sbits + ebits)) + wheel5_->index()) & smask;
				__cur_node = wheel5_->node(__index);
				__wheel = wheel5_;
			}
			if (__cur_node && __wheel)
			{
				__wheel->insert_tail(&__timer_node->node_,__cur_node);
			}
		}

		easy_bool delete_timer(timer_node* __timer_node)
		{
			if (!_check_timer(__timer_node))
			{
				return false;
			}
			wheel1_->erase(&__timer_node->node_);
			_init_timer(__timer_node);
			return true;
		}

		void expires()
		{
			easy_uint32 __jeffies = easy::EasyTime::get_cur_sys_time();
			easy_int32 __count = (__jeffies - jeffies_) / millisecond_;
			easy_int32 __count_temp = __count;
			while(__count >= 0)
			{
				list_node* __head = NULL;
				list_node* __cur = NULL;
				list_node* __next = NULL;
				__head = wheel1_->node(wheel1_->index());
				__cur = __head->next_;
				while(__cur!= __head)
				{
					timer_node* __timer_node = NULL;
					__next = __cur->next_;
					__timer_node = (timer_node*)( (char*)__cur - offsetof(timer_node,node_));
					void* __fun = __timer_node->fun_;
					static_cast<base_event*>(__fun)->time_out(__timer_node);
					delete_timer(__timer_node);
					if (__timer_node->expire_ > 0)
					{
						delete __timer_node;
					}
					__cur = __next;
				}
				--__count;
				//	record the current tick time,and set the wheel index
				if (jeffies_ + millisecond_ <= __jeffies && __count_temp > 0)
				{
					jeffies_ += millisecond_;
					wheel1_->set_index((wheel1_->index() + 1) & emask);
					//	after rotate a circle 
					if (0 == wheel1_->index())
					{
						easy_uint32 __n = 1;
						do 
						{
							_cascade_timer(vecs_[__n]);
						} while (vecs_[__n]->index() == 0 && ++__n < lnum);
					}
				}
			}
		}
	private:

		void _init_timer(timer_node* __timer_node)
		{
			__timer_node->node_.next_ = __timer_node->node_.prev_ = NULL;
		}

		easy_bool _check_timer(timer_node* __timer_node)
		{
			return __timer_node->node_.next_ != NULL;
		}

		void _mod_timer(timer_node* __timer_node)
		{
			if (__timer_node->expire_ < jeffies_)
			{
				__timer_node->expire_ = jeffies_;
			}
			add_timer(__timer_node);
		}

		void _cascade_timer(link_list* __list)
		{
			list_node* __head = NULL;
			list_node* __cur = NULL;
			list_node* __next = NULL;
			__head = __list->node(__list->index());
			__cur = __head->next_;
			while(__cur != __head)
			{
				//	offsetof Retrieves the offset of a member from the beginning of its parent structure.
				timer_node* __timer_node = (timer_node*)( (easy_char*)__cur - offsetof(timer_node,node_));
				__list->erase(__cur);
				_mod_timer(__timer_node);
				__cur = __next;
			}
		}
	private:
		//	five wheel
		link_list*	wheel1_;
		link_list*	wheel2_;
		link_list*	wheel3_;
		link_list*	wheel4_;
		link_list*	wheel5_;
		link_list** vecs_;
		//	global tick
		easy_ulong	jeffies_;
		//	timer precision
		easy_ulong	millisecond_;
	};
}
#endif // easy_timer_h__