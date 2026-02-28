#ifndef H_KDBX
#define H_KDBX

#include "../data/hash/sha2.h"
#include "../data/crypt/aes.h"
#include "../data/crypt/cr_m.h"
#include "fl_gzip.h"
#include "../data/convert/base64.h"
#include "../data/crypt/salsa.h"

namespace fl_pr
{
	class kdbx
	{
		uint_fast8_t ver;

		bool read_Ver(byteReader &br)
		{
			uint8_t hdr[8];
			if( !br.readN(hdr, 8) )
				return false;
			if(std::memcmp(hdr, "\x03\xd9\xa2\x9a", 4) != 0)
				return false;
			if(std::memcmp(hdr + 5, "\xfb\x4b\xb5", 3) != 0)
				return false;
			ver = hdr[4] - 0x64;
			return true;
		}

		bool g;
		uint8_t key[32];

		void make_key(const uint8_t* t_seed, uint_fast64_t tr_rnd, const uint8_t* m_seed, uint_fast8_t msz)
		{
			crypt::AES::Enc en(t_seed, 32);
			for(uint_fast64_t i = 0; i < tr_rnd; i++)
			{
				en.process(key);
				en.process(key + 16);
			}

			hash::SHA2_256 hs;
			hs.Update(key, 32);
			hs.Final(key);
			hs.Update(m_seed, msz);
			hs.Update(key, 32);
			hs.Final(key);
		}

		std::string comment;
		bool compressed;
		std::vector<uint8_t> data;
		uint32_t s_id;
		uint8_t p_key[32];

		bool Decrypt1(byteReader &br)
		{
			uint32_t f;
			if(!br.readC<4, endianness::LITTLE_ENDIAN>(f))
				return false;
			if(!br.skip(4))
				return false;

			uint8_t iv[16];
			uint8_t hash[32];
			{
				uint8_t t_seed[32];
				uint32_t tr_rnd;
				uint8_t m_seed[16];
				if(!br.readN(m_seed, 16))
					return false;
				if(!br.readN(iv, 16))
					return false;
				if(!br.skip(8))
					return false;
				if(!br.readN(hash, 32))
					return false;
				if(!br.readN(t_seed, 32))
					return false;
				if(!br.readC<4, endianness::LITTLE_ENDIAN>(tr_rnd))
					return false;
				make_key(t_seed, tr_rnd, m_seed, 16);
			}

			br.readN(data, br.get_rsize());
			if((data.size() & 15) != 0)
				return false;
			crypt::AES::Dec de(key, 32);
			crypt::CR_CBC::Dec<crypt::AES> dec(de, iv);
			for(std::size_t i = 0; i < data.size(); i += 16)
			{
				dec.process(data.data() + i);
			}
			const uint_fast8_t pad = data.back();
			if(pad > data.size())
				return false;
			data.resize(data.size() - pad);
			{
				hash::SHA2_256 hs;
				hs.Update(data.data(), data.size());
				uint8_t h[32];
				hs.Final(h);
				if(std::memcmp(h, hash, 32) != 0)
					return false;
			}
			return true;
		}

		bool Decrypt2(byteReader &br)
		{
			if(!br.skip(4))
				return false;

			std::vector<uint8_t> iv;
			uint8_t s_start[32];
			{
				uint8_t t_seed[32];
				uint64_t tr_rnd;
				uint8_t m_seed[32];
				for(;;)
				{
					uint8_t t;
					if(!br.get(t))
						return false;
					uint16_t sz;
					if(!br.readC<2, endianness::LITTLE_ENDIAN>(sz))
						return false;
					if(t == 0)
					{
						if(!br.skip(sz))
							return false;
						break;
					}
					switch(t)
					{
					case 1:
					{
						if(!br.readN(comment, sz))
							return false;
						break;
					}
					case 2:
					{
						if(sz != 16)
							return false;
						uint8_t h[16];
						if(!br.readN(h, 16))
							return false;
						if(std::memcmp(h, "\x31\xc1\xf2\xe6\xbf\x71\x43\x50\xbe\x58\x05\x21\x6a\xfc\x5a\xff", 16) != 0)
							return false;
						break;
					}
					case 3:
					{
						if(sz != 4)
							return false;
						uint32_t c;
						if(!br.readC<4, endianness::LITTLE_ENDIAN>(c))
							return false;
						if(c > 1)
							return false;
						compressed = c == 1;
						break;
					}
					case 4:
					{
						if(sz != 32)
							return false;
						if(!br.readN(m_seed, 32))
							return false;
						break;
					}
					case 5:
					{
						if(sz != 32)
							return false;
						if(!br.readN(t_seed, sz))
							return false;
						break;
					}
					case 6:
					{
						if(sz != 8)
							return false;
						if(!br.readC<8, endianness::LITTLE_ENDIAN>(tr_rnd))
							return false;
						break;
					}
					case 7:
					{
						if(!br.readN(iv, sz))
							return false;
						break;
					}
					case 8:
					{
						std::vector<uint8_t> tmp;
						if(!br.readN(tmp, sz))
							return false;
						hash::SHA2_256 hs;
						hs.Update(tmp.data(), sz);
						hs.Final(p_key);
						break;
					}
					case 9:
					{
						if(sz != 32)
							return false;
						if(!br.readN(s_start, sz))
							return false;
						break;
					}
					case 10:
					{
						if(sz != 4)
							return false;
						if(!br.readC<4, endianness::LITTLE_ENDIAN>(s_id))
							return false;
						break;
					}
					default:
						return false;
					}
				}
				make_key(t_seed, tr_rnd, m_seed, 32);
			}

			{
				crypt::AES::Dec de(key, 32);
				crypt::CR_CBC::Dec<crypt::AES> dec(de, iv.data());

				uint8_t st[32];
				if(!br.readN(st, 32))
					return false;
				dec.process(st);
				dec.process(st + 16);
				if(std::memcmp(st, s_start, 32) != 0)
					return false;

				br.readN(data, br.get_rsize());
				if((data.size() & 15) != 0)
					return false;
				for(std::size_t i = 0; i < data.size(); i += 16)
				{
					dec.process(data.data() + i);
				}
			}

			{
				std::size_t hb = 0;

				hash::SHA2_256 hs;
				for(;;)
				{
					const std::size_t he = hb + 40;
					if(he > data.size())
						return false;
					const uint32_t sz = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(data.data() + hb + 36);
					if(sz == 0)
					{
						data.resize(hb);
						break;
					}
					if(he + sz > data.size())
						return false;

					hs.Update(data.data() + he, sz);
					uint8_t hash[32];
					hs.Final(hash);
					if(std::memcmp(data.data() + hb + 4, hash, 32) != 0)
						return false;

					data.erase(data.begin() + hb, data.begin() + he);
					hb += sz;
				}
			}

			return true;
		}

		bool key_decode(std::vector<uint8_t> &out) const
		{
			const std::string s("<Value Protected=\"True\">");
			auto e = std::search(out.begin(), out.end(), s.cbegin(), s.cend());
			if(e == out.end())
				return true;

			if(s_id != 2)
				return false;

			crypt::Salsa20::Decr dec(p_key, 32, reinterpret_cast<const uint8_t*>("\xe8\x30\x09\x4b\x97\x20\x5d\x2a"));
			for(;;)
			{
				e[6] = '>';
				e += 7;
				const auto f = std::find(e, out.end(), '<');
				if(f == out.end())
					return false;
				{
					auto sz = std::distance(e, f) - 1;
					while(e[sz] == '=')
					{
						sz--;
					}
					sz -= 16;

					std::vector<uint8_t> tmp;
					bw_array bw(tmp);
					convert::base64::Decoder de(bw);
					de.writeN(&(*e) + 17, sz);
					de.Fin();
					dec.process(tmp.data(), tmp.size());
					sz = tmp.size();

					out.erase(e + sz, f);
					std::copy_n(tmp.data(), sz, e);
					e += sz;
				}
				e = std::search(e, out.end(), s.cbegin(), s.cend());
				if(e == out.end())
					return true;
			}
		}
	public:
		kdbx(byteReader &br, const uint8_t* psw, std::size_t psz) : g(false)
		{
			if(!read_Ver(br))
				return;

			hash::SHA2_256 hs;
			hs.Update(psw, psz);
			hs.Final(key);

			if(ver == 1)
			{
				g = Decrypt1(br);
			}
			else
			{
				hs.Update(key, 32);
				hs.Final(key);
				g = Decrypt2(br);
			}
			std::fill_n(key, 32, uint8_t(0));
		}

		bool GetData(byteWriter &bw)
		{
			if(!g)
				return false;

			if(ver == 1)
			{
				bw.writeN(data.data(), data.size());
				data.clear();
				return true;
			}

			std::vector<uint8_t> out;

			if(compressed)
			{
				br_array bgz(data.data(), data.size());
				fl_pr::F_gzip gz;
				if(!gz.read(&bgz))
					return false;
				bw_array bwz(out);
				if(!gz.GetData(bwz))
					return false;
				data.clear();
			}
			else
			{
				out = std::move(data);
			}

			if(!key_decode(out))
				return false;

			bw.writeN(out.data(), out.size());

			return true;
		}
	};
}

#endif
