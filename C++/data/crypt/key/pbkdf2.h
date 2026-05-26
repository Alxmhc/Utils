#ifndef H_PBKDF2
#define H_PBKDF2

#include <vector>

#include "../../../arr.h"
#include "../../pack.h"
#include "../../hash/hmac.h"

template<class H>
class PBKDF2_HMAC
{
	const std::size_t c;
public:
	PBKDF2_HMAC(std::size_t k) : c(k) {}

	void gen(const uint8_t* passw, std::size_t psz, const uint8_t* salt, std::size_t ssz, uint8_t* key, std::size_t ksz) const
	{
		if(ksz == 0)
			return;
		hash::HMAC<H> h(passw, psz);
		uint8_t num[4] = {};
		for(;;)
		{
			h.Update(salt, ssz);
			v_BE::incr(num, 4);
			h.Update(num, 4);
			uint8_t tmp[H::hash_size];
			h.Final(tmp);
			const bool is_fin = ksz <= H::hash_size;
			const uint_fast8_t sz = is_fin ? static_cast<uint_fast8_t>(ksz) : H::hash_size;
			std::copy_n(tmp, sz, key);
			for(std::size_t j = 1; j < c; j++)
			{
				h.Update(tmp, H::hash_size);
				h.Final(tmp);
				v_xor(key, tmp, sz);
			}
			if(is_fin)
				break;
			key += H::hash_size;
			ksz -= H::hash_size;
		}
	}
};

#endif
