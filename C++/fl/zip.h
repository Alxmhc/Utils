namespace fl_pr
{
	class F_zip
	{
		br_fstream br;

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

		struct infF
		{
			size_t data_pos;
			size_t data_size;
			std::string name;

			uint_fast32_t fsize;
			uint8_t crc32[4];

			uint_fast16_t method;
			uint_fast8_t  encryption;
		};
		std::vector<infF> infFs;

		std::vector<uint8_t> psw;

		void read_inf()
		{
			for(;;)
			{
				uint8_t hdr[4];
				if(!br.readN(hdr, 4))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) != 0)
					break;
				uint8_t h[26];
				if(!br.readN(h, 26))
					break;
				infF r;
				r.encryption = h[2] & 1;
				r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+4);
				std::copy_n(h+10, 4, r.crc32);
				r.data_size = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+14);
				r.fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+18);

				const auto szfn = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+22);
				if( !br.readN(r.name, szfn) )
					break;

				const auto szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
				if(szex != 0)
				{
					std::vector<uint8_t> ext;
					if( !br.readN(ext, szex) )
						break;
					if(r.method == 99)
					{
						if(szex < 11)
							break;
						r.encryption = ext[8] + 1;
						r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(ext.data() + 9);
					}
				}
				r.data_pos = br.get_pos();
				if( !br.skip(r.data_size) )
					break;
				infFs.push_back(r);
			}
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

		struct iv_zip
		{
			static void incr(uint8_t* v, const uint_fast8_t sz)
			{
				for(uint_fast8_t i = 0; i < sz; i++)
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
			h.Update(data.data(), data.size());
			uint8_t hs[hash::SHA1::hash_size];
			h.Final(hs);
			if(std::memcmp(hsh, hs, 10) != 0)
				return false;

			std::vector<uint8_t> res;
			res.reserve(data.size());
			bw_array bw(res);
			AES a(key.data(), ssz*2);
			const uint8_t iv[AES::block_size] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			CR_CTR::Decoder<AES, iv_zip> cr(a, iv, bw);
			cr.writeN(data.data(), data.size());
			cr.Fin();
			data = res;
			return true;
		}

		bool Decrypt(const infF &inf, std::vector<uint8_t> &data)
		{
			switch(inf.encryption)
			{
			case eNO:
				break;
			case eZIP:
				if( !decryptZIP(data) )
					return false;
				break;
			case eAES128:
				if( !decryptAES(8, data) )
					return false;
				break;
			case eAES192:
				if( !decryptAES(12, data) )
					return false;
				break;
			case eAES256:
				if( !decryptAES(16, data) )
					return false;
				break;
			default:
				return false;
			}
			return true;
		}

		bool Decompress(const infF &inf, std::vector<uint8_t> &data)
		{
			switch(inf.method)
			{
			case cNO:
				break;
			case cDeflate:
			case cDeflate64:
				{
				std::vector<uint8_t> tmp;
				tmp.reserve(inf.fsize);
				if( !compr::deflate::Decode(data.data(), data.size(), tmp) )
					return false;
				data = tmp;
				break;
				}
			default:
				return false;
			}
			return true;
		}

		void getData(size_t n, std::vector<uint8_t> &data)
		{
			br.set_pos(infFs[n].data_pos);
			br.readN(data, infFs[n].data_size);
		}
	public:
		bool open(const char* fl)
		{
			infFs.clear();
			if( !br.open(fl) )
				return false;
			read_inf();
			return true;
		}
		void close()
		{
			br.close();
		}

		size_t sz() const
		{
			return infFs.size();
		}

		std::vector<std::string> names() const
		{
			const size_t sz = infFs.size();
			std::vector<std::string> res(sz);
			for(size_t i = 0; i < sz; i++)
			{
				res[i] = infFs[i].name;
			}
			return res;
		}

		void set_psw(const uint8_t* passw, size_t psz)
		{
			psw.assign(passw, passw + psz);
		}

		bool GetData(size_t n, std::vector<uint8_t> &data)
		{
			if(infFs[n].fsize == 0)
			{
				data.clear();
				return true;
			}
			getData(n, data);
			if( !Decrypt(infFs[n], data) )
				return false;
			if( !Decompress(infFs[n], data) )
				return false;
			return true;
		}
	};
}
