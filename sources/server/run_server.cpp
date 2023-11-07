#include "matt-daemon.hpp"


typedef typename std::map<int, std::shared_ptr<ssl::SSocket>> Clients;


static void __broadcast(Clients &clients, int fd, std::string const &msg)
{
	for (auto it = clients.begin(); it != clients.end(); ++it)
		if (it->first != fd)
			it->second->send(msg);
}

static void	__acceptClient(ssl::SSocket &server, Epoll &epoll, Clients &clients)
{
	std::shared_ptr<ssl::SSocket>	client = std::make_shared<ssl::SSocket>(server.accept());
	if (clients.size() < MAX_CLIENT) {
		epoll.subscribe(client->fd, EPOLLIN);
		clients.insert(std::pair<int, std::shared_ptr<ssl::SSocket>>(client->fd, client));
		g_global.logger.log("Client " + std::to_string(client->fd) + " connected.");
		__broadcast(clients, client->fd, "Client " + std::to_string(client->fd) + " connected.\n");
	}
	else {
		client->send("Server is full.");
		client->close();
		g_global.logger.logWarning("Attempting to connect to a full server.");
	}
}

static void	__removeClient(Epoll &epoll, Clients &clients, int fd)
{
	epoll.unsubscribe(fd);
	clients[fd]->close();
	clients.erase(fd);
	g_global.logger.log("Client " + std::to_string(fd) + " disconnected.");
	__broadcast(clients, fd, "Client " + std::to_string(fd) + " disconnected.\n");
}


static bool	__doCommand(std::string const &command, Clients &clients, int fd)
{
	if (command == "quit") {
		g_global.logger.logInfo("Client " + std::to_string(fd) + " sent quit command.");
		__broadcast(clients, fd, "Server is shutting down.\n");
		g_global.is_running = false;
		return (true);
	}
	else if (command == "mail") {
		smtp::Smtp<Socket>	smtp(SMTP_SERVER, SMTP_PORT);
		smtp::Mail			mail("matt-daemon-server@yopmail.com", "matt-daemon-client@yopmail.com", "Test", "Test");

		g_global.logger.log(mail.toString());
		g_global.logger.logInfo("Client " + std::to_string(fd) + " sent mail command.");
		smtp.sendMail(mail);
		g_global.logger.logInfo("Mail sent.");
		return (true);
	}
	return (false);
}


static void	__readClient(Epoll &epoll, Clients &clients, int fd)
{
	ssl::SSocket::RecvData	data = clients[fd]->recv();

	if (data.second == 0)
		__removeClient(epoll, clients, fd);
	else {
		if (data.first.back() == '\n')
			data.first.pop_back();
		if (__doCommand(data.first, clients, fd) == true)
			return ;
		g_global.logger.log("Client " + std::to_string(fd) + ": " + data.first);
		__broadcast(clients, fd, "Client " + std::to_string(fd) + ": " + data.first + "\n");
	}
}

void	run_server(ssl::SSocket &server)
{
	Epoll	epoll;
	Clients	clients;

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
