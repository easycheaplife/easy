/********************************************************************
created:	2011/11/21
created:	21:11:2011   9:49
file base:	easy_ring_buffer
file ext:	h
author:		Lee

purpose:	a ring buffer 
*********************************************************************/
#ifndef easy_ring_buffer_h__
#define easy_ring_buffer_h__
#include <string>
#include <string.h>
#if 0
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__
#else
#include "easy_assist.h"
#endif

namespace easy
{
	template<class _Type,class _Alloc >
	class EasyRingbuffer 
	{
	public:
		typedef _Alloc allocator_type;

		explicit  EasyRingbuffer(size_t size):
		size_(size),
			wpos_(0),
			rpos_(0)
		{
			buffer_ = _allocate(size_);
		}

		void reset()
		{
			wpos_ = 0;
			rpos_ = 0;
		}

		~EasyRingbuffer() { _deallocate(buffer_,size_); }

		template<typename T> void append(T val) 
		{ 
			append((easy_uint8*)&val,sizeof(val));
#ifdef _DEBUG
			std::cout << "append---buffer size = " << size_ << " rpos_ = " << rpos_ << " wpos_ = " << wpos_ << std::endl;
#endif // _DEBUG
		}

		void append(const easy_uint8* src, size_t cnt)
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
				}
				else
				{
					if (size_ - wpos_ + rpos_ > cnt)	// >= is ok>
					{
						memmove(buffer_ + wpos_, src, size_ - wpos_);
						memmove(buffer_, src + size_ - wpos_, cnt - (size_ - wpos_));
						wpos_ = cnt - (size_ - wpos_);
					}
					else
					{
						_Type* new_buffer = _allocate(size_ + cnt - (size_ - wpos_));
						memmove(new_buffer,buffer_,wpos_);
						memmove(new_buffer + wpos_, src, cnt);
						_deallocate(buffer_,size_);
						size_ = size_ + cnt - (size_ - wpos_);
						wpos_ += cnt;
						buffer_ = new_buffer;
					}
				}
			}
			//	case 2: rpos_ > wpos_ 
			else if(rpos_ > wpos_)
			{
				if (rpos_ - wpos_ >= cnt)	
				{
					memmove(buffer_ + wpos_,src,cnt);
					wpos_ += cnt;
				}
				else
				{
					reallocate(size_);
					memmove(buffer_ + wpos_,src,cnt);
					wpos_ += cnt;
				}
			}
		}
		
		void reallocate(size_t __extra_buffer_size)
		{
			_Type* new_buffer = _allocate(size_ + __extra_buffer_size);
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
				memmove(new_buffer,buffer_ + rpos_,__tail_not_read_size);
				size_t __head_not_read_size = wpos_; 
				memmove(new_buffer + __tail_not_read_size,buffer_,__head_not_read_size);
				wpos_ = __tail_not_read_size + __head_not_read_size;
			}
			rpos_ = 0;
			_deallocate(buffer_,size_);
			size_ = size_ + __extra_buffer_size;
			buffer_ = new_buffer;			
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

		easy_bool pre_read(easy_uint8* des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ > wpos_)
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

		easy_bool pre_read(std::string& des,size_t len)
		{
			if (read_finish())
			{
				return false;
			}
			if (rpos_ < wpos_)
			{
				if (wpos_ - rpos_ >= len)
				{
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					for(int __i = 0; __i < len; ++__i)
					{
						des += (buffer_ + rpos_)[__i];
					}
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ > wpos_)
			{
				if (size_ - rpos_ >= len)
				{
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					for(int __i = 0; __i < len; ++__i)
					{
						des += (buffer_ + rpos_)[__i];
					}
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
						for(size_t __i = 0; __i < size_ - rpos_; ++__i)
						{
							des += (buffer_ + rpos_)[__i];
						}
						for(size_t __j = 0; __j < len - (size_ - rpos_); ++__j)
						{
							des += (buffer_)[__j];
						}
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
				if (wpos_ - rpos_ >= len)
				{
					memmove(des,buffer_ + rpos_,len);
					rpos_ += len;
				}
				else
				{
					return false;
				}
				
			}
			else if (rpos_ > wpos_)
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
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					for(int __i = 0; __i < len; ++__i)
					{
						des += (buffer_ + rpos_)[__i];
					}
					rpos_ += len;
				}
				else
				{
					return false;
				}

			}
			else if (rpos_ > wpos_)
			{
				if (size_ - rpos_ >= len)
				{
#if 0
					memmove(des,buffer_ + rpos_,len);
#endif
					for(int __i = 0; __i < len; ++__i)
					{
						des += (buffer_ + rpos_)[__i];
					}
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
						for(size_t __i = 0; __i < size_ - rpos_; ++__i)
						{
							des += (buffer_ + rpos_)[__i];
						}
						for(size_t __j = 0; __j < len - (size_ - rpos_); ++__j)
						{
							des += (buffer_)[__j];
						}
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

		easy_bool read_finish() { return wpos_ == rpos_; }
		
	public:
		//	be careful to use,for special uses !
		_Type* buffer() {return buffer_; }

		void set_rpos(size_t __rpos) { rpos_ = __rpos; }

		void set_wpos(size_t __wpos) { wpos_ = __wpos; }

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
		size_t			size_;

		_Type*			buffer_;

		size_t			wpos_;

		size_t			rpos_;

		allocator_type	alloc_type_;
	};
}

#endif // easy_ring_buffer_h__