#ifndef H_HEX
#define H_HEX

#include "base.h"
#include "../byte_writer.h"

namespace convert
{
	namespace hex
	{
		static const char* dct_lc = "0123456789abcdef";
		static const char* dct_uc = "0123456789ABCDEF";

		namespace Enc
		{
			static std::string pr_string(const uint8_t* v, std::size_t n, bool isU = false)
			{
				std::string res;
				res.reserve(n*2);
				const char* dict = isU ? dct_uc : dct_lc;
				for(std::size_t i = 0; i < n; i++)
				{
					res.push_back(dict[v[i]>>4]);
					res.push_back(dict[v[i]&0x0f]);
				}
				return res;
			}

			static std::string pr_num(std::size_t n, bool isU = false)
			{
				return base::Enc::pr_num(n, isU ? dct_uc : dct_lc, 16);
			}
		}

		class Encoder : public byteWriter
		{
			byteWriter* bw;
			const char* dict;
		public:
			Encoder(byteWriter &b, bool isU = false) : bw(&b), dict(isU ? dct_uc : dct_lc) {}

			void writeN(const uint8_t* v, std::size_t n)
			{
				for(std::size_t i = 0; i < n; i++)
				{
					const uint8_t t[2] = {dict[v[i]>>4], dict[v[i]&0x0f]};
					bw->writeN(t, 2);
				}
			}

			void Fin()
			{
				bw->Fin();
			}
		};

		namespace Dec
		{
			static uint8_t pr_char(char c)
			{
				if(c >= '0' && c <= '9')
					return c - '0';
				if(c >= 'A' && c <= 'F')
					return c - 'A' + 10;
				if(c >= 'a' && c <= 'f')
					return c - 'a' + 10;
				return 16;
			}

			static uint8_t pr_byte(const char* s)
			{
				uint8_t r = pr_char(s[0]);
				r <<= 4;
				r |= pr_char(s[1]);
				return r;
			}

			template<unsigned char sz>
			static typename UINT_<sz>::uint pr_num(const char* s, uint_fast8_t n)
			{
				typename UINT_<sz>::uint res = 0;
				for(unsigned char i = 0; i < n; i++)
				{
					res = (res<<4) | pr_char(s[i]);
				}
				return res;
			}

			static void pr_string(const char* s, std::size_t sz, uint8_t* out)
			{
				for(std::size_t i = 0; i < (sz >> 1); i++)
				{
					out[i] = pr_byte(s);
					s += 2;
				}
			}

			static std::vector<uint8_t> pr_string(const char* s, std::size_t sz)
			{
				std::vector<uint8_t> res(sz >> 1);
				pr_string(s, sz, res.data());
				return res;
			}

			static std::vector<uint8_t> pr_string(const char* s)
			{
				return pr_string(s, strlen(s));
			}
		}

		class Decoder : public byteWriterBuf<2>
		{
			byteWriter* bw;
			void process(const uint8_t* v)
			{
				const uint8_t c = Dec::pr_byte(reinterpret_cast<const char*>(v));
				bw->write(c);
			}
		public:
			Decoder(byteWriter &b) : bw(&b) {}

			void Fin()
			{
				reset();
				bw->Fin();
			}
		};
	}
}

#endif
