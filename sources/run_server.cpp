/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:27:55 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/02 15:15:51 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

static void	__acceptClient(Socket &server, Epoll &epoll, std::map<int, std::shared_ptr<Socket>> &clients)
{
	std::shared_ptr<Socket>	client =  std::make_shared<Socket>(server.accept());
	if (clients.size() < MAX_CLIENT) {
		epoll.subscribe(client->fd, EPOLLIN);
		clients.insert(std::pair<int, std::shared_ptr<Socket>>(client->fd, client));
		g_global.logger.log("Client " + std::to_string(client->fd) + " connected.");
	}
	else {
		client->send("Server is full.");
		client->close();
		g_global.logger.logWarning("Attempting to connect to a full server.");
	}
}

static void	__removeClient(Epoll &epoll, std::map<int, std::shared_ptr<Socket>> &clients, int fd)
{
	epoll.unsubscribe(fd);
	clients[fd]->close();
	clients.erase(fd);
	g_global.logger.log("Client " + std::to_string(fd) + " disconnected.");
}

static void	__readClient(Epoll &epoll, std::map<int, std::shared_ptr<Socket>> &clients, int fd)
{
	Socket::RecvData	data = clients[fd]->recv();

	if (data.second == 0)
		__removeClient(epoll, clients, fd);
	else {
		data.first.pop_back();
		g_global.logger.log("Client " + std::to_string(fd) + ": " + data.first);
		if (data.first == "quit")
			g_global.is_running = false;
	}
}

void	run_server(Socket &server)
{
	Epoll	epoll;
	std::map<int, std::shared_ptr<Socket>>	clients;

	epoll.subscribe(server.fd, EPOLLIN);
	while (g_global.is_running == true)
	{
		Epoll::Events	events = epoll.poll();
		for (auto it = events.begin(); it != events.end(); ++it)
		{
			if (it->second & EPOLLIN)
			{
				if (it->first == server.fd)
					__acceptClient(server, epoll, clients);
				else
					__readClient(epoll, clients, it->first);
			}
		}
	}
}
