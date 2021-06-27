void AESCTREncrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data, void incr(uint8_t*, size_t))
{
	AES enc(key, ksz);

	uint8_t tmp[16];
	uint8_t iv[16] = {};

	size_t offset = 0;
	while(offset < data.size() - 16)
	{
		incr(iv, 16);
		memcpy(tmp, iv, 16);
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
		incr(iv, 16);
		memcpy(tmp, iv, 16);
		enc.Encrypt(tmp);
		for(uint_fast8_t i = 0; i < o; i++)
		{
			data[i+offset] ^= tmp[i];
		}
	}
}

static void incrBE(uint8_t *v, size_t sz)
{
	size_t i = sz;
	for(;;)
	{
		i--;
		v[i]++;
		if(v[i] != 0 || i == 0)
			break;
	}
}
static void incrLE(uint8_t *v, size_t sz)
{
	for(size_t i = 0; i < sz; i++)
	{
		v[i]++;
		if(v[i] != 0)
			break;
	}
}

void AESCTRDecryptLE(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTREncrypt(key, ksz, data, incrLE);
}

void AESCTRDecryptBE(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTREncrypt(key, ksz, data, incrBE);
}
