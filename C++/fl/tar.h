namespace fl_pr
{
	class F_tar
	{
		br_fstream br;

		enum
		{
			tFile = 0,
			tDir  = 5
		};

		struct infF
		{
			size_t data_pos;
			size_t data_size;
			std::string name;

			uint8_t type;
		};
		std::vector<infF> infFs;

		bool read_inf(infF &r)
		{
			br.read_string('\0', r.name);
			if(r.name.length() == 0)
				return false;
			br.skip(123 - r.name.length());
			uint8_t sz[12];
			if( !br.readN(sz, 12) )
				return false;
			r.data_size = strtoul(reinterpret_cast<const char*>(sz), nullptr, 8);
			if( !br.skip(20) )
				return false;
			if( !br.get(r.type) )
				return false;
			if( !br.skip(355) )
				return false;
			r.data_pos = br.get_pos();
			if(r.data_size != 0)
			{
				const size_t bsize = ((r.data_size + 0x1ff)>>9)<<9;
				if( !br.skip(bsize) )
					return false;
			}
			return true;
		}

		void getData(size_t n, std::vector<uint8_t> &data)
		{
			br.set_pos(infFs[n].data_pos);
			br.readN(data, infFs[n].data_size);
		}
	public:
		bool open(const char* fl)
		{
			infFs.clear();
			if( !br.open(fl) )
				return false;
			for(;;)
			{
				infF r;
				if( !read_inf(r) )
					break;
				infFs.push_back(r);
			}
			return true;
		}

		size_t sz() const
		{
			return infFs.size();
		}

		std::vector<std::string> names() const
		{
			const size_t sz = infFs.size();
			std::vector<std::string> res(sz);
			for(size_t i = 0; i < sz; i++)
			{
				res[i] = infFs[i].name;
			}
			return res;
		}

		bool GetData(size_t n, std::vector<uint8_t> &data)
		{
			getData(n, data);
			return true;
		}
	};
}
