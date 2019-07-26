long int f_size(FILE *f)
{
	if(f == nullptr)
		return -1;
	fseek(f, 0, SEEK_END);
	return ftell(f);
}

long int fsize(const char *fl)
{
	FILE *f = fopen(fl, "rb");
	auto s = f_size(f);
	if(s >= 0)
	{
		fclose(f);
	}
	return s;
}

std::vector<uint8_t> frd(const char *fl)
{
	std::vector<uint8_t> v;
	FILE *f = fopen(fl, "rb");
	auto s = f_size(f);
	if(s < 0)
		return v;
	if(s > 0)
	{
		fseek(f, 0, SEEK_SET);
		v.resize(s);
		fread(&v[0], s, 1, f);
	}
	fclose(f);
	return v;
}

bool fwt(const char *fl, const uint8_t *v, const std::size_t n)
{
	FILE *f = fopen(fl, "wb");
	if(f == nullptr)
		return false;
	bool s = (fwrite(v, n, 1, f) == 1);
	fclose(f);
	return s;
}
