namespace convert
{
	namespace hex
	{
		const std::string dct_l = "0123456789abcdef";
		const std::string dct_u = "0123456789ABCDEF";

		class Encoder
		{
			std::string en;
		public:
			Encoder(const std::string &b) : en(b) {}
			
			std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n) const
			{
				std::vector<uint8_t> out;
				out.reserve(n<<1);

				for(std::size_t i = 0; i < n; i++)
				{
					out.push_back(en[v[i]>>4]);
					out.push_back(en[v[i]&0x0f]);
				}
				return out;
			}
		};

		class Decoder
		{
		public:
			Decoder(){}

			static std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n)
			{
				std::vector<uint8_t> out;
				out.reserve(n>>1);

				char t[3] = {};
				for(std::size_t i = 0; i + 1 < n; i += 2)
				{
					t[0] = v[i];
					t[1] = v[i+1];
					out.push_back( static_cast<uint8_t>(strtoul(t, nullptr, 16)) );
				}
				return out;
			}
		};
	}
}
