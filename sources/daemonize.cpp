/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:16:04 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 19:51:43 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

void daemonize(std::string path)
{
	pid_t pid = fork();
	pid_t sid;

	g_global.logger->logInfo("Entering Deamon mode.");
	if (pid < 0) {
		g_global.logger->logError("Fork failed.");
		throw std::runtime_error("fork: " + std::string(strerror(errno)));
	} else if (pid > 0) {
		exit(EXIT_SUCCESS);
	} else {
		umask(0);
		if ((sid = setsid()) < 0) {
			g_global.logger->logError("Setsid failed.");
			throw std::runtime_error("setsid: " + std::string(strerror(errno)));
		}
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		if (chdir(path.c_str()) < 0) {
			g_global.logger->logError("Chdir failed.");
			throw std::runtime_error("chdir: " + std::string(strerror(errno)));
		}
		g_global.logger->logInfo("started. PID: " + std::to_string(sid));
	}
}
