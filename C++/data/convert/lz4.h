namespace convert
{
	namespace lz4
	{
		class Decoder
		{
			static size_t get_size(byteReader &br, size_t sz)
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

			static std::vector<uint8_t> Decode_block(byteReader &br, size_t sz = 0)
			{
				std::vector<uint8_t> out;
				out.reserve(sz);

				uint8_t b;
				while(br.get(b))
				{
					size_t size = get_size(br, b >> 4);
					if(!br.readN_v(out, size))
						break;
					uint8_t o[2];
					if(!br.readN(o, 2))
						break;
					auto offset = bconv<2, endianness::LITTLE_ENDIAN>::pack(o);
					size_t len = get_size(br, b & 0xf) + 4;
					LZ77_repeat(len, offset, out);
				}
				return out;
			}
		};
	}
}
