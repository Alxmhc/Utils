std::vector<uint8_t> frd(const char *fl)
{
	std::vector<uint8_t> v;
	std::ifstream f(fl, std::ios_base::binary | std::ios_base::ate);
	if(!f.is_open())
		return v;
	const std::size_t sz = static_cast<std::size_t>(f.tellg());
	if(sz != 0)
	{
		v.resize(sz);
		f.seekg(std::ios_base::beg);
		f.read(reinterpret_cast<char*>(&v[0]), sz);
	}
	f.close();
	return v;
}

bool fwt(const char *fl, const char *c, const std::size_t n)
{
	std::ofstream f(fl, std::ios_base::binary);
	if(!f.is_open())
		return false;
	f.write(c, n);
	f.close();
	return true;
}

bool fwt(const char *fl, const uint8_t *v, const std::size_t n)
{
	return fwt(fl, reinterpret_cast<const char*>(v), n);
}
