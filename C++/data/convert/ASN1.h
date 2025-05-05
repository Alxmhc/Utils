#ifndef H_ASN1
#define H_ASN1

#include "../byte_reader.h"
#include "../../math/b_int.h"

namespace ASN1
{
	static bool len_decode(byteReader &br, bool &e, std::size_t &sz)
	{
		uint8_t k;
		if(!br.get(k))
			return false;

		if((k & 0x80) == 0)
		{
			sz = k;
			e = true;
			return true;
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

		return true;
	}

	static b_sint Int_decode(uint8_t* v, std::size_t sz)
	{
		b_uint c;
		if(sz == 0)
			return b_sint(c, true);
		if((v[0] & 0x80) == 0)
		{
			c.fromB(v, sz);
			return b_sint(c, true);
		}
		for(std::size_t i = 0; i < sz; i++)
		{
			v[i] = ~v[i];
		}
		c.fromB(v, sz);
		c += 1;
		return b_sint(c, false);
	}

	static bool get_data(byteReader &br, std::vector<uint8_t> &d)
	{
		std::size_t sz;
		bool e;
		if(!len_decode(br, e, sz))
			return false;
		if(e)
			return br.readN(d, sz);
		return false;
	}

	static bool get_Int(byteReader &br, b_sint &r)
	{
		std::vector<uint8_t> v;
		if(!get_data(br, v))
			return false;
		r = Int_decode(v.data(), v.size());
		return true;
	}
}

#endif
