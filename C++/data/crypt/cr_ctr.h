namespace CR_CTR
{
	template<class C, class IV>
	class Encoder : public byteWriterBuf<C::block_size>
	{
		IV c;
		const C *cr;
		byteWriter *bw;

		void upd(const uint8_t *v, size_t sz)
		{
			uint8_t tmp[C::block_size];
			std::copy_n(c.data(), C::block_size, tmp);
			c.incr();
			cr->Enc.process(tmp);
			std::transform(v, v + sz, tmp, tmp, [](uint8_t a, uint8_t b){return a ^ b;});
			bw->writeN(tmp, sz);
		}

		void process(const uint8_t *v)
		{
			upd(v, C::block_size);
		}
	public:
		Encoder(const C &c, byteWriter &b) : cr(&c), bw(&b) {}

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

	template<class C, class IV>
	class Decoder : public Encoder<C, IV>
	{
	public:
		Decoder(const C &c, byteWriter &b) : Encoder<C, IV>(c, b) {}
	};
}
