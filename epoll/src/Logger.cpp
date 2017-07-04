#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "Logger.h"

void Logger::log(int mode, const char* format, ...)
{
	va_list argList;
	
	time_t now;
	//time_t current_time = time(NULL);
	struct tm *ptm;
	char timeinfo[50];
	char filename[128];
	char filecontents[512];
	char modes[10];
	
	now = time(NULL);
	ptm = localtime(&now);
	strftime(timeinfo, 50, "%Y-%m-%d", ptm);
	
	strcpy(filename, LOGDIR);
	strcat(filename, "server.");
	strcat(filename, timeinfo);
	strcat(filename, ".log");
	
	FILE* file = fopen(filename, "a");
	
	if (file != NULL)
	{
		va_start(argList, format);
		vsprintf(filecontents, format, argList);
		if (strlen(filecontents) == 0)
		{
			sprintf(filecontents, "%s", format);
		}
		va_end(argList);
		
		
		strftime(timeinfo, 50, "%Y-%m-%d %H:%M:%S", ptm);
		
		//fprintf(file, "[%s%s\n", ctime(&current_time), str); // __DATE__, __TIME__
		//fprintf(file, "[%s] %s - line %d: %s\n", timeinfo, from, line, filecontents);
		fprintf(file, "[%s] - [%s]: %s\n", timeinfo, MODES[mode], filecontents);
		fclose(file);
	}
}

const char* const Logger::MODES[] = {"INFO ", "DEBUG", "ERROR"};