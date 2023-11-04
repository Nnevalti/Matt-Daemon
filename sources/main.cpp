#include "matt-daemon.hpp"

t_glob g_global = {
	.is_running = true,
	.fd_lock = 0,
	.logger = TintinReporter()
};

int main()
{
	Socket	server;

	checkRoot();
	init_logger();
	if (lock_file() == false)
		return(EXIT_FAILURE);
	try {
		ssl::init();
		g_global.logger.logInfo("Started.");
		init_server(server);
		daemonize();
		init_signals();
		run_server(server);
	}
	catch (std::exception &e) {
		g_global.logger.logError(e.what());
		cleanup();
		return (EXIT_FAILURE);
	}
	cleanup();
	return (EXIT_SUCCESS);
}
