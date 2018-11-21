template<std::size_t SZ>
class rbuf
{
	std::size_t offset;
public:
	void init()
	{
		offset = 0;
	}

	static const std::size_t sz = SZ;
	uint8_t d[sz];

	void clear()
	{
		memset(d, 0, sz);
	}
	template<class C>
	void process(const uint8_t *v, const std::size_t n, C &cl)
	{
		if(n + offset < sz)
		{
			std::copy_n(v, n, d + offset);
			offset += n;
			return;
		}
		std::size_t part = 0;
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
	std::size_t size() const
	{
		return offset;
	}
	std::size_t sz_e() const
	{
		return sz - offset;
	}
	void nul()
	{
		std::fill_n(d + offset, sz - offset, 0);
	}
	void push(uint8_t c)
	{
		d[offset] = c;
		offset++;
	}
};
