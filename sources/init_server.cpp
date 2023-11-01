/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:26:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 19:25:16 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

void	init_server(Epoll &server)
{
	g_global.logger.logInfo("Creating server.");
	server.reuseAddr();
	server.bind("127.0.0.1", PORT + 1);
	server.listen(MAX_CLIENT);
	server.create();
	g_global.logger.logInfo("Server started.");
}
