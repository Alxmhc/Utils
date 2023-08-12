#ifndef H_FL_TAR
#define H_FL_TAR

#include "../cont.h"

namespace fl_pr
{
	class F_tar : public cont_n
	{
		enum
		{
			tFile = 0,
			tDir  = 5
		};

		struct infF
		{
			std::string fname;
			uint8_t type;
		};
		std::vector<infF> infFs;

		bool read_hdr(infF &inf, inf_1 &p)
		{
			br->read_string(0, inf.fname);
			if(inf.fname.length() == 0)
				return false;
			br->skip(123 - inf.fname.length());
			uint8_t sz[12];
			if( !br->readN(sz, 12) )
				return false;
			p.data_size = strtoul(reinterpret_cast<const char*>(sz), nullptr, 8);
			if( !br->skip(20) )
				return false;
			if( !br->get(inf.type) )
				return false;
			if( !br->skip(355) )
				return false;
			p.data_pos = br->get_pos();
			return true;
		}
	public:
		bool read(byteReader* r)
		{
			br = r;

			infFs.clear();
			for(;;)
			{
				infF inf;
				inf_1 p;
				if( !read_hdr(inf, p) )
					break;
				if(p.data_size != 0)
				{
					const std::size_t bsize = ((p.data_size + 0x1ff)>>9)<<9;
					if( !br->skip(bsize) )
						break;
				}
				inf_n.push_back(p);
				infFs.push_back(inf);
			}
			return true;
		}

		std::string name(std::size_t n) const
		{
			return infFs[n].fname;
		}

		bool GetData(std::size_t n, byteWriter &bw)
		{
			Init(n);
			copy(*br, bw);
			return true;
		}
	};
}

#endif
