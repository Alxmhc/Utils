namespace convert
{
	namespace base64
	{
		const char* en = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		class Encoder : public byteWriterBuf<3>
		{
			byteWriter *bw;
			void process(const uint8_t *v)
			{
				uint_fast32_t t = (v[0] << 16) | (v[1] << 8) | v[2];
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
				if(offset == 0)
					return;
				uint8_t c[3];
				c[0] = en[buf[0]>>2];
				if(offset == 1)
				{
					c[1] = en[(buf[0] & 0x03) << 4];
					bw->writeN(c, 2);
				}
				else
				{
					c[1] = en[((buf[0] & 0x03) << 4) + (buf[1] >> 4)];
					c[2] = en[(buf[1] & 0x0f) << 2];
					bw->writeN(c, 3);
				}
				offset = 0;
				bw->Fin();
			}
		};

		class Decoder : public byteWriterBuf<4>
		{
			std::array<uint8_t, 256> de;
			byteWriter *bw;
		public:
			void process(const uint8_t *v)
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
				de.fill(-1);
				uint8_t sz = 64;
				while(sz != 0)
				{
					sz--;
					de[en[sz]] = sz;
				}
			}

			void Fin()
			{
				if(offset < 2)
					return;
				const uint8_t c1 = de[buf[0]];
				const uint8_t c2 = de[buf[1]];
				uint8_t c = (c1 << 2) + ((c2 & 0x30) >> 4);
				bw->write(c);
				if(offset == 3)
				{
					const uint8_t c3 = de[buf[2]];
					c = ((c2 & 0xf) << 4) + ((c3 & 0x3c) >> 2);
					bw->write(c);
				}
				offset = 0;
				bw->Fin();
			}
		};
	}
}
