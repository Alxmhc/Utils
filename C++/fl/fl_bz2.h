#ifndef H_FL_BZ2
#define H_FL_BZ2

#include "../data/compr/bzip2.h"

namespace fl_pr
{
	class F_bz2
	{
		byteReader* br;
	public:
		bool read(byteReader* r)
		{
			br = r;
			return true;
		}

		bool GetData(byteWriter &bw)
		{
			br->set_pos(0);
			if( !compr::bzip2::Decode(*br, bw) )
				return false;
			return true;
		}
	};
}

#endif
