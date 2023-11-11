#include "matt-daemon.hpp"

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
					acceptClient(server, epoll, clients);
				else
					readClient(epoll, clients, it->first);
			}
		}
	}
}
