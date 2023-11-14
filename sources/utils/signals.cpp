#include "matt-daemon.hpp"

void	signal_handler(int signum)
{
	static std::map<int, std::string> msg = {
		{SIGINT, "SIGINT"},
		{SIGTERM, "SIGTERM"},
		{SIGQUIT, "SIGQUIT"},
		{SIGKILL, "SIGKILL"},
		{SIGSTOP, "SIGSTOP"},
		{SIGHUP, "SIGHUP"},
	};

	g_global.logger.logInfo("Signal " + msg[signum] + " received.");
	g_global.is_running = false;
}

void	init_signals(void)
{
	g_global.logger.logInfo("Signals handler.");
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGSTOP, signal_handler);
	signal(SIGHUP, signal_handler);
}
