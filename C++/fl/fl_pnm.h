#ifndef H_FL_PNM
#define H_FL_PNM

#include "../data/byte_writer.h"

namespace fl_pr
{
	class F_pnm
	{
	public:
		enum type
		{
			tGr1  = 1, //pbm
			tGr8  = 2, //pgm
			tRGB8 = 3  //ppm
		};

		static bool WriteBhdr(byteWriter &bw, unsigned int sx, unsigned int sy, type t, uint8_t mx = 255)
		{
			bw.write('P');
			switch(t)
			{
			case tGr1:
				bw.write('4');
				break;
			case tGr8:
				bw.write('5');
				break;
			case tRGB8:
				bw.write('6');
				break;
			default:
				return false;
			}
			bw.write('\n');
			bw.writeS(std::to_string(sx));
			bw.write(' ');
			bw.writeS(std::to_string(sy));
			bw.write('\n');
			if(t != tGr1)
			{
				bw.writeS(std::to_string(mx));
				bw.write('\n');
			}
			return true;
		}
	};
}

#endif
