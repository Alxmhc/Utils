#ifndef H_DECOMPR
#define H_DECOMPR

#include "../byte_writer.h"

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

static void write_part(std::vector<uint8_t> &v, std::size_t d_sz, byteWriter &bw, std::size_t buf_sz = 1024*1024)
{
	const auto sz = v.size();
	if(sz > d_sz && sz - d_sz > buf_sz)
	{
		bw.writeN(v.data(), v.size() - d_sz);
		v.erase(v.begin(), v.end() - d_sz);
	}
}

#endif
