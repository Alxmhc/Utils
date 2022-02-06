namespace convert
{
	namespace hex
	{
		class Encoder : public byteWriter
		{
			byteWriter *bw;
			const char *dict;
		public:
			Encoder(byteWriter &b, bool isU = false) : bw(&b), dict(isU ? "0123456789ABCDEF" : "0123456789abcdef") {}

			void writeN(const uint8_t* v, size_t n)
			{
				for(size_t i = 0; i < n; i++)
				{
					bw->write(dict[v[i]>>4]);
					bw->write(dict[v[i]&0x0f]);
				}
			}
		};

		std::vector<uint8_t> Decode(const char *s, const size_t n)
		{
			std::vector<uint8_t> out;
			out.reserve(n>>1);

			char t[3] = {};
			for(size_t i = 0; i + 1 < n; i += 2)
			{
				t[0] = s[i];
				t[1] = s[i+1];
				out.push_back( static_cast<uint8_t>(strtoul(t, nullptr, 16)) );
			}
			return out;
		}
	}
}
