#ifndef H_CR_M
#define H_CR_M

#include "../../arr.h"
#include "../byte_writer.h"

template<class CR>
class cr_str : public byteWriterBuf<CR::block_size>
{
	uint8_t iv[CR::block_size];
protected:
	const CR* cr;
	uint8_t iv_cur[CR::block_size];
	byteWriter* bw;

	virtual void upd(const uint8_t*, std::size_t) = 0;

	void process(const uint8_t* v)
	{
		upd(v, CR::block_size);
	}
public:
	cr_str(const CR &c, const uint8_t* v, byteWriter &b) : cr(&c), bw(&b)
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
	class Encoder : public cr_str<CR>
	{
		void upd(const uint8_t* v, std::size_t sz)
		{
			uint8_t tmp[CR::block_size];
			std::copy_n(this->iv_cur, CR::block_size, tmp);
			this->cr->Enc.process(tmp);
			v_xor(tmp, v, sz);
			this->bw->writeN(tmp, sz);
			INCR::incr(this->iv_cur, CR::block_size);
		}
	public:
		Encoder(const CR &c, const uint8_t* v, byteWriter &b) : cr_str<CR>(c, v, b) {}
	};

	template<class CR, class INCR>
	class Decoder : public Encoder<CR, INCR>
	{
	public:
		Decoder(const CR &c, const uint8_t* v, byteWriter &b) : Encoder<CR, INCR>(c, v, b) {}
	};
}

namespace CR_OFB
{
	template<class CR>
	class Encoder : public cr_str<CR>
	{
		void upd(const uint8_t* v, std::size_t sz)
		{
			this->cr->Enc.process(this->iv_cur);
			uint8_t tmp[CR::block_size];
			std::copy_n(v, sz, tmp);
			v_xor(tmp, this->iv_cur, sz);
			this->bw->writeN(tmp, sz);
		}
	public:
		Encoder(const CR &c, const uint8_t* v, byteWriter &b) : cr_str<CR>(c, v, b) {}
	};

	template<class CR>
	class Decoder : public Encoder<CR>
	{
	public:
		Decoder(const CR &c, const uint8_t* v, byteWriter &b) : Encoder<CR>(c, v, b) {}
	};
}

namespace CR_CFB
{
	template<class CR>
	class Encoder : public cr_str<CR>
	{
		void upd(const uint8_t* v, std::size_t sz)
		{
			this->cr->Enc.process(this->iv_cur);
			v_xor(this->iv_cur, v, sz);
			this->bw->writeN(this->iv_cur, sz);
		}
	public:
		Encoder(const CR &c, const uint8_t* v, byteWriter &b) : cr_str<CR>(c, v, b) {}
	};

	template<class CR>
	class Decoder : public cr_str<CR>
	{
		void upd(const uint8_t* v, std::size_t sz)
		{
			this->cr->Enc.process(this->iv_cur);
			v_xor(this->iv_cur, v, sz);
			this->bw->writeN(this->iv_cur, sz);
			std::copy_n(v, sz, this->iv_cur);
		}
	public:
		Decoder(const CR &c, const uint8_t* v, byteWriter &b) : cr_str<CR>(c, v, b) {}
	};
}

namespace CR_ECB
{
	template<class CR>
	class Enc
	{
		const CR* cr;
	public:
		Enc(const CR &c) : cr(&c) {}
		void process(uint8_t* v)
		{
			cr->Enc.process(v);
		}
	};

	template<class CR>
	class Dec
	{
		const CR* cr;
	public:
		Dec(const CR &c) : cr(&c) {}
		void process(uint8_t* v)
		{
			cr->Dec.process(v);
		}
	};
}

namespace CR_CBC
{
	template<class CR>
	class Enc
	{
		const CR* cr;
		uint8_t iv[CR::block_size];
		uint8_t iv_cur[CR::block_size];
	public:
		Enc(const CR &c, const uint8_t* v) : cr(&c)
		{
			std::copy_n(v, CR::block_size, iv);
			std::copy_n(v, CR::block_size, iv_cur);
		}
		void process(uint8_t* v)
		{
			v_xor(v, iv_cur, CR::block_size);
			cr->Enc.process(v);
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
		const CR* cr;
		uint8_t iv[CR::block_size];
		uint8_t iv_cur[CR::block_size];
	public:
		Dec(const CR &c, const uint8_t* v) : cr(&c)
		{
			std::copy_n(v, CR::block_size, iv);
			std::copy_n(v, CR::block_size, iv_cur);
		}
		void process(uint8_t* v)
		{
			uint8_t iv_tmp[CR::block_size];
			std::copy_n(iv_cur, CR::block_size, iv_tmp);
			std::copy_n(v, CR::block_size, iv_cur);

			cr->Dec.process(v);
			v_xor(v, iv_tmp, CR::block_size);
		}
		void reset()
		{
			std::copy_n(iv, CR::block_size, iv_cur);
		}
	};
}

#endif
