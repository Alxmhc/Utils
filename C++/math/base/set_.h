template<typename T>
std::set<T> operator&(const std::set<T> &a, const std::set<T> &b)
{
	std::set<T> res;
	std::set_intersection(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::inserter(res, res.begin()));
	return res;
}

template<typename T>
std::set<T> operator|(const std::set<T> &a, const std::set<T> &b)
{
	std::set<T> res;
	std::set_union(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::inserter(res, res.begin()));
	return res;
}

template<typename T>
std::set<T> operator^(const std::set<T> &a, const std::set<T> &b)
{
	std::set<T> res;
	std::set_symmetric_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::inserter(res, res.begin()));
	return res;
}

template<typename T>
std::set<T> operator-(const std::set<T> &a, const std::set<T> &b)
{
	std::set<T> res;
	std::set_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::inserter(res, res.begin()));
	return res;
}
