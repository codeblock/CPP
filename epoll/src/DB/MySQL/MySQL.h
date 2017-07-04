#pragma once

#ifndef __MYSQL_H__
#define __MYSQL_H__

#include <mysql.h>
#include "../DB.h"

//#pragma comment(lib, "libmysql")
//#pragma comment(lib, "mysqlclient")

#define MYSQL_CHARSET "euckr"
#define MYSQL_COMMAND "SET NAMES euckr"

class MySQL : public DB
{
private:
	MYSQL* conn;
	bool init(void);
	void clear(void);
public:
	MySQL(void);
	~MySQL(void);
	void* connect(const char *host, const char *user, const char *passwd, unsigned int port, const char *db = "");
	bool  commit(void);
	bool  rollback(void);
	void  disconnect(void);
	Row*  select(const char *sql, const char **bind = NULL);
	unsigned int dml(const char *sql, const char **bind = NULL);
	unsigned int rowCount(void);
	unsigned int fieldCount(void);
	int errorCode(void);
	char* errorMessage(void);
};

#endif
