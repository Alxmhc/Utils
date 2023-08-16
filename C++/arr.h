#ifndef H_ARR
#define H_ARR

#include <algorithm>

template<typename T>
void v_xor(T* a, const T* b, std::size_t sz)
{
	std::transform(a, a + sz, b, a, [](T a, T b){return a ^ b;});
}

template<typename T>
void v_add(T* a, const T* b, std::size_t sz)
{
	std::transform(a, a + sz, b, a, [](T a, T b){return a + b;});
}

#endif
