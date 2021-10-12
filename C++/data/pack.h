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

template <unsigned char sz, char E>
struct bconv{};

template<>
struct bconv<2, endianness::LITTLE_ENDIAN>
{
	static uint_fast16_t pack(const uint8_t *a)
	{
		uint_fast16_t r = a[1];
		return (r<<8) | a[0];
	}
	static void unpack(uint_fast16_t c, uint8_t *a)
	{
		a[0] = c & 0xff;
		a[1] = (c >> 8) & 0xff;
	}
};
template<>
struct bconv<2, endianness::BIG_ENDIAN>
{
	static uint_fast16_t pack(const uint8_t *a)
	{
		uint_fast16_t r = a[0];
		return (r<<8) | a[1];
	}
	static void unpack(uint_fast16_t c, uint8_t *a)
	{
		a[0] = (c >> 8) & 0xff;
		a[1] = c & 0xff;
	}
};

template<>
struct bconv<4, endianness::LITTLE_ENDIAN>
{
	static uint_fast32_t pack(const uint8_t *a)
	{
		uint_fast32_t r = bconv<2, endianness::LITTLE_ENDIAN>::pack(a + 2);
		return (r<<16) | bconv<2, endianness::LITTLE_ENDIAN>::pack(a);
	}
	static void unpack(uint_fast32_t c, uint8_t *a)
	{
		bconv<2, endianness::LITTLE_ENDIAN>::unpack(c & 0xffff, a);
		bconv<2, endianness::LITTLE_ENDIAN>::unpack((c >> 16) & 0xffff, a + 2);
	}
};
template<>
struct bconv<4, endianness::BIG_ENDIAN>
{
	static uint_fast32_t pack(const uint8_t *a)
	{
		uint_fast32_t r = bconv<2, endianness::BIG_ENDIAN>::pack(a);
		return (r<<16) | bconv<2, endianness::BIG_ENDIAN>::pack(a + 2);
	}
	static void unpack(uint_fast32_t c, uint8_t *a)
	{
		bconv<2, endianness::BIG_ENDIAN>::unpack((c >> 16) & 0xffff, a);
		bconv<2, endianness::BIG_ENDIAN>::unpack(c & 0xffff, a + 2);
	}
};

template<>
struct bconv<8, endianness::LITTLE_ENDIAN>
{
	static uint_fast64_t pack(const uint8_t *a)
	{
		uint_fast64_t r = bconv<4, endianness::LITTLE_ENDIAN>::pack(a + 4);
		return (r<<32) | bconv<4, endianness::LITTLE_ENDIAN>::pack(a);
	}
	static void unpack(uint_fast64_t c, uint8_t *a)
	{
		bconv<4, endianness::LITTLE_ENDIAN>::unpack(c & 0xffffffff, a);
		bconv<4, endianness::LITTLE_ENDIAN>::unpack((c >> 32) & 0xffffffff, a + 4);
	}
};
template<>
struct bconv<8, endianness::BIG_ENDIAN>
{
	static uint_fast64_t pack(const uint8_t *a)
	{
		uint_fast64_t r = bconv<4, endianness::BIG_ENDIAN>::pack(a);
		return (r<<32) | bconv<4, endianness::BIG_ENDIAN>::pack(a + 4);
	}
	static void unpack(uint_fast64_t c, uint8_t *a)
	{
		bconv<4, endianness::BIG_ENDIAN>::unpack((c >> 32) & 0xffffffff, a);
		bconv<4, endianness::BIG_ENDIAN>::unpack(c & 0xffffffff, a + 4);
	}
};

namespace conv
{
	template<char E, typename T>
	void pack(const uint8_t *a, const size_t n, T *r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n);
			return;
		}
		const uint_fast8_t sz = sizeof(T);
		for(size_t i = 0; i < n / sz; ++i)
		{
			r[i] = bconv<sz, E>::pack(a + i*sz);
		}
	}
	template<char E, typename T>
	void unpack(const T *a, size_t n, uint8_t *r)
	{
		const uint_fast8_t sz = sizeof(T);
		if(E == endianness::current)
		{
			memcpy(r, a, n*sz);
			return;
		}
		for(size_t i = 0; i < n; ++i)
		{
			bconv<sz, E>::unpack(a[i], r + i*sz);
		}
	}
}
