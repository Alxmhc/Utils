#ifndef H_TEST
#define H_TEST

#include "fl/fl_.h"

template<std::size_t BSIZE>
bool compare(const char* fl1, const char* fl2)
{
	br_fstream fs1, fs2;
	if(!fs1.open(fl1)
	|| !fs2.open(fl2))
		return false;
	if(fs1.get_size() != fs2.get_size())
		return false;
	uint8_t buf1[BSIZE], buf2[BSIZE];
	for(;;)
	{
		const auto sz = fs1.readMx(buf1, BSIZE);
		fs2.readN(buf2, sz);
		if(std::memcmp(buf1, buf2, sz) != 0)
			return false;
		if(sz != BSIZE)
			break;
	}
	return true;
}

template<std::size_t BSIZE>
bool compare(const uint8_t* v, std::size_t sz, const char* fname)
{
	br_fstream fs;
	if( !fs.open(fname) )
		return false;
	if(fs.get_size() != sz)
		return false;
	uint8_t buf[BSIZE];
	for(;;)
	{
		const auto s = fs.readMx(buf, BSIZE);
		if(std::memcmp(v, buf, s) != 0)
			return false;
		if(s != BSIZE)
			break;
		v += BSIZE;
	}
	return true;
}

#endif
