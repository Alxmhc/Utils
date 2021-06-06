bool LZ77_repeat(size_t sz, size_t dist, std::vector<uint8_t> &out)
{
	if(dist == 0)
		return false;
	auto osz = out.size();
	if(osz < dist)
		return false;
	if(sz == 0)
		return true;
	out.resize(osz + sz);
	size_t n = osz - dist;
	while(sz > dist)
	{
		std::copy_n(out.begin() + n, dist, out.begin() + osz);
		osz += dist;
		sz -= dist;
	}
	std::copy_n(out.begin() + n, sz, out.begin() + osz);
	return true;
}
