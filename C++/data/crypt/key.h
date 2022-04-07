template<class H>
class PBKDF2_HMAC
{
	hash::HMAC<H> h;
public:
	static const uint_fast8_t out_size = H::hash_size;

	PBKDF2_HMAC(const uint8_t* passw, size_t sz) : h(passw, sz) {}

	void Calc(const uint8_t* salt, size_t ssize, const uint8_t* nm, uint8_t* res)
	{
		h.Init();
		h.Update(salt, ssize);
		h.Update(nm, 4);
		h.Final(res);
	}

	void Calc(uint8_t* res)
	{
		h.Init();
		h.Update(res, H::hash_size);
		h.Final(res);
	}
};

//rfc 2898
template<class F>
std::vector<uint8_t> PBKDF2(const uint8_t* passw, size_t psz, const uint8_t* salt, size_t ssz, size_t c, size_t ksz)
{
	std::vector<uint8_t> key;
	if(ksz == 0)
		return key;
	auto kl = (ksz - 1) / F::out_size + 1;
	key.reserve(kl * F::out_size);

	F fcr(passw, psz);
	for(uint_fast32_t i = 1; i <= kl; i++)
	{
		uint8_t tmp[F::out_size];
		{
			uint8_t num[4];
			bconv<4, endianness::BIG_ENDIAN>::unpack(i, num);
			fcr.Calc(salt, ssz, num, tmp);
		}
		const size_t b = key.size();
		key.insert(key.end(), tmp, tmp + F::out_size);
		for(size_t j = 1; j < c; j++)
		{
			fcr.Calc(tmp);
			for(uint_fast8_t k = 0; k < F::out_size; k++)
			{
				key[b + k] ^= tmp[k];
			}
		}
	}
	key.resize(ksz);
	return key;
}
