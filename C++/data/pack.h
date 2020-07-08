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
	template<char E>
	void read(const uint8_t *a, uint8_t &c)
	{
		c = a[0];
	}

	template<char E>
	void read(const uint8_t *a, uint16_t &c);
	template<>
	void read<endianness::LITTLE_ENDIAN>(const uint8_t *a, uint16_t &c)
	{
		uint16_t t = a[1];
		t = (t<<8) | a[0];
		c = t;
	}
	template<>
	void read<endianness::BIG_ENDIAN>(const uint8_t *a, uint16_t &c)
	{
		uint16_t t = a[0];
		t = (t<<8) | a[1];
		c = t;
	}

	template<char E>
	void read(const uint8_t *a, uint32_t &c);
	template<>
	void read<endianness::LITTLE_ENDIAN>(const uint8_t *a, uint32_t &c)
	{
		uint32_t t = a[3];
		t = (t<<8) | a[2];
		t = (t<<8) | a[1];
		t = (t<<8) | a[0];
		c = t;
	}
	template<>
	void read<endianness::BIG_ENDIAN>(const uint8_t *a, uint32_t &c)
	{
		uint32_t t = a[0];
		t = (t<<8) | a[1];
		t = (t<<8) | a[2];
		t = (t<<8) | a[3];
		c = t;
	}

	template<char E>
	void read(const uint8_t *a, uint64_t &c);
	template<>
	void read<endianness::LITTLE_ENDIAN>(const uint8_t *a, uint64_t &c)
	{
		uint64_t t = a[7];
		t = (t<<8) | a[6];
		t = (t<<8) | a[5];
		t = (t<<8) | a[4];
		t = (t<<8) | a[3];
		t = (t<<8) | a[2];
		t = (t<<8) | a[1];
		t = (t<<8) | a[0];
		c = t;
	}
	template<>
	void read<endianness::BIG_ENDIAN>(const uint8_t *a, uint64_t &c)
	{
		uint64_t t = a[0];
		t = (t<<8) | a[1];
		t = (t<<8) | a[2];
		t = (t<<8) | a[3];
		t = (t<<8) | a[4];
		t = (t<<8) | a[5];
		t = (t<<8) | a[6];
		t = (t<<8) | a[7];
		c = t;
	}

	template<std::size_t SZ, char E>
	void pack(const uint8_t *a, std::array<uint32_t, (SZ>>2)> &r)
	{
		if(endianness::current == E)
		{
			memcpy(r.data(), a, SZ);
			return;
		}
		for(std::size_t i = 0; i < (SZ>>2); ++i)
		{
			read<E>(a+(i<<2), r[i]);
		}
	}

	template<std::size_t SZ>
	void unpack_le(const uint32_t *a, uint8_t *r)
	{
		if(endianness::current == endianness::LITTLE_ENDIAN)
		{
			memcpy(r, a, SZ<<2);
			return;
		}
		for(std::size_t i = 0; i < SZ; ++i)
		{
			r[i*4] = a[i] & 0xff;
			r[i*4+1] = (a[i]>>8) & 0xff;
			r[i*4+2] = (a[i]>>16) & 0xff;
			r[i*4+3] = (a[i]>>24) & 0xff;
		}
	}

	template<std::size_t SZ>
	void unpack_be(const uint32_t *a, uint8_t *r)
	{
		if(endianness::current == endianness::BIG_ENDIAN)
		{
			memcpy(r, a, SZ<<2);
			return;
		}
		for(std::size_t i = 0; i < SZ; ++i)
		{
			r[i*4] = (a[i]>>24) & 0xff;
			r[i*4+1] = (a[i]>>16) & 0xff;
			r[i*4+2] = (a[i]>>8) & 0xff;
			r[i*4+3] = a[i] & 0xff;
		}
	}
}
