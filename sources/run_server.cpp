/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:27:55 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 18:28:59 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

void	run_server(Socket &server)
{
	Socket client = server.accept();
	g_global.logger.logInfo("New client connected.");
	while (g_global.is_running) {
		std::string data = client.recv();
		data.pop_back();
		if (data == "quit")
			g_global.is_running = false;
		g_global.logger.log(data);
	}
}
