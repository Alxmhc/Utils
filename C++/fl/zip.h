namespace fl_pr
{
	class F_zip
	{
		struct inf
		{
			size_t data_pos;
			size_t data_size;

			std::string fname;
			uint_fast32_t fsize;
			bool isDir;
			uint8_t crc32[4];

			uint_fast16_t method;
			uint_fast8_t encryption;
		};

		br_fstream br;
		std::vector<inf> f_inf;
		std::vector<uint8_t> psw;

		bool read_inf(inf &r)
		{
			uint8_t h[26];
			if(!br.readN(h, 26))
				return false;
			r.encryption = h[2] & 1;
			r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+4);
			std::copy_n(h+10, 4, r.crc32);
			r.data_size = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+14);
			r.fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+18);

			const auto szfn = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+22);
			if( !br.readN(r.fname, szfn) )
				return false;
			r.isDir = (r.fname[r.fname.length() - 1] == '/');

			const auto szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
			if(szex != 0)
			{
				std::vector<uint8_t> ext;
				if( !br.readN(ext, szex) )
					return false;
				if(r.method == 99)
				{
					if(szex < 11)
						return false;
					r.encryption = ext[8] + 1;
					r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(ext.data() + 9);
				}
			}
			r.data_pos = br.get_pos();
			return true;
		}

		static void keyUpd(uint32_t* key, uint8_t c, const uint32_t* tbl)
		{
			key[0] = tbl[(key[0] & 0xff) ^ c] ^ (key[0] >> 8);
			key[1] = ( key[1] + (key[0] & 0xff) ) * 0x8088405 + 1;
			key[2] = tbl[(key[2] ^ (key[1]>>24)) & 0xff] ^ (key[2] >> 8);
		}
		bool decryptZIP(std::vector<uint8_t> &data) const
		{
			if(psw.size() == 0)
				return false;
			if(data.size() <= 12)
				return false;

			uint32_t crcTbl[256];
			hash::CRC32::InitTable(crcTbl, 0xedb88320);

			uint32_t key[3] = {0x12345678, 0x23456789, 0x34567890};
			for(size_t i = 0; i < psw.size(); i++)
			{
				keyUpd(key, psw[i], crcTbl);
			}

			for(size_t i = 0; i < data.size(); i++)
			{
				uint32_t tmp = key[2] | 2;
				tmp = (tmp * (tmp ^ 1)) >> 8;
				data[i] ^= tmp & 0xff;

				keyUpd(key, data[i], crcTbl);
			}
			data.erase(data.begin(), data.begin() + 12);
			return true;
		}

		class iv_aes
		{
			uint8_t v[16];
		public:
			iv_aes()
			{
				static const uint8_t iv[16] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
				std::copy(iv, iv + 16, v);
			}
			const uint8_t* data() const
			{
				return v;
			}
			void incr()
			{
				for(uint_fast8_t i = 0; i < 16; i++)
				{
					if(v[i] != 255)
					{
						v[i]++;
						break;
					}
					v[i] = 0;
				}
			}
		};
		bool decryptAES(uint_fast8_t ssz, std::vector<uint8_t> &data) const
		{
			if(psw.size() == 0)
				return false;
			if(data.size() <= static_cast<uint_fast8_t>(ssz + 12))
				return false;

			auto key = PBKDF2<PBKDF2_HMAC<hash::SHA1>>(psw.data(), psw.size(), data.data(), ssz, 1000, ssz*4 + 2);
			if(key[ssz*4] != data[ssz] || key[ssz*4+1] != data[ssz+1])
				return false;

			uint8_t hsh[10];
			std::copy_n(data.end() - 10, 10, hsh);
			data = std::vector<uint8_t>(data.begin() + ssz + 2, data.end() - 10);

			hash::HMAC<hash::SHA1> h(key.data() + ssz*2, ssz*2);
			h.Init();
			h.Update(data.data(), data.size());
			uint8_t hs[hash::SHA1::hash_size];
			h.Final(hs);
			if(std::memcmp(hsh, hs, 10) != 0)
				return false;

			std::vector<uint8_t> res;
			res.reserve(data.size());
			bw_array bw(res);
			AES a(key.data(), ssz*2);
			CR_CTR::Decoder<AES, iv_aes> cr(a, bw);
			cr.writeN(data.data(), data.size());
			cr.Fin();
			data = res;
			return true;
		}

		enum
		{
			eNO       =  0,
			eZIP      =  1,
			eAES128   =  2,
			eAES192   =  3,
			eAES256   =  4
		};
		enum
		{
			cNO        =   0,
			cDeflate   =   8,
			cDeflate64 =   9,
			cBZIP2     =  12,
			cLZMA      =  14,
			cWavPack   =  97,
			cPPMd      =  98
		};
	public:
		bool open(const char* fl)
		{
			f_inf.clear();
			if( !br.open(fl) )
				return false;
			for(;;)
			{
				uint8_t hdr[4];
				if(!br.readN(hdr, 4))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r;
					if( !read_inf(r) )
						break;
					if( !br.skip(r.data_size) )
						break;
					f_inf.push_back(r);
				}
				else
					break;
			}
			return true;
		}

		size_t sz() const
		{
			return f_inf.size();
		}

		std::vector<std::string> names() const
		{
			std::vector<std::string> res(f_inf.size());
			for(size_t i = 0; i < f_inf.size(); i++)
			{
				res[i] = f_inf[i].fname;
			}
			return res;
		}

		void set_psw(const uint8_t* passw, size_t psz)
		{
			psw.assign(passw, passw + psz);
		}

		bool getData(size_t n, std::vector<uint8_t> &data)
		{
			br.set_pos(f_inf[n].data_pos);
			br.readN(data, f_inf[n].data_size);
			switch(f_inf[n].encryption)
			{
			case eNO:
				return true;
			case eZIP:
				return decryptZIP(data);
			case eAES128:
				return decryptAES(8, data);
			case eAES192:
				return decryptAES(12, data);
			case eAES256:
				return decryptAES(16, data);
			default:
				return false;
			}
		}

		bool Extract(size_t n, std::vector<uint8_t> &data)
		{
			if( !getData(n, data) )
				return false;
			switch(f_inf[n].method)
			{
			case cNO:
				return true;
			case cDeflate:
				{
				std::vector<uint8_t> tmp;
				tmp.reserve(f_inf[n].fsize);
				if( !compr::deflate::Decode(data.data(), data.size(), tmp) )
					return false;
				data = tmp;
				return true;
				}
			default:
				return false;
			}
		}
	};
}
