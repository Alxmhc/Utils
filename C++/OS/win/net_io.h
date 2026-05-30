#ifndef H_NET_IO
#define H_NET_IO

#include <winsock2.h>

#include "../../data/byte_writer.h"

namespace nnet
{
	class Start
	{
		WSADATA ws;
		bool isS;
	public:
		Start()
		{
			isS = WSAStartup(MAKEWORD(2, 2), &ws) == NO_ERROR;
		}
		~Start()
		{
			if (IsStarted())
			{
				WSACleanup();
			}
		}

		bool IsStarted() const
		{
			return isS;
		}
	};

	class Socket
	{
	protected:
		SOCKET s;
	public:
		~Socket()
		{
			if (!isErr())
			{
				closesocket(s);
			}
		}

		bool isErr() const
		{
			return s == INVALID_SOCKET;
		}

		bool Send(const char* d, unsigned int sz) const
		{
			return send(s, d, sz, 0) != SOCKET_ERROR;
		}

		template<std::size_t BSIZE>
		bool Read_all(byteWriter &bw) const
		{
			char buf[BSIZE];
			for(;;)
			{
				const auto sz = recv(s, buf, BSIZE, 0);
				if (sz == SOCKET_ERROR)
					return false;
				if(sz != 0)
				{
					bw.writeS(buf, sz);
				}
				if(sz < BSIZE)
					return true;
			}
		}
	};

	class Socket_TCP4 : public Socket
	{
	public:
		Socket_TCP4()
		{
			s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

		bool Connect(const uint8_t* ip, uint_fast16_t port) const
		{
			sockaddr_in addr = {};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			bconv<1, endianness::LITTLE_ENDIAN>::pack(ip, 4, addr.sin_addr.s_addr);
			return connect(s, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr)) != SOCKET_ERROR;
		}
	};
}

#endif
