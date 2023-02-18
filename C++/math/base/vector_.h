#ifndef H_VECTOR_
#define H_VECTOR_

#include <vector>
#include <algorithm>

template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const T &a)
{
	std::for_each(v.begin(), v.end(), [&](T &e){e += a;});
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const T &a)
{
	std::for_each(v.begin(), v.end(), [&](T &e){e -= a;});
	return v;
}

template<typename T>
const std::vector<T>& operator*=(std::vector<T> &v, long double k)
{
	std::for_each(v.begin(), v.end(), [&](T &e){e *= k;});
	return v;
}

template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const std::vector<T> &a)
{
	std::transform(v.begin(), v.end(), a.cbegin(), v.begin(), [](T a, T b){return a + b;});
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const std::vector<T> &a)
{
	std::transform(v.begin(), v.end(), a.cbegin(), v.begin(), [](T a, T b){return a - b;});
	return v;
}

template<typename T>
T sum(const std::vector<T> &v)
{
	auto n = v.size();
	if(n == 0)
		return T();
	--n;
	T s = v[n];
	while(n != 0)
	{
		--n;
		s += v[n];
	}
	return s;
}

template<typename T>
T avr(const std::vector<T> &v)
{
	auto n = v.size();
	if(n == 0)
		return T();
	return sum(v) / static_cast<long double>(n);
}

template<typename T>
long double dot(const std::vector<T> &a, const std::vector<T> &b)
{
	long double s = 0.0;
	for(auto n = a.size(); n > 0;)
	{
		--n;
		s += dot(a[n], b[n]);
	}
	return s;
}

#endif
