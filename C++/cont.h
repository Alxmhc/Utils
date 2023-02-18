#ifndef H_CONT
#define H_CONT

#include "data/byte_reader.h"
#include "data/byte_writer.h"

class cont_1
{
protected:
	byteReader* br;

	std::size_t data_pos;
	std::size_t data_size;

	void Init()
	{
		br->set_size(data_pos + data_size);
		br->set_pos(data_pos);
	}
};

class cont_n
{
protected:
	byteReader* br;

	struct inf_1
	{
		std::size_t data_pos;
		std::size_t data_size;
	};
	std::vector<inf_1> inf_n;

	void Init(std::size_t n)
	{
		br->set_size(inf_n[n].data_pos + inf_n[n].data_size);
		br->set_pos(inf_n[n].data_pos);
	}
public:
	std::size_t sz() const
	{
		return inf_n.size();
	}
};

bool copy(byteReader &br, byteWriter &bw)
{
	std::vector<uint8_t> data;
	br.readN(data, br.get_rsize());
	bw.writeN(data.data(), data.size());
	return true;
}

#endif
