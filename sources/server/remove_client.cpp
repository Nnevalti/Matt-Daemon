#include "matt-daemon.hpp"

void	removeClient(Epoll &epoll, Clients &clients, int fd)
{
	epoll.unsubscribe(fd);
	clients.erase(fd);
	g_global.logger.log("Client " + std::to_string(fd) + " disconnected.");
	broadcast(clients, fd, "Client " + std::to_string(fd) + " disconnected.\n");
}
