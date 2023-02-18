#ifndef H_LZ4
#define H_LZ4

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
		static bool Decode(byteReader &br, byteWriter &bw)
		{
			std::vector<uint8_t> out;
			for(;;)
			{
				uint8_t b;
				if(!br.get(b))
					return false;
				size_t size = b >> 4;
				if(!get_size(br, size))
					return false;
				if(!br.addN(out, size))
					return false;
				uint_fast16_t offset;
				if( !br.readC<2, endianness::LITTLE_ENDIAN>(offset) )
					break;
				size_t len = b & 0xf;
				if(!get_size(br, len))
					return false;
				if(!LZ77_repeat(len + 4, offset, out))
					return false;
			}
			bw.writeN(out.data(), out.size());
			bw.Fin();
			return true;
		}
	};
}

#endif
