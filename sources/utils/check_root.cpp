#include "matt-daemon.hpp"

void	checkRoot(void)
{
	if (getuid() != 0) {
		std::cerr << "You must be root to run this program." << std::endl;
		exit(EXIT_FAILURE);
	}
}
