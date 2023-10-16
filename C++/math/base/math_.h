#ifndef H_MATH_
#define H_MATH_

#include <cmath>

template <typename B>
B rotl(B x, unsigned char d)
{
	return (x<<d) | (x>>((sizeof(B)<<3)-d));
}

template <typename B>
B rotr(B x, unsigned char d)
{
	return (x>>d) | (x<<((sizeof(B)<<3)-d));
}

const long double pi = 3.1415926535897932384626;

template<typename T>
T operator+(const T &a, const T &b)
{
	T r(a);
	r += b;
	return r;
}
template<typename T>
T operator-(const T &a, const T &b)
{
	T r(a);
	r -= b;
	return r;
}

static long double dot(long double a, long double b)
{
	return a*b;
}

namespace vt
{
	template<typename T>
	long double abs(const T &a)
	{
		return std::sqrt(dot(a, a));
	}
	static long double abs(long double a)
	{
		return std::abs(a);
	}

	template<typename T>
	void nrm(T &a)
	{
		a /= abs(a);
	}
}

#endif
