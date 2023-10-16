#ifndef H_HEX
#define H_HEX

#include "../byte_writer.h"

static uint8_t HexDecodeChar(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 16;
}

template<unsigned char sz>
static typename UINT_<sz>::uint HexDecodeNum(const char* s, uint_fast8_t n)
{
	typename UINT_<sz>::uint res = 0;
	for(unsigned char i = 0; i < n; i++)
	{
		res = (res<<4) | HexDecodeChar(s[i]);
	}
	return res;
}

namespace convert
{
	namespace hex
	{
		class Encoder : public byteWriter
		{
			byteWriter* bw;
			const char* dict;
		public:
			Encoder(byteWriter &b, bool isU = false) : bw(&b), dict(isU ? "0123456789ABCDEF" : "0123456789abcdef") {}

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

		class Decoder : public byteWriterBuf<2>
		{
			byteWriter* bw;
			void process(const uint8_t* v)
			{
				const char* s = reinterpret_cast<const char*>(v);
				uint8_t c = (HexDecodeChar(s[0]) << 4) | HexDecodeChar(s[1]);
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
