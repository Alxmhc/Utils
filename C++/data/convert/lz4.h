namespace convert
{
	namespace lz4
	{
		class Decoder
		{
			static bool get_size(byteReader &br, size_t &sz)
			{
				if(sz != 15)
					return true;
				for(;;)
				{
					uint8_t b;
					if(!br.get(b))
						return false;
					sz += b;
					if(b != 255)
						return true;
				}
			}
		public:
			static std::vector<uint8_t> Decode_block(byteReader &br, size_t sz = 0)
			{
				std::vector<uint8_t> out;
				out.reserve(sz);

				uint8_t b;
				while(br.get(b))
				{
					size_t size = b >> 4;
					get_size(br, size);
					if(!br.readN_v(out, size))
						break;
					uint8_t o[2];
					if(!br.readN(o, 2))
						break;
					auto offset = bconv<2, endianness::LITTLE_ENDIAN>::pack(o);
					size_t len = b & 0xf;
					get_size(br, len);
					LZ77_repeat(len + 4, offset, out);
				}
				return out;
			}
		};
	}
}
