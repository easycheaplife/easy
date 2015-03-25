/****************************************************************************
 Copyright (c) 2013-2014 King Lee

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef easy_ring_buffer_h__
#define easy_ring_buffer_h__
/************************************************************************/
/*  
 *  a ring buffer to work with network buffer cache ,usually it use at read/write thread model, that is to say, one thread write data to buffer, 
 *  another thread read data from buffer. when write data to the end of buffer, write from head of buffer again.
 *  
 *  
 *  bugs:
 *  #20001	2014-11-19 
 *  when reading from buffer,wpos_ changed at another thread,but is as a factor for read,that will cause a overflow. 
 *  #20002  2014-12-05
 *	data coverage:
 *		
 *		rpos_
 *		wpos_
 *		^
 *	(1)
 *		|---------------------------------------------------------|(size)
 *	
 *		rpos_									wpos_
 *		^										  ^
 *	(2)	
 *		|---------------------------------------------------------|(size)	
 *	
 *							   rpos_							wpos_
 *								 ^								  ^
 *	(3)	
 *		|---------------------------------------------------------|(size)

 *				  wpos_							rpos_
 *					^							  ^
 *	(4)	
 *		|---------------------------------------------------------|(size)
 *							   wpos_
 *							   rpos_
 *								 ^          
 *	(5)	
 *		|---------------------------------------------------------|(size)
 *	
 *							   rpos_				  wpos_(data coverage)
 *								 ^						^
 *	(6)	
 *		|---------------------------------------------------------|(size)
 *	
 *	solution:	record the bytes to be read,incr and desc the count of bytes,when writing data,copy the the nmmber of data
 *	which less than size_ - bytes_,it can avoid data coverage,that happened at the write is quickly fast than read.
 *	reference http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html
 *	#20007	2015-3-10
 *	when append function called and the buffer is not enough, reallocate will be called, it maybe some problem at multi thread: 
 *	a thread calls reallocate but not execute finished, but b thread get the time slice, the data maybe not completely or is old buffer. 
 *
 */
/************************************************************************/
#include <string>
#include <string.h>
#include <iostream>
#include "easy_lock.h"
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

namespace easy
{
	template<class _Type,class _Alloc,class _Lock >
	class EasyRingbuffer 
	{
	public:
		typedef _Alloc allocator_type;
		typedef _Lock lock;

		explicit  EasyRingbuffer(size_t size):
            bytes_(0),
            size_(size),
			wpos_(0),
			rpos_(0),
			lock_flag_(0)
			
		{
			buffer_ = _allocate(size_);
		}

		void reset()
		{
			wpos_ = 0;
			rpos_ = 0;
			bytes_ = 0;
			lock_flag_.decr();
		}

		~EasyRingbuffer() { _deallocate(buffer_,size_); }

		template<typename T> void append(T val) 
		{ 
			append((easy_uint8*)&val,sizeof(val));
#ifdef _DEBUG
			std::cout << "append---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
#endif // _DEBUG
		}

		void append(const easy_uint8* src, size_t cnt,easy_bool __debug = false)
		{
			if (!cnt)
			{
				return;
			}

			//	case 1: rpos_ <= wpos_
			if (rpos_ <= wpos_)
			{
				if (size_ - wpos_ >= cnt)
				{
					memmove(buffer_ + wpos_,src,cnt);
					wpos_ += cnt;
					if(__debug)
					{
						printf("#1\n");
					}
				}
				else
				{
					if (size_ - wpos_ + rpos_ > cnt)	// do not >= , reserev at lest on byte avoid data coveage!
					{
						size_t __buf_wpos_tail_left = size_ - wpos_;
						memmove(buffer_ + wpos_, src, __buf_wpos_tail_left);
						memmove(buffer_, src + size_ - wpos_, cnt - __buf_wpos_tail_left);
						wpos_ = cnt - __buf_wpos_tail_left;
						if(__debug)
						{
							printf("#2\n");
						}
					}
					else
					{
						size_t __new_size = size_ + cnt - (size_ - wpos_);
						reallocate(__new_size,true);
						memmove(buffer_ + wpos_,src,cnt);
						wpos_ += cnt;
						if(__debug)
						{
							printf("#3\n");
						}
					}
				}
			}
			//	case 2: rpos_ > wpos_ 
			else if(rpos_ > wpos_)
			{
				if (rpos_ - wpos_ <= cnt)	// (rpos_ - wpos_ > cnt)  do not >= , reserev at lest on byte avoid data coveage!
				{
					easy_uint32 __new_size = size_ * 2;
					if ( __new_size <= cnt)
					{
						__new_size = size_ * 2 + cnt;
					}
					reallocate(__new_size,true);
					if(__debug)
					{
						printf("#4\n");
					}
				}
				memmove(buffer_ + wpos_,src,cnt);
				wpos_ += cnt;
				if(__debug)
				{
					printf("#5\n");
				}
			}
		}
		
		void reallocate(size_t __extra_buffer_size,easy_bool __debug = false)
		{
			lock_flag_.incr();
			buf_lock_.acquire_lock();
			size_t __new_size = size_ + __extra_buffer_size;
			_Type* new_buffer = _allocate(__new_size);
			//	case 1: rpos_ <= wpos_
			if (rpos_ <= wpos_)
			{
				size_t __actually_size = wpos_ - rpos_;
				memmove(new_buffer,buffer_ + rpos_,__actually_size);
				wpos_ = __actually_size;
			}
			//	case 2: rpos_ > wpos_ 
			else if(rpos_ > wpos_)
			{
				size_t __tail_not_read_size = size_ - rpos_; 
				if(0 != __tail_not_read_size)
				{
					memmove(new_buffer,buffer_ + rpos_,__tail_not_read_size);
				}
				size_t __head_not_read_size = wpos_; 
				if (0 != __head_not_read_size)
				{
					memmove(new_buffer + __tail_not_read_size,buffer_,__head_not_read_size);
				}
				wpos_ = __tail_not_read_size + __head_not_read_size;
			}
			rpos_ = 0;
			_deallocate(buffer_,size_);
			size_ = __new_size;
			buffer_ = new_buffer;
			if(__debug)
			{
				std::cout << "reallocate---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
			}
			buf_lock_.release_lock();
			lock_flag_.decr();
		}

		EasyRingbuffer& operator << (easy_bool val)
		{
			append<easy_bool>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint8 val)
		{
			append<easy_uint8>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint16 val)
		{
			append<easy_uint16>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint32 val)
		{
			append<easy_uint32>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_uint64 val)
		{
			append<easy_uint64>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int8 val)
		{
			append<easy_int8>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int16 val)
		{
			append<easy_int16>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int32 val)
		{
			append<easy_int32>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_int64 val)
		{
			append<easy_int64>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_float val)
		{
			append<easy_float>(val);
			return *this;
		}

		EasyRingbuffer& operator << (easy_double val)
		{
			append<easy_double>(val);
			return *this;
		}

		EasyRingbuffer& operator << (const std::string& val)
		{
			append((easy_uint8 const*)val.c_str(),val.length());
			return *this;
		}

		EasyRingbuffer& operator << (const char* val)
		{
			append((easy_uint8 const *)val, val ? strlen(val) : 0);
			return *this;
		}

		template<typename T> T read() 
		{
			T r;
			read((easy_uint8*)&r,sizeof(T));
#ifdef _DEBUG
			std::cout << "read---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
#endif // _DEBUG
			return r;
		}

		easy_bool peek(easy_uint8* des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
					//	fix bug #20001:
					if((rpos_ + len) > size_)
					{
						return false;
					}
					memmove(des,buffer_ + rpos_,len);
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ >= wpos_)
			{
				if (size_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
				}
				else
				{
					//	is enough
					if(size_ - rpos_ + wpos_ >= len)
					{
						memmove(des,buffer_ + rpos_, size_ - rpos_);
						memmove(des + size_ - rpos_, buffer_, len - (size_ - rpos_));
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}

		easy_bool peek(std::string& des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
					//	fix bug #20001:
					if((rpos_ + len) > size_)
					{
						return false;
					}
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					des.insert(0,(const char*)buffer_ + rpos_,len);
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ >= wpos_)
			{
				if (size_ - rpos_ >= len)
				{
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					des.insert(0,(const char*)buffer_ + rpos_,len);
				}
				else
				{
					//	is enough
					if(size_ - rpos_ + wpos_ >= len)
					{
#if 0
						memmove(des,buffer_ + rpos_, size_ - rpos_);
						memmove(des + size_ - rpos_, buffer_, len - (size_ - rpos_));
#endif
						des.insert(0,(const char*)buffer_ + rpos_,size_ - rpos_);
						des.insert(size_ - rpos_,(const char*)buffer_,len - (size_ - rpos_));
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}

		easy_bool read(easy_uint8* des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				/*	
				fix bug #20001:
					for two thread,include read/write thread, the value of wpos_ maybe changed, you should consider this case:
					(rpos_ < wpos_) is true, (wpos_ - rpos_ >= len) is false, if wpos_ 's value is not changed, it ok,just return false;
					(rpos_ < wpos_) is true, (wpos_ - rpos_ >= len) is true because of wpos_ 's value is changed, it dangerous! when memmove called,(rpos_ + len) is more than size_, overflow will happend.
				*/
				if (wpos_ - rpos_ >= len)
				{
					if((rpos_ + len) > size_)
					{
						return false;
					}
					memmove(des,buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					return false;
				}
				
			}
			else if (rpos_ >= wpos_)
			{
				if (size_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					//	is enough
					if(size_ - rpos_ + wpos_ >= len)
					{
						memmove(des,buffer_ + rpos_, size_ - rpos_);
						memmove(des + size_ - rpos_, buffer_, len - (size_ - rpos_));
						rpos_ = len - (size_ - rpos_);
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}

		easy_bool read(std::string& des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
					//	fix bug #20001:
					if((rpos_ + len) > size_)
					{
						return false;
					}
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					des.insert(0,(const char*)buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ >= wpos_)
			{
				if (size_ - rpos_ >= len)
				{
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					des.insert(0,(const char*)buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					//	is enough
					if(size_ - rpos_ + wpos_ >= len)
					{
#if 0
						memmove(des,buffer_ + rpos_, size_ - rpos_);
						memmove(des + size_ - rpos_, buffer_, len - (size_ - rpos_));
#endif
						des.insert(0,(const char*)buffer_ + rpos_,size_ - rpos_);
						des.insert(size_ - rpos_,(const char*)buffer_,len - (size_ - rpos_));
						rpos_ = len - (size_ - rpos_);
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}

		void pack(easy_uint8* __des,easy_uint32& __length)
		{
			if (wpos_ > rpos_)
			{
				memmove(__des,buffer_ + rpos_,wpos_ - rpos_);
				__length = wpos_ - rpos_;
			}
			else if (wpos_ < rpos_)
			{
				memmove(__des,buffer_ + rpos_,size_ - rpos_);
				memmove(__des + size_ - rpos_,buffer_,wpos_);
				__length = size_ - rpos_ + wpos_;
			}
			rpos_ = wpos_;
		}

		EasyRingbuffer& operator >> (easy_bool& val)
		{
			val = read<easy_bool>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint8& val)
		{
			val = read<easy_uint8>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint16& val)
		{
			val = read<easy_uint16>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint32& val)
		{
			val = read<easy_uint32>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_uint64& val)
		{
			val = read<easy_uint64>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int8& val)
		{
			val = read<easy_int8>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int16& val)
		{
			val = read<easy_int16>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int32& val)
		{
			val = read<easy_int32>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_int64& val)
		{
			val = read<easy_int64>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_float& val)
		{
			val = read<easy_float>();
			return *this;
		}

		EasyRingbuffer& operator >> (easy_double& val)
		{
			val = read<easy_double>();
			return *this;
		}

		size_t size() const { return size_; }

		size_t rpos() const { return rpos_; }

		size_t wpos() const { return wpos_; }

		size_t bytes() const { return bytes_;}

		easy_bool read_finish() 
		{ 
			return wpos_ == rpos_; 
		}

		easy_bool write_full() { return wpos_ == rpos_ && bytes_ >= size_; }
		
	public:
		//	be careful to use,for special uses !
		_Type* buffer() {return buffer_; }

		void set_rpos(size_t __rpos) 
		{ 
			rpos_ = __rpos; 
		}

		void set_wpos(size_t __wpos) 
		{ 
			wpos_ = __wpos; 
		}

		//	fix bug #20002:
		void set_bytes(size_t __bytes) 
		{
			bytes_lock_.acquire_lock();
			bytes_ = __bytes; 
			bytes_lock_.release_lock();
		}

		void increase_bytes(size_t __bytes) 
		{ 
			bytes_lock_.acquire_lock();
			bytes_ += __bytes; 
			bytes_lock_.release_lock();
		}

		void decrease_bytes(size_t __bytes)
		{ 
			bytes_lock_.acquire_lock();
			bytes_ -= __bytes;
			bytes_lock_.release_lock();
		}

		easy_bool	is_lock() const { return lock_flag_.ref_count_; }

	private:
		_Type* _allocate(size_t size) 
		{ 
			_Type* res = 0;
			res = static_cast<_Type*>(alloc_type_.allocate(size)); 
			return res;
		}

		void _deallocate(void* p,size_t size) 
		{ 
			alloc_type_.deallocate(p,size); 
		}

		void _reallocate(void* p,size_t old_size,size_t new_size) { alloc_type_.reallocate(p,old_size,new_size); }

	private:
		EasyRingbuffer ( const EasyRingbuffer& );
		EasyRingbuffer& operator = ( const EasyRingbuffer& );
	private:
		//	fix bug #20002:
		easy_int32		bytes_;		//	factually, is the total byte to be read.

		size_t			size_;		//	factually,buffer 's size.

		_Type*			buffer_;

		size_t			wpos_;

		size_t			rpos_;

		allocator_type	alloc_type_;

		//				maybe useless lock_flag_ is used
		lock			bytes_lock_;

		lock			buf_lock_;

		//	fix bug #20007, set the flag is true when the buffer is reallocate at one thread, when thread switchs, the other thread check the flag,
		//	if the flag is true, do nothing until the flag is reset. it looks like a spin lock.
		easy::_Refcount_Base		lock_flag_;
	};
}

#endif // easy_ring_buffer_h__