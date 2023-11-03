#include "class/ssl.hpp"

namespace ssl
{
	SSocket::SSocket(int family, int type, int protocol, int fd) : Socket(family, type, protocol, fd)
	{
	}
	
	SSocket::SSocket(const SSocket &instance) : Socket(instance)
	{
	}
	
	SSocket::~SSocket(void)
	{
	}
};