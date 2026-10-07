#ifndef H_CHUNKED
#define H_CHUNKED

#include "byte_reader.h"
#include "byte_writer.h"
#include "convert/base.h"

namespace chunked
{
	bool decode(uint8_t* v, std::size_t &n)
	{
		const uint8_t* b = v;
		uint8_t* p = v;
		for(;;)
		{
			std::size_t sz = 0;
			while(n != 0)
			{
				if(*p == '\r')
					break;
				uint8_t c;
				if(!convert::base::Dec::pr_char(*p, 16, c))
				{
					if(sz == 0)
						break;
					return false;
				}
				sz = (sz << 4) | c;
				p++;
				n--;
			}
			if(sz == 0)
			{
				n = v - b;
				return true;
			}
			if(n < sz + 4)
				break;
			n -= sz + 4;

			p += 2;
			std::memmove(v, p, sz);
			v += sz;
			p += sz + 2;
		}
		return false;
	}

	bool read(byteReader &br, byteWriter &bw)
	{
		for(;;)
		{
			std::size_t sz = 0;
			for(;;)
			{
				uint8_t c;
				if(!br.get(c))
					return false;
				if(c == '\r')
					break;
				if(!convert::base::Dec::pr_char(c, 16, c))
					return sz == 0;
				sz = (sz << 4) | c;
			}
			if(sz == 0)
				return true;
			if(!br.skip(1))
				return false;
			{
				std::vector<uint8_t> data;
				if (!br.readN(data, sz))
					return false;
				bw.writeN(data.data(), data.size());
			}
			if(!br.skip(2))
				return false;
		}
	}

	class Writer : public byteWriter
	{
	protected:
		byteWriter* bw;
	public:
		Writer(byteWriter &w) : bw(&w) {}

		void writeN(const uint8_t* v, std::size_t n)
		{
			if (n == 0)
				return;
			const auto s = convert::base::Enc::pr_num<16>(n, convert::base::dict);
			bw->writeS(s);
			bw->writeS("\r\n");
			bw->writeN(v, n);
			bw->writeS("\r\n");
		}

		void Fin()
		{
			bw->writeS("0\r\n\r\n");
			bw->Fin();
		}
	};
}

#endif
