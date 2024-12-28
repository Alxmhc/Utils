#ifndef H_PACK
#define H_PACK

#include <cstdint>
#include <cstring>

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

		const uint& operator=(const uint &c)
		{
			l = c.l;
			h = c.h;
			return *this;
		}
		const uint& operator=(UINT_<8>::uint c)
		{
			l = c;
			h = 0;
			return *this;
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

template<unsigned char sz>
inline typename UINT_<sz>::uint uint_from(typename UINT_<sz/2>::uint l, typename UINT_<sz/2>::uint h)
{
	typename UINT_<sz>::uint res = h;
	return (res<<(4*sz)) | l;
}
template<>
inline UINT_<16>::uint uint_from<16>(UINT_<8>::uint l, UINT_<8>::uint h)
{
	return UINT_<16>::uint(l, h);
}

template<unsigned char sz>
inline typename UINT_<sz/2>::uint uint_getL(typename UINT_<sz>::uint c)
{
	return static_cast<typename UINT_<sz/2>::uint>(c);
}
template<>
inline UINT_<8>::uint uint_getL<16>(UINT_<16>::uint c)
{
	return c.getL();
}

template<unsigned char sz>
inline typename UINT_<sz/2>::uint uint_getH(typename UINT_<sz>::uint c)
{
	return static_cast<typename UINT_<sz/2>::uint>(c>>(4*sz));
}
template<>
inline UINT_<8>::uint uint_getH<16>(UINT_<16>::uint c)
{
	return c.getH();
}

template <unsigned char sz, unsigned char k, char E>
struct bconv{};

template <unsigned char sz, unsigned char k>
struct bconv<sz, k, endianness::LITTLE_ENDIAN>
{
	static typename UINT_<k*sz>::uint pack(const typename UINT_<sz>::uint* a)
	{
		const auto l = bconv<sz, k/2, endianness::LITTLE_ENDIAN>::pack(a);
		const auto h = bconv<sz, k/2, endianness::LITTLE_ENDIAN>::pack(a + k/2);
		return uint_from<k*sz>(l, h);
	}
	static void unpack(typename UINT_<k*sz>::uint c, typename UINT_<sz>::uint* a)
	{
		bconv<sz, k/2, endianness::LITTLE_ENDIAN>::unpack(uint_getL<k*sz>(c), a);
		bconv<sz, k/2, endianness::LITTLE_ENDIAN>::unpack(uint_getH<k*sz>(c), a + k/2);
	}
};
template<unsigned char sz>
struct bconv<sz, 1, endianness::LITTLE_ENDIAN>
{
	static typename UINT_<sz>::uint pack(const typename UINT_<sz>::uint* a)
	{
		return *a;
	}
	static void unpack(typename UINT_<sz>::uint c, typename UINT_<sz>::uint* a)
	{
		*a = c;
	}
};

template<unsigned char sz, unsigned char k>
struct bconv<sz, k, endianness::BIG_ENDIAN>
{
	static typename UINT_<k*sz>::uint pack(const typename UINT_<sz>::uint* a)
	{
		const auto l = bconv<sz, k/2, endianness::BIG_ENDIAN>::pack(a + k/2);
		const auto h = bconv<sz, k/2, endianness::BIG_ENDIAN>::pack(a);
		return uint_from<k*sz>(l, h);
	}
	static void unpack(typename UINT_<k*sz>::uint c, typename UINT_<sz>::uint* a)
	{
		bconv<sz, k/2, endianness::BIG_ENDIAN>::unpack(uint_getL<k*sz>(c), a + k/2);
		bconv<sz, k/2, endianness::BIG_ENDIAN>::unpack(uint_getH<k*sz>(c), a);
	}
};
template<unsigned char sz>
struct bconv<sz, 1, endianness::BIG_ENDIAN>
{
	static typename UINT_<sz>::uint pack(const typename UINT_<sz>::uint* a)
	{
		return *a;
	}
	static void unpack(typename UINT_<sz>::uint c, typename UINT_<sz>::uint* a)
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
