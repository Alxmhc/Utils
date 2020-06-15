namespace convert
{
	namespace lz4
	{
		class Decoder
		{
			static std::size_t get_size(byteReader &br, std::size_t sz)
			{
				if(sz != 15)
					return sz;
				for(;;)
				{
					uint8_t b;
					br.get(b);
					sz += b;
					if(b != 255)
						break;
				}
				return sz;
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
					std::size_t size = get_size(br, b >> 4);
					if(br.read_v(out, size) < size)
						break;
					uint16_t offset;
					if(!br.getC<endianness::LITTLE_ENDIAN>(offset))
						break;
					std::size_t len = get_size(br, b & 0xf) + 4;
					LZ77_repeat(len, offset, out);
				}
				return out;
			}
		};
	}
}
