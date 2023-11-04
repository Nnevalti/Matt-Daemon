#include "class/socket.hpp"

std::map<int, int> _socketRefCounter = {};

Socket::Socket(int family, int type, int protocol, int fd) : _closed(false),  _addrinfo(NULL), family(family), type(type), protocol(protocol), fd(fd)
{
	if (this->fd == -1)
		this->fd = socket(this->family, this->type, this->protocol);
	if (this->fd == -1)
		throw std::runtime_error("socket: " + std::string(strerror(errno)));
	this->__upRef();
}

Socket::Socket(const Socket &instance) : _closed(false), _addrinfo(NULL), fd(-1)
{
	*this = instance;
}

Socket::~Socket(void)
{
	if (this->_addrinfo != NULL && _socketRefCounter.find(this->fd) == _socketRefCounter.end())
		freeaddrinfo(this->_addrinfo);
	this->close();
}

Socket	&Socket::operator=(const Socket &instance)
{
	if (this == &instance)
		return (*this);

	this->__upRef();
	this->_closed = instance._closed;
	this->fd = instance.fd;
	this->_addrinfo = instance._addrinfo;
	this->family = instance.family;
	this->type = instance.type;
	this->protocol = instance.protocol;

	return (*this);
}

void	Socket::close(void)
{
	if (this->_closed == false) {
		this->__downRef();
		if (_socketRefCounter.find(this->fd) == _socketRefCounter.end()) {
			if (::close(this->fd) == -1)
				throw std::runtime_error("close: " + std::string(strerror(errno)));
		}
		this->_closed = true;
	}
}

void	Socket::bind(std::string address, int port)
{
	this->bind(address, std::to_string(port));
}

void	Socket::bind(std::string address, std::string port)
{
	addrinfo	hints;

	if (this->_addrinfo != NULL)
		throw std::runtime_error("bind: socket already used");
	bzero(&hints, sizeof(hints));
	hints.ai_family = this->family;
	hints.ai_socktype = this->type;
	hints.ai_protocol = this->protocol;
	if (getaddrinfo(address.c_str(), port.c_str(), &hints, &this->_addrinfo) != 0)
		throw std::runtime_error("getaddrinfo: " + std::string(strerror(errno)));
	if (::bind(this->fd, this->_addrinfo->ai_addr, this->_addrinfo->ai_addrlen) == -1)
		throw std::runtime_error("bind: " + std::string(strerror(errno)));
}

void	Socket::listen(int backlog)
{
	if (::listen(this->fd, backlog) == -1)
		throw std::runtime_error("listen: " + std::string(strerror(errno)));
}

Socket	Socket::accept(void)
{
	int fd = ::accept(this->fd, NULL, NULL);
	if (fd == -1)
		throw std::runtime_error("accept: " + std::string(strerror(errno)));
	return (Socket(this->family, this->type, this->protocol, fd));
}

void	Socket::connect(const std::string &host, int port) 
{
	this->connect(host, std::to_string(port));
}

void	Socket::connect(const std::string &host, std::string port)
{
	addrinfo	hints;
	addrinfo	*res;

	if (this->_addrinfo != NULL)
		throw std::runtime_error("connect: socket already used");
	bzero(&hints, sizeof(hints));
	hints.ai_family = this->family;
	hints.ai_socktype = this->type;
	hints.ai_protocol = this->protocol;
	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(errno)));
	if (::connect(this->fd, res->ai_addr, res->ai_addrlen) == -1)
		throw std::runtime_error("connect: " + std::string(strerror(errno)));
	freeaddrinfo(res);
}

ssize_t	Socket::send(const std::string &msg, int flags)
{
	return (this->send(msg.c_str(), msg.size(), flags));
}

ssize_t	Socket::send(const void *data, size_t size, int flags)
{
	ssize_t	ret;

	if ((ret = ::send(this->fd, data, size, flags)) == -1)
		throw std::runtime_error("send: " + std::string(strerror(errno)));
	return (ret);
}

Socket::RecvData Socket::recv(int flags)
{
	std::string	data;
	ssize_t		size = 0;
	ssize_t		total_size = 0;
	char		buffer[MAX_BUFFER_SIZE + 1];

	while ((size = ::recv(this->fd, buffer, MAX_BUFFER_SIZE, flags)) > 0) {
		data.append(buffer, size);
		total_size += size;
		if (size < MAX_BUFFER_SIZE)
			break;
	}
	if (size == -1)
		throw std::runtime_error("recv: " + std::string(strerror(errno)));
	return (std::make_pair(data, total_size));
}

ssize_t	Socket::recv(void *buffer, size_t size, int flags)
{
	ssize_t	ret;

	if ((ret = ::recv(this->fd, buffer, size, flags)) == -1)
		throw std::runtime_error("recv: " + std::string(strerror(errno)));
	return (ret);
}

void	Socket::setNonBlocking(void)
{
	int	flags;

	if ((flags = fcntl(this->fd, F_GETFL, 0)) == -1)
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
	if (fcntl(this->fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
}

void	Socket::setBlocking(void)
{
	int	flags;

	if ((flags = fcntl(this->fd, F_GETFL, 0)) == -1)
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
	if (fcntl(this->fd, F_SETFL, flags & ~O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
}

void	Socket::setSockOpt(int level, int optname, int optval)
{
	if (setsockopt(this->fd, level, optname, &optval, sizeof(optval)) == -1)
		throw std::runtime_error("setsockopt: " + std::string(strerror(errno)));
}

void	Socket::shutdown(int how)
{
	if (::shutdown(this->fd, how) == -1)
		throw std::runtime_error("shutdown: " + std::string(strerror(errno)));
}

void	Socket::reuseAddress(void)
{
	int	optval;

	optval = 1;
	this->setSockOpt(SOL_SOCKET, SO_REUSEADDR, optval);
}

Socket	Socket::clone(void)
{
	Socket	new_socket;

	new_socket.fd = dup(this->fd);
	if (new_socket.fd == -1)
		throw std::runtime_error("dup: " + std::string(strerror(errno)));
	if (this->_addrinfo != NULL) {
		struct addrinfo *res;
		if (getaddrinfo(NULL, "0", this->_addrinfo, &res) != 0)
			throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(errno)));
		new_socket._addrinfo = res;
	}
	return (new_socket);
}


bool Socket::operator==(const Socket &instance) const
{
	return (this->operator==(instance.fd));
}

bool	Socket::operator==(const int &fd) const
{
    if (this->fd == fd)
        return (true);
    else {
        struct sockaddr_storage addr1, addr2;
        socklen_t len1 = sizeof(addr1);
        socklen_t len2 = sizeof(addr2);

        if (getsockname(this->fd, (struct sockaddr *)&addr1, &len1) == -1 ||
            getsockname(fd, (struct sockaddr *)&addr2, &len2) == -1) {
            throw std::runtime_error("getsockname: " + std::string(strerror(errno)));
        }
        if (len1 == len2 && memcmp(&addr1, &addr2, len1) == 0)
            return (true);
    }
    return (false);
}

bool	Socket::operator!=(const Socket &instance) const
{
	return (this->operator!=(instance.fd));
}

bool	Socket::operator!=(const int &fd) const
{
	return (!this->operator==(fd));
}

Socket::operator int(void) const
{
	return (this->fd);
}



void	Socket::__upRef(void)
{
	if (_socketRefCounter.find(this->fd) == _socketRefCounter.end())
		_socketRefCounter[this->fd] = 1;
	else
		_socketRefCounter[this->fd]++;
}

void	Socket::__downRef(void)
{
	if (_socketRefCounter.find(this->fd) != _socketRefCounter.end()) {
		_socketRefCounter[this->fd]--;
		if (_socketRefCounter[this->fd] == 0)
			_socketRefCounter.erase(this->fd);
	}
}
