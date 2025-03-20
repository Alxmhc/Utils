#ifndef H_CONV
#define H_CONV

#include <cstdint>
#include <vector>
#include <numeric>

namespace convert
{
	namespace BWT
	{
		template<typename T>
		static void Decode(T* res, std::size_t sz, std::size_t p)
		{
			std::vector<std::pair<T, std::size_t>> t(sz);
			for(std::size_t i = 0; i < sz; i++)
			{
				t[i].first = res[i];
				t[i].second = i;
			}
			std::stable_sort(t.begin(), t.end());
			for(std::size_t i = 0; i < sz; i++)
			{
				res[i] = t[p].first;
				p = t[p].second;
			}
		}
	}

	namespace MTF
	{
		void Decode(uint8_t* res, std::size_t sz, uint8_t* m)
		{
			for(std::size_t i = 0; i < sz; i++)
			{
				const auto c = res[i];
				res[i] = m[c];
				std::rotate(m, m + c, m + c + 1);
			}
		}
	}
}

#endif
