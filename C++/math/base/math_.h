const long double pi = 3.1415926535897932384626;

long double dot(long double a, long double b)
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
template<typename T>
T operator*(const T &a, long double k)
{
	T r(a);
	r *= k;
	return r;
}
template<typename T>
T operator/(const T &a, long double k)
{
	T r(a);
	r *= (1.0/k);
	return r;
}
