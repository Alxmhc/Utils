#ifndef H_PACK
#define H_PACK

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace endianness
{
	enum
	{
		UNKNOWN,
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};
	static char get_endianness()
	{
		const char a[4] = {1,2,3,4};
		uint32_t t;
		memcpy(&t, a, 4);
		if(t == 0x04030201)
			return LITTLE_ENDIAN;
		if(t == 0x01020304)
			return BIG_ENDIAN;
		return UNKNOWN;
	}
	const char current = get_endianness();
}

template<unsigned char SZ> struct UINT_{};
template<> struct UINT_<1>
{
	typedef uint8_t uint;
	typedef uint_fast8_t uint_f;
};
template<> struct UINT_<2>
{
	typedef uint16_t uint;
	typedef uint_fast16_t uint_f;
};
template<> struct UINT_<4>
{
	typedef uint32_t uint;
	typedef uint_fast32_t uint_f;
};
template<> struct UINT_<8>
{
	typedef uint64_t uint;
	typedef uint_fast64_t uint_f;
};
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

		explicit operator UINT_<8>::uint() const
		{
			return l;
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
				l = (l>>n)|(h<<(64-n));
				h >>= n;
			}
			else
			{
				l = n < 128 ? h>>(n-64) : 0;
				h = 0;
			}
			return *this;
		}
		const uint& operator<<=(uint_fast8_t n)
		{
			if(n < 64)
			{
				h = (h<<n)|(l>>(64-n));
				l <<= n;
			}
			else
			{
				h = n < 128 ? l<<(n-64) : 0;
				l = 0;
			}
			return *this;
		}

		uint operator|(const uint &c) const
		{
			uint r(*this);
			return r |= c;
		}
		uint operator^(const uint &c) const
		{
			uint r(*this);
			return r ^= c;
		}

		uint operator>>(uint_fast8_t n) const
		{
			uint r(*this);
			return r >>= n;
		}
		uint operator<<(uint_fast8_t n) const
		{
			uint r(*this);
			return r <<= n;
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

template <unsigned char SZ, unsigned char k, char E>
struct bconv{};

template <unsigned char SZ, unsigned char k>
struct bconv<SZ, k, endianness::LITTLE_ENDIAN>
{
	static typename UINT_<k*SZ>::uint pack(const typename UINT_<SZ>::uint* a)
	{
		typename UINT_<k*SZ>::uint res(bconv<SZ, k/2, endianness::LITTLE_ENDIAN>::pack(a + k/2));
		res <<= 4*k*SZ;
		res |= bconv<SZ, k/2, endianness::LITTLE_ENDIAN>::pack(a);
		return res;
	}
	static void unpack(typename UINT_<k*SZ>::uint c, typename UINT_<SZ>::uint* a)
	{
		bconv<SZ, k/2, endianness::LITTLE_ENDIAN>::unpack(static_cast<typename UINT_<k*SZ/2>::uint>(c), a);
		bconv<SZ, k/2, endianness::LITTLE_ENDIAN>::unpack(static_cast<typename UINT_<k*SZ/2>::uint>(c >> (4*k*SZ)), a + k/2);
	}
};
template<unsigned char SZ>
struct bconv<SZ, 1, endianness::LITTLE_ENDIAN>
{
	static typename UINT_<SZ>::uint pack(const typename UINT_<SZ>::uint* a)
	{
		return *a;
	}
	static void unpack(typename UINT_<SZ>::uint c, typename UINT_<SZ>::uint* a)
	{
		*a = c;
	}
};

template<unsigned char SZ, typename T>
static void packLE(const typename UINT_<SZ>::uint* a, uint_fast8_t k, T &c)
{
	c = 0;
	while(k--)
	{
		c <<= (SZ << 3);
		c |= a[k];
	}
}

template<unsigned char SZ, unsigned char k>
struct bconv<SZ, k, endianness::BIG_ENDIAN>
{
	static typename UINT_<k*SZ>::uint pack(const typename UINT_<SZ>::uint* a)
	{
		typename UINT_<k*SZ>::uint res(bconv<SZ, k/2, endianness::BIG_ENDIAN>::pack(a));
		res <<= 4*k*SZ;
		res |= bconv<SZ, k/2, endianness::BIG_ENDIAN>::pack(a + k/2);
		return res;
	}
	static void unpack(typename UINT_<k*SZ>::uint c, typename UINT_<SZ>::uint* a)
	{
		bconv<SZ, k/2, endianness::BIG_ENDIAN>::unpack(static_cast<typename UINT_<k*SZ/2>::uint>(c), a + k/2);
		bconv<SZ, k/2, endianness::BIG_ENDIAN>::unpack(static_cast<typename UINT_<k*SZ/2>::uint>(c >> (4*k*SZ)), a);
	}
};
template<unsigned char SZ>
struct bconv<SZ, 1, endianness::BIG_ENDIAN>
{
	static typename UINT_<SZ>::uint pack(const typename UINT_<SZ>::uint* a)
	{
		return *a;
	}
	static void unpack(typename UINT_<SZ>::uint c, typename UINT_<SZ>::uint* a)
	{
		*a = c;
	}
};

namespace conv
{
	template<unsigned char SZ, char E, typename m>
	void pack(const uint8_t* a, const std::size_t n, m* r)
	{
		for(std::size_t i = 0; i < n / SZ; i++)
		{
			r[i] = bconv<1, SZ, E>::pack(a + i*SZ);
		}
	}
	template<unsigned char SZ, char E, typename m>
	void unpack(const m* a, std::size_t n, uint8_t* r)
	{
		for(std::size_t i = 0; i < n; i++)
		{
			bconv<1, SZ, E>::unpack(a[i], r + i*SZ);
		}
	}
}

#endif
