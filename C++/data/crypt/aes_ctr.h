template<class IV>
void AESCTREncrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AES enc(key, ksz);
	IV v;

	size_t offset = 0;
	while(offset + 16 < data.size())
	{
		v.incr();
		uint8_t tmp[16];
		std::copy_n(v.data(), 16, tmp);
		enc.Encrypt(tmp);
		std::transform(tmp, tmp + 16, data.begin() + offset, data.begin() + offset, [](uint8_t a, uint8_t b){return a ^ b;});
		offset += 16;
	}
	v.incr();
	uint8_t tmp[16];
	std::copy_n(v.data(), 16, tmp);
	enc.Encrypt(tmp);
	std::transform(tmp, tmp + data.size() - offset, data.begin() + offset, data.begin() + offset, [](uint8_t a, uint8_t b){return a ^ b;});
}

template<class IV>
void AESCTRDecrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTREncrypt<IV>(key, ksz, data);
}
