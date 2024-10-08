#ifndef H_FL_TAR
#define H_FL_TAR

#include "../cont.h"

namespace fl_pr
{
	class F_tar
	{
		enum
		{
			tFile = 0,
			tDir  = 5
		};

		byteReader* br;
		struct infF
		{
			std::string fname;
			uint8_t type;

			std::size_t data_pos;
			std::size_t data_size;
		};
		std::vector<infF> infFs;

		bool read_hdr(infF &inf)
		{
			br->read_string(0, inf.fname);
			if(inf.fname.length() == 0)
				return false;
			br->skip(123 - inf.fname.length());
			uint8_t sz[12];
			if( !br->readN(sz, 12) )
				return false;
			inf.data_size = strtoul(reinterpret_cast<const char*>(sz), nullptr, 8);
			if( !br->skip(20) )
				return false;
			if( !br->get(inf.type) )
				return false;
			if( !br->skip(355) )
				return false;
			inf.data_pos = br->get_pos();
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
				if( !read_hdr(inf) )
					break;
				const std::size_t bsize = ((inf.data_size + 0x1ff)>>9)<<9;
				if( !br->skip(bsize) )
					break;
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
			br->set_pos(infFs[n].data_pos);
			br->set_rsize(infFs[n].data_size);

			copy(*br, bw);
			return true;
		}
	};
}

#endif
