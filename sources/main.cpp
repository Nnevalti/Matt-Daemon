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

	try {
		ssl::init();
		ssl::SSLContext	ctx(CERTIFICATE_PATH, PRIVATE_KEY_PATH, SSLv23_server_method());
		ssl::SSocket	server(ctx);
	
		init_logger();

		if (lock_file() == false)
			throw std::runtime_error("lock_file: " + std::string(strerror(errno)));

		g_global.logger.logInfo("Started.");
		init_server(server);
		init_signals();
		run_server(server);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		g_global.logger.logError(e.what());
	}

	cleanup();
	return (EXIT_SUCCESS);
}
