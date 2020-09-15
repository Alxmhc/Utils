namespace fl_pr
{
	namespace F_7z
	{
		bool getNum(byteReader &s, uint_fast64_t &c)
		{
			uint8_t f;
			if(!s.get(f))
				return false;
			if((f & 0x80) == 0)
			{
				c = f;
				return true;
			}
			uint_fast8_t m = 0x40;
			uint_fast8_t p = 0;
			c = 0;

			uint8_t b;
			while(s.get(b))
			{
				c |= static_cast<uint_fast64_t>(b) << p;
				p += 8;
				if((f & m) == 0)
				{
					if(m > 1)
					{
						f &= m - 1;
						c |= static_cast<uint_fast64_t>(f) << p;
					}
					return true;
				}
				m >>= 1;
			}
			return false;
		}

		std::vector<uint8_t> getHeader(byteReader &s)
		{
			std::vector<uint8_t> h;

			uint8_t header[32];
			if(s.read(header, 32) != 32)
				return h;
			if(std::memcmp(header, "\x37\x7a\xbc\xaf\x27\x1c", 6) != 0)
				return h;

			hash::CRC32 hs;
			uint8_t hash[hash::CRC32::hash_size];

			hs.Update(header + 12, 20);
			hs.Final(hash);
			if(std::memcmp(hash, header + 8, hash::CRC32::hash_size) != 0)
				return h;

			const uint_fast64_t NHoffset = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 12);
			const uint_fast64_t NHsize = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 20);
			s.skip(NHoffset);

			std::vector<uint8_t> hdr(NHsize);
			if(s.read(&hdr[0], NHsize) != NHsize)
				return h;
			hs.Update(hdr.data(), hdr.size());
			hs.Final(hash);
			if(std::memcmp(hash, header + 28, hash::CRC32::hash_size) != 0)
				return h;
			return hdr;
		}
	}
}
