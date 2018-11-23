template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const T &a)
{
	for(auto n = v.size(); n > 0;)
	{
		--n;
		v[n] += a;
	}
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const T &a)
{
	for(auto n = v.size(); n > 0;)
	{
		--n;
		v[n] -= a;
	}
	return v;
}

template<typename T>
const std::vector<T>& operator*=(std::vector<T> &v, long double k)
{
	for(auto n = v.size(); n > 0;)
	{
		--n;
		v[n] *= k;
	}
	return v;
}

template<typename T>
const std::vector<T>& operator+=(std::vector<T> &v, const std::vector<T> &a)
{
	for(auto n = v.size(); n > 0;)
	{
		--n;
		v[n] += a[n];
	}
	return v;
}

template<typename T>
const std::vector<T>& operator-=(std::vector<T> &v, const std::vector<T> &a)
{
	for(auto n = v.size(); n > 0;)
	{
		--n;
		v[n] -= a[n];
	}
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
long double operator*(const std::vector<T> &a, const std::vector<T> &b)
{
	long double s = 0.0;
	for(auto n = a.size(); n > 0;)
	{
		--n;
		s += a[n]*b[n];
	}
	return s;
}
