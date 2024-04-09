#ifndef H_HKDF
#define H_HKDF

#include <vector>

#include "../../hash/hmac.h"

template<class H>
class HKDF
{
	std::vector<uint8_t> inf;
public:
	HKDF(const uint8_t* info, std::size_t sz) : inf(info, info + sz) {}

	void gen(const uint8_t* passw, std::size_t psz, const uint8_t* salt, std::size_t ssz, uint8_t* key, std::size_t ksz) const
	{
		if(ksz == 0)
			return;

		uint8_t k[H::hash_size];
		{
			hash::HMAC<H> hs(salt, ssz);
			hs.Update(passw, psz);
			hs.Final(k);
		}

		hash::HMAC<H> hs(k, H::hash_size);
		std::size_t o = 0;
		uint8_t i = 0;
		for(;;)
		{
			if(inf.size() != 0)
			{
				hs.Update(inf.data(), inf.size());
			}
			i++;
			hs.Update(&i, 1);
			hs.Final(k);
			if(ksz - o <= H::hash_size)
			{
				std::copy_n(k, ksz - o, key + o);
				break;
			}
			std::copy_n(k, H::hash_size, key + o);
			hs.Update(k, H::hash_size);
			o += H::hash_size;
		}
	}
};

#endif
