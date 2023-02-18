#ifndef H_TEST
#define H_TEST

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
bool compare(const std::vector<uint8_t> &t, const char* fname)
{
	br_fstream fs;
	if( !fs.open(fname) )
		return false;
	if(fs.get_size() != t.size())
		return false;
	uint8_t buf[BSIZE];
	std::size_t o = 0;
	for(;;)
	{
		const auto sz = fs.readMx(buf, BSIZE);
		if(std::memcmp(t.data() + o, buf, sz) != 0)
			return false;
		if(sz != BSIZE)
			break;
		o += BSIZE;
	}
	return true;
}

#endif
