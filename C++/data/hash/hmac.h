namespace hash
{
	//rfc 2104
	template<class H>
	class HMAC
	{
		H hash;
		uint8_t ki[H::block_size], ko[H::block_size];
	public:
		static const uint_fast8_t hash_size = H::hash_size;

		void SetKey(const uint8_t *key, size_t ksize)
		{
			std::fill(ki, ki + H::block_size, 0x36);
			std::fill(ko, ko + H::block_size, 0x5c);
			if(ksize > H::block_size)
			{
				uint8_t tmp[H::hash_size];
				hash.Update(key, ksize);
				hash.Final(tmp);
				for(uint_fast16_t i = 0; i < H::hash_size; ++i)
				{
					ki[i] ^= tmp[i];
					ko[i] ^= tmp[i];
				}
			}
			else
			{
				for(uint_fast16_t i = 0; i < ksize; ++i)
				{
					ki[i] ^= key[i];
					ko[i] ^= key[i];
				}
			}
		}
		void Init()
		{
			hash.Update(ki, H::block_size);
		}
		void Update(const uint8_t *data, size_t dsize)
		{
			hash.Update(data, dsize);
		}
		void Final(uint8_t *res)
		{
			hash.Final(res);
			hash.Update(ko, H::block_size);
			hash.Update(res, H::hash_size);
			hash.Final(res);
		}
	};
}
