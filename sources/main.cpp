/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:48:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 18:50:33 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

t_glob g_global = {true, 0, nullptr};

int main()
{
	TintinReporter reporter;
	// if (getuid() != 0) {
	// 	std::cerr << "You must be root to run this program." << std::endl;
	// 	return (EXIT_FAILURE);
	// }

	init_signals();
	if (!lock_file())
		return(EXIT_FAILURE);
	try {
		reporter.openLogFile();
		g_global.logger = &reporter;
		daemonize();
		g_global.logger->log("Matt Daemon started.", TintinReporter::INFO);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		cleanup();
		return (EXIT_FAILURE);
	}

	while (g_global.is_running) { sleep(1); }

	cleanup();

	g_global.logger->log("Matt Daemon stopped.", TintinReporter::INFO);

	return (EXIT_SUCCESS);
}
