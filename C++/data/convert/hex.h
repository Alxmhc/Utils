#ifndef H_HEX
#define H_HEX

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
				const char t[3] = {v[0],v[1],0};
				const uint8_t c = static_cast<uint8_t>(strtoul(t, nullptr, 16));
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
