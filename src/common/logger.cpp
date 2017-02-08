#include "logger.h"
#include <fstream>

Logger Logger::logger;

Logger::Logger()
{
}

Logger::~Logger()
{
	writeFile();
}

// Get the static logger.
Logger & Logger::getLogger(LogLevel level)
{
	logger << getLevelString(level);
	system(getLevelColor(level));
	return logger;
}

// Get the prefix of a log by its log level.
const char * Logger::getLevelString(LogLevel level)
{
	static const char * const levels[5] =
	{
		"[INFO] ",
		"[DEBUG] ", 
		"[WARNING] ",
		"[ERROR] ",
		"[FATAL] "
	};
	return levels[level];
}

// Get the color string of a log when display it on the console by its log level.
const char * Logger::getLevelColor(LogLevel level)
{
	static const char * const colors[5] =
	{
		"color 07",
		"color 07",
		"color 0E",
		"color 0C",
		"color 0C"
	};
	return colors[level];
}

// Output all logs to a .txt file.
void Logger::writeFile()
{
	static std::ofstream fout;
	fout.open("log.txt");
	fout << mContent.str();
	fout.close();
	mContent.str("");
}