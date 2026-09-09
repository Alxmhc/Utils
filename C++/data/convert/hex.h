#ifndef H_HEX
#define H_HEX

#include "base.h"
#include "../byte_writer.h"

namespace convert
{
	namespace hex
	{
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
			Encoder(byteWriter &b, bool isU = false) : bw(b), dict(isU ? base::dct_h_u : base::dct_h_l) {}

			void writeN(const uint8_t* v, std::size_t n)
			{
				write_v(bw, v, n, dict);
			}

			void Fin()
			{
				bw.Fin();
			}

			static std::string pr_string(const uint8_t* v, std::size_t n, bool isU = false)
			{
				std::string res;
				res.reserve(n*2);
				bw_string bw(res);
				write_v(bw, v, n, isU ? base::dct_h_u : base::dct_h_l);
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

			static uint8_t pr_byte(const char* s)
			{
				uint8_t r0, r1;
				base::Dec::pr_char<16>(s[0], r0);
				base::Dec::pr_char<16>(s[1], r1);
				return (r0 << 4) | r1;
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
