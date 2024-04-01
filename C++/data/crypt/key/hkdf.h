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

	std::vector<uint8_t> gen(const uint8_t* passw, std::size_t psz, const uint8_t* salt, std::size_t ssz, std::size_t ksz) const
	{
		std::vector<uint8_t> key;
		if(ksz == 0)
			return key;
		auto kl = (ksz - 1) / H::hash_size + 1;
		key.reserve(kl * H::hash_size);

		uint8_t k[H::hash_size];
		{
			hash::HMAC<H> hs(salt, ssz);
			hs.Update(passw, psz);
			hs.Final(k);
		}

		hash::HMAC<H> hs(k, H::hash_size);
		for(uint8_t i = 1; i <= kl; i++)
		{
			if(i != 1)
			{
				hs.Update(k, H::hash_size);
			}
			if(inf.size() != 0)
			{
				hs.Update(inf.data(), inf.size());
			}
			hs.Update(&i, 1);
			hs.Final(k);
			key.insert(key.end(), k, k + H::hash_size);
		}
		key.resize(ksz);
		return key;
	}
};

#endif
