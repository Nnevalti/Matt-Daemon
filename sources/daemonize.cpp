/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:16:04 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 18:32:14 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

void daemonize(std::string path)
{
	pid_t pid = fork();

	if (pid < 0) {
		g_global.logger->log("Fork failed.", TintinReporter::ERROR);
		throw std::runtime_error("fork: " + std::string(strerror(errno)));
	} else if (pid > 0) {
		exit(EXIT_SUCCESS);
	} else {
		g_global.logger->log("Daemon started.", TintinReporter::INFO);
		umask(0);
		if (setsid() < 0) {
			g_global.logger->log("Setsid failed.", TintinReporter::ERROR);
			throw std::runtime_error("setsid: " + std::string(strerror(errno)));
		}
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		if (chdir(path.c_str()) < 0) {
			g_global.logger->log("Chdir failed.", TintinReporter::ERROR);
			throw std::runtime_error("chdir: " + std::string(strerror(errno)));
		}
	}
}
