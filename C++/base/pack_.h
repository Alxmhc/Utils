namespace conv
{
	template<std::size_t SZ>
	void pack_le(const uint8_t *m, std::array<uint32_t, (SZ>>2)> &r)
	{
		for(std::size_t i = 0; i < SZ; i+=4)
		{
			uint32_t t = m[i+3];
			t = (t<<8) | m[i+2];
			t = (t<<8) | m[i+1];
			t = (t<<8) | m[i];
			r[i>>2] = t;
		}
	}

	template<std::size_t SZ>
	void pack_be(const uint8_t *m, std::array<uint32_t, (SZ>>2)> &r)
	{
		for(std::size_t i = 0; i < SZ; i+=4)
		{
			uint32_t t = m[i];
			t = (t<<8) | m[i+1];
			t = (t<<8) | m[i+2];
			t = (t<<8) | m[i+3];
			r[i>>2] = t;
		}
	}

	template<std::size_t SZ>
	void unpack_le(const uint32_t *m, std::array<uint8_t, (SZ<<2)> &r)
	{
		for(std::size_t i = 0; i < SZ; i++)
		{
			r[i*4] = m[i] & 0xff;
			r[i*4+1] = (m[i]>>8) & 0xff;
			r[i*4+2] = (m[i]>>16) & 0xff;
			r[i*4+3] = (m[i]>>24) & 0xff;
		}
	}

	template<std::size_t SZ>
	void unpack_be(const uint32_t *m, std::array<uint8_t, (SZ<<2)> &r)
	{
		for(std::size_t i = 0; i < SZ; i++)
		{
			r[i*4] = (m[i]>>24) & 0xff;
			r[i*4+1] = (m[i]>>16) & 0xff;
			r[i*4+2] = (m[i]>>8) & 0xff;
			r[i*4+3] = m[i] & 0xff;
		}
	}
}
