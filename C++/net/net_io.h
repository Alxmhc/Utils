#ifndef H_NET_IO
#define H_NET_IO

#include "../data/byte_writer.h"

struct sock_client
{
	template<std::size_t BSIZE>
	static bool read(int s, byteWriter &bw)
	{
		char buf[BSIZE];
		for(;;)
		{
			const int sz = recv(s, buf, BSIZE, 0);
			if(sz < 0)
				return false;
			if(sz != 0)
			{
				bw.writeN(reinterpret_cast<const uint8_t*>(buf), sz);
			}
			if(sz < BSIZE)
				return true;
		}
	}
};

#endif
