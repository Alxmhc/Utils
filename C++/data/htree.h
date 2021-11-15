template<typename T>
class huffmanTree
{
	binTree<T> t;
public:
	huffmanTree(){}
	huffmanTree(T *m, T n)
	{
		Set(m, n);
	}

	void Set(T *m, T n)
	{
		typedef std::pair<uint_fast16_t, uint_fast16_t> pr;
		std::vector<pr> tmp(n);
		T sz = 0;
		for(T i = 0; i < n; ++i)
		{
			if(m[i] == 0)
				continue;
			tmp[sz].first = i;
			tmp[sz].second = m[i];
			sz++;
		}
		std::sort(tmp.begin(), tmp.begin() + sz, [](const pr &a, const pr &b){return a.second == b.second ? a.first < b.first : a.second < b.second;});

		for(T i = 0, k = 0, p = 0; i < sz; ++i)
		{
			const T s = tmp[i].second;
			if(p != s)
			{
				k <<= s - p;
				p = s;
			}
			t.add(k, s, tmp[i].first);
			k++;
		}
	}

	bool find(bitReader &rd, T &c) const
	{
		return t.find(rd, c);
	}
};
