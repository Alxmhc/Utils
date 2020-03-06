namespace fl_pr
{
	namespace gzip
	{
		struct inf
		{
			std::string fname, comment;
			uint32_t time, fsize, crc32;
			std::size_t c_fsize;
		};

		inf read_inf(byteReader &s)
		{
			inf res = {};
			{
				uint8_t hdr[3];
				s.read(hdr, 3);
				if(std::memcmp(hdr, "\x1f\x8b\x08", 3) != 0)
					return res;
			}
			uint8_t flg;
			s.get(flg);
			if(flg > 31)
				return res;
			s.getC<endianness::LITTLE_ENDIAN>(res.time);
			s.skip(2);
			if( (flg & 4) != 0 )
			{
				uint16_t sz;
				if( !s.getC<endianness::LITTLE_ENDIAN>(sz) )
					return res;
				s.skip(sz);
			}
			if( (flg & 8) != 0 )
			{
				res.fname = s.read_string(0);
			}
			if( (flg & 16) != 0 )
			{
				res.comment = s.read_string(0);
			}
			if( (flg & 2) != 0 )
			{
				s.skip(2);
			}

			const auto st = s.get_pos();
			s.set_pos(-8);
			const auto end = s.get_pos();
			if(st >= end)
				return res;
			res.c_fsize = end - st;
			s.getC<endianness::LITTLE_ENDIAN>(res.crc32);
			s.getC<endianness::LITTLE_ENDIAN>(res.fsize);
			s.set_pos(st);
			return res;
		}

		//rfc 1952
		class CRC32
		{
			uint32_t tbl[256];
			uint32_t crc;
		public:
			CRC32() : crc(0xffffffff)
			{
				uint_fast8_t n = 0;
				do {
					n--;
					uint32_t c = n;
					for (uint_fast8_t k = 0; k < 8; k++)
					{
						if ((c & 1) != 0)
						{
							c = (c >> 1) ^ 0xedb88320;
						}
						else
						{
							c >>= 1;
						}
					}
					tbl[n] = c;
				} while(n != 0);
			}

			void Update(const uint8_t *v, const std::size_t n)
			{
				for(std::size_t i = 0; i < n; i++)
				{
					crc = tbl[(crc & 0xff) ^ v[i]] ^ (crc >> 8);
				}
			}

			uint32_t Final()
			{
				auto res = ~crc;
				crc = 0xffffffff;
				return res;
			}
		};
	}
}
