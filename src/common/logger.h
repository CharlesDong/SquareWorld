#pragma once

#include <string>
#include <sstream>
#include <iostream>

enum LogLevel { Info, Debug, Warning, Error, Fatal };

class Logger
{
private:
	static Logger logger;
	std::ostringstream mContent;

	void writeFile();

	static const char * getLevelString(LogLevel level);
	static const char * getLevelColor(LogLevel level);
public:
	Logger();
	~Logger();
	template <typename T> Logger & operator<<(const T & val);

	static Logger & getLogger(LogLevel level);
};

// logger streams
#define infos		Logger::getLogger(Info)		// infomation for user
#define debugs		Logger::getLogger(Debug)	// information for developer
#define warnings	Logger::getLogger(Warning)	// problems may affect game quality, but not serious
#define errors		Logger::getLogger(Error)	// errors occured, but not very serious
#define fatals		Logger::getLogger(Fatal)	// very serious errors, game termination is required

// Add a log to the logger.
template <typename T>
Logger & Logger::operator<<(const T & val)
{
	mContent << val;
	std::cout << val;
	return *this;
}