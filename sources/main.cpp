/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:48:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 17:50:10 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

t_glob g_global = {true, 0, nullptr};

void signal_handler(int signum)
{
	(void)signum;
	g_global.is_running = false;
}

int main()
{
	TintinReporter reporter;
	// if (getuid() != 0) {
	// 	std::cerr << "You must be root to run this program." << std::endl;
	// 	return (EXIT_FAILURE);
	// }

	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return (EXIT_FAILURE);
	if (!handle_lock_file())
		return(EXIT_FAILURE);

	try {
		reporter.openLogFile();
		g_global.logger = &reporter;
		g_global.logger->log("Matt Daemon started.", TintinReporter::INFO);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	while (g_global.is_running) { sleep(1); }

	flock(g_global.fd_lock, LOCK_UN);
	close(g_global.fd_lock);
	unlink(LOCK_FILE);

	g_global.logger->log("Matt Daemon stopped.", TintinReporter::INFO);

	return (EXIT_SUCCESS);
}
