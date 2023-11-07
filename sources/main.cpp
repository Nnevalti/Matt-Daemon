#include "matt-daemon.hpp"

t_glob g_global = {
	.is_running = true,
	.fd_lock = 0,
	.logger = TintinReporter()
};


int main()
{
	ssl::init();
	ssl::SSLContext	ctx(CERTIFICATE_PATH, PRIVATE_KEY_PATH, SSLv23_server_method());
	ssl::SSocket	server(ctx);

	checkRoot();
	init_logger();
	if (lock_file() == false)
		return(EXIT_FAILURE);
	try {
		g_global.logger.logInfo("Started.");
		init_server(server);
		daemonize();
		init_signals();
		run_server(server);
	}
	catch (std::exception &e) {
		g_global.logger.logError(e.what());
		ssl::cleanup();
		cleanup();
		return (EXIT_FAILURE);
	}
	ssl::cleanup();
	cleanup();
	return (EXIT_SUCCESS);
}
