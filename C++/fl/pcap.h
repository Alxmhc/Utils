#ifndef H_PCAP
#define H_PCAP

#include "../data/byte_reader.h"

namespace fl_pr
{
	class F_pcap
	{
		byteReader* br;
	public:
		bool read(byteReader* r)
		{
			if( !r->skip(24) )
				return false;
			br = r;
			return true;
		}
		bool nxt(std::vector<uint8_t> &data)
		{
			if( !br->skip(8) )
				return false;
			uint_fast32_t size;
			if( !br->readC<4, endianness::LITTLE_ENDIAN>(size) )
				return false;
			if( !br->skip(4) )
				return false;
			if( !br->readN(data, size) )
				return false;
			return true;
		}
	};
}

#endif
