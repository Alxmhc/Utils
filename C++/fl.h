template<typename C>
bool fwt(const C *fl, const char *c, const std::size_t n)
{
	std::ofstream f(fl, std::ios_base::binary);
	if(!f.is_open())
		return false;
	f.write(c, n);
	f.close();
	return true;
}

template<typename C>
bool fwt(const C *fl, const uint8_t *v, const std::size_t n)
{
	return fwt(fl, reinterpret_cast<const char*>(v), n);
}

struct fl_inf
{
	std::size_t size, pos;
};
