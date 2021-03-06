namespace fl_pr
{
	class F_zip
	{
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
			if(!s.readN(h, 26))
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
			if( !s.readN(reinterpret_cast<uint8_t*>(&r.fname[0]), szfn) )
				return false;

			uint_fast16_t szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
			if(szex != 0)
			{
				r.Hsize += szex;
				std::vector<uint8_t> ext;
				if( !s.readN(ext, szex) )
					return false;
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

		static bool decryptZIP(const uint8_t *passw, size_t psz, std::vector<uint8_t> &res)
		{
			if(res.size() <= 12)
				return false;

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
			return true;
		}

		static bool decryptAES(const uint8_t *passw, size_t psz, uint_fast8_t ssz, std::vector<uint8_t> &data)
		{
			if(data.size() <= ssz + 12)
				return false;
			auto key = PBKDF2<PBKDF2_HMAC<hash::SHA1>>(passw, psz, data.data(), ssz, 1000, ssz*4 + 2);
			if(key[ssz*4] != data[ssz] || key[ssz*4+1] != data[ssz+1])
				return false;

			uint8_t hsh[10];
			std::copy_n(data.end() - 10, 10, hsh);
			data = std::vector<uint8_t>(data.begin() + ssz + 2, data.end() - 10);

			hash::HMAC<hash::SHA1> h;
			h.SetKey(key.data() + ssz*2, ssz*2);
			h.Init();
			h.Update(data.data(), data.size());
			uint8_t hs[hash::SHA1::hash_size];
			h.Final(hs);
			if(std::memcmp(hsh, hs, 10) != 0)
				return false;

			AESCTRDecryptLE(key.data(), ssz*2, data);
			return true;
		}
	public:
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

		static std::vector<inf> read_inf(byteReader &s)
		{
			std::vector<inf> res;
			for(;;)
			{
				uint8_t hdr[4];
				if(!s.readN(hdr, 4))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r;
					if( !read_file_hdr(s, r) )
						break;
					r.isDir = (r.fname[r.fname.length() - 1] == '/');
					res.push_back(r);
					s.skip(r.Dsize);
				}
				else
					break;
			}
			return res;
		}

		static bool Decrypt(byteReader &s, const inf &inf, const uint8_t *passw, size_t psz, std::vector<uint8_t> &data)
		{
			s.set_pos(inf.Hpos + inf.Hsize);
			if( !s.readN(data, inf.Dsize) )
				return false;
			switch(inf.encryption)
			{
			case eZIP:
				return decryptZIP(passw, psz, data);
			case eAES128:
				return decryptAES(passw, psz, 8, data);
			case eAES192:
				return decryptAES(passw, psz, 12, data);
			case eAES256:
				return decryptAES(passw, psz, 16, data);
			}
			return false;
		}
	};
}
