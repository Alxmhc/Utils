template<class C, class IV>
class CR_CTR : public byteWriterBuf<C::block_size>
{
	IV c;
	C cr;
	byteWriter *bw;

	void upd(const uint8_t *v, size_t sz = bsize)
	{
		c.incr();
		uint8_t tmp[bsize];
		std::copy_n(c.data(), bsize, tmp);
		cr.Encrypt(tmp);
		std::transform(v, v + sz, tmp, tmp, [](uint8_t a, uint8_t b){return a ^ b;});
		bw->writeN(tmp, sz);
	}

	void process(const uint8_t *v)
	{
		upd(v);
	}
public:
	CR_CTR(const uint8_t *key, size_t ksz, byteWriter &b) : cr(key, ksz), bw(&b) {}

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
