//rfc 2104
template<class H, std::size_t BSZ>
void HMAC(const uint8_t *key, std::size_t ksize, const uint8_t *data, std::size_t dsize, uint8_t *res)
{
	uint8_t v[BSZ] = {};
	H hash;
	if(ksize > BSZ)
	{
		hash.Update(key, ksize);
		hash.Final(v);
	}
	else
	{
		std::copy(key, key + ksize, v);
	}
	std::for_each(v, v + BSZ, [&](uint8_t &e){e ^= 0x36;});
	hash.Update(v, BSZ);
	hash.Update(data, dsize);
	hash.Final(res);
	std::for_each(v, v + BSZ, [&](uint8_t &e){e ^= 0x6a;}); // 0x36 ^ 0x5c
	hash.Update(v, BSZ);
	hash.Update(res, H::hash_size);
	hash.Final(res);
}
