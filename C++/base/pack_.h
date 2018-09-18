namespace conv
{
	std::vector<uint32_t> pack4_le(const uint8_t *m, const std::size_t n)
	{
		std::vector<uint32_t> r;
		r.reserve(n>>2);
		for(std::size_t i = 0; i < n; i+=4)
		{
			uint32_t t = m[i+3];
			t = (t<<8) | m[i+2];
			t = (t<<8) | m[i+1];
			t = (t<<8) | m[i];
			r.push_back(t);
		}
		return r;
	}

	template<std::size_t SZ>
	std::array<uint32_t, (SZ>>2)> pack4_le(const uint8_t *m)
	{
		std::array<uint32_t, (SZ>>2)> r;
		for(std::size_t i = 0; i < SZ; i+=4)
		{
			uint32_t t = m[i+3];
			t = (t<<8) | m[i+2];
			t = (t<<8) | m[i+1];
			t = (t<<8) | m[i];
			r[i>>2] = t;
		}
		return r;
	}

	template<std::size_t SZ>
	std::array<uint32_t, (SZ>>2)> pack4_be(const uint8_t *m)
	{
		std::array<uint32_t, (SZ>>2)> r;
		for(std::size_t i = 0; i < SZ; i+=4)
		{
			uint32_t t = m[i];
			t = (t<<8) | m[i+1];
			t = (t<<8) | m[i+2];
			t = (t<<8) | m[i+3];
			r[i>>2] = t;
		}
		return r;
	}

	std::vector<uint8_t> unpack1_le(const uint32_t *m, const std::size_t n)
	{
		std::vector<uint8_t> r;
		r.reserve(n<<2);
		for(std::size_t i = 0; i < n; i++)
		{
			r.push_back(m[i] & 0xff);
			r.push_back((m[i]>>8) & 0xff);
			r.push_back((m[i]>>16) & 0xff);
			r.push_back((m[i]>>24) & 0xff);
		}
		return r;
	}

	std::vector<uint8_t> unpack1_be(const uint32_t *m, const std::size_t n)
	{
		std::vector<uint8_t> r;
		r.reserve(n<<2);
		for(std::size_t i = 0; i < n; i++)
		{
			r.push_back((m[i]>>24) & 0xff);
			r.push_back((m[i]>>16) & 0xff);
			r.push_back((m[i]>>8) & 0xff);
			r.push_back(m[i] & 0xff);
		}
		return r;
	}

	template<std::size_t SZ>
	std::array<uint8_t, (SZ<<2)> unpack1_le(const uint32_t *m)
	{
		std::array<uint8_t, (SZ<<2)> r;
		for(std::size_t i = 0; i < SZ; i++)
		{
			r[i*4] = m[i] & 0xff;
			r[i*4+1] = (m[i]>>8) & 0xff;
			r[i*4+2] = (m[i]>>16) & 0xff;
			r[i*4+3] = (m[i]>>24) & 0xff;
		}
		return r;
	}

	template<std::size_t SZ>
	std::array<uint8_t, (SZ<<2)> unpack1_be(const uint32_t *m)
	{
		std::array<uint8_t, (SZ<<2)> r;
		for(std::size_t i = 0; i < SZ; i++)
		{
			r[i*4] = (m[i]>>24) & 0xff;
			r[i*4+1] = (m[i]>>16) & 0xff;
			r[i*4+2] = (m[i]>>8) & 0xff;
			r[i*4+3] = m[i] & 0xff;
		}
		return r;
	}
}
