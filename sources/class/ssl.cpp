#include "class/ssl.hpp"

namespace ssl
{
	void init()
	{
		SSL_load_error_strings();
		SSL_library_init();
		OpenSSL_add_all_algorithms();
	}




	SSLContext::SSLContext()
	{
		this->_ctx = SSL_CTX_new(SSLv23_server_method());
		if (this->_ctx == nullptr)
			throw std::runtime_error("SSL_CTX_new(): " + SSLErrorString);
		this->setOptions(SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
	}

	SSLContext::SSLContext(const SSLContext &instance)
	{
		*this = instance;
	}

	SSLContext::SSLContext(const std::string &certificate, const std::string &privateKey)
	{
		this->_ctx = SSL_CTX_new(SSLv23_server_method());
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
	}





	SSocket::SSocket(SSLContext &ctx, int family, int type, int protocol) : Socket(family, type, protocol)
	{
		this->_ctx = ctx;
		this->_ssl = SSL_new(this->_ctx.getCtx());
		if (this->_ssl == nullptr)
			throw std::runtime_error("SSL_new(): " + SSLErrorString);
		if (SSL_set_fd(this->_ssl, this->fd) == 0)
			throw std::runtime_error("SSL_set_fd(): " + SSLErrorString);
	}

	SSocket::~SSocket()
	{
		SSL_free(this->_ssl);
	}
}