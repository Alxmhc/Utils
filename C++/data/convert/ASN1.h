#ifndef H_ASN1
#define H_ASN1

#include "../../str.h"
#include "../byte_reader.h"
#include "../../math/b_int.h"

namespace BER
{
	static bool len_decode(byteReader &br, bool &e, std::size_t &sz)
	{
		uint8_t k;
		if(!br.get(k))
			return false;
		if((k & 0x80) == 0)
		{
			e = true;
			sz = k;
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

	static bool get_data(byteReader &br, std::vector<uint8_t> &d)
	{
		std::size_t sz;
		bool e;
		if(!len_decode(br, e, sz))
			return false;
		if(e)
			return br.readN(d, sz);
		sz = br.find(bytes("\x00\x00"), 2);
		if(sz == br.get_rsize())
			return false;
		br.readN(d, sz);
		br.skip(2);
		return true;
	}

	static b_sint Int_decode(const uint8_t* v, std::size_t sz)
	{
		b_uint c;
		if(sz == 0)
			return b_sint(c, true);
		c.fromB(v, sz);
		if((v[0] & 0x80) == 0)
			return b_sint(c, true);

		const auto p = (sz << 3) - 1;
		c.setBit(p, false);
		b_uint t;
		t.setBit(p, true);
		t -= c;
		return b_sint(t, false);
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
