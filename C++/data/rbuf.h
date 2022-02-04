template<size_t SZ>
class rbuf
{
	uint8_t d[SZ];
	size_t offset;
public:
	static const size_t sz = SZ;
	rbuf() : offset(0) {}
	void clear()
	{
		offset = 0;
		nul();
	}

	template<class C>
	void process(const uint8_t *v, const size_t n, C &cl)
	{
		if(n < sz - offset)
		{
			std::copy_n(v, n, d + offset);
			offset += n;
			return;
		}
		size_t part = 0;
		if(offset != 0)
		{
			part = sz - offset;
			std::copy_n(v, part, d + offset);
			cl.process_block(d);
		}
		for (; part + sz <= n; part += sz)
		{
			cl.process_block(v + part);
		}
		offset = n - part;
		std::copy_n(v + part, offset, d);
	}

	template<class C>
	size_t process(byteReader &br, C &cl)
	{
		size_t size = 0;
		if(offset != 0)
		{
			size = br.readMx(d + offset, sz - offset);
			offset += size;
			if(offset != sz)
				return size;
			cl.process_block(d);
		}
		for(;;)
		{
			offset = br.readMx(d, sz);
			size += offset;
			if(offset != sz)
				break;
			cl.process_block(d);
		}
		return size;
	}

	template<class C>
	void push(uint8_t c, C &cl)
	{
		d[offset] = c;
		offset++;
		if(offset == sz)
		{
			cl.process_block(d);
			offset = 0;
		}
	}
	const uint8_t* data() const
	{
		return d;
	}
	size_t size() const
	{
		return offset;
	}
	size_t sz_e() const
	{
		return sz - offset;
	}
	void nul()
	{
		std::fill(d + offset, d + sz, 0);
	}
};
