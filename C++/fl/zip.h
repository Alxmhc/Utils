namespace fl_pr
{
	namespace F_zip
	{
		namespace comprType
		{
			enum
			{
				NO        =   0,
				Deflate   =   8,
				Deflate64 =   9,
				BZIP2     =  12,
				LZMA      =  14,
				PPMd      =  98
			};
		}
		namespace encrType
		{
			enum
			{
				NO       =  0,
				UNKNOWN  =  1,
				AES128   =  2,
				AES192   =  3,
				AES256   =  4
			};
		}

		struct inf
		{
			uint_fast8_t encryption;
			std::string fname;
			uint16_t method;
			uint32_t fsize, crc32;
			fl_inf f_inf;
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
					r.encryption = flg & 1;
					s.getC<endianness::LITTLE_ENDIAN>(r.method);
					s.skip(4);
					s.getC<endianness::LITTLE_ENDIAN>(r.crc32);
					uint32_t sz;
					s.getC<endianness::LITTLE_ENDIAN>(sz);
					s.getC<endianness::LITTLE_ENDIAN>(r.fsize);
					uint16_t szfn, szex;
					s.getC<endianness::LITTLE_ENDIAN>(szfn);
					s.getC<endianness::LITTLE_ENDIAN>(szex);
					r.fname.resize(szfn);
					s.read(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn);
					if(szex != 0)
					{
						std::vector<uint8_t> ext(szex);
						s.read(ext.data(), szex);
						if(r.method == 99)
						{
							r.encryption = ext[8] + 1;
							r.method = ext[9] | (ext[10]<<8);
						}
					}
					if(r.fname[szfn-1] == '/') //folder
						continue;
					r.f_inf.pos = s.get_pos();
					r.f_inf.size = sz;
					s.skip(sz);
					res.push_back(r);
				}
				else
					return res;
			}
		}
	}
}
