#include "matt-daemon.hpp"

static void	__acceptClient(ssl::SSocket &server, Epoll &epoll, std::map<int, std::shared_ptr<ssl::SSocket>> &clients)
{
	std::shared_ptr<ssl::SSocket>	client = std::make_shared<ssl::SSocket>(server.accept());
	if (clients.size() < MAX_CLIENT) {
		epoll.subscribe(client->fd, EPOLLIN);
		clients.insert(std::pair<int, std::shared_ptr<ssl::SSocket>>(client->fd, client));
		g_global.logger.log("Client " + std::to_string(client->fd) + " connected.");
	}
	else {
		client->send("Server is full.");
		client->close();
		g_global.logger.logWarning("Attempting to connect to a full server.");
	}
}

static void	__removeClient(Epoll &epoll, std::map<int, std::shared_ptr<ssl::SSocket>> &clients, int fd)
{
	epoll.unsubscribe(fd);
	clients[fd]->close();
	clients.erase(fd);
	g_global.logger.log("Client " + std::to_string(fd) + " disconnected.");
}

static void __broadcast(std::map<int, std::shared_ptr<ssl::SSocket>> &clients, int fd, std::string const &msg)
{
	for (auto it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first != fd)
			it->second->send(msg);
	}
}

static void	__readClient(Epoll &epoll, std::map<int, std::shared_ptr<ssl::SSocket>> &clients, int fd)
{
	ssl::SSocket::RecvData	data = clients[fd]->recv();

	if (data.second == 0)
		__removeClient(epoll, clients, fd);
	else {
		if (data.first.back() == '\n')
			data.first.pop_back();
		g_global.logger.log("Client " + std::to_string(fd) + ": " + data.first);
		if (data.first == "quit") {
			g_global.is_running = false;
			__broadcast(clients, fd, "Server is shutting down.");
		}
		else
			__broadcast(clients, fd, "Client " + std::to_string(fd) + ": " + data.first);
	}
}

void	run_server(ssl::SSocket &server)
{
	Epoll	epoll;
	std::map<int, std::shared_ptr<ssl::SSocket>>	clients;

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
