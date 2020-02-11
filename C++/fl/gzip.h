namespace gzip
{
	struct inf
	{
		std::string fname, comment;
		uint32_t time;
		uint8_t crc32[4];
		uint32_t fsize;
	};

	uint32_t read(byteReader &s, inf &res)
	{
		char hdr[3];
		s.read(hdr, 3);
		if(hdr[0] != '\x1f' || hdr[1] != '\x8b' || hdr[2] != '\x08')
			return 0;
		uint8_t flg;
		s.get(flg);
		if(flg < 0 || flg > 31)
			return 0;
		s.getC<endianness::LITTLE_ENDIAN>(res.time);
		s.skip(2);
		if( (flg & 4) != 0 )
		{
			uint16_t sz;
			s.getC<endianness::LITTLE_ENDIAN>(sz);
			s.skip(sz);
		}
		std::string fname, comment;
		if( (flg & 8) != 0 )
		{
			fname = s.read_string(0);
		}
		if( (flg & 16) != 0 )
		{
			comment = s.read_string(0);
		}
		res.fname = fname;
		res.comment = comment;
		if( (flg & 2) != 0 )
		{
			s.skip(2);
		}

		const auto st = s.get_pos();
		s.set_pos(-8);
		const auto end = s.get_pos();
		if(st >= end)
			return 0;
		s.read(res.crc32, 4);
		s.getC<endianness::LITTLE_ENDIAN>(res.fsize);
		s.set_pos(st);
		return static_cast<uint32_t>(end - st);
	}
}
