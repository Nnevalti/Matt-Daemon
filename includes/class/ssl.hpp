#pragma once

#include "socket.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

#define SSLErrorString std::string(ERR_error_string(ERR_get_error(), nullptr))

namespace ssl
{
	void init();
	void cleanup(void);

	class SSLContext
	{
		private:
			SSL_CTX	*_ctx;

		public:
			SSLContext(const SSL_METHOD *method = SSLv23_method());
			SSLContext(const SSLContext &instance);
			SSLContext(const std::string &certificate, const std::string &privateKey, const SSL_METHOD *method = SSLv23_method());
			~SSLContext();

			SSLContext &operator=(const SSLContext &instance);

			SSL_CTX	*getCtx(void) const;
			void	setOptions(long options);
			void	loadCertificate(const std::string &path);
			void	loadPrivateKey(const std::string &path);
			void	loadCertificateAndPrivateKey(const std::string &certificate, const std::string &privateKey);
			void	setCipherList(const std::string &cipherList);
	};

	class SSocket : public Socket
	{
		private:
			SSL			*_ssl;
			SSLContext	_ctx;
		
		public:
			SSocket(int family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP, int fd = -1);
			SSocket(SSLContext &ctx, int family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP, int fd = -1);
			SSocket(const SSocket &instance);
			~SSocket() override;

			SSocket		&operator=(const SSocket &instance);

			void		setContext(SSLContext &ctx);
			SSocket		accept(void);
			void		connect(const std::string &host, int port) override;
			void		connect(const std::string &host, std::string port) override;
			ssize_t		send(const std::string &msg, int flags = 0) override;
			ssize_t		send(const void *data, size_t size, int flags = 0) override;
			RecvData	recv(int flags = 0) override;
			ssize_t		recv(void *buffer, size_t size, int flags = 0) override;
			void		shutdown(int how = SHUT_RDWR) override;
	};
};
