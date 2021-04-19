namespace fl_pr
{
	class F_zip
	{
		enum
		{
			cNO        =   0,
			cDeflate   =   8,
			cDeflate64 =   9,
			cBZIP2     =  12,
			cLZMA      =  14,
			cPPMd      =  98
		};
		enum
		{
			eNO       =  0,
			eZIP      =  1,
			eAES128   =  2,
			eAES192   =  3,
			eAES256   =  4
		};

		struct inf
		{
			size_t Hpos;
			uint_fast16_t Hsize;

			uint_fast8_t encryption;
			uint_fast16_t method;
			uint8_t crc32[4];
			uint_fast32_t Dsize;
			uint_fast32_t fsize;
			std::string fname;
			bool isDir;
		};

		static bool read_file_hdr(byteReader &s, inf &r)
		{
			uint8_t h[26];
			if(!s.readN(h, sizeof(h)))
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
			s.readN(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn);

			uint_fast16_t szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
			if(szex != 0)
			{
				r.Hsize += szex;
				std::vector<uint8_t> ext(szex);
				s.readN(ext.data(), szex);
				if(r.method == 99)
				{
					r.encryption = ext[8] + 1;
					r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(ext.data() + 9);
				}
			}
			return true;
		}

		static void keyUpd(uint32_t *key, uint8_t c, const uint32_t *tbl)
		{
			key[0] = tbl[(key[0] & 0xff) ^ c] ^ (key[0] >> 8);
			key[1] = ( key[1] + (key[0] & 0xff) ) * 0x8088405 + 1;
			key[2] = tbl[(key[2] ^ (key[1]>>24)) & 0xff] ^ (key[2] >> 8);
		}

		static void decryptZIP(const uint8_t *passw, size_t psz, std::vector<uint8_t> &res)
		{
			uint32_t crcTbl[256];
			hash::CRC32::InitTable(crcTbl, 0xedb88320);

			uint32_t key[3] = {0x12345678, 0x23456789, 0x34567890};
			for(size_t i = 0; i < psz; i++)
			{
				keyUpd(key, passw[i], crcTbl);
			}

			for(size_t i = 0; i < res.size(); i++)
			{
				uint32_t tmp = key[2] | 2;
				tmp = (tmp * (tmp ^ 1)) >> 8;
				res[i] ^= tmp & 0xff;

				keyUpd(key, res[i], crcTbl);
			}
			res.erase(res.begin(), res.begin() + 12);
		}
	public:
		static std::vector<inf> read_inf(byteReader &s)
		{
			std::vector<inf> res;
			for(;;)
			{
				uint8_t hdr[4];
				if(!s.readN(hdr, sizeof(hdr)))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r;
					if( !read_file_hdr(s, r) )
						break;
					r.isDir = (r.fname[r.fname.length() - 1] == '/');
					res.push_back(r);
					s.set_pos(r.Dsize, std::ios_base::cur);
				}
				else
					break;
			}
			return res;
		}

		static std::vector<uint8_t> Decrypt(byteReader &s, const inf &inf, const uint8_t *passw, size_t psz)
		{
			switch(inf.encryption)
			{
			case eZIP:
				s.set_pos(inf.Hpos + inf.Hsize, std::ios_base::beg);
				std::vector<uint8_t> res(inf.Dsize);
				s.readN(res.data(), inf.Dsize);
				decryptZIP(passw, psz, res);
				return res;
			}
			return std::vector<uint8_t>();
		}
	};
}
