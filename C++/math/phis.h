#ifndef H_PHIS
#define H_PHIS

#include <vector>

template<typename T>
long double Ek(const std::vector<long double> &m, const std::vector<T> &v)
{
	long double s = 0;
	auto n = m.size();
	while(n--)
	{
		s += m[n] * dot(v[n], v[n]);
	}
	return 0.5*s;
}

template<typename T>
long double Ek(long double m, const std::vector<T> &v)
{
	return 0.5 * m * dot(v, v);
}

#endif
