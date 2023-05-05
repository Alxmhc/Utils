#ifndef H_MATH_
#define H_MATH_

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

static long double dot(long double a, long double b)
{
	return a*b;
}

template<typename T>
const T& operator/=(T &a, long double k)
{
	return a*=(1.0/k);
}

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

#endif
