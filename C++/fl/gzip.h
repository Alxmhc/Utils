namespace gzip
{
	struct inf
	{
		std::string fname, comment;
		uint8_t crc32[4];
		uint32_t fsize;
	};

	uint32_t read(std::istream &s, inf &res)
	{
		char hdr[3];
		s.read(hdr, 3);
		if(hdr[0] != '\x1f' || hdr[1] != '\x8b' || hdr[2] != '\x08')
			return 0;
		char flg;
		s.get(flg);
		if(flg < 0 || flg > 31)
			return 0;
		s.seekg(6, std::ios_base::cur);
		if( (flg & 4) != 0 )
		{
			uint16_t sz;
			stream::read<endianness::LITTLE_ENDIAN>(s, sz);
			s.seekg(sz, std::ios_base::cur);
		}

		std::string fname, comment;
		if( (flg & 8) != 0 )
		{
			fname = stream::read_string(s);
		}
		if( (flg & 16) != 0 )
		{
			comment = stream::read_string(s);
		}
		res.fname = fname;
		res.comment = comment;

		if( (flg & 2) != 0 )
		{
			s.seekg(2, std::ios_base::cur);
		}
		auto st = s.tellg();
		s.seekg(-8, std::ios_base::end);
		auto end = s.tellg();
		if(st >= end)
			return 0;
		s.read(reinterpret_cast<char*>(res.crc32), 4);
		stream::read<endianness::LITTLE_ENDIAN>(s, res.fsize);
		s.seekg(st, std::ios_base::beg);
		return static_cast<uint32_t>(end - st);
	}
}
