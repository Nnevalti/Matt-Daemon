/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:37:53 by vdescham          #+#    #+#             */
/*   Updated: 2023/11/01 19:30:39 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/epoll.hpp"
#include "matt-daemon.hpp"

Epoll::Epoll() {}

void Epoll::create() {
	_epfd = epoll_create1(0);
	if (_epfd == -1) {
		g_global.logger.logError("epoll_create1: " + std::string(strerror(errno)));
		throw std::runtime_error("epoll_create1: " + std::string(strerror(errno)));
	}
	_event.data.fd = this->fd;
	_event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, this->fd, &_event);
}

int Epoll::wait() {
	if ((nfds = epoll_wait(_epfd, this->_events_pool, MAX_EV, -1)) == -1) {
		g_global.logger.logError("epoll_wait: " + std::string(strerror(errno)));
		throw std::runtime_error("epoll_wait: " + std::string(strerror(errno)));
	}
	return (nfds);
}

void Epoll::acceptClient() {
	int new_socket = 0;

	if ((new_socket = ::accept(fd, NULL, NULL)) == -1) {
		g_global.logger.logError("accept: " + std::string(strerror(errno)));
		throw std::runtime_error("accept: " + std::string(strerror(errno)));
	}
	g_global.logger.logInfo("New client connected. epoll");
	fcntl(new_socket, F_SETFL, O_NONBLOCK);
	_event.data.fd = new_socket;
	_event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, new_socket, &_event);
}

void Epoll::recvDataFromClient(int index) {
	int recv_len = 0;
	char request[1024];
	
	if ((recv_len = ::recv(this->_events_pool[index].data.fd, &request, 1023, 0)) < 0) {
		g_global.logger.logError("recv: " + std::string(strerror(errno)));
	}
	else if (recv_len == 0) {
		g_global.logger.logInfo("Client disconnected.");
		::close(this->_events_pool[index].data.fd);
		epoll_ctl(_epfd, EPOLL_CTL_DEL, this->_events_pool[index].data.fd, NULL);
	}
	else {
		request[recv_len - 1] = '\0';
		g_global.logger.logInfo("Received: " + std::string(request));
	}
}

void Epoll::poll() {
	int nfds = this->wait();

	for (int i = 0; i < nfds; i++) {
		if (this->_events_pool[i].events & (EPOLLHUP | EPOLLERR))
		{
			g_global.logger.logError("epoll_wait: " + std::string(strerror(errno)));
			::close(this->_events_pool[i].data.fd);
			continue;
		}
		if (this->_events_pool[i].data.fd == this->fd)
			this->acceptClient();
		else if (this->_events_pool[i].events & EPOLLIN)
			this->recvDataFromClient(i);
	}
}