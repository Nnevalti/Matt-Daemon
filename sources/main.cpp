#include "matt-daemon.hpp"

t_glob g_global = {
	.is_running = true,
	.fd_lock = 0,
	.logger = TintinReporter()
};


int main()
{
	// checkRoot();
	// daemonize();
	ssl::init();
	ssl::SSLContext	ctx(CERTIFICATE_PATH, PRIVATE_KEY_PATH, SSLv23_server_method());
	ssl::SSocket	server(ctx);

	try {
		init_logger();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		ssl::cleanup();
		exit(EXIT_FAILURE);
	}
	if (lock_file() == false) {
		ssl::cleanup();
		return(EXIT_FAILURE);
	}

	try {
		g_global.logger.logInfo("Started.");
		init_server(server);
		// daemonize();
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
