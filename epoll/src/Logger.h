#ifndef __LOGGER_H__
#define __LOGGER_H__

#define LOGDIR "/home/tonic/ktm/server/log/"

class Logger
{
public:
	static void log(int mode, const char* format, ...);
private:
	Logger(void);
	Logger(const Logger& rhw);
	Logger& operator=(const Logger& rhw);
	~Logger(void);
public:
	static const int INFO = 0;
	static const int DEBUG = 1;
	static const int ERROR = 2;
	static const char* const MODES[];
};

#endif