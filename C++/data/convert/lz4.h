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
			static bool Decode_block(byteReader &br, std::vector<uint8_t> &out)
			{
				for(;;)
				{
					uint8_t b;
					if(!br.get(b))
						break;
					size_t size = b >> 4;
					if(!get_size(br, size))
						break;
					if(!br.addN(out, size))
						break;
					uint8_t o[2];
					if(!br.readN(o, 2))
						return true;
					auto offset = bconv<2, endianness::LITTLE_ENDIAN>::pack(o);
					size_t len = b & 0xf;
					if(!get_size(br, len))
						break;
					if(!LZ77_repeat(len + 4, offset, out))
						break;
				}
				return false;
			}
		};
	}
}
