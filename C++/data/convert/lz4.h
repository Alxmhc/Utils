namespace convert
{
	namespace lz4
	{
		class Decoder
		{
			static void add_size(byteReader &br, std::size_t &sz)
			{
				if(sz != 15)
					return;
				for(;;)
				{
					uint8_t b;
					br.get(b);
					sz += b;
					if(b != 255)
						break;
				}
			}
		public:
			Decoder() {}

			static std::vector<uint8_t> Decode_block(byteReader &br, std::size_t sz = 0)
			{
				std::vector<uint8_t> out;
				out.reserve(sz);

				uint8_t b;
				while(br.get(b))
				{
					std::size_t size = b >> 4;
					std::size_t len = b & 15;
					add_size(br, size);
					if(br.read(out, size) < size)
						break;
					uint16_t offset;
					if(!br.getC<endianness::LITTLE_ENDIAN>(offset))
						break;
					add_size(br, len);
					len += 4;
					LZ77_repeat(len, offset, out);
				}
				return out;
			}
		};
	}
}
