namespace endianness
{
	enum {
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
template<> struct UINT_<1>{typedef  uint_fast8_t uint_;};
template<> struct UINT_<2>{typedef uint_fast16_t uint_;};
template<> struct UINT_<4>{typedef uint_fast32_t uint_;};
template<> struct UINT_<8>{typedef uint_fast64_t uint_;};
template<> struct UINT_<16>
{
	class uint_
	{
		static const uint_fast8_t hsz = 64;
		typedef uint64_t htype;
		htype h, l;
	public:
		uint_(htype ln = 0, htype hn = 0) : h(ln), l(hn) {}

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

		void unpack(htype &cl, htype &ch)
		{
			cl = l;
			ch = h;
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
	};
};

template <unsigned char sz, char E>
struct bconv{};

template<>
struct bconv<2, endianness::LITTLE_ENDIAN>
{
	static UINT_<2>::uint_ pack(const uint8_t* a)
	{
		UINT_<2>::uint_ r = a[1];
		return (r<<8) | a[0];
	}
	static void unpack(UINT_<2>::uint_ c, uint8_t* a)
	{
		a[0] = c & 0xff;
		a[1] = (c >> 8) & 0xff;
	}
};
template<>
struct bconv<2, endianness::BIG_ENDIAN>
{
	static UINT_<2>::uint_ pack(const uint8_t* a)
	{
		UINT_<2>::uint_ r = a[0];
		return (r<<8) | a[1];
	}
	static void unpack(UINT_<2>::uint_ c, uint8_t* a)
	{
		a[0] = (c >> 8) & 0xff;
		a[1] = c & 0xff;
	}
};

template<>
struct bconv<4, endianness::LITTLE_ENDIAN>
{
	static UINT_<4>::uint_ pack(const uint8_t* a)
	{
		UINT_<4>::uint_ r = bconv<2, endianness::LITTLE_ENDIAN>::pack(a + 2);
		return (r<<16) | bconv<2, endianness::LITTLE_ENDIAN>::pack(a);
	}
	static void unpack(UINT_<4>::uint_ c, uint8_t* a)
	{
		bconv<2, endianness::LITTLE_ENDIAN>::unpack(c & 0xffff, a);
		bconv<2, endianness::LITTLE_ENDIAN>::unpack((c >> 16) & 0xffff, a + 2);
	}
};
template<>
struct bconv<4, endianness::BIG_ENDIAN>
{
	static UINT_<4>::uint_ pack(const uint8_t* a)
	{
		UINT_<4>::uint_ r = bconv<2, endianness::BIG_ENDIAN>::pack(a);
		return (r<<16) | bconv<2, endianness::BIG_ENDIAN>::pack(a + 2);
	}
	static void unpack(UINT_<4>::uint_ c, uint8_t* a)
	{
		bconv<2, endianness::BIG_ENDIAN>::unpack((c >> 16) & 0xffff, a);
		bconv<2, endianness::BIG_ENDIAN>::unpack(c & 0xffff, a + 2);
	}
};

template<>
struct bconv<8, endianness::LITTLE_ENDIAN>
{
	static UINT_<8>::uint_ pack(const uint8_t* a)
	{
		UINT_<8>::uint_ r = bconv<4, endianness::LITTLE_ENDIAN>::pack(a + 4);
		return (r<<32) | bconv<4, endianness::LITTLE_ENDIAN>::pack(a);
	}
	static void unpack(UINT_<8>::uint_ c, uint8_t* a)
	{
		bconv<4, endianness::LITTLE_ENDIAN>::unpack(c & 0xffffffff, a);
		bconv<4, endianness::LITTLE_ENDIAN>::unpack((c >> 32) & 0xffffffff, a + 4);
	}
};
template<>
struct bconv<8, endianness::BIG_ENDIAN>
{
	static UINT_<8>::uint_ pack(const uint8_t* a)
	{
		UINT_<8>::uint_ r = bconv<4, endianness::BIG_ENDIAN>::pack(a);
		return (r<<32) | bconv<4, endianness::BIG_ENDIAN>::pack(a + 4);
	}
	static void unpack(UINT_<8>::uint_ c, uint8_t* a)
	{
		bconv<4, endianness::BIG_ENDIAN>::unpack((c >> 32) & 0xffffffff, a);
		bconv<4, endianness::BIG_ENDIAN>::unpack(c & 0xffffffff, a + 4);
	}
};

namespace conv
{
	template<unsigned char SZ, char E>
	void pack(const uint8_t* a, const size_t n, typename UINT_<SZ>::uint_ *r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n);
			return;
		}
		for(size_t i = 0; i < n / SZ; ++i)
		{
			r[i] = bconv<SZ, E>::pack(a + i*SZ);
		}
	}
	template<unsigned char SZ, char E>
	void unpack(typename UINT_<SZ>::uint_ const *a, size_t n, uint8_t* r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n*SZ);
			return;
		}
		for(size_t i = 0; i < n; ++i)
		{
			bconv<SZ, E>::unpack(a[i], r + i*SZ);
		}
	}
}
