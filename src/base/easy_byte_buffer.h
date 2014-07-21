/********************************************************************
	created:	2011/11/19
	created:	19:11:2011   19:58
	file base:	easy_byte_buffer
	file ext:	h
	author:		Lee
	
	purpose:	a fixed buffer
*********************************************************************/
#ifndef easy_byte_buffer_h__
#define easy_byte_buffer_h__
#include <vector>
#include <list>
#include <map>
#include <string>
#include <assert.h>
#include <stdio.h>

#if 0
#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__
#else
#include "easy_assist.h"
#endif

#define MAX_PACKET_LENGTH		1024*1024*8
namespace easy
{
	class EasyByteBufferException
	{
	public:
		EasyByteBufferException(easy_bool add, size_t pos, size_t esize, size_t size)
			:add_(add),pos_(pos),esize_(esize),size_(size)
		{
			PrintfPosError();
		}

		void PrintfPosError() const 
		{
				printf("Attempted to %s in EasyByteBuffer (pos: " "%lu" " size_: " "%lu" ") value with size: " "%lu",(add_ ? "put" : "get"), \
				pos_, size_, esize_);
		}
	private:
		easy_bool	add_;
		size_t		pos_;
		size_t		esize_;		//the real size will to read or write
		size_t		size_;

	};

	class EasyByteBuffer
	{
	public:
		const static size_t DEFAULT_SIZE = 0x1000;

		/*
		 *	construct function, reserve DEFAULT_SIZE bytes
		 */
		EasyByteBuffer() : rpos_(0),wpos_(0)
		{
			storage_.reserve(DEFAULT_SIZE);
		}

		/*
		 *	construct function, reserve res bytes
		 */
		EasyByteBuffer(size_t res) : rpos_(0),wpos_(0)
		{
			storage_.reserve(res);
		}

		/*
		 *	copy construct function
		 */
		EasyByteBuffer(const EasyByteBuffer& buffer) : rpos_(buffer.rpos_), wpos_(buffer.wpos_),storage_(buffer.storage_) {}
	
		/*
		 *	clear data
		 */
		void clear()
		{
			rpos_ = wpos_ = 0;
			storage_.clear();
			std::vector<easy_uint8>().swap(storage_);
		}

		/*
		 *	append any type data to buffer
		 */
		template<typename T> void append(T val)
		{
			append((easy_uint8*)&val,sizeof(val));
		}

		/*
		 *	append cnt bytes to buffer
		 */
		void append(const easy_uint8* src, size_t cnt)
		{
			if (!cnt)
			{
				return;
			}
			assert(size() < MAX_PACKET_LENGTH);
			if (storage_.size() < wpos_ + cnt)
			{
				storage_.resize(wpos_ + cnt);
			}
			memmove(&storage_[wpos_],src,cnt);
			wpos_ += cnt;
		}

		/*
		 *	a template function put val to buffer at the position pos,just instead of the old buffer
		 */
		template<typename T> void put(size_t pos, T val)
		{
			put(pos,(easy_uint8*)&val,sizeof(val));
		}

		/*
		 *	put cnt val to buffer,cover the old data
		 */
		void put(size_t pos, const easy_uint8* src, size_t cnt)
		{
			if (pos + cnt > size())
			{
				throw EasyByteBufferException(true,pos,cnt,size());
			}
			memmove(&storage_[pos],src,cnt);
		}

		EasyByteBuffer& operator << (easy_uint8 val)
		{
			append<easy_uint8>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_uint16 val)
		{
			append<easy_uint16>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_uint32 val)
		{
			append<easy_uint32>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_uint64 val)
		{
			append<easy_uint64>(val);
			return *this;
		}

		/*
		 *	signed as in 2e complement
		 */ 
		EasyByteBuffer& operator << (easy_int8 val)
		{
			append<easy_int8>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_int16 val)
		{
			append<easy_int16>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_int32 val)
		{
			append<easy_int32>(val);
			return *this;
		}

		EasyByteBuffer& operator << (easy_int64 val)
		{
			append<easy_int64>(val);
			return *this;
		}

		/*
		 *	float type
		 */	
		EasyByteBuffer& operator << (float val)
		{
			append<float>(val);
			return *this;
		}

		EasyByteBuffer& operator << (double val)
		{
			append<double>(val);
			return *this;
		}

		/*
		 *	string type
		 */
		EasyByteBuffer& operator << (const std::string& val)
		{
			append((easy_uint8 const*)val.c_str(),val.length());
			append((easy_uint8)0);
			return *this;
		}

		EasyByteBuffer& operator << (const easy_char* val)
		{
			append((easy_uint8 const *)val, val ?  strlen(val) : 0);
			append((easy_uint8)0);
			return *this;
		}

		EasyByteBuffer& operator >> (easy_bool& val)
		{
			val = read<easy_char>() > 0 ? true : false;
			return *this;
		}

		EasyByteBuffer& operator >> (easy_uint8& val)
		{
			val = read<easy_uint8>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_uint16& val)
		{
			val = read<easy_uint16>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_uint32& val)
		{
			val = read<easy_uint32>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_uint64& val)
		{
			val = read<easy_uint64>();
			return *this;
		}

		/*
		 *	signed as in 2e complement
		 */
		EasyByteBuffer& operator >> (easy_int8& val)
		{
			val = read<easy_int8>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_int16& val)
		{
			val = read<easy_int16>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_int32& val)
		{
			val = read<easy_int32>();
			return *this;
		}

		EasyByteBuffer& operator >> (easy_int64& val)
		{
			val = read<easy_int64>();
			return *this;
		}

		EasyByteBuffer& operator >> (float& val)
		{
			val = read<float>();
			return *this;
		}

		EasyByteBuffer& operator >> (double& val)
		{
			val = read<double>();
			return *this;
		}

		EasyByteBuffer& operator >> (std::string& val)
		{
			val.clear();
			 //		prevent crash at wrong string format in packet
			while (rpos() < size())	
			{
				easy_char c = read<easy_char>();
				if (0 == c)
				{
					break;
				}
				val += c;
			}
			return *this;
		}

		easy_uint8 operator[] (size_t pos) const
		{
			return read<easy_uint8>(pos);
		}

		/*
		 *	read a type T from buffer
		 */
		template<typename T> T read()
		{
			T r = read<T>(rpos_);
			rpos_ += sizeof(T);
			return r;
		}

		/*
		 *	read a type T from buffer at the position of pos
		 */
		template<typename T > T read(size_t pos) const
		{
			if (pos + sizeof(T) > size())
			{
				throw EasyByteBufferException(false,pos,sizeof(T),size());
			}
			T val = *((T const*)&storage_[pos]);
			return val;
		}

		/*
		 *	read len bytes data from the buffer
		 */
		void read(easy_uint8* des, size_t len)
		{
			if (rpos_ + len > size()) 
			{
				throw EasyByteBufferException(false,rpos_,len,size());
			}
			memcpy(des,&storage_[rpos_],len);
			rpos_ += len;
		}

		template<typename T> void read_skip() { return read_skip(sizeof(T));}

		void read_skip(size_t skip)
		{
			if (rpos_ + skip > size())
			{
				throw EasyByteBufferException(false,rpos_,skip,size());
			}
			rpos_ += skip;
		}

		const easy_uint8* contents() const { return &storage_[0]; }

		size_t size() const { return storage_.size();}

		easy_bool empty() const { return storage_.empty(); } 

		size_t rpos() const { return rpos_; }

		size_t rpos(size_t rpos)
		{
			rpos_ = rpos;
			return rpos_;
		}

		size_t wpos() const { return wpos_; }

		size_t wpos(size_t wpos)
		{
			wpos_ = wpos;
			return wpos_;
		}

		/*
		 *	read finish
		 */
		void rfinish()
		{
			rpos_ = wpos_;
		}

		void resize(size_t newsize)
		{
			storage_.resize(newsize);
			rpos_ = 0;
			wpos_ = size();
		}

		void reserve(size_t ressize)
		{
			if (ressize > size())
			{
				storage_.reserve(ressize);
			}
		}

	private:
		size_t					rpos_;
		size_t					wpos_;
		std::vector<easy_uint8>	storage_;
	};

	template<typename T>
	inline EasyByteBuffer& operator << (EasyByteBuffer& b, std::vector<T> v)
	{
		b << (easy_uint32)v.size();
		for (typename std::vector<T>::iterator i = v.begin(); i != v.end() ; ++i)
		{
			b << *i;
		}
		return b;
	}

	template<typename T>
	inline EasyByteBuffer& operator >> (EasyByteBuffer& b, std::vector<T> v)
	{
		easy_uint32 vsize;
		b >> vsize;
		v.clear();
		while (vsize--)
		{
			T t;
			b >> t;
			v.push_back(t);
		}
		return b;
	}

	template<typename T>
	inline EasyByteBuffer& operator << (EasyByteBuffer& b, std::list<T> v)
	{
		b << (easy_uint32)v.size();
		for (typename std::vector<T>::iterator i = v.begin(); i != v.end() ; ++i)
		{
			b << *i;
		}
		return b;
	}

	template<typename T>
	inline EasyByteBuffer& operator >> (EasyByteBuffer& b, std::list<T> v)
	{
		easy_uint32 vsize;
		b >> vsize;
		v.clear();
		while (vsize--)
		{
			T t;
			b >> t;
			v.push_back(t);
		}
		return b;
	}

	template<typename K, typename V>
	inline EasyByteBuffer& operator << (EasyByteBuffer& b,std::map<K,V>& m)
	{
		b << (easy_uint32)m.size();
		for (typename std::map<K,V>::iterator i = m.begin(); i != m.end(); ++i)
		{
			b << i->first << i->second;
		}
		return b;
	}

	template<typename K, typename V >
	inline EasyByteBuffer& operator >> (EasyByteBuffer& b,std::map<K,V>& m)
	{
		easy_uint32 msize;
		b >> msize;
		m.clear();
		while(msize--)
		{
			K k;
			V v;
			b >> k >> v;
			m.insert(make_pair(k,v));
		}
		return b;
	}

	template<> inline std::string EasyByteBuffer::read<std::string>()
	{
		std::string tmp;
		*this >> tmp;
		return tmp;
	}

	template<> inline void EasyByteBuffer::read_skip<char*>()
	{
		std::string temp;
		*this >> temp;
	}

	template<> inline void EasyByteBuffer::read_skip<char const*>()
	{
		read_skip<char*>();
	}

	template<> inline void EasyByteBuffer::read_skip<std::string>()
	{
		read_skip<char*>();
	}
}
#endif // easy_byte_buffer_h__