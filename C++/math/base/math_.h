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
template<typename T1, typename T2>
T1 operator*(const T1 &a, const T2 &k)
{
	T1 r(a);
	r *= k;
	return r;
}
template<typename T1, typename T2>
T1 operator/(const T1 &a, const T2 &k)
{
	T1 r(a);
	r /= k;
	return r;
}
