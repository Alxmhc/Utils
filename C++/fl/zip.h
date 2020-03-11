namespace fl_pr
{
	namespace zip
	{
		struct inf
		{
			bool is_encrypted;
			std::string fname;
			uint16_t method;
			uint32_t fsize, crc32;
			std::size_t f_size, f_pos;
			std::vector<uint8_t> ext;
		};

		std::vector<inf> read_inf(byteReader &s)
		{
			std::vector<inf> res;
			for(;;)
			{
				uint8_t hdr[4];
				if(s.read(hdr, 4) != 4)
					return res;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r = {};
					s.skip(2);
					uint16_t flg;
					s.getC<endianness::LITTLE_ENDIAN>(flg);
					r.is_encrypted = (flg & 1) != 0;
					s.getC<endianness::LITTLE_ENDIAN>(r.method);
					s.skip(4);
					s.getC<endianness::LITTLE_ENDIAN>(r.crc32);
					s.getC<endianness::LITTLE_ENDIAN>(r.f_size);
					s.getC<endianness::LITTLE_ENDIAN>(r.fsize);
					uint16_t szfn, szex;
					s.getC<endianness::LITTLE_ENDIAN>(szfn);
					s.getC<endianness::LITTLE_ENDIAN>(szex);
					r.fname.resize(szfn);
					s.read(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn);
					if(szex != 0)
					{
						r.ext.resize(szex);
						s.read(r.ext.data(), szex);
					}
					r.f_pos = s.get_pos();
					res.push_back(r);
					s.skip(r.f_size);
				}
				else
					return res;
			}
		}
	}
}
