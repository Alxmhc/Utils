#ifndef H_HASH
#define H_HASH

#include <cstdint>

template<uint_fast8_t HSIZE, uint_fast8_t BSIZE>
class HASH
{
public:
	static const uint_fast8_t hash_size = HSIZE;
	static const uint_fast8_t block_size = BSIZE;

	virtual void Update(const uint8_t*, const std::size_t) = 0;
	virtual void Final(uint8_t*) = 0;
};

#endif
