#ifndef H_ARR
#define H_ARR

#include <cstdint>
#include <cstring>

namespace arr
{
	template<typename T>
	void xor(T* a, const T* b, std::size_t sz)
	{
		while(sz--)
		{
			a[sz] ^= b[sz];
		}
	}
	template<typename T>
	void add(T* a, const T* b, std::size_t sz)
	{
		while(sz--)
		{
			a[sz] += b[sz];
		}
	}
	template<typename T>
	void sub(T* a, const T* b, std::size_t sz)
	{
		while(sz--)
		{
			a[sz] -= b[sz];
		}
	}
}

template<uint8_t SZ>
class byteProcBuf
{
	uint8_t offset;
protected:
	uint8_t buf[SZ];

	byteProcBuf() : offset(0) {}

	void reset()
	{
		offset = 0;
	}

	virtual void gen() = 0;

	virtual void post_proc(uint8_t* v, uint8_t* b, std::size_t sz) const
	{
		arr::xor(v, b, sz);
	}
public:
	void process(uint8_t* v, std::size_t n)
	{
		if(offset != 0)
		{
			const uint8_t k = SZ - offset;
			if(n <= k)
			{
				post_proc(v, buf + offset, n);
				offset = n < k ? static_cast<uint8_t>(offset + n) : 0;
				return;
			}
			post_proc(v, buf + offset, k);
			v += k;
			n -= k;
		}
		while(n >= SZ)
		{
			gen();
			post_proc(v, buf, SZ);
			v += SZ;
			n -= SZ;
		}
		offset = static_cast<uint8_t>(n);
		if(n != 0)
		{
			gen();
			post_proc(v, buf, n);
		}
	}
};

#endif
