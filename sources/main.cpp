/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:48:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 18:30:04 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

t_glob g_global = {true, 0, TintinReporter()};

// static void	__checkRoot(void)
// {
// 	if (getuid() != 0) {
// 		std::cerr << "You must be root to run this program." << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }

int main()
{
	Socket	server;

	// __checkRoot();
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
		cleanup();
		return (EXIT_FAILURE);
	}
	cleanup();
	return (EXIT_SUCCESS);
}
