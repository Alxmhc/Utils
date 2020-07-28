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

namespace conv
{
	template<char E, typename T>
	void pack(const uint8_t *a, T &c)
	{
		memcpy(&c, a, sizeof(T));
		if(E != endianness::current)
		{
			ByteRev(c);
		}
	}
	template<char E, typename T>
	void pack(const uint8_t *a, const std::size_t n, T *r)
	{
		memcpy(r, a, n);
		if(E != endianness::current)
		{
			for(std::size_t i = 0; i < n / sizeof(T); ++i)
			{
				ByteRev(r[i]);
			}
		}
	}

	template<char E, typename T>
	void unpack(T c, uint8_t *a)
	{
		if(E != endianness::current)
		{
			ByteRev(c);
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
