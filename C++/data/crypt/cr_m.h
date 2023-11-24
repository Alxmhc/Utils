#ifndef H_CR_M
#define H_CR_M

#include "../../arr.h"

struct iv_ctr
{
	static void incr(uint8_t* v, const uint_fast8_t sz)
	{
		uint_fast8_t i = sz;
		while(i != 0)
		{
			i--;
			if(v[i] != 255)
			{
				v[i]++;
				break;
			}
			v[i] = 0;
		}
	}
};

namespace CR_CTR
{
	template<class CR, class INCR>
	class Encr : public byteProcBuf<CR::block_size>
	{
		const typename CR::Enc cr;
		uint8_t iv[CR::block_size];

		void gen()
		{
			std::copy_n(iv, CR::block_size, this->buf);
			cr.process(this->buf);
			INCR::incr(iv, CR::block_size);
		}
	public:
		Encr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
		{
			std::copy_n(v, CR::block_size, iv);
		}
	};

	template<class CR, class INCR>
	class Decr : public Encr<CR, INCR>
	{
	public:
		Decr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : Encr<CR, INCR>(k, ksz, v) {}
	};
}

namespace CR_OFB
{
	template<class CR>
	class Encr : public byteProcBuf<CR::block_size>
	{
		const typename CR::Enc cr;

		void gen()
		{
			cr.process(this->buf);
		}
	public:
		Encr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
		{
			std::copy_n(v, CR::block_size, this->buf);
		}
	};

	template<class CR>
	class Decr : public Encr<CR>
	{
	public:
		Decr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : Encr<CR>(k, ksz, v) {}
	};
}

namespace CR_CFB
{
	template<class CR>
	class Encr : public byteProcBuf<CR::block_size>
	{
		const typename CR::Enc cr;

		void gen()
		{
			cr.process(this->buf);
		}

		void post_proc(uint8_t* v, uint8_t* b, std::size_t sz) const override
		{
			v_xor(v, b, sz);
			std::copy_n(v, sz, b);
		}
	public:
		Encr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
		{
			std::copy_n(v, CR::block_size, this->buf);
		}
	};

	template<class CR>
	class Decr : public byteProcBuf<CR::block_size>
	{
		const typename CR::Enc cr;

		void gen()
		{
			cr.process(this->buf);
		}

		void post_proc(uint8_t* v, uint8_t* b, std::size_t sz) const override
		{
			uint8_t t[CR::block_size];
			std::copy_n(v, sz, t);
			v_xor(v, b, sz);
			std::copy_n(t, sz, b);
		}
	public:
		Decr(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
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
		const typename CR::Enc cr;
	public:
		Enc(const uint8_t* k, uint_fast8_t ksz) : cr(k, ksz) {}
		void process(uint8_t* v)
		{
			cr.process(v);
		}
	};

	template<class CR>
	class Dec
	{
		const typename CR::Dec cr;
	public:
		Dec(const uint8_t* k, uint_fast8_t ksz) : cr(k, ksz) {}
		void process(uint8_t* v)
		{
			cr.process(v);
		}
	};
}

namespace CR_CBC
{
	template<class CR>
	class Enc
	{
		const typename CR::Enc cr;
		uint8_t iv[CR::block_size];
		uint8_t iv_cur[CR::block_size];
	public:
		Enc(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
		{
			std::copy_n(v, CR::block_size, iv);
			std::copy_n(v, CR::block_size, iv_cur);
		}
		void process(uint8_t* v)
		{
			v_xor(v, iv_cur, CR::block_size);
			cr.process(v);
			std::copy_n(v, CR::block_size, iv_cur);
		}
		void reset()
		{
			std::copy_n(iv, CR::block_size, iv_cur);
		}
	};

	template<class CR>
	class Dec
	{
		const typename CR::Dec cr;
		uint8_t iv[CR::block_size];
		uint8_t iv_cur[CR::block_size];
	public:
		Dec(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v) : cr(k, ksz)
		{
			std::copy_n(v, CR::block_size, iv);
			std::copy_n(v, CR::block_size, iv_cur);
		}
		void process(uint8_t* v)
		{
			uint8_t iv_tmp[CR::block_size];
			std::copy_n(iv_cur, CR::block_size, iv_tmp);
			std::copy_n(v, CR::block_size, iv_cur);

			cr.process(v);
			v_xor(v, iv_tmp, CR::block_size);
		}
		void reset()
		{
			std::copy_n(iv, CR::block_size, iv_cur);
		}
	};
}

#endif
