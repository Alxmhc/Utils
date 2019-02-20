namespace endianness
{
	const int UNKNOWN = 0;
	const int LITTLE_ENDIAN = 1;
	const int BIG_ENDIAN = 2;
	int get_endianness()
	{
		const char a[4] = {1,2,3,4};
		uint32_t t;
		memcpy(&t, a, 4);
		if(t == 67305985)
			return LITTLE_ENDIAN;
		if(t == 16909060)
			return BIG_ENDIAN;
		return UNKNOWN;
	}
	const int current = get_endianness();
};

namespace conv
{
	uint32_t pack_le4(const uint8_t *a)
	{
		uint32_t t = a[3];
		t = (t<<8) | a[2];
		t = (t<<8) | a[1];
		t = (t<<8) | a[0];
		return t;
	}

	uint32_t pack_be4(const uint8_t *a)
	{
		uint32_t t = a[0];
		t = (t<<8) | a[1];
		t = (t<<8) | a[2];
		t = (t<<8) | a[3];
		return t;
	}

	template<std::size_t SZ>
	void pack_le(const uint8_t *a, std::array<uint32_t, (SZ>>2)> &r)
	{
		if(endianness::current == endianness::LITTLE_ENDIAN)
		{
			memcpy(r.data(), a, SZ);
			return;
		}
		for(std::size_t i = 0; i < (SZ>>2); ++i)
		{
			r[i] = pack_le4(a+(i<<2));
		}
	}

	template<std::size_t SZ>
	void pack_be(const uint8_t *a, std::array<uint32_t, (SZ>>2)> &r)
	{
		if(endianness::current == endianness::BIG_ENDIAN)
		{
			memcpy(r.data(), a, SZ);
			return;
		}
		for(std::size_t i = 0; i < (SZ>>2); ++i)
		{
			r[i] = pack_be4(a+(i<<2));
		}
	}

	template<std::size_t SZ>
	void unpack_le(const uint32_t *a, std::array<uint8_t, (SZ<<2)> &r)
	{
		if(endianness::current == endianness::LITTLE_ENDIAN)
		{
			memcpy(r.data(), a, SZ<<2);
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
	void unpack_be(const uint32_t *a, std::array<uint8_t, (SZ<<2)> &r)
	{
		if(endianness::current == endianness::BIG_ENDIAN)
		{
			memcpy(r.data(), a, SZ<<2);
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
