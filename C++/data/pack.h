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
	void sw_e(uint16_t &c)
	{
		c = (c << 8) | (c >> 8);
	}
	void sw_e(uint32_t &c)
	{
		c = (c << 16) | (c >> 16);
		c = ((c & 0xff00ff00) >> 8) | ((c & 0x00ff00ff) << 8);
	}
	void sw_e(uint64_t &c)
	{
		c = (c << 32) | (c >> 32);
		c = ((c & 0xffff0000ffff0000) >> 16) | ((c & 0x0000ffff0000ffff) << 16);
		c = ((c & 0xff00ff00ff00ff00) >> 8) | ((c & 0x00ff00ff00ff00ff) << 8);
	}

	template<char E, typename T>
	void pack(const uint8_t *a, T &c)
	{
		memcpy(&c, a, sizeof(T));
		if(E != endianness::current)
		{
			sw_e(c);
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
				sw_e(r[i]);
			}
		}
	}

	template<char E, typename T>
	void unpack(T c, uint8_t *a)
	{
		if(E != endianness::current)
		{
			sw_e(c);
		}
		memcpy(a, &c, sizeof(T));
	}
	template<char E>
	void unpack(const uint32_t *a, std::size_t n, uint8_t *r)
	{
		if(E == endianness::current)
		{
			memcpy(r, a, n<<2);
			return;
		}
		for(std::size_t i = 0; i < n; ++i)
		{
			unpack<E>(a[i], r + (i<<2));
		}
	}
}
