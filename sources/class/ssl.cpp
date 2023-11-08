#include "class/ssl.hpp"

namespace ssl
{
	void init(void)
	{
		SSL_load_error_strings();
		SSL_library_init();
		OpenSSL_add_all_algorithms();
	}

	void cleanup(void)
	{
		EVP_cleanup();
		ERR_free_strings();
	}



	SSLContext::SSLContext(const SSL_METHOD *method)
	{
		this->_ctx = SSL_CTX_new(method);
		if (this->_ctx == nullptr)
			throw std::runtime_error("SSL_CTX_new(): " + SSLErrorString);
		this->setOptions(SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
	}

	SSLContext::SSLContext(const SSLContext &instance): _ctx(nullptr)
	{
		*this = instance;
	}

	SSLContext::SSLContext(const std::string &certificate, const std::string &privateKey, const SSL_METHOD *method)
	{
		this->_ctx = SSL_CTX_new(method);
		if (this->_ctx == nullptr)
			throw std::runtime_error("SSL_CTX_new(): " + SSLErrorString);
		this->setOptions(SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
		this->loadCertificate(certificate);
		this->loadPrivateKey(privateKey);
	}

	SSLContext::~SSLContext()
	{
		SSL_CTX_free(this->_ctx);
	}

	SSL_CTX *SSLContext::getCtx(void) const
	{
		return (this->_ctx);
	}

	SSLContext &SSLContext::operator=(const SSLContext &instance)
	{
		if (this != &instance) {
			if (this->_ctx != nullptr)
				SSL_CTX_free(this->_ctx);
			this->_ctx = instance._ctx;
			SSL_CTX_up_ref(this->_ctx);
		}
		return (*this);
	}

	void SSLContext::setOptions(long options)
	{
		if (SSL_CTX_set_options(this->_ctx, options) == 0)
			throw std::runtime_error("SSL_CTX_set_options(): " + SSLErrorString);
	}

	void SSLContext::loadCertificate(const std::string &path)
	{
		if (SSL_CTX_use_certificate_file(this->_ctx, path.c_str(), SSL_FILETYPE_PEM) == 0)
			throw std::runtime_error("SSL_CTX_use_certificate_file(): " + SSLErrorString);
	}

	void SSLContext::loadPrivateKey(const std::string &path)
	{
		if (SSL_CTX_use_PrivateKey_file(this->_ctx, path.c_str(), SSL_FILETYPE_PEM) == 0)
			throw std::runtime_error("SSL_CTX_use_PrivateKey_file(): " + SSLErrorString);
		if (!SSL_CTX_check_private_key(this->_ctx))
			throw std::runtime_error("SSL_CTX_check_private_key(): " + SSLErrorString);
	}

	void SSLContext::loadCertificateAndPrivateKey(const std::string &certificate, const std::string &privateKey)
	{
		this->loadCertificate(certificate);
		this->loadPrivateKey(privateKey);
	}

	void SSLContext::setCipherList(const std::string &cipherList)
	{
		if (SSL_CTX_set_cipher_list(this->_ctx, cipherList.c_str()) == 0)
			throw std::runtime_error("SSL_CTX_set_cipher_list(): " + SSLErrorString);
	}






	SSocket::SSocket(SSLContext &ctx, int family, int type, int protocol, int fd) : Socket(family, type, protocol, fd), _ctx(ctx)
	{
		this->_ssl = SSL_new(this->_ctx.getCtx());
		if (this->_ssl == nullptr)
			throw std::runtime_error("SSL_new(): " + SSLErrorString);
	}

	SSocket::SSocket(int family, int type, int protocol, int fd) : Socket(family, type, protocol, fd)
	{
		this->_ctx = SSLContext();
		this->_ssl = SSL_new(this->_ctx.getCtx());
		if (this->_ssl == nullptr)
			throw std::runtime_error("SSL_new(): " + SSLErrorString);
	}

	SSocket::SSocket(const SSocket &instance) : Socket(instance)
	{
		*this = instance;
	}

	SSocket::~SSocket()
	{
		SSL_free(this->_ssl);
	}

	SSocket &SSocket::operator=(const SSocket &instance)
	{
		if (this != &instance) {
			Socket::operator=(instance);
			this->_ctx = instance._ctx;
			this->_ssl = instance._ssl;
			SSL_up_ref(this->_ssl);
		}
		return (*this);
	}

	void SSocket::setContext(SSLContext &ctx)
	{
		this->_ctx = ctx;
	}

	SSocket	SSocket::accept(void)
	{
		int fd = ::accept(this->fd, NULL, NULL);
		if (fd == -1)
			throw std::runtime_error("accept: " + std::string(strerror(errno)));
		SSocket	client(this->_ctx, this->family, this->type, this->protocol, fd);
		if (SSL_set_fd(client._ssl, client.fd) == 0)
			throw std::runtime_error("SSL_set_fd(): " + SSLErrorString);
		if (SSL_accept(client._ssl) == -1)
			throw std::runtime_error("SSL_accept(): " + SSLErrorString);
		return (client);
	}

	void	SSocket::connect(const std::string &host, int port)
	{
		this->connect(host, std::to_string(port));
	}

	void SSocket::connect(const std::string &host, std::string port)
	{
		Socket::connect(host, port);
		if (SSL_set_fd(this->_ssl, this->fd) == 0)
			throw std::runtime_error("SSL_set_fd(): " + SSLErrorString);
		if (SSL_connect(this->_ssl) == -1)
			throw std::runtime_error("SSL_connect(): " + SSLErrorString);
	}

	ssize_t SSocket::send(const std::string &msg, int flags)
	{
		(void)flags;
		return (this->send(msg.c_str(), msg.size()));
	}

	ssize_t SSocket::send(const void *data, size_t size, int flags)
	{
		(void)flags;
		ssize_t ret = SSL_write(this->_ssl, data, size);
		if (ret == -1)
			throw std::runtime_error("SSL_write(): " + SSLErrorString);
		return (ret);
	}

	Socket::RecvData SSocket::recv(int flags)
	{
		std::string	data;
		ssize_t		size = 0;
		ssize_t		total_size = 0;
		char		buffer[MAX_BUFFER_SIZE + 1];

		(void)flags;
		while ((size = SSL_read(this->_ssl, buffer, MAX_BUFFER_SIZE)) > 0) {
			data.append(buffer, size);
			total_size += size;
			if (size < MAX_BUFFER_SIZE)
				break;
		}
		if (size == -1)
			throw std::runtime_error("recv: " + std::string(strerror(errno)));
		return (std::make_pair(data, total_size));
	}

	ssize_t SSocket::recv(void *buffer, size_t size, int flags)
	{
		(void)flags;
		ssize_t ret = SSL_read(this->_ssl, buffer, size);
		if (ret == -1)
			throw std::runtime_error("SSL_read(): " + SSLErrorString);
		return (ret);
	}

	void SSocket::shutdown(int how)
	{
		Socket::shutdown(how);
		if (SSL_shutdown(this->_ssl) == -1)
			throw std::runtime_error("SSL_shutdown(): " + SSLErrorString);
	}
}