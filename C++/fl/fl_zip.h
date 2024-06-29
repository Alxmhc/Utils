#ifndef H_FL_ZIP
#define H_FL_ZIP

#include "../cont.h"
#include "../data/hash/sha1.h"
#include "../data/crypt/key/pbkdf2.h"
#include "../data/crypt/aes.h"
#include "../data/crypt/cr_m.h"
#include "../data/compr/deflate.h"
#include "../data/compr/bzip2.h"
#include "../data/compr/lzma.h"

namespace fl_pr
{
	class F_zip : public cont_n
	{
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
			std::string fname;

			uint_fast32_t fsize;
			uint8_t crc32[4];

			uint_fast16_t method;
			uint_fast8_t  encryption;
		};
		std::vector<infF> infFs;

		std::vector<uint8_t> psw;

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
			for(std::size_t i = 0; i < psw.size(); i++)
			{
				keyUpd(key, psw[i], crcTbl);
			}

			for(std::size_t i = 0; i < data.size(); i++)
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

			const PBKDF2<PBKDF2_HMAC<hash::SHA1>> kg(1000);
			std::vector<uint8_t> key(ssz*4 + 2);
			kg.gen(psw.data(), psw.size(), data.data(), ssz, key.data(), key.size());
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

			const uint8_t iv[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			crypt::AES::Enc de(key.data(), ssz*2);
			crypt::CR_CTR::Decr<crypt::AES, iv_zip> cr(de, iv);
			cr.process(data.data(), data.size());
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

		static bool Decompress(const infF &inf, byteReader &br, byteWriter &bw)
		{
			switch(inf.method)
			{
			case cNO:
				copy(br, bw);
				break;
			case cDeflate:
			case cDeflate64:
				if( !compr::deflate::Decode(br, bw) )
					return false;
				break;
			case cBZIP2:
				if( !compr::bzip2::Decode(br, bw) )
					return false;
				break;
			case cLZMA:
			{
				if( !br.skip(4) )
					return false;
				if( !compr::lzma::Decode(br, bw, inf.fsize) )
					return false;
				break;
			}
			default:
				return false;
			}
			return true;
		}

		bool read_hdr(infF &inf, inf_1 &p)
		{
			uint8_t h[26];
			if(!br->readN(h, 26))
				return false;
			inf.encryption = h[2] & 1;
			inf.method = bconv<1, 2, endianness::LITTLE_ENDIAN>::pack(h+4);
			std::copy_n(h+10, 4, inf.crc32);
			p.data_size = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(h+14);
			inf.fsize = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(h+18);

			const auto szfn = bconv<1, 2, endianness::LITTLE_ENDIAN>::pack(h+22);
			if( !br->readN(inf.fname, szfn) )
				return false;

			auto szex = bconv<1, 2, endianness::LITTLE_ENDIAN>::pack(h+24);
			if(szex != 0)
			{
				if(inf.method == 99)
				{
					if(szex < 11)
						return false;
					uint8_t ext[11];
					if( !br->readN(ext, 11) )
						return false;
					inf.encryption = ext[8] + 1;
					inf.method = bconv<1, 2, endianness::LITTLE_ENDIAN>::pack(ext + 9);
					szex -= 11;
				}
				if( !br->skip(szex) )
					return false;
			}
			p.data_pos = br->get_pos();
			return true;
		}
	public:
		bool read(byteReader* r)
		{
			br = r;

			inf_n.clear();
			infFs.clear();
			for(;;)
			{
				uint8_t hdr[4];
				if(!br->readN(hdr, 4))
					break;
				if(std::memcmp(hdr, "\x50\x4b\x03\x04", 4) != 0)
					break;
				infF inf;
				inf_1 p;
				if( !read_hdr(inf, p) )
					break;
				if( !br->skip(p.data_size) )
					break;
				inf_n.push_back(p);
				infFs.push_back(inf);
			}
			return true;
		}

		std::string name(std::size_t n) const
		{
			return infFs[n].fname;
		}

		void set_psw(const uint8_t* passw, std::size_t psz)
		{
			psw.assign(passw, passw + psz);
		}

		bool GetData(std::size_t n, byteWriter &bw)
		{
			if(infFs[n].fsize == 0)
				return true;
			Init(n);
			if(infFs[n].encryption == eNO)
			{
				if( !Decompress(infFs[n], *br, bw) )
					return false;
				return true;
			}

			std::vector<uint8_t> data;
			br->readN(data, br->get_rsize());
			if( !Decrypt(infFs[n], data) )
				return false;

			if(infFs[n].method == cNO)
			{
				bw.writeN(data.data(), data.size());
				return true;
			}
			br_array b(data.data(), data.size());
			if( !Decompress(infFs[n], b, bw) )
				return false;
			return true;
		}
	};
}

#endif
