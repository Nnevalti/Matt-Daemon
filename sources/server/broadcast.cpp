#include "matt-daemon.hpp"

void	broadcast(Clients &clients, int fd, std::string const &msg)
{
	for (auto it = clients.begin(); it != clients.end(); ++it)
		if (it->first != fd)
			it->second->send(msg);
}
