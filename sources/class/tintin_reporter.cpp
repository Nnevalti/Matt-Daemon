/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:36 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 18:33:04 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/tintin_reporter.hpp"

TintinReporter::TintinReporter() {}

TintinReporter::TintinReporter(TintinReporter const &instance) {
	*this = instance;
}

TintinReporter::~TintinReporter() {
	this->_file.close();
}

TintinReporter &TintinReporter::operator=(TintinReporter const &instance) {
	if (this != &instance)
		return (*this);
	if (instance._file.is_open()) {
    	this->_file.open(LOG_FILE, std::ios_base::app);
    	this->_file << instance._file.rdbuf();
	}
	return (*this);
}

void TintinReporter::openLogFile(void) {
	this->_file = std::ofstream(LOG_FILE, std::ios_base::app);
	if (!this->_file.is_open())
		throw std::runtime_error("TintinReporter: Can't open log file. " + std::string(strerror(errno)));}

void TintinReporter::log(const std::string &message, const LogType type) {
	this->_file << getTime() << " " << this->_logTypeString[type] << " " << message << std::endl;
}

void TintinReporter::logInfo(const std::string &message) {
	this->log(message, INFO);
}

void TintinReporter::logWarning(const std::string &message) {
	this->log(message, WARNING);
}

void TintinReporter::logError(const std::string &message) {
	this->log(message, ERROR);
}

std::string TintinReporter::getTime(void) const {
	char mbstr[100];
	std::time_t t = std::time(nullptr);
	std::strftime(mbstr, sizeof(mbstr), "[%d/%m/%Y - %H:%M:%S]", std::localtime(&t));

	return (std::string(mbstr));
}