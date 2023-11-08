#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <memory>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#include "class/tintin_reporter.hpp"
#include "class/socket.hpp"
#include "class/epoll.hpp"
#include "class/ssl.hpp"
#include "class/smtp.hpp"

#define PORT 4242
#define LOCK_FILE "/var/lock/matt_daemon.lock"
#define MAX_CLIENT 3
#define CERTIFICATE_PATH "./matt-daemon.crt"
#define PRIVATE_KEY_PATH "./matt-daemon.key"
#define SMTP_SERVER "smtp.freesmtpservers.com"

typedef struct glob {
	bool 			is_running;
	int				fd_lock;
	TintinReporter	logger;
} t_glob;

extern t_glob g_global;


// Server
void	run_server(ssl::SSocket &server);

// Setup
void	cleanup(void);
void	init_logger(void);
void	init_server(ssl::SSocket &server);


// Utils
bool	lock_file(void);
void	signal_handler(int signum);
void	init_signals(void);
void	checkRoot(void);
void	daemonize(std::string path = "/");
