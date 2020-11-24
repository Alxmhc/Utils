namespace fl_pr
{
	namespace F_gzip
	{
		struct inf
		{
			std::string fname, comment;
			uint32_t fsize;
			uint8_t crc32[4];
			std::size_t psize, ppos;
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
			s.skip(6);
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
			res.ppos = st;
			res.psize = end - st;
			s.read(res.crc32, 4);
			s.getC<endianness::LITTLE_ENDIAN>(res.fsize);
			return res;
		}
	}
}
