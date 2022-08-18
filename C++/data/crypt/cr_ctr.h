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
	class Encoder : public byteWriterBuf<CR::block_size>
	{
		const CR* cr;
		uint8_t iv[CR::block_size];
		byteWriter* bw;

		void upd(const uint8_t* v, size_t sz)
		{
			uint8_t tmp[CR::block_size];
			std::copy_n(iv, CR::block_size, tmp);
			cr->Enc.process(tmp);
			std::transform(v, v + sz, tmp, tmp, [](uint8_t a, uint8_t b){return a ^ b;});
			bw->writeN(tmp, sz);
			INCR::incr(iv, CR::block_size);
		}

		void process(const uint8_t* v)
		{
			upd(v, CR::block_size);
		}
	public:
		Encoder(const CR &c, const uint8_t* v, byteWriter &b) : cr(&c), bw(&b)
		{
			std::copy_n(v, CR::block_size, iv);
		}

		void Fin()
		{
			if(size() != 0)
			{
				upd(data(), size());
				reset();
			}
			bw->Fin();
		}
	};

	template<class CR, class INCR>
	class Decoder : public Encoder<CR, INCR>
	{
	public:
		Decoder(const CR &c, const uint8_t* v, byteWriter &b) : Encoder<CR, INCR>(c, v, b) {}
	};
}
