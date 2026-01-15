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
					auto d = data();
					bw->write(en[d[0]>>2]);
					if(size() == 1)
					{
						bw->write(en[(d[0] & 0x03) << 4]);
					}
					else
					{
						bw->write(en[((d[0] & 0x03) << 4) | (d[1] >> 4)]);
						bw->write(en[(d[1] & 0x0f) << 2]);
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
				const uint8_t c1 = de[v[1]];
				const uint8_t c2 = de[v[2]];
				uint8_t r[3];
				r[0] = (de[v[0]] << 2) | ((c1 >> 4) & 0x03);
				r[1] = ((c1 & 0x0f) << 4) | ((c2 >> 2) & 0x0f);
				r[2] = ((c2 & 0x03) << 6) | de[v[3]];
				bw->writeN(r, 3);
			}

			Decoder(byteWriter &b) : bw(&b)
			{
				std::fill_n(de, 256, -1);
				uint_fast8_t sz = 64;
				while(sz--)
				{
					de[en[sz]] = sz;
				}
			}

			void Fin()
			{
				if(size() != 0)
				{
					auto d = data();
					const uint8_t c1 = de[d[1]];
					bw->write((de[d[0]] << 2) | ((c1 >> 4) & 0x03));
					if(size() == 3)
					{
						bw->write(((c1 & 0x0f) << 4) | ((de[d[2]] >> 2) & 0x0f));
					}
					reset();
				}
				bw->Fin();
			}
		};
	}
}

#endif
