#ifndef H_TAR
#define H_TAR

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
	public:
		bool read(byteReader* r)
		{
			br = r;

			infFs.clear();
			for(;;)
			{
				infF r;
				br->read_string(0, r.fname);
				if(r.fname.length() == 0)
					break;
				br->skip(123 - r.fname.length());
				uint8_t sz[12];
				if( !br->readN(sz, 12) )
					break;
				inf_1 inf;
				inf.data_size = strtoul(reinterpret_cast<const char*>(sz), nullptr, 8);
				if( !br->skip(20) )
					break;
				if( !br->get(r.type) )
					break;
				if( !br->skip(355) )
					break;
				inf.data_pos = br->get_pos();
				if(inf.data_size != 0)
				{
					const std::size_t bsize = ((inf.data_size + 0x1ff)>>9)<<9;
					if( !br->skip(bsize) )
						break;
				}
				inf_n.push_back(inf);
				infFs.push_back(r);
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
