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

		class Encoder : public byteWriter
		{
			byteWriter &bw;
			const char* dict;

			static void write_v(byteWriter &bw, const uint8_t* v, std::size_t n, const char* dict)
			{
				for (std::size_t i = 0; i < n; i++)
				{
					bw.write(dict[v[i] >> 4]);
					bw.write(dict[v[i] & 0x0f]);
				}
			}
		public:
			Encoder(byteWriter &b, bool isU = false) : bw(b), dict(isU ? dct_uc : dct_lc) {}

			void writeN(const uint8_t* v, std::size_t n)
			{
				write_v(bw, v, n, dict);
			}

			void Fin()
			{
				bw.Fin();
			}

			static std::string pr_num(std::size_t n, bool isU = false)
			{
				return base::Enc::pr_num<16>(n, isU ? dct_uc : dct_lc);
			}

			static std::string pr_string(const uint8_t* v, std::size_t n, bool isU = false)
			{
				std::string res;
				res.reserve(n*2);
				bw_string bw(res);
				write_v(bw, v, n, isU ? dct_uc : dct_lc);
				return res;
			}
		};

		class Decoder : public byteWriterBuf<2>
		{
			byteWriter &bw;

			void process(const uint8_t* v)
			{
				const uint8_t c = pr_byte(reinterpret_cast<const char*>(v));
				bw.write(c);
			}
		public:
			Decoder(byteWriter &b) : bw(b) {}

			void Fin()
			{
				reset();
				bw.Fin();
			}

			static uint8_t pr_char(char c)
			{
				switch (c)
				{
				case '0':
					return 0;
				case '1':
					return 1;
				case '2':
					return 2;
				case '3':
					return 3;
				case '4':
					return 4;
				case '5':
					return 5;
				case '6':
					return 6;
				case '7':
					return 7;
				case '8':
					return 8;
				case '9':
					return 9;
				case 'a':
				case 'A':
					return 10;
				case 'b':
				case 'B':
					return 11;
				case 'c':
				case 'C':
					return 12;
				case 'd':
				case 'D':
					return 13;
				case 'e':
				case 'E':
					return 14;
				case 'f':
				case 'F':
					return 15;
				default:
					return 16;
				}
			}

			static uint8_t pr_byte(const char* s)
			{
				const uint8_t r = pr_char(s[0]);
				return (r << 4) | pr_char(s[1]);
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
		};
	}
}

#endif
