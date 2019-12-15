namespace conv
{
	const std::string hex_l = "0123456789abcdef";
	const std::string hex_u = "0123456789ABCDEF";
	struct hex
	{
		class Encoder
		{
			std::string out;
			std::string en;
		public:
			Encoder(const std::string &b) : en(b) {}
			
			std::string Convert(const uint8_t *v, const std::size_t n)
			{
				Update(v, n);
				return Final();
			}

			void Update(const uint8_t *v, const std::size_t n)
			{
				for(std::size_t i = 0; i < n; ++i)
				{
					out.push_back(en[v[i]>>4]);
					out.push_back(en[v[i]&0x0f]);
				}
			}

			std::string Read()
			{
				std::string res;
				std::swap(out, res);
				return res;
			}

			std::string Final()
			{
				return Read();
			}
		};

		class Decoder
		{
			std::vector<uint8_t> out;
			rbuf<2> buf;
		public:
			Decoder(){}

			std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n)
			{
				Update(v, n);
				return Final();
			}

			void process_block(const uint8_t *v)
			{
				char t[3] = {v[0], v[1], 0};
				out.push_back( static_cast<uint8_t>(strtoul(t, nullptr, 16)) );
			}

			void Update(const uint8_t *v, const std::size_t n)
			{
				buf.process(v, n, *this);
			}

			std::vector<uint8_t> Read()
			{
				std::vector<uint8_t> res;
				std::swap(out, res);
				return res;
			}

			std::vector<uint8_t> Final()
			{
				if(buf.sz != 0)
				{
					buf.clear();
				}
				return Read();
			}
		};
	};
}
