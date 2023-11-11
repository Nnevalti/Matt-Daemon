#include "class/tintin_reporter.hpp"

TintinReporter::TintinReporter(std::string const &logFile): _logFile(logFile) {}

TintinReporter::TintinReporter(TintinReporter const &instance) {
	*this = instance;
}

TintinReporter::~TintinReporter() {
	this->_file.close();
}

TintinReporter &TintinReporter::operator=(TintinReporter const &instance) {
	if (this != &instance)
		return (*this);
	
	this->_logFile = instance._logFile;
	if (instance._file.is_open()) {
    	this->_file.open(this->_logFile, std::ios_base::app);
    	this->_file << instance._file.rdbuf();
	}
	return (*this);
}

void TintinReporter::openLogFile(void) {
	this->_file = std::ofstream(this->_logFile, std::ios_base::app);
	if (!this->_file.is_open())
		throw std::runtime_error("TintinReporter: Can't open log file. " + std::string(strerror(errno)));
}

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