#ifndef H_ARR
#define H_ARR

#include <algorithm>

template<typename T>
void v_xor(T* a, const T* b, std::size_t sz)
{
	std::transform(a, a + sz, b, a, [](uint8_t a, uint8_t b){return a ^ b;});
}

#endif
