class aesIV
{
public:
	std::vector<uint8_t> iv;
	aesIV(size_t n) : iv(n) {}
	virtual void incr() = 0;
};

class aesIVBE : public aesIV
{
public:
	aesIVBE(size_t n) : aesIV(n) {}

	void incr()
	{
		size_t i = iv.size();
		for(;;)
		{
			i--;
			iv[i]++;
			if(iv[i] != 0 || i == 0)
				break;
		}
	}
};

class aesIVLE : public aesIV
{
public:
	aesIVLE(size_t n) : aesIV(n) {}

	void incr()
	{
		for(size_t i = 0; i < iv.size(); i++)
		{
			iv[i]++;
			if(iv[i] != 0)
				break;
		}
	}
};

template<class IV>
void AESCTRDecrypt(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	//
}

void AESCTRDecryptLE(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTRDecrypt<aesIVLE>(key, ksz, data);
}

void AESCTRDecryptBE(const uint8_t *key, size_t ksz, std::vector<uint8_t> &data)
{
	AESCTRDecrypt<aesIVBE>(key, ksz, data);
}
