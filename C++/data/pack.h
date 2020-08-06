namespace endianness
{
	enum {
		UNKNOWN,
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};
	char get_endianness()
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
};
template<>
struct bconv<2, endianness::BIG_ENDIAN>
{
	static uint_fast16_t pack(const uint8_t *a)
	{
		uint_fast16_t r = a[0];
		return (r<<8) | a[1];
	}
};

template<>
struct bconv<4, endianness::LITTLE_ENDIAN>
{
	static uint_fast32_t pack(const uint8_t *a)
	{
		uint_fast32_t r = bconv<2, endianness::LITTLE_ENDIAN>::pack(a+2);
		return (r<<16) | bconv<2, endianness::LITTLE_ENDIAN>::pack(a);
	}
};
template<>
struct bconv<4, endianness::BIG_ENDIAN>
{
	static uint_fast32_t pack(const uint8_t *a)
	{
		uint_fast32_t r = bconv<2, endianness::BIG_ENDIAN>::pack(a);
		return (r<<16) | bconv<2, endianness::BIG_ENDIAN>::pack(a+2);
	}
};

template<>
struct bconv<8, endianness::LITTLE_ENDIAN>
{
	static uint_fast64_t pack(const uint8_t *a)
	{
		uint_fast64_t r = bconv<4, endianness::LITTLE_ENDIAN>::pack(a+4);
		return (r<<16) | bconv<4, endianness::LITTLE_ENDIAN>::pack(a);
	}
};
template<>
struct bconv<8, endianness::BIG_ENDIAN>
{
	static uint_fast64_t pack(const uint8_t *a)
	{
		uint_fast64_t r = bconv<4, endianness::BIG_ENDIAN>::pack(a);
		return (r<<16) | bconv<4, endianness::BIG_ENDIAN>::pack(a+4);
	}
};

namespace conv
{
	template<char E, typename T>
	void pack(const uint8_t *a, const std::size_t n, T *r)
	{
		memcpy(r, a, n);
		if(E != endianness::current)
		{
			for(std::size_t i = 0; i < n / sizeof(T); ++i)
			{
				ByteRev<sizeof(T)>(r[i]);
			}
		}
	}

	template<char E, typename T>
	void unpack(T c, uint8_t *a)
	{
		if(E != endianness::current)
		{
			ByteRev<sizeof(T)>(c);
		}
		memcpy(a, &c, sizeof(T));
	}
	template<char E, typename T>
	void unpack(const T *a, std::size_t n, uint8_t *r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n*sizeof(T));
			return;
		}
		for(std::size_t i = 0; i < n; ++i)
		{
			unpack<E>(a[i], r + i*sizeof(T));
		}
	}
}
