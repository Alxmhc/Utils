template<class C, class IV>
class CR_CTR : public byteWriterBuf
{
	IV c;
	const C *cr;
	byteWriter *bw;

	void upd(const uint8_t *v, size_t sz)
	{
		c.incr();
		std::vector<uint8_t> tmp(c.data(), c.data() + bsize());
		cr->process(tmp.data());
		std::transform(v, v + sz, tmp.begin(), tmp.begin(), [](uint8_t a, uint8_t b){return a ^ b;});
		bw->writeN(tmp.data(), sz);
	}

	void process(const uint8_t *v)
	{
		upd(v, bsize());
	}
public:
	CR_CTR(const C &c, byteWriter &b) : byteWriterBuf(C::block_size), cr(&c), bw(&b) {}

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
