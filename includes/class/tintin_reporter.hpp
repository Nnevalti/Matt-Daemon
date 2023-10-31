/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:39 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 19:43:22 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <array>

#include <errno.h>
#include <string.h>

// #define LOG_FILE "/var/log/matt_daemon.log"

#define LOG_FILE "/tmp/matt_daemon.log"

#define COLOR_RESET std::string("\033[0m")
#define COLOR_RED std::string("\033[0;31m")
#define COLOR_GREEN std::string("\033[0;32m")
#define COLOR_YELLOW std::string("\033[0;33m")
#define COLOR_BLUE std::string("\033[0;34m")

class TintinReporter {
public:
	explicit TintinReporter();
	TintinReporter(TintinReporter const &instance);
	~TintinReporter();

	TintinReporter &operator=(TintinReporter const &instance);

	void openLogFile(void);

	enum LogType {
		INFO,
		WARNING,
		ERROR
	};

	void log(const std::string &message, const LogType type);

private:

	std::array<std::string, 3> logTypeString = {
		COLOR_BLUE + "[ INFO ]" + COLOR_RESET,
		COLOR_YELLOW + "[ WARNING ]" + COLOR_RESET,
		COLOR_RED + "[ ERROR ]" + COLOR_RESET
	};

	std::ofstream _file;

	std::string getTime(void) const;
};
