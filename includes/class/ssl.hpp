#pragma once

#include "socket.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace ssl
{
	class SSLContext
	{
		public:
			SSL_CTX *ctx;
	};


	class SSocket : public Socket
	{
		public:


			explicit SSocket(int family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP, int fd = -1);
			SSocket(const SSocket &instance);
			~SSocket();
	};
};
