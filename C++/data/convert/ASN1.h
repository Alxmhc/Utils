#ifndef H_ASN1
#define H_ASN1

#include "../byte_reader.h"
#include "../../math/b_int.h"

namespace ASN1
{
	bool len_decode(byteReader &br, bool &e, std::size_t &sz)
	{
		uint8_t k;
		if(!br.get(k))
			return false;

		if((k & 0x80) == 0)
		{
			sz = k;
			e = true;
			return br.get_rsize() >= sz;
		}
		k &= 0x7f;

		if(k == 0)
		{
			e = false;
			return true;
		}

		e = true;
		sz = 0;
		while(k--)
		{
			uint8_t t;
			if(!br.get(t))
				return false;
			sz = (sz << 8) | t;
		}

		return br.get_rsize() >= sz;
	}

	b_sint Int_decode(byteReader &br, std::size_t sz)
	{
		b_uint c;
		if(sz == 0)
			return b_sint(c, true);

		std::vector<uint8_t> v(sz);
		br.readN(v.data(), sz);

		if((v[0] & 0x80) == 0)
		{
			c.fromB(v.data(), sz);
			return b_sint(c, true);
		}

		for(std::size_t i = 0; i < sz; i++)
		{
			v[i] = ~v[i];
		}
		c.fromB(v.data(), sz);
		c += 1;
		return b_sint(c, false);
	}
}

#endif
