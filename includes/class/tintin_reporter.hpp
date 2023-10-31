/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:39 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 17:49:06 by vdescham         ###   ########.fr       */
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

#define LOG_FILE "matt_daemon.log"

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
		"[ INFO ]",
		"[ WARNING ]",
		"[ ERROR ]"
	};

	std::ofstream _file;

	std::string getTime(void) const;
};
