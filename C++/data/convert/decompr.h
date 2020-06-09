void LZ77_repeat(std::size_t sz, std::size_t dist, std::vector<uint8_t> &out)
{
	std::size_t osz = out.size();
	out.resize(osz + sz);
	std::size_t n = osz - dist;
	while(sz > dist)
	{
		std::copy_n(out.begin() + n, dist, out.begin() + osz);
		osz += dist;
		sz -= dist;
	}
	std::copy_n(out.begin() + n, sz, out.begin() + osz);
}
