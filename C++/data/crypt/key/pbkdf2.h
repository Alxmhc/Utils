#ifndef H_PBKDF2
#define H_PBKDF2

#include <vector>

#include "../../../arr.h"
#include "../../pack.h"
#include "../../hash/hmac.h"

template<class H>
class PBKDF2_HMAC
{
	hash::HMAC<H> h;
public:
	static const uint_fast8_t out_size = H::hash_size;

	PBKDF2_HMAC(const uint8_t* passw, std::size_t sz) : h(passw, sz) {}

	void Calc(const uint8_t* salt, std::size_t ssize, const uint8_t* nm, uint8_t* res)
	{
		if(ssize != 0)
		{
			h.Update(salt, ssize);
		}
		h.Update(nm, 4);
		h.Final(res);
	}

	void Calc(uint8_t* res)
	{
		h.Update(res, H::hash_size);
		h.Final(res);
	}
};

//rfc 2898
template<class F>
class PBKDF2
{
	const std::size_t c;
public:
	PBKDF2(std::size_t k) : c(k) {}

	void gen(const uint8_t* passw, std::size_t psz, const uint8_t* salt, std::size_t ssz, uint8_t* key, std::size_t ksz) const
	{
		if(ksz == 0)
			return;
		F fcr(passw, psz);
		uint_fast32_t i = 0;
		std::size_t o = 0;
		for(;;)
		{
			uint8_t tmp[F::out_size];
			{
				i++;
				uint8_t num[4];
				bconv<1, 4, endianness::BIG_ENDIAN>::unpack(i, num);
				fcr.Calc(salt, ssz, num, tmp);
			}
			const bool is_fin = (ksz - o <= F::out_size);
			const uint_fast8_t sz = is_fin ? static_cast<uint_fast8_t>(ksz - o) : F::out_size;
			std::copy_n(tmp, sz, key + o);
			for(std::size_t j = 1; j < c; j++)
			{
				fcr.Calc(tmp);
				v_xor(key + o, tmp, sz);
			}
			if(is_fin)
				break;
			o += F::out_size;
		}
	}
};

#endif
