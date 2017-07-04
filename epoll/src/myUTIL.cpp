#include "stdafx.h"
#include <cmath>
#include "myUTIL.h"

int myUTIL::isNumber(const char *str)
{
	int rtn = 1;

	while (*str != '\0')
	{
		if (!('0' <= *str && *str <= '9'))
		{
			rtn = 0;
			break;
		}
		str++;
	}

	return rtn;
}

size_t myUTIL::strLen(const char *str)
{
	size_t rtn = 0;

	while (str[rtn++] != '\0');

	return --rtn;
}

char* myUTIL::strCopy(char *destination, const char *src)
{
	char *dest = destination;
	
	//if (strLen(destination) >= strlen(src))
	//{
		while (*src != '\0')
		{
			*dest++ = *src++;
		}
		*dest = '\0';
	//}

	return destination;
}

unsigned char* myUTIL::ustrCopy(unsigned char *destination, const unsigned char *src)
{
	unsigned char *dest = destination;
	
	//if (strLen(destination) >= strlen(src))
	//{
		while (*src != '\0')
		{
			*dest++ = *src++;
		}
		*dest = '\0';
	//}

	return destination;
}

// destination is buffer
// src is ...
// 00000110 00110100 00000011 00000000 00000000 11111110
//        6       52        3        0        0      254
// -> ustrCopyAndPadWithNull(destination, src, 6) ->
// 00000110 00110100 00000011 00000000 00000000 00000000
//        6       52        3        0        0        0
unsigned char* myUTIL::ustrCopyAndPadWithNull(unsigned char *destination, const unsigned char *src, int cntbit)
{
	unsigned char *dest = destination;
	int i = 0;
	
	//if (strLen(destination) >= strlen(src))
	//{
		while (i < cntbit)
		{
			if (*src != '\0')
			{
				*dest++ = *src++;
			}
			else
			{
				*dest++ = '\0';
			}
			//printf("[%d]", *(dest - 1));
			i++;
		}
	//}

	return destination;
}

char* myUTIL::strFromInt(unsigned int n, char *buffer)
{
	char rtn[12];
	const char ascii_num[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	short i, cnt;
	unsigned int n_org;

	n_org = n;
	cnt = 0;
	do {
		n_org /= 10;
		cnt++;
	} while (n_org >= 1);
	n_org = n;
	for (i = cnt - 1; i >= 0; i--) {
		*(rtn + i) = *(ascii_num + n_org % 10);
		n_org /= 10;
	}
	*(rtn + cnt) = '\0';

	strCopy(buffer, rtn);

	return buffer;
}

int myUTIL::startsWithi(const char *pre, const char *str)
{
	int rtn = 0;
	int alpha_ignore = 'a' - 'A';

	if (strLen(pre) <= strLen(str)) // ���۹��ڿ��� �˻����ڿ����� ���̰� �۰ų� ���ƾ� �Ѵ�.
	{
		// ('a' <= *str && *str <= 'z' ? alpha_ignore : 0) : ���ĺ� �ҹ����� ��� �빮�ڷ� ���߱� ����
		while (*pre != '\0' && *pre != '\0' && *pre + ('a' <= *str && *str <= 'z' ? alpha_ignore : 0) == *str)
		{
			rtn += (*pre + ('a' <= *str && *str <= 'z' ? alpha_ignore : 0) - *str == 0) ? 1 : 0;
			pre++;
			str++;
		}
	}

	return rtn == strLen(pre - rtn); // rtn : �����ġ�ߴٸ� strlen(pre), pre - rtn : strlen �� ���� pre �� �ּҸ� ó������ ����
}
