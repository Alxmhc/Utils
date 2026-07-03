#ifndef H_TEST
#define H_TEST

#include "fl/fl_.h"

template<std::size_t BSIZE>
bool compare(byteReader &br1, byteReader &br2)
{
	std::size_t sz = br1.get_rsize();
	if (br2.get_rsize() != sz)
		return false;
	if (sz == 0)
		return true;
	uint8_t buf1[BSIZE], buf2[BSIZE];
	while (sz > BSIZE)
	{
		br1.readN(buf1, BSIZE);
		br2.readN(buf2, BSIZE);
		if (std::memcmp(buf1, buf2, BSIZE) != 0)
			return false;
		sz -= BSIZE;
	}
	br1.readN(buf1, sz);
	br2.readN(buf2, sz);
	if (std::memcmp(buf1, buf2, sz) != 0)
		return false;
	return true;
}

template<std::size_t BSIZE>
bool compare(const char* fl1, const char* fl2)
{
	br_fstream fs1, fs2;
	if(!fs1.open(fl1) || !fs2.open(fl2))
		return false;
	return compare<BSIZE>(fs1, fs2);
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
	while(sz > BSIZE)
	{
		fs.readN(buf, BSIZE);
		if(std::memcmp(v, buf, BSIZE) != 0)
			return false;
		v += BSIZE;
		sz -= BSIZE;
	}
	fs.readN(buf, sz);
	if (std::memcmp(v, buf, sz) != 0)
		return false;
	return true;
}

#endif
