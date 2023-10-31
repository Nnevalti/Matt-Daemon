/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:35:54 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 19:29:44 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include "matt-daemon.hpp"

void	signal_handler(int signum)
{
	static std::map<int, std::string> msg = {
		{SIGINT, "SIGINT"},
		{SIGTERM, "SIGTERM"},
		{SIGQUIT, "SIGQUIT"},
		{SIGKILL, "SIGKILL"},
		{SIGSTOP, "SIGSTOP"},
	};

	g_global.logger->log("Signal " + msg[signum] + " received.", TintinReporter::INFO);
	g_global.is_running = false;
}

void	init_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGSTOP, signal_handler);
}
