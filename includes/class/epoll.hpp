/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:39:57 by vdescham          #+#    #+#             */
/*   Updated: 2023/11/01 19:30:11 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "socket.hpp"
#include <sys/epoll.h>

#define MAX_EV 42

class Epoll: public Socket {
public:
	Epoll();
	void create();
	void poll();
	int wait();
	void acceptClient();
	void recvDataFromClient(int index);


	int timeout = 200; // set timeout to 0.5 sec
	int nfds = 0;
private:
	int						_epfd;
	struct epoll_event		_event;
	struct epoll_event		_events_pool[MAX_EV];
};
