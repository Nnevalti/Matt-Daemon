#include "matt-daemon.hpp"

void	init_server(Socket &server)
{
	g_global.logger.logInfo("Creating server.");
	server.reuseAddr();
	server.bind("0.0.0.0", PORT);
	server.listen();
	g_global.logger.logInfo("Server started.");
}
