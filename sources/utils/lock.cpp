#include "matt-daemon.hpp"

bool lock_file(void) {
	g_global.fd_lock = open(LOCK_FILE, O_CREAT | O_RDWR, 644);
	if (flock(g_global.fd_lock, LOCK_EX | LOCK_NB) < 0) {
		g_global.logger.logError("File already locked.");
		std::cerr << "Can't open " << LOCK_FILE << std::endl;
		return false;
	}
	return true;
}