#ifndef H_CR_M
#define H_CR_M

#include "../../arr.h"
#include "../byte_writer.h"

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

template<class CR>
class cr_str : public byteWriterBuf<CR::block_size>
{
	uint8_t iv[CR::block_size];
protected:
	uint8_t iv_cur[CR::block_size];
	byteWriter* bw;

	virtual void upd(const uint8_t*, std::size_t) = 0;

	void process(const uint8_t* v)
	{
		upd(v, CR::block_size);
	}
public:
	cr_str(const uint8_t* v, byteWriter &b) : bw(&b)
	{
		std::copy_n(v, CR::block_size, iv);
		std::copy_n(v, CR::block_size, iv_cur);
	}

	void Fin()
	{
		if(size() != 0)
		{
			upd(data(), size());
			reset();
		}
		bw->Fin();
		std::copy_n(iv, CR::block_size, iv_cur);
	}
};

namespace CR_CFB
{
	template<class CR>
	class Encoder : public cr_str<CR>
	{
		const typename CR::Enc cr;

		void upd(const uint8_t* v, std::size_t sz)
		{
			cr.process(this->iv_cur);
			v_xor(this->iv_cur, v, sz);
			this->bw->writeN(this->iv_cur, sz);
		}
	public:
		Encoder(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v, byteWriter &b) : cr_str<CR>(v, b), cr(k, ksz) {}
	};

	template<class CR>
	class Decoder : public cr_str<CR>
	{
		const typename CR::Enc cr;

		void upd(const uint8_t* v, std::size_t sz)
		{
			cr.process(this->iv_cur);
			v_xor(this->iv_cur, v, sz);
			this->bw->writeN(this->iv_cur, sz);
			std::copy_n(v, sz, this->iv_cur);
		}
	public:
		Decoder(const uint8_t* k, uint_fast8_t ksz, const uint8_t* v, byteWriter &b) : cr_str<CR>(v, b), cr(k, ksz) {}
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
