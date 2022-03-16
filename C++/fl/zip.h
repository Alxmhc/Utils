namespace fl_pr
{
	class F_zip
	{
		struct inf
		{
			size_t hdr_pos;
			size_t hdr_size;
			size_t data_pos;
			size_t data_size;

			std::string fname;
			uint_fast32_t fsize;
			uint8_t crc32[4];

			uint_fast16_t method;
			uint_fast8_t encryption;
			bool isDir;
		};

		br_fstream br;
		std::vector<inf> f_inf;

		bool read_file_hdr(inf &r)
		{
			uint8_t h[26];
			if(!br.readN(h, 26))
				return false;
			r.hdr_pos = br.get_pos() - 26;
			r.hdr_size = 26;

			r.encryption = h[2] & 1;
			r.method = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+4);
			std::copy_n(h+10, 4, r.crc32);
			r.data_size = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+14);
			r.fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(h+18);

			uint_fast16_t szfn = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+22);
			r.hdr_size += szfn;
			if( !br.readN(r.fname, szfn) )
				return false;
			r.isDir = (r.fname[r.fname.length() - 1] == '/');

			uint_fast16_t szex = bconv<2, endianness::LITTLE_ENDIAN>::pack(h+24);
			if(szex != 0)
			{
				r.hdr_size += szex;
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
			r.data_pos = r.hdr_pos + r.hdr_size;
			return true;
		}
		void read_inf()
		{
			for(;;)
			{
				uint8_t hdr[4];
				if(!br.readN(hdr, 4))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) == 0)
				{
					inf r;
					if( !read_file_hdr(r) )
						break;
					f_inf.push_back(r);
					br.skip(r.data_size);
				}
				else
					break;
			}
		}

		static void keyUpd(uint32_t *key, uint8_t c, const uint32_t *tbl)
		{
			key[0] = tbl[(key[0] & 0xff) ^ c] ^ (key[0] >> 8);
			key[1] = ( key[1] + (key[0] & 0xff) ) * 0x8088405 + 1;
			key[2] = tbl[(key[2] ^ (key[1]>>24)) & 0xff] ^ (key[2] >> 8);
		}
		static bool decryptZIP(const uint8_t *passw, size_t psz, std::vector<uint8_t> &data, byteWriter &bw)
		{
			if(data.size() <= 12)
				return false;

			uint32_t crcTbl[256];
			hash::CRC32::InitTable(crcTbl, 0xedb88320);

			uint32_t key[3] = {0x12345678, 0x23456789, 0x34567890};
			for(size_t i = 0; i < psz; i++)
			{
				keyUpd(key, passw[i], crcTbl);
			}

			for(size_t i = 0; i < data.size(); i++)
			{
				uint32_t tmp = key[2] | 2;
				tmp = (tmp * (tmp ^ 1)) >> 8;
				data[i] ^= tmp & 0xff;

				keyUpd(key, data[i], crcTbl);
			}
			bw.writeN(data.data() + 12, data.size() - 12);
			bw.Fin();
			return true;
		}

		class iv_aes
		{
			uint8_t v[AES::block_size];
		public:
			iv_aes()
			{
				std::fill_n(v, AES::block_size, 0);
			}

			const uint8_t* data() const
			{
				return v;
			}

			void incr()
			{
				for(size_t i = 0; i < AES::block_size; i++)
				{
					v[i]++;
					if(v[i] != 0)
						break;
				}
			}
		};
		static bool decryptAES(const uint8_t *passw, size_t psz, uint_fast8_t ssz, std::vector<uint8_t> &data, byteWriter &bw)
		{
			if(data.size() <= static_cast<uint_fast8_t>(ssz + 12))
				return false;
			auto key = PBKDF2<PBKDF2_HMAC<hash::SHA1>>(passw, psz, data.data(), ssz, 1000, ssz*4 + 2);
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

			AES a(key.data(), ssz*2);
			CR_CTR<AES::en, iv_aes> cr(a.Enc, bw);
			cr.writeN(data.data(), data.size());
			cr.Fin();
			return true;
		}

		bool read(size_t n, std::vector<uint8_t> &data)
		{
			br.set_pos(f_inf[n].data_pos);
			if( !br.readN(data, f_inf[n].data_size) )
				return false;
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

		bool open(const char* fl)
		{
			f_inf.clear();
			if( !br.open(fl) )
				return false;
			read_inf();
			return true;
		}

		bool getData(size_t n, byteWriter &bw)
		{
			std::vector<uint8_t> data;
			if(!read(n, data))
				return false;
			bw.writeN(data.data(), data.size());
			bw.Fin();
			return true;
		}

		bool getDataK(size_t n, const uint8_t *passw, size_t psz, byteWriter &bw)
		{
			std::vector<uint8_t> data;
			if(!read(n, data))
				return false;
			switch(f_inf[n].encryption)
			{
			case eZIP:
				return decryptZIP(passw, psz, data, bw);
			case eAES128:
				return decryptAES(passw, psz, 8, data, bw);
			case eAES192:
				return decryptAES(passw, psz, 12, data, bw);
			case eAES256:
				return decryptAES(passw, psz, 16, data, bw);
			default:
				return false;
			}
		}
	};
}
