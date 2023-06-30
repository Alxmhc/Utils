#ifndef H_FL_BZ2
#define H_FL_BZ2

#include "../cont.h"
#include "../data/compr/bzip2.h"

namespace fl_pr
{
	class F_bz2 : public cont_1
	{
	public:
		bool read(byteReader* r)
		{
			br = r;
			data_pos = 0;
			data_size = r->get_size();
			return true;
		}

		bool GetData(byteWriter &bw)
		{
			Init();
			if( !compr::bzip2::Decode(*br, bw) )
				return false;
			return true;
		}
	};
}

#endif
