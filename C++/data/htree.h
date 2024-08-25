#ifndef H_HTREE
#define H_HTREE

#include "bin_tree.h"

template<typename T>
class hTree : public binTree<T>
{
public:
	hTree(hTree &&t) : binTree<T>(std::move(t)) {}

	hTree(const uint_fast8_t* m, T n)
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
		std::stable_sort(tmp.begin(), tmp.begin() + sz);

		uint_fast64_t k = 0;
		uint_fast8_t p = 0;
		for(T i = 0; i < sz; ++i)
		{
			const auto s = tmp[i].first;
			if(p != s)
			{
				k <<= s - p;
				p = s;
			}
			this->add(k, s, tmp[i].second);
			k++;
		}
	}
};
#endif
