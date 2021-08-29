template<class IV>
void AESCTREncrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AES enc(key, ksz);

	uint8_t tmp[16];
	IV v;

	size_t offset = 0;
	while(offset < data.size() - 16)
	{
		v.incr();
		memcpy(tmp, v.data(), 16);
		enc.Encrypt(tmp);
		for(uint_fast8_t i = 0; i < 16; i++)
		{
			data[i+offset] ^= tmp[i];
		}
		offset += 16;
	}
	size_t o = data.size() - offset;
	if(o != 0)
	{
		v.incr();
		memcpy(tmp, v.data(), 16);
		enc.Encrypt(tmp);
		for(uint_fast8_t i = 0; i < o; i++)
		{
			data[i+offset] ^= tmp[i];
		}
	}
}

template<class IV>
void AESCTRDecrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTREncrypt<IV>(key, ksz, data);
}
