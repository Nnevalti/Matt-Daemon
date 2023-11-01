/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:48:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 17:27:07 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

t_glob g_global = {true, 0, nullptr};

// static void	__checkRoot(void)
// {
// 	if (getuid() != 0) {
// 		std::cerr << "You must be root to run this program." << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }

int main()
{
	Socket			server;
	TintinReporter	reporter;

	// __checkRoot();

	try {
		reporter.openLogFile();
		g_global.logger = &reporter;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	if (lock_file() == false)
		return(EXIT_FAILURE);

	try {
		g_global.logger->logInfo("Started.");
		g_global.logger->logInfo("Creating server.");
		server.reuseAddr();
		server.bind("127.0.0.1", PORT + 1);
		server.listen(MAX_CLIENT);
		g_global.logger->logInfo("Server started.");
		daemonize();
		init_signals();
		Socket client = server.accept();
		g_global.logger->logInfo("New client connected.");
		
		while (g_global.is_running) {
			std::string data = client.recv();
			data.pop_back();
			if (data == "quit")
				g_global.is_running = false;
			g_global.logger->log(data);
		}
	}
	catch (std::exception &e) {
		g_global.logger->logError(e.what());
		cleanup();
		return (EXIT_FAILURE);
	}
	cleanup();
	return (EXIT_SUCCESS);
}
