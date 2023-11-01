/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:38:58 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 17:29:50 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/socket.hpp"

Socket::Socket(int family, int type, int protocol, int fd) : _closed(false),  _addrinfo(NULL), family(family), type(type), protocol(protocol), fd(fd)
{
	this->fd = socket(this->family, this->type, this->protocol);
	if (this->fd == -1)
		throw std::runtime_error("socket: " + std::string(strerror(errno)));
}

Socket::Socket(int fd) : _closed(false), _addrinfo(NULL), fd(fd)  {}

Socket::Socket(const Socket &instance) : _closed(false)
{
	*this = instance;
}

Socket::~Socket(void)
{
	if (this->_addrinfo != NULL)
		freeaddrinfo(this->_addrinfo);
	this->close();
}

Socket	&Socket::operator=(const Socket &instance)
{
	if (this == &instance)
		return (*this);
	this->fd = instance.fd;
	this->family = instance.family;
	this->type = instance.type;
	this->protocol = instance.protocol;
	this->_closed = instance._closed;
	//! this->_addrinfo = instance._addrinfo;
	return (*this);
}

void	Socket::close(void)
{
	if (this->_closed == false) {
		if (::close(this->fd) == -1)
			throw std::runtime_error("close: " + std::string(strerror(errno)));
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

	bzero(&hints, sizeof(hints));
	if (this->_addrinfo != NULL)
		freeaddrinfo(this->_addrinfo);
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

Socket	Socket::accept(sockaddr *addr, socklen_t *addrlen)
{
	Socket	new_socket;

	new_socket.fd = ::accept(this->fd, addr, addrlen);
	if (new_socket.fd == -1)
		throw std::runtime_error("accept: " + std::string(strerror(errno)));
	return (new_socket);
}

void	Socket::connect(const std::string &host, int port) 
{
	this->connect(host, std::to_string(port));
}

void	Socket::connect(const std::string &host, std::string port)
{
	addrinfo	hints;

	bzero(&hints, sizeof(hints));
	if (this->_addrinfo != NULL)
		freeaddrinfo(this->_addrinfo);
	hints.ai_family = this->family;
	hints.ai_socktype = this->type;
	hints.ai_protocol = this->protocol;
	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &this->_addrinfo) != 0)
		throw std::runtime_error("getaddrinfo: " + std::string(strerror(errno)));
	if (::connect(this->fd, this->_addrinfo->ai_addr, this->_addrinfo->ai_addrlen) == -1)
		throw std::runtime_error("connect: " + std::string(strerror(errno)));
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

std::string Socket::recv(int flags)
{
	std::string	data;
	ssize_t		size = 0;
	char		buffer[1024];

	while ((size = ::recv(this->fd, buffer, 1024, flags)) > 0) {
		data.append(buffer, size);
		if (size < 1024)
			break;
	}

	if (size == -1)
		throw std::runtime_error("recv: " + std::string(strerror(errno)));

	return (data);
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

void	Socket::reuseAddr(void)
{
	int	optval;

	optval = 1;
	this->setSockOpt(SOL_SOCKET, SO_REUSEADDR, optval);
}
