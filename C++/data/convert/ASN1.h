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
			sz = k;
			e = true;
			return true;
		}
		k &= 0x7f;

		if(k == 0)
		{
			sz = br.find(bytes("\x00\x00"), 2);
			if (sz == br.get_rsize())
				return false;
			e = false;
			return true;
		}

		if(!br.readC<endianness::BIG_ENDIAN>(k, sz))
			return false;
		if(sz > br.get_rsize())
			return false;
		e = true;
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
}

#endif
