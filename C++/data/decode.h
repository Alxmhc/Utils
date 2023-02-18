#ifndef H_DECODE
#define H_DECODE

namespace decode
{
	bool unchunk(const uint8_t* v, const size_t n, byteWriter &bw)
	{
		const uint8_t* e = v + n;
		for(;;)
		{
			const auto sz = strtoul(reinterpret_cast<const char*>(v), nullptr, 16);
			if(sz == 0)
				return true;
			v = std::find(v, e, '\n');
			if(v == e)
				break;
			v++;
			if(static_cast<size_t>(e - v) < sz + 2)
				break;
			bw.writeN(v, sz);
			v += sz + 1;
			if(*v != '\n')
				break;
			v++;
		}
		return false;
	}
}

#endif
