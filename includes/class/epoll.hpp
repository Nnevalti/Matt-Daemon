#pragma once

#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <exception>
#include <stdexcept>
#include <list>
#include <cstring>

class Epoll {

private:
	int			_fd;
	int			_max_events;
	bool		_closed;
	epoll_event	*_events;


public:
	typedef typename std::list<std::pair<int, int> >	Events;
	
	Epoll(int max_events = 20);
	~Epoll();
	void	subscribe(int fd, uint32_t events = 0);
	void	unsubscribe(int fd);
	void	modify(int fd, uint32_t events = 0);
	Events	poll(int timeout = -1);
	void	close(void);
	bool	closed(void);
	int	fileno(void);
};
