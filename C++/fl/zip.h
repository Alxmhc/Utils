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

		bool read_file_hdr(byteReader &s, inf &r)
		{
			uint8_t h[26];
			if(s.read(h, sizeof(h)) != sizeof(h))
				return false;
			r.encryption = h[2] & 1;
			conv::pack<endianness::LITTLE_ENDIAN>(h+4, r.method);
			conv::pack<endianness::LITTLE_ENDIAN>(h+10, r.crc32);
			conv::pack<endianness::LITTLE_ENDIAN>(h+18, r.fsize);

			uint32_t sz;
			conv::pack<endianness::LITTLE_ENDIAN>(h+14, sz);
			r.f_inf.size = sz;

			uint16_t szfn;
			conv::pack<endianness::LITTLE_ENDIAN>(h+22, szfn);
			r.fname.resize(szfn);
			s.read(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn);

			uint16_t szex;
			conv::pack<endianness::LITTLE_ENDIAN>(h+24, szex);
			if(szex != 0)
			{
				std::vector<uint8_t> ext(szex);
				s.read(ext.data(), szex);
				if(r.method == 99)
				{
					r.encryption = ext[8] + 1;
					conv::pack<endianness::LITTLE_ENDIAN>(ext.data()+9, r.method);
				}
			}

			r.f_inf.pos = s.get_pos();
			return true;
		}

		std::vector<inf> read_inf(byteReader &s)
		{
			std::vector<inf> res;
			for(;;)
			{
				uint8_t hdr[4];
				if(s.read(hdr, sizeof(hdr)) != sizeof(hdr))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r;
					if( !read_file_hdr(s, r) )
						break;
					if( r.fname[r.fname.length() - 1] == '/' ) //folder
						continue;
					res.push_back(r);
					s.skip(r.f_inf.size);
				}
				else
					break;
			}
			return res;
		}
	}
}
