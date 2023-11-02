#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <utility>
#include <string>
#include <stdexcept>
#include <vector>

class Socket
{
private:
	bool		_closed;
	addrinfo	*_addrinfo;

public:
	int family;
	int type;
	int protocol;
	int	fd;

	typedef typename std::pair<std::string, ssize_t>	RecvData;

	explicit Socket(int family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP, int fd = -1);
	explicit Socket(const int fd);
	Socket(const Socket &instance);
	~Socket();

	Socket		&operator=(const Socket &instance);

	void		bind(std::string address, int port);
	void		bind(std::string address, std::string port);
	void		listen(int backlog = 5);
	Socket		accept(sockaddr *addr = NULL, socklen_t *addrlen = NULL);
	void		connect(const std::string &host, int port);
	void		connect(const std::string &host, std::string port);
	ssize_t		send(const std::string &msg, int flags = 0);
	ssize_t		send(const void *data, size_t size, int flags = 0);
	RecvData	recv(int flags = 0);
	ssize_t		recv(void *buffer, size_t size, int flags = 0);
	void		close(void);
	void		setNonBlocking(void);
	void		setBlocking(void);
	void		setSockOpt(int level, int optname, int optval);
	void		shutdown(int how = SHUT_RDWR);
	void		reuseAddress(void);

	bool		operator==(const Socket &instance) const;
	bool		operator==(const int &fd) const;
};
