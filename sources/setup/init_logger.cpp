#include "matt-daemon.hpp"

void	init_logger(void)
{
	TintinReporter	reporter;

	g_global.logger = reporter;
	g_global.logger.openLogFile();
}
