namespace fl_pr
{
	namespace F_gzip
	{
		struct inf
		{
			size_t hdr_pos;
			size_t hdr_size;
			size_t data_pos;
			size_t data_size;

			std::string fname;
			uint_fast32_t fsize;
			uint8_t crc32[4];

			std::string comment;
		};

		bool read_inf(byteReader &s, inf &res)
		{
			if (s.get_size() < 18) //10 + 8
				return false;
			{
				uint8_t hdr[3];
				s.readN(hdr, 3);
				if(std::memcmp(hdr, "\x1f\x8b\x08", 3) != 0)
					return false;
			}
			uint8_t flg;
			s.get(flg);
			if(flg > 31)
				return false;
			s.skip(6);
			if( (flg & 4) != 0 )
			{
				uint_fast16_t sz;
				if (!s.readC<2, endianness::LITTLE_ENDIAN>(sz))
					return false;
				if (!s.skip(sz))
					return false;
			}
			if( (flg & 8) != 0 )
			{
				 if( !s.read_string(0, res.fname) )
					 return false;
			}
			if( (flg & 16) != 0 )
			{
				 if( !s.read_string(0, res.comment) )
					 return false;
			}
			if( (flg & 2) != 0 )
			{
				s.skip(2);
			}

			const auto st = s.get_pos();
			const auto ef = s.get_size() - 8;
			if(st > ef)
				return false;
			res.hdr_pos = 0;
			res.hdr_size = st;
			res.data_pos = res.hdr_size;
			res.data_size = ef - st;

			s.set_pos(ef);
			s.readN(res.crc32, 4);
			s.readC<4, endianness::LITTLE_ENDIAN>(res.fsize);
			return true;
		}
	}
}
