/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt-daemon.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:49:39 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 18:50:08 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#include "class/tintin_reporter.hpp"

#define PORT 4242
#define LOCK_FILE "/var/lock/matt_daemon.lock"
#define MAX_CLIENT 3

typedef struct glob {
	bool is_running;
	int fd_lock;
	TintinReporter *logger;
} t_glob;

extern t_glob g_global;

// LOCK.CPP
bool	lock_file(void);

// DAEMONIZE.CPP
void	daemonize(std::string path = "/");

// CLEANUP.CPP
void	cleanup(void);

// Utils
void	signal_handler(int signum);
void	init_signals(void);
