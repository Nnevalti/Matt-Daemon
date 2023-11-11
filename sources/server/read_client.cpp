#include "matt-daemon.hpp"

void	readClient(Epoll &epoll, Clients &clients, int fd)
{
	ssl::SSocket::RecvData	data = clients[fd]->recv();

	if (data.second == 0)
		removeClient(epoll, clients, fd);
	else {
		if (data.first.back() == '\n')
			data.first.pop_back();
		if (doCommand(data.first, clients, fd) == true)
			return ;
		g_global.logger.log("Client " + std::to_string(fd) + ": " + data.first);
		broadcast(clients, fd, "Client " + std::to_string(fd) + ": " + data.first + "\n");
	}
}
