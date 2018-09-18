template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const T &a)
{
	auto n = v.size();
	while(n--){v[n] += a;}
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const T &a)
{
	auto n = v.size();
	while(n--){v[n] -= a;}
	return v;
}

template<typename T>
const std::vector<T>& operator*=(std::vector<T> &v, long double k)
{
	auto n = v.size();
	while(n--){v[n] *= k;}
	return v;
}

template<typename T>
std::vector<T> operator*(const std::vector<T> &v, long double k)
{
	std::vector<T> r = v;
	r *= k;
	return r;
}

template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const std::vector<T> &a)
{
	auto n = v.size();
	while(n--){v[n] += a[n];}
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const std::vector<T> &a)
{
	auto n = v.size();
	while(n--){v[n] -= a[n];}
	return v;
}

template<typename T>
T sum(const std::vector<T> &v)
{
	auto n = v.size();
	if(n == 0){return T();}
	T s = v[--n];
	while(n--){s += v[n];}
	return s;
}

template<typename T>
T avr(const std::vector<T> &v)
{
	auto n = v.size();
	if(n == 0){return T();}
	return sum(v) / static_cast<long double>(n);
}

template<typename T>
long double operator*(const std::vector<T> &a, const std::vector<T> &b)
{
	auto n = a.size();
	long double s=0.0;
	while(n--){s += a[n]*b[n];}
	return s;
}
