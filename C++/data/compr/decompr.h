#ifndef H_DECOMPR
#define H_DECOMPR

#include <cstdint>
#include <vector>
#include <algorithm>

static bool LZ77_repeat(std::size_t sz, std::size_t dist, std::vector<uint8_t> &out)
{
	auto osz = out.size();
	if(dist == 0 || osz < dist)
		return false;
	if(sz == 0)
		return true;
	out.resize(osz + sz);
	const auto beg = osz - dist;
	while(sz > dist)
	{
		std::copy_n(out.begin() + beg, dist, out.begin() + osz);
		sz -= dist;
		osz += dist;
		dist += dist;
	}
	std::copy_n(out.begin() + beg, sz, out.begin() + osz);
	return true;
}

#endif
