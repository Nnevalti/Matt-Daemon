#include "class/epoll.hpp"

Epoll::Epoll(int max_events) : _max_events(max_events), _closed(false)
{
	this->_fd = epoll_create1(0);
	if (this->_fd == -1)
		throw std::runtime_error("epoll_create1: " + std::string(strerror(errno)));
	this->_events = new epoll_event[this->_max_events];
	if (this->_events == NULL)
		throw std::runtime_error("new: " + std::string(strerror(errno)));
}

Epoll::~Epoll()
{
	this->close();
	delete[] this->_events;
}

void	Epoll::subscribe(int fd, uint32_t events)
{
	epoll_event event;
	bzero(&event, sizeof(event));
	
	if (this->_closed)
		throw std::runtime_error("Epoll is closed");
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("epoll_ctl(EPOLL_CTL_ADD): " + std::string(strerror(errno)));
}

void	Epoll::unsubscribe(int fd)
{
	if (this->_closed)
		throw std::runtime_error("Epoll is closed");
	
	if (epoll_ctl(this->_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("epoll_ctl(EPOLL_CTL_DEL): " + std::string(strerror(errno)));
}

void	Epoll::modify(int fd, uint32_t events)
{
	epoll_event event;
	bzero(&event, sizeof(event));

	
	if (this->_closed)
		throw std::runtime_error("Epoll is closed");
	event.events = events;
	event.data.fd = fd;
	if (epoll_ctl(this->_fd, EPOLL_CTL_MOD, fd, &event) == -1)
		throw std::runtime_error("epoll_ctl(EPOLL_CTL_MOD): " + std::string(strerror(errno)));
}

Epoll::Events	Epoll::poll(int timeout)
{
	if (this->_closed)
		throw std::runtime_error("Epoll is closed");
	int		nb_events;
	nb_events = epoll_wait(this->_fd, this->_events, this->_max_events, timeout);
	if (nb_events == -1)
		throw std::runtime_error("epoll_wait: " + std::string(strerror(errno)));
	Events	events;
	for (int i = 0; i < nb_events; i++) {
		int fd = this->_events[i].data.fd;
		uint32_t events_tmp = this->_events[i].events;
		events.push_back(std::make_pair(fd, events_tmp));
	}
	return (events);
}

void	Epoll::close(void)
{
	if (this->_closed)
		return ;
	this->_closed = true;
	if (::close(this->_fd) == -1)
		throw std::runtime_error("close: " + std::string(strerror(errno)));
}

bool	Epoll::closed(void)
{
	return (this->_closed);
}

int	Epoll::fileno(void)
{
	return (this->_fd);
}