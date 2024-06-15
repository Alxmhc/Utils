#ifndef H_FL_7Z
#define H_FL_7Z

#include "C:/TEST/Utils/C++/cont.h"
#include "C:/TEST/Utils/C++/data/hash/crc32.h"

namespace fl_pr
{
	class F_7z : public cont_n
	{
	public:
		enum ID //from description
		{
			pEnd,
			pHeader,
			pArchiveProperties,
			pAdditionalStreamsInfo,
			pMainStreamsInfo,
			pFilesInfo,
			pPackInfo,
			pUnpackInfo,
			pSubStreamsInfo,
			pSize,
			pCRC,
			pFolder,
			pCodersUnpackSize,
			pNumUnpackStream,
			pEmptyStream,
			pEmptyFile,
			pAnti,
			pName,
			pCTime,
			pATime,
			pMTime,
			pAttributes,
			pComment,
			pEncodedHeader,
			pStartPos,
			pDummy
		};

		static bool getNum(byteReader &br, uint64_t &c)
		{
			uint8_t f;
			if(!br.get(f))
				return false;
			if((f & 0x80) == 0)
			{
				c = f;
				return true;
			}

			c = 0;
			uint_fast8_t m = 0x40, p = 0;
			for(;;)
			{
				uint8_t b;
				if( !br.get(b) )
					return false;
				c |= static_cast<uint_fast64_t>(b) << p;
				p += 8;
				if((f & m) == 0)
					break;
				m >>= 1;
			}
			if(m > 1)
			{
				f &= m - 1;
				c |= static_cast<uint_fast64_t>(f) << p;
			}
			return true;
		}

		static bool getNum(byteReader &br, uint32_t &c)
		{
			uint64_t t;
			if( !getNum(br, t) )
				return false;
			c = static_cast<uint32_t>(t);
			return c == t;
		}

		static bool getHeader(byteReader &br, std::vector<uint8_t> &hdr)
		{
			uint8_t header[32];
			if(!br.readN(header, 32))
				return false;
			if(std::memcmp(header, "\x37\x7a\xbc\xaf\x27\x1c", 6) != 0)
				return false;
			if(header[6] != 0)
				return false;

			hash::CRC32 hs;
			uint8_t hash[hash::CRC32::hash_size];

			hs.Update(header + 12, 20);
			hs.Final(hash);
			if(std::memcmp(hash, header + 8, hash::CRC32::hash_size) != 0)
				return false;

			const auto NHoffset = bconv<1, 8, endianness::LITTLE_ENDIAN>::pack(header + 12);
			if(!br.skip(static_cast<size_t>(NHoffset)))
				return false;

			const auto NHsize = bconv<1, 8, endianness::LITTLE_ENDIAN>::pack(header + 20);
			if(NHsize == 0)
				return false;
			if(!br.readN(hdr, static_cast<size_t>(NHsize)))
				return false;

			hs.Update(hdr.data(), hdr.size());
			hs.Final(hash);
			if(std::memcmp(hash, header + 28, hash::CRC32::hash_size) != 0)
				return false;
			return true;
		}
	};
}

#endif
