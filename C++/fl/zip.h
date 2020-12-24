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
			std::size_t Hpos;
			uint_fast16_t Hsize;

			uint_fast8_t encryption;
			uint_fast16_t method;
			uint8_t crc32[4];
			uint_fast32_t Dsize;
			uint_fast32_t fsize;
			std::string fname;
		};

		bool read_file_hdr(byteReader &s, inf &r)
		{
			uint8_t h[26];
			if(s.read(h, sizeof(h)) != sizeof(h))
				return false;
			r.Hpos = s.get_pos() - 26;
			r.Hsize = 26;

			r.encryption = h[2] & 1;
			r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+4);
			std::copy_n(h+10, 4, r.crc32);
			r.Dsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+14);
			r.fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+18);

			uint_fast16_t szfn = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+22);
			r.Hsize += szfn;
			r.fname.resize(szfn);
			s.read(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn);

			uint_fast16_t szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
			r.Hsize += szex;
			if(szex != 0)
			{
				std::vector<uint8_t> ext(szex);
				s.read(ext.data(), szex);
				if(r.method == 99)
				{
					r.encryption = ext[8] + 1;
					r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(ext.data() + 9);
				}
			}
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
					s.skip(r.Dsize);
				}
				else
					break;
			}
			return res;
		}
	}
}
