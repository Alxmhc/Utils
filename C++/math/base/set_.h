#ifndef H_SET_
#define H_SET_

#include <set>
#include <algorithm>

template<typename T>
std::set<T> operator&(const std::set<T> &a, const std::set<T> &b)
{
	std::set<T> res;
	std::set_intersection(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::inserter(res, res.begin()));
	return res;
}

template<typename T>
void operator|=(std::set<T> &a, const std::set<T> &b)
{
	a.insert(b.cbegin(), b.cend());
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

#endif
