#ifndef H_HMAC
#define H_HMAC

#include <algorithm>
#include "./hash.h"

namespace hash
{
	//rfc 2104
	template<class H>
	class HMAC : public HASH<H::hash_size, H::block_size>
	{
		H hash;
		uint8_t ki[H::block_size], ko[H::block_size];

		void Init()
		{
			hash.Update(ki, H::block_size);
		}
	public:
		HMAC(const uint8_t* key, std::size_t ksize)
		{
			std::fill(ki, ki + H::block_size, 0x36);
			std::fill(ko, ko + H::block_size, 0x5c);
			if(ksize <= H::block_size)
			{
				for(uint_fast8_t i = 0; i < ksize; ++i)
				{
					ki[i] ^= key[i];
					ko[i] ^= key[i];
				}
			}
			else
			{
				hash.Update(key, ksize);
				uint8_t tmp[H::hash_size];
				hash.Final(tmp);
				for(uint_fast8_t i = 0; i < H::hash_size; ++i)
				{
					ki[i] ^= tmp[i];
					ko[i] ^= tmp[i];
				}
			}
			Init();
		}
		void Update(const uint8_t* data, const std::size_t dsize)
		{
			hash.Update(data, dsize);
		}
		void Final(uint8_t* res)
		{
			hash.Final(res);
			hash.Update(ko, H::block_size);
			hash.Update(res, H::hash_size);
			hash.Final(res);
			Init();
		}
	};
}

#endif
