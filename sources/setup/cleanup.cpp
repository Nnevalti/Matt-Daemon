#include "matt-daemon.hpp"

void	cleanup(void)
{
	ssl::cleanup();
	g_global.logger.logInfo("Quitting.");
	flock(g_global.fd_lock, LOCK_UN);
	close(g_global.fd_lock);
	unlink(LOCK_FILE);
}
