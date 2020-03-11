template<std::size_t SZ>
class rbuf
{
	uint8_t d[SZ];
	std::size_t offset;
public:
	static const std::size_t sz = SZ;
	rbuf() : offset(0) {}
	void clear()
	{
		offset = 0;
		nul();
	}

	template<class C>
	void process(const uint8_t *v, const std::size_t n, C &cl)
	{
		if(n < sz - offset)
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
		std::fill(d + offset, d + sz, 0);
	}
};
