#include "matt-daemon.hpp"

void	init_logger(void)
{
	TintinReporter	reporter;

	try {
		g_global.logger = reporter;
		g_global.logger.openLogFile();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}
