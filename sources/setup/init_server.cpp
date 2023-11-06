#include "matt-daemon.hpp"

void	init_server(ssl::SSocket &server)
{
	g_global.logger.logInfo("Creating server.");
	server.reuseAddress();
	server.bind("0.0.0.0", PORT);
	server.listen();
	g_global.logger.logInfo("Server started.");
}
