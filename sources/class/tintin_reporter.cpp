/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:36 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 17:49:21 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/tintin_reporter.hpp"

TintinReporter::TintinReporter() {}

TintinReporter::TintinReporter(TintinReporter const &instance) {
	*this = instance;
}

TintinReporter::~TintinReporter() {
	_file.close();
}

TintinReporter &TintinReporter::operator=(TintinReporter const &instance) {
	if (this != &instance)
		*this = instance;
	return (*this);
}

void TintinReporter::openLogFile(void) {
	_file = std::ofstream(LOG_FILE, std::ios_base::app);
	if (!_file.is_open())
		throw std::runtime_error("TintinReporter: Can't open log file. " + std::string(strerror(errno)));}

void TintinReporter::log(const std::string &message, const LogType type) {
	_file << getTime() << " " << logTypeString[type] << " " << message << std::endl;
}

std::string TintinReporter::getTime(void) const {
	char mbstr[100];
	std::time_t t = std::time(nullptr);
	std::strftime(mbstr, sizeof(mbstr), "[%d/%m/%Y - %H:%M:%S]", std::localtime(&t));

	return (std::string(mbstr));
}