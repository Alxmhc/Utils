#ifndef H_LZ4
#define H_LZ4

#include "../compr/decompr.h"
#include "../byte_reader.h"

namespace compr
{
	class lz4
	{
		static const std::size_t d_sz = 65535;

		static bool get_size(byteReader &br, std::size_t &sz)
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
		static bool decode_block(byteReader &br, std::vector<uint8_t> &out, byteWriter &bw)
		{
			for(;;)
			{
				uint8_t b;
				if(!br.get(b))
					return false;
				std::size_t size = b >> 4;
				if(!get_size(br, size))
					return false;
				if(!br.addN(out, size))
					return false;
				uint_fast16_t offset;
				if( !br.readC<endianness::LITTLE_ENDIAN>(2, offset) )
				{
					if (br.get_rsize() != 0)
						return false;
					write_part(out, d_sz, bw);
					return true;
				}
				std::size_t len = b & 0xf;
				if(!get_size(br, len))
					return false;
				if(!LZ77_repeat(len + 4, offset, out))
					return false;
				write_part(out, d_sz, bw);
			}
		}
	};
}

#endif
