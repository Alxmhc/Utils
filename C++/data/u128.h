#ifndef H_U128
#define H_U128

#include "pack.h"

template<> struct UINT_<16>
{
	class uint
	{
		UINT_<8>::uint l, h;
	public:
		explicit uint(UINT_<8>::uint ln = 0, UINT_<8>::uint hn = 0) : l(ln), h(hn) {}

		uint& operator=(UINT_<8>::uint c)
		{
			l = c;
			h = 0;
			return *this;
		}
		uint& operator=(const uint &c)
		{
			l = c.l;
			h = c.h;
			return *this;
		}

		template <unsigned char SZ, char E>
		void pack(const typename UINT_<SZ>::uint* a)
		{
			bconv<SZ, E>::pack(a + 8 / SZ, 8 / SZ, h);
			bconv<SZ, E>::pack(a, 8 / SZ, l);
			if (E == endianness::BIG_ENDIAN)
			{
				std::swap(l, h);
			}
		}
		template <unsigned char SZ, char E>
		void unpack(typename UINT_<SZ>::uint* a) const
		{
			UINT_<8>::uint l1 = l;
			UINT_<8>::uint h1 = h;
			if (E == endianness::BIG_ENDIAN)
			{
				std::swap(l1, h1);
			}
			bconv<SZ, E>::unpack(l1, 8 / SZ, a);
			bconv<SZ, E>::unpack(h1, 8 / SZ, a + 8 / SZ);
		}

		UINT_<8>::uint getL() const
		{
			return l;
		}
		UINT_<8>::uint getH() const
		{
			return h;
		}

		const uint& operator+=(UINT_<8>::uint c)
		{
			l += c;
			if(l < c)
			{
				h++;
			}
			return *this;
		}

		const uint& operator|=(UINT_<8>::uint c)
		{
			l |= c;
			return *this;
		}
		const uint& operator|=(const uint &c)
		{
			l |= c.l;
			h |= c.h;
			return *this;
		}

		const uint& operator^=(const uint &c)
		{
			l ^= c.l;
			h ^= c.h;
			return *this;
		}

		const uint& operator>>=(uint_fast8_t n)
		{
			if(n < 64)
			{
				l = (l >> n) | (h << (64 - n));
				h >>= n;
			}
			else
			{
				l = n < 128 ? h >> (n - 64) : 0;
				h = 0;
			}
			return *this;
		}
		const uint& operator<<=(uint_fast8_t n)
		{
			if(n < 64)
			{
				h = (h << n) | (l >> (64 - n));
				l <<= n;
			}
			else
			{
				h = n < 128 ? l << (n - 64) : 0;
				l = 0;
			}
			return *this;
		}

		uint operator^(const uint &c) const
		{
			uint r(*this);
			return r ^= c;
		}
	};
};
static UINT_<16>::uint rotl(UINT_<16>::uint x, unsigned char d)
{
	d %= 128;
	auto l = x.getL();
	auto h = x.getH();
	if(d >= 64)
	{
		std::swap(l, h);
		d -= 64;
	}
	if (d == 0)
		return UINT_<16>::uint(l, h);
	return UINT_<16>::uint((l << d) | (h >> (64 - d)), (h << d) | (l >> (64 - d)));
}
static UINT_<16>::uint rotr(UINT_<16>::uint x, unsigned char d)
{
	d %= 128;
	auto l = x.getL();
	auto h = x.getH();
	if(d >= 64)
	{
		std::swap(l, h);
		d -= 64;
	}
	if (d == 0)
		return UINT_<16>::uint(l, h);
	return UINT_<16>::uint((l >> d) | (h << (64 - d)), (h >> d) | (l << (64 - d)));
}

#endif
