#ifndef H_FL_KDBX
#define H_FL_KDBX

#include "../data/hash/sha2.h"
#include "../data/crypt/aes.h"
#include "../data/crypt/cr_m.h"
#include "fl_gzip.h"
#include "../data/convert/base64.h"
#include "../data/crypt/salsa.h"

namespace fl_pr
{
	class F_kdbx
	{
		byteReader* br;
		std::size_t data_pos;

		uint_fast8_t ver;

		uint8_t t_seed[32];
		std::vector<uint8_t> m_seed;
		uint_fast64_t tr_rnd;
		uint8_t key[32];

		uint8_t chck[32];
		std::vector<uint8_t> iv;

		std::string comment;
		bool compressed;
		uint32_t s_id;
		uint8_t p_key[32];

		bool read_1()
		{
			if(!br->skip(8))
				return false;
			if(!br->readN(m_seed, 16))
				return false;
			if(!br->readN(iv, 16))
				return false;
			if(!br->skip(8))
				return false;
			if(!br->readN(chck, 32))
				return false;
			if(!br->readN(t_seed, 32))
				return false;
			if(!br->readC<endianness::LE>(4, tr_rnd))
				return false;
			return true;
		}

		bool read_2()
		{
			if (!br->skip(4))
				return false;
			for (;;)
			{
				uint8_t t;
				if (!br->get(t))
					return false;
				uint_fast16_t sz;
				if (!br->readC<endianness::LE>(2, sz))
					return false;
				if (t == 0)
				{
					if (!br->skip(sz))
						return false;
					return true;
				}
				switch(t)
				{
				case 1:
				{
					if (!br->readN(comment, sz))
						return false;
					break;
				}
				case 2:
				{
					if (sz != 16)
						return false;
					auto h = br->read_data(16);
					if (h == nullptr)
						return false;
					if (std::memcmp(h, "\x31\xc1\xf2\xe6\xbf\x71\x43\x50\xbe\x58\x05\x21\x6a\xfc\x5a\xff", 16) != 0)
						return false;
					break;
				}
				case 3:
				{
					if (sz != 4)
						return false;
					uint32_t c;
					if (!br->readC<endianness::LE>(4, c))
						return false;
					if (c > 1)
						return false;
					compressed = c == 1;
					break;
				}
				case 4:
				{
					if (sz != 32)
						return false;
					if (!br->readN(m_seed, 32))
						return false;
					break;
				}
				case 5:
				{
					if (sz != 32)
						return false;
					if (!br->readN(t_seed, 32))
						return false;
					break;
				}
				case 6:
				{
					if (sz != 8)
						return false;
					if (!br->readC<endianness::LE>(8, tr_rnd))
						return false;
					break;
				}
				case 7:
				{
					if (!br->readN(iv, sz))
						return false;
					break;
				}
				case 8:
				{
					std::vector<uint8_t> tmp;
					if (!br->readN(tmp, sz))
						return false;
					hash::SHA2_256 hs;
					hs.Update(tmp.data(), sz);
					hs.Final(p_key);
					break;
				}
				case 9:
				{
					if (sz != 32)
						return false;
					if (!br->readN(chck, 32))
						return false;
					break;
				}
				case 10:
				{
					if (sz != 4)
						return false;
					if (!br->readC<endianness::LE>(4, s_id))
						return false;
					break;
				}
				default:
					return false;
				}
			}
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
					{
						bw_vector bw(tmp);
						convert::base64::Decoder bde(bw);
						bde.writeN(&(*e) + 17, sz);
						bde.Fin();
					}
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
		bool read(byteReader &r)
		{
			br = &r;
			{
				auto hdr = r.read_data(8);
				if (hdr == nullptr)
					return false;
				if (std::memcmp(hdr, "\x03\xd9\xa2\x9a", 4) != 0)
					return false;
				if (std::memcmp(hdr + 5, "\xfb\x4b\xb5", 3) != 0)
					return false;
				ver = hdr[4] - 0x64;
			}
			if (ver == 1)
			{
				if (!read_1())
					return false;
			}
			else
			{
				if (!read_2())
					return false;
			}
			if ((r.get_rsize() & 15) != 0)
				return false;
			data_pos = r.get_pos();
			return true;
		}

		void set_psw(const uint8_t* passw, std::size_t psz)
		{
			hash::SHA2_256 hs;
			hs.Update(passw, psz);
			hs.Final(key);
			if (ver != 1)
			{
				hs.Update(key, 32);
				hs.Final(key);
			}
			crypt::AES::Enc en(t_seed, 32);
			for (uint_fast64_t i = 0; i < tr_rnd; i++)
			{
				en.process(key);
				en.process(key + 16);
			}
			hs.Update(key, 32);
			hs.Final(key);
			hs.Update(m_seed.data(), m_seed.size());
			hs.Update(key, 32);
			hs.Final(key);
		}

		bool GetData(byteWriter &bw)
		{
			br->set_pos(data_pos);

			crypt::AES::Dec de(key, 32);
			crypt::CR_CBC::Dec<crypt::AES> dec(de, iv.data());

			if(ver != 1)
			{
				uint8_t st[32];
				if (!br->readN(st, 32))
					return false;
				dec.process(st);
				dec.process(st + 16);
				if (std::memcmp(st, chck, 32) != 0)
					return false;
			}

			std::vector<uint8_t> data;
			br->readN(data, br->get_rsize());
			for (std::size_t i = 0; i < data.size(); i += 16)
			{
				dec.process(data.data() + i);
			}

			if (ver == 1)
			{
				const uint_fast8_t pad = data.back();
				if (pad > data.size())
					return false;
				data.resize(data.size() - pad);
				{
					hash::SHA2_256 hs;
					hs.Update(data.data(), data.size());
					uint8_t h[32];
					hs.Final(h);
					if (std::memcmp(h, chck, 32) != 0)
						return false;
				}
				bw.writeN(data.data(), data.size());
				return true;
			}

			{
				std::size_t hb = 0;
				hash::SHA2_256 hs;
				for (;;)
				{
					const std::size_t he = hb + 40;
					if (he > data.size())
						return false;
					uint32_t sz;
					bconv<1, endianness::LE>::pack(data.data() + hb + 36, 4, sz);
					if (sz == 0)
					{
						data.resize(hb);
						break;
					}
					if (he + sz > data.size())
						return false;

					hs.Update(data.data() + he, sz);
					uint8_t hash[32];
					hs.Final(hash);
					if (std::memcmp(data.data() + hb + 4, hash, 32) != 0)
						return false;

					data.erase(data.begin() + hb, data.begin() + he);
					hb += sz;
				}
			}

			if(compressed)
			{
				br_array bgz(data.data(), data.size());
				fl_pr::F_gzip gz;
				if(!gz.read(bgz))
					return false;
				std::vector<uint8_t> out;
				bw_vector bwz(out);
				if(!gz.GetData(bwz))
					return false;
				data = std::move(out);
			}

			if(!key_decode(data))
				return false;

			bw.writeN(data.data(), data.size());

			return true;
		}
	};
}

#endif
