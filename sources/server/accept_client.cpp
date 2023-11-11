#include "matt-daemon.hpp"

void	acceptClient(ssl::SSocket &server, Epoll &epoll, Clients &clients)
{
	std::shared_ptr<ssl::SSocket>	client = std::make_shared<ssl::SSocket>(server.accept());
	if (clients.size() < MAX_CLIENT) {
		epoll.subscribe(client->fd, EPOLLIN);
		clients.insert(std::pair<int, std::shared_ptr<ssl::SSocket>>(client->fd, client));
		g_global.logger.log("Client " + std::to_string(client->fd) + " connected.");
		broadcast(clients, client->fd, "Client " + std::to_string(client->fd) + " connected.\n");
	}
	else {
		client->send("Server is full.");
		client->close();
		g_global.logger.logWarning("Attempting to connect to a full server.");
	}
}
