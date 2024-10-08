#ifndef H_CONT
#define H_CONT

#include "data/byte_reader.h"
#include "data/byte_writer.h"

static bool copy(byteReader &br, byteWriter &bw)
{
	std::vector<uint8_t> data;
	br.readN(data, br.get_rsize());
	bw.writeN(data.data(), data.size());
	return true;
}

#endif
