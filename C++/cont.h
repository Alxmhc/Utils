#ifndef H_CONT
#define H_CONT

#include "data/byte_reader.h"
#include "data/byte_writer.h"

static bool copy(byteReader &br, byteWriter &bw, std::size_t sz)
{
	std::vector<uint8_t> data;
	if( !br.readN(data, sz) )
		return false;
	bw.writeN(data.data(), sz);
	return true;
}

#endif
