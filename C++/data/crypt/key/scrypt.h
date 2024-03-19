#ifndef H_SCRYPT
#define H_SCRYPT

#include "../../hash/sha2.h"
#include "pbkdf2.h"
#include "../salsa.h"

//rfc7914
class scrypt
{
	const uint_fast32_t bsize;
	const uint_fast64_t cost;
	const uint_fast32_t par;

	void Mix(uint32_t* b, uint32_t* t) const
	{
		uint32_t X[16];
		std::copy_n(b + 32*bsize - 16, 16, X);
		for(uint_fast32_t i = 0; i < 2*bsize; i++)
		{
			v_xor(b + 16*i, X, 16);
			crypt::Salsa20::cr(X, b + 16*i, 8);
			std::copy_n(X, 16, t + 8*(i + (i&1)*(2*bsize - 1)));
		}
		std::copy_n(t, 32*bsize, b);
	}
public:
	scrypt(uint_fast32_t bsz, uint_fast64_t c, uint_fast32_t p) : bsize(bsz), cost(c), par(p) {}

	std::vector<uint8_t> gen(const uint8_t* passw, std::size_t psz, const uint8_t* salt, std::size_t ssz, std::size_t ksz) const
	{
		const auto bsz = bsize * 32;
		const PBKDF2<PBKDF2_HMAC<hash::SHA2_256>> k(1);

		auto B = k.gen(passw, psz, salt, ssz, 4*bsz*par);
		std::vector<uint32_t> Bp(bsz*par);
		conv::pack<4,endianness::LITTLE_ENDIAN>(B.data(), B.size(), Bp.data());
		{
			std::vector<uint32_t> T(bsz);
			uint32_t* t = T.data();

			std::vector<uint32_t> V(bsz * cost);
			for(uint_fast32_t i = 0; i < par; i++)
			{
				for(uint_fast64_t e = 0; e < cost; e++)
				{
					std::copy_n(Bp.data() + i*bsz, bsz, V.data() + e*bsz);
					Mix(Bp.data() + i*bsz, t);
				}
				for(uint_fast64_t e = 0; e < cost; e++)
				{
					const auto d = bconv<4, 2, endianness::LITTLE_ENDIAN>::pack(Bp.data() + i*bsz + bsz - 16) & (cost - 1);
					v_xor(Bp.data() + i*bsz, V.data() + d * bsz, bsz);
					Mix(Bp.data() + i*bsz, t);
				}
			}
		}
		conv::unpack<4,endianness::LITTLE_ENDIAN>(Bp.data(), Bp.size(), B.data());

		return k.gen(passw, psz, B.data(), B.size(), ksz);
	}
};

#endif
