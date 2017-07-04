#ifndef __MYUTIL_H__
#define __MYUTIL_H__

#include <cstdio>

namespace myUTIL
{
	int isNumber(const char *str);
	size_t strLen(const char *str);
	char* strCopy(char *dest, const char *src);
	unsigned char* ustrCopy(unsigned char *destination, const unsigned char *src);
	unsigned char* ustrCopyAndPadWithNull(unsigned char *destination, const unsigned char *src, int cntbit);
	char* strFromInt(unsigned int, char *buffer);
	int startsWithi(const char *pre, const char *str);
}

#endif
