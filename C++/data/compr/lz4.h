namespace compr
{
	class lz4
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
		static bool Decode(byteReader &br, std::vector<uint8_t> &out)
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
				uint_fast16_t offset;
				if( !br.readC<2, endianness::LITTLE_ENDIAN>(offset) )
					return true;
				size_t len = b & 0xf;
				if(!get_size(br, len))
					break;
				if(!LZ77_repeat(len + 4, offset, out))
					break;
			}
			return false;
		}

		static bool Decode(const uint8_t* v, size_t sz, std::vector<uint8_t> &out)
		{
			br_array br(v, sz);
			return Decode(br, out);
		}
	};
}
