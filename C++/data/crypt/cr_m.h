#ifndef H_CR_M
#define H_CR_M

#include <algorithm>

#include "../../arr.h"

namespace crypt
{
	struct iv_ctr
	{
		static void incr(uint8_t* v, uint_fast8_t n)
		{
			while(n--)
			{
				if(v[n] != 0xff)
				{
					v[n]++;
					break;
				}
				v[n] = 0;
			}
		}
	};

	namespace CR_CTR
	{
		template<class CR, class INCR>
		class Encr : public byteProcBuf<CR::block_size>
		{
			const typename CR::Enc* cr;
			uint8_t iv[CR::block_size];

			void gen()
			{
				std::copy_n(iv, CR::block_size, this->buf);
				cr->process(this->buf);
				INCR::incr(iv, CR::block_size);
			}
		public:
			Encr(const typename CR::Enc &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, iv);
			}
		};

		template<class CR, class INCR>
		class Decr : public Encr<CR, INCR>
		{
		public:
			Decr(const typename CR::Enc &c, const uint8_t* v) : Encr<CR, INCR>(c, v) {}
		};
	}

	namespace CR_OFB
	{
		template<class CR>
		class Encr : public byteProcBuf<CR::block_size>
		{
			const typename CR::Enc* cr;

			void gen()
			{
				cr->process(this->buf);
			}
		public:
			Encr(const typename CR::Enc &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, this->buf);
			}
		};

		template<class CR>
		class Decr : public Encr<CR>
		{
		public:
			Decr(const typename CR::Enc &c, const uint8_t* v) : Encr<CR>(c, v) {}
		};
	}

	namespace CR_CFB
	{
		template<class CR>
		class Encr : public byteProcBuf<CR::block_size>
		{
			const typename CR::Enc* cr;

			void gen()
			{
				cr->process(this->buf);
			}

			void post_proc(uint8_t* v, uint8_t* b, std::size_t sz) const override
			{
				v_xor(v, b, sz);
				std::copy_n(v, sz, b);
			}
		public:
			Encr(const typename CR::Enc &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, this->buf);
			}
		};

		template<class CR>
		class Decr : public byteProcBuf<CR::block_size>
		{
			const typename CR::Enc* cr;

			void gen()
			{
				cr->process(this->buf);
			}

			void post_proc(uint8_t* v, uint8_t* b, std::size_t sz) const override
			{
				std::swap_ranges(b, b + sz, v);
				v_xor(v, b, sz);
			}
		public:
			Decr(const typename CR::Enc &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, this->buf);
			}
		};
	}

	namespace CR_ECB
	{
		template<class CR>
		class Enc
		{
			const typename CR::Enc* cr;
		public:
			Enc(const typename CR::Enc &c) : cr(&c) {}
			void process(uint8_t* v) const
			{
				cr->process(v);
			}
		};

		template<class CR>
		class Dec
		{
			const typename CR::Dec* cr;
		public:
			Dec(const typename CR::Dec &c) : cr(&c) {}
			void process(uint8_t* v) const
			{
				cr->process(v);
			}
		};
	}

	namespace CR_CBC
	{
		template<class CR>
		class Enc
		{
			const typename CR::Enc* cr;
			uint8_t iv[CR::block_size];
		public:
			Enc(const typename CR::Enc &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, iv);
			}
			void process(uint8_t* v)
			{
				v_xor(v, iv, CR::block_size);
				cr->process(v);
				std::copy_n(v, CR::block_size, iv);
			}
		};

		template<class CR>
		class Dec
		{
			const typename CR::Dec* cr;
			uint8_t iv[CR::block_size];
		public:
			Dec(const typename CR::Dec &c, const uint8_t* v) : cr(&c)
			{
				std::copy_n(v, CR::block_size, iv);
			}
			void process(uint8_t* v)
			{
				uint8_t iv_tmp[CR::block_size];
				std::copy_n(iv, CR::block_size, iv_tmp);
				std::copy_n(v, CR::block_size, iv);

				cr->process(v);
				v_xor(v, iv_tmp, CR::block_size);
			}
		};
	}
}

#endif
