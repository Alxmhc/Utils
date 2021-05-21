template <unsigned char sz>
struct byte_rev{};
template<>
struct byte_rev<1>
{
	template<typename T>
	static void ByteRev(T &c)
	{
	}
};
template<>
struct byte_rev<2>
{
	template<typename T>
	static void ByteRev(T &c)
	{
		c = ((c & 0x00ff) << 8) | ((c & 0xff00) >> 8);
	}
};
template<>
struct byte_rev<4>
{
	template<typename T>
	static void ByteRev(T &c)
	{
		c = ((c & 0x0000ffff) << 16) | ((c & 0xffff0000) >> 16);
		c = ((c & 0x00ff00ff) <<  8) | ((c & 0xff00ff00) >>  8);
	}
};
template<>
struct byte_rev<8>
{
	template<typename T>
	static void ByteRev(T &c)
	{
		c = ((c & 0x00000000ffffffff) << 32) | ((c & 0xffffffff00000000) >> 32);
		c = ((c & 0x0000ffff0000ffff) << 16) | ((c & 0xffff0000ffff0000) >> 16);
		c = ((c & 0x00ff00ff00ff00ff) <<  8) | ((c & 0xff00ff00ff00ff00) >>  8);
	}
};
template<unsigned char sz, typename T>
void ByteRev(T &c)
{
	byte_rev<sz>::ByteRev(c);
}

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
	void pack(const uint8_t *a, const size_t n, T *r)
	{
		memcpy(r, a, n);
		if(E != endianness::current)
		{
			for(size_t i = 0; i < n / sizeof(T); ++i)
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
	void unpack(const T *a, size_t n, uint8_t *r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n*sizeof(T));
			return;
		}
		for(size_t i = 0; i < n; ++i)
		{
			unpack<E>(a[i], r + i*sizeof(T));
		}
	}
}
