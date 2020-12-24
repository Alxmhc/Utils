template<class H>
class PBKDF2_HMAC
{
	hash::HMAC<H> h;
public:
	static const uint_fast8_t out_size = H::hash_size;

	PBKDF2_HMAC(const uint8_t *passw, std::size_t sz)
	{
		h.SetKey(passw, sz);
	}

	void Calc(const uint8_t *salt, std::size_t ssize, const uint8_t *nm, uint8_t *res)
	{
		h.Init();
		h.Update(salt, ssize);
		h.Update(nm, 4);
		h.Final(res);
	}

	void Calc(uint8_t *res)
	{
		h.Init();
		h.Update(res, H::hash_size);
		h.Final(res);
	}
};

//rfc 2898
template<class F>
std::vector<uint8_t> PBKDF2(const uint8_t *passw, std::size_t psz, const uint8_t *salt, std::size_t ssz, std::size_t c, std::size_t ksz)
{
	std::vector<uint8_t> key;
	if(ksz == 0)
		return key;
	uint32_t kl = (ksz - 1) / F::out_size + 1;
	key.reserve(kl * F::out_size);

	F fcr(passw, psz);
	for(uint_fast32_t i = 1; i <= kl; i++)
	{
		uint8_t tmp[F::out_size], res[F::out_size];
		uint8_t num[4] = {i>>24, i>>16, i>>8, i};
		fcr.Calc(salt, ssz, num, tmp);
		std::copy_n(tmp, F::out_size, res);

		for(std::size_t j = 1; j < c; j++)
		{
			fcr.Calc(tmp);
			std::transform(res, res + F::out_size, tmp, res, std::bit_xor<uint8_t>());
		}
		key.insert(key.end(), res, res + F::out_size);
	}
	key.resize(ksz);
	return key;
}
