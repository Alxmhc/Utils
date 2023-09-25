#ifndef H_BASE64
#define H_BASE64

#include "../byte_writer.h"

namespace convert
{
	namespace base64
	{
		static const char* en = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		class Encoder : public byteWriterBuf<3>
		{
			byteWriter* bw;
			void process(const uint8_t* v)
			{
				uint_fast32_t t = v[0];
				t = (t << 8) | v[1];
				t = (t << 8) | v[2];
				uint8_t r[4];
				r[0] = en[(t>>18) & 0x3f];
				r[1] = en[(t>>12) & 0x3f];
				r[2] = en[(t>>6) & 0x3f];
				r[3] = en[t & 0x3f];
				bw->writeN(r, 4);
			}
		public:
			Encoder(byteWriter &b) : bw(&b) {}

			void Fin()
			{
				if(size() != 0)
				{
					uint8_t c[3];
					c[0] = en[data()[0]>>2];
					if(size() == 1)
					{
						c[1] = en[(data()[0] & 0x03) << 4];
						bw->writeN(c, 2);
					}
					else
					{
						c[1] = en[((data()[0] & 0x03) << 4) + (data()[1] >> 4)];
						c[2] = en[(data()[1] & 0x0f) << 2];
						bw->writeN(c, 3);
					}
					reset();
				}
				bw->Fin();
			}
		};

		class Decoder : public byteWriterBuf<4>
		{
			uint8_t de[256];
			byteWriter* bw;
		public:
			void process(const uint8_t* v)
			{
				const uint8_t c1 = de[v[0]];
				const uint8_t c2 = de[v[1]];
				const uint8_t c3 = de[v[2]];
				const uint8_t c4 = de[v[3]];
				uint8_t r[3];
				r[0] = (c1 << 2) + ((c2 & 0x30) >> 4);
				r[1] = ((c2 & 0x0f) << 4) + ((c3 & 0x3c) >> 2);
				r[2] = ((c3 & 0x03) << 6) + c4;
				bw->writeN(r, 3);
			}

			Decoder(byteWriter &b) : bw(&b)
			{
				std::fill_n(de, 256, -1);
				uint8_t sz = 64;
				while(sz != 0)
				{
					sz--;
					de[en[sz]] = sz;
				}
			}

			void Fin()
			{
				if(size() != 0)
				{
					const uint8_t c1 = de[data()[0]];
					const uint8_t c2 = de[data()[1]];
					uint8_t c = (c1 << 2) + ((c2 & 0x30) >> 4);
					bw->write(c);
					if(size() == 3)
					{
						const uint8_t c3 = de[data()[2]];
						c = ((c2 & 0xf) << 4) + ((c3 & 0x3c) >> 2);
						bw->write(c);
					}
					reset();
				}
				bw->Fin();
			}
		};
	}
}

#endif
