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
	typedef uint_fast8_t uint_;
};
template<> struct UINT_<2>
{
	typedef uint_fast16_t uint_;
	static UINT_<1>::uint_ getL(uint_ c)
	{
		return c & 0xff;
	}
	static UINT_<1>::uint_ getH(uint_ c)
	{
		return (c >> 8) & 0xff;
	}
	static uint_ from(UINT_<1>::uint_ l, UINT_<1>::uint_ h)
	{
		uint_ r = h;
		return (r << 8) | l;
	}
};
template<> struct UINT_<4>
{
	typedef uint_fast32_t uint_;
	static UINT_<2>::uint_ getL(uint_ c)
	{
		return c & 0xffff;
	}
	static UINT_<2>::uint_ getH(uint_ c)
	{
		return (c >> 16) & 0xffff;
	}
	static uint_ from(UINT_<2>::uint_ l, UINT_<2>::uint_ h)
	{
		uint_ r = h;
		return (r << 16) | l;
	}
};
template<> struct UINT_<8>
{
	typedef uint_fast64_t uint_;
	static UINT_<4>::uint_ getL(uint_ c)
	{
		return c & 0xffffffff;
	}
	static UINT_<4>::uint_ getH(uint_ c)
	{
		return (c >> 32) & 0xffffffff;
	}
	static uint_ from(UINT_<4>::uint_ l, UINT_<4>::uint_ h)
	{
		uint_ r = h;
		return (r << 32) | l;
	}
};
template<> struct UINT_<16>
{
	class uint_
	{
		static const uint_fast8_t hsz = 64;
		typedef uint64_t htype;
		htype l, h;
	public:
		uint_(htype ln = 0, htype hn = 0) : l(ln), h(hn) {}

		const uint_& operator=(const uint_ &c)
		{
			l = c.l;
			h = c.h;
			return *this;
		}
		const uint_& operator=(htype c)
		{
			l = c;
			h = 0;
			return *this;
		}

		htype getL() const
		{
			return l;
		}
		htype getH() const
		{
			return h;
		}

		const uint_& operator+=(htype c)
		{
			l += c;
			if(l < c)
			{
				h++;
			}
			return *this;
		}

		const uint_& operator|=(const uint_& c)
		{
			l |= c.l;
			h |= c.h;
			return *this;
		}
		const uint_& operator^=(const uint_& c)
		{
			l ^= c.l;
			h ^= c.h;
			return *this;
		}
		const uint_& operator>>=(uint_fast8_t n)
		{
			if(n < hsz)
			{
				l = (l>>n)|(h<<(hsz-n));
				h >>= n;
			}
			else
			{
				l = n < 2*hsz ? h>>(n-hsz) : 0;
				h = 0;
			}
			return *this;
		}
		const uint_& operator<<=(uint_fast8_t n)
		{
			if(n < hsz)
			{
				h = (h<<n)|(l>>(hsz-n));
				l <<= n;
			}
			else
			{
				h = n < 2*hsz ? l<<(n-hsz) : 0;
				l = 0;
			}
			return *this;
		}

		uint_ operator|(const uint_ &c) const
		{
			uint_ r(*this);
			return r |= c;
		}
		uint_ operator^(const uint_ &c) const
		{
			uint_ r(*this);
			return r ^= c;
		}

		uint_ operator>>(uint_fast8_t n) const
		{
			uint_ r(*this);
			return r >>= n;
		}
		uint_ operator<<(uint_fast8_t n) const
		{
			uint_ r(*this);
			return r <<= n;
		}
	};
	static UINT_<8>::uint_ getL(uint_ c)
	{
		return c.getL();
	}
	static UINT_<8>::uint_ getH(uint_ c)
	{
		return c.getH();
	}
	static uint_ from(UINT_<8>::uint_ l, UINT_<8>::uint_ h)
	{
		return uint_(l, h);
	}
};

template <unsigned char sz, unsigned char k, char E>
struct bconv{};

template <unsigned char sz, unsigned char k>
struct bconv<sz, k, endianness::LITTLE_ENDIAN>
{
	template<typename m>
	static typename UINT_<k*sz>::uint_ pack(const m* a)
	{
		const auto l = bconv<sz, k/2, endianness::LITTLE_ENDIAN>::pack(a);
		const auto h = bconv<sz, k/2, endianness::LITTLE_ENDIAN>::pack(a + k/2);
		return UINT_<k*sz>::from(l, h);
	}
	template<typename m>
	static void unpack(typename UINT_<k*sz>::uint_ c, m* a)
	{
		bconv<sz, k/2, endianness::LITTLE_ENDIAN>::unpack(UINT_<k*sz>::getL(c), a);
		bconv<sz, k/2, endianness::LITTLE_ENDIAN>::unpack(UINT_<k*sz>::getH(c), a + k/2);
	}
};
template<unsigned char sz>
struct bconv<sz, 1, endianness::LITTLE_ENDIAN>
{
	template<typename m>
	static typename UINT_<sz>::uint_ pack(const m* a)
	{
		return *a;
	}
	template<typename m>
	static void unpack(typename UINT_<sz>::uint_ c, m* a)
	{
		*a = c;
	}
};

template<unsigned char sz, unsigned char k>
struct bconv<sz, k, endianness::BIG_ENDIAN>
{
	template<typename m>
	static typename UINT_<k*sz>::uint_ pack(const m* a)
	{
		const auto l = bconv<sz, k/2, endianness::BIG_ENDIAN>::pack(a + k/2);
		const auto h = bconv<sz, k/2, endianness::BIG_ENDIAN>::pack(a);
		return UINT_<k*sz>::from(l, h);
	}
	template<typename m>
	static void unpack(typename UINT_<k*sz>::uint_ c, m* a)
	{
		bconv<sz, k/2, endianness::BIG_ENDIAN>::unpack(UINT_<k*sz>::getL(c), a + k/2);
		bconv<sz, k/2, endianness::BIG_ENDIAN>::unpack(UINT_<k*sz>::getH(c), a);
	}
};
template<unsigned char sz>
struct bconv<sz, 1, endianness::BIG_ENDIAN>
{
	template<typename m>
	static typename UINT_<sz>::uint_ pack(const m* a)
	{
		return *a;
	}
	template<typename m>
	static void unpack(typename UINT_<sz>::uint_ c, m* a)
	{
		*a = c;
	}
};

namespace conv
{
	template<unsigned char SZ, char E, typename m>
	void pack(const uint8_t* a, const std::size_t n, m* r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n);
			return;
		}
		for(std::size_t i = 0; i < n / SZ; i++)
		{
			r[i] = bconv<1, SZ, E>::pack(a + i*SZ);
		}
	}
	template<unsigned char SZ, char E, typename m>
	void unpack(const m* a, std::size_t n, uint8_t* r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n*SZ);
			return;
		}
		for(std::size_t i = 0; i < n; i++)
		{
			bconv<1, SZ, E>::unpack(a[i], r + i*SZ);
		}
	}
}

#endif
