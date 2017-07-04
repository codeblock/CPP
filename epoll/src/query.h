#ifndef __QUERY_H__
#define __QUERY_H__

// C header
#include <sys/epoll.h>

#include <cstdio>
#include <cstring>

// C++ header
#include <string>

// Additional header

// ----------------------------------------------- winnt.h
typedef long LONG;
// ----------------------------------------------- winnt.h
// ----------------------------------------------- minwindef.h
//#define FAR                 far
//#define NEAR                near
// ----------------------------------------------- minwindef.h
// ----------------------------------------------- windef.h
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT;//, NEAR *NPPOINT, FAR *LPPOINT;
// ----------------------------------------------- windef.h

#include "myUTIL.h"
#include "DB/DB.h"
#include "DB/MySQL/MySQL.h"
#include "Pool.h"

#include "Packet.h"

extern struct epoll_event *server_ips_index[3];
extern SERVER_IP server_ips;

struct tagProcessArgs
{
	struct epoll_event *evt; // event id
	unsigned char *buf; // send buffer
	ssize_t *cnt; // send count
	int end; // 1: is closed
};
typedef struct tagProcessArgs processArgs;

int processCallBack(processArgs);
void processSendServerIP(unsigned char* ustr, ssize_t* n);
void process(processArgs);

#endif