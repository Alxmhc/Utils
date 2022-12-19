class cont_1
{
protected:
	byteReader* br;

	size_t data_pos;
	size_t data_size;

	void getData(std::vector<uint8_t> &data)
	{
		br->set_pos(data_pos);
		br->readN(data, data_size);
	}
};

class cont_n
{
protected:
	byteReader* br;

	struct inf_1
	{
		size_t data_pos;
		size_t data_size;
	};
	std::vector<inf_1> inf_n;

	void getData(size_t n, std::vector<uint8_t> &data)
	{
		br->set_pos(inf_n[n].data_pos);
		br->readN(data, inf_n[n].data_size);
	}
public:
	size_t sz() const
	{
		return inf_n.size();
	}
};
