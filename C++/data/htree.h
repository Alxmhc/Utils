template<typename T>
class huffmanTree
{
	binTree<T> t;
public:
	huffmanTree(){}
	huffmanTree(uint_fast8_t *m, T n)
	{
		Set(m, n);
	}

	void Set(uint_fast8_t *m, T n)
	{
		typedef std::pair<uint_fast8_t, T> pr;
		std::vector<pr> tmp(n);
		T sz = 0;
		for(T i = 0; i < n; ++i)
		{
			if(m[i] == 0)
				continue;
			tmp[sz].first = m[i];
			tmp[sz].second = i;
			sz++;
		}
		std::sort(tmp.begin(), tmp.begin() + sz);

		uint_fast32_t k = 0;
		uint_fast8_t p = 0;
		for(T i = 0; i < sz; ++i)
		{
			const uint_fast8_t s = tmp[i].first;
			if(p != s)
			{
				k <<= s - p;
				p = s;
			}
			t.add(k, s, tmp[i].second);
			k++;
		}
	}

	bool find(bitReader &rd, T &c) const
	{
		return t.find(rd, c);
	}
};
