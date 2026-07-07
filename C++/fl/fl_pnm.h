#ifndef H_FL_PNM
#define H_FL_PNM

#include "../data/convert/base.h"
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
			{
				const std::string str = convert::base::Enc::pr_num<10>(sx, convert::base::dict_d);
				bw.writeS(str.c_str(), str.length());
			}
			bw.write(' ');
			{
				const std::string str = convert::base::Enc::pr_num<10>(sy, convert::base::dict_d);
				bw.writeS(str.c_str(), str.length());
			}
			bw.write('\n');
			if(t != tGr1)
			{
				const std::string str = convert::base::Enc::pr_num<10>(mx, convert::base::dict_d);
				bw.writeS(str.c_str(), str.length());
				bw.write('\n');
			}
			return true;
		}
	};
}

#endif
