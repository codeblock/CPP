#pragma once

#ifndef __DB_H__
#define __DB_H__

#ifndef NULL
#define NULL 0
#endif

#define SIZE_ERR_MSG 256

typedef struct tagField
{
	struct tagField* next;
	char* data;
} Field;

typedef struct tagRow
{
	struct tagRow* next;
	struct tagField* data;
} Row;

class DB
{
protected:
	void* conn;
	Row* row;
	int error_code;
	char error_message[SIZE_ERR_MSG];
	unsigned int num_row;
	unsigned int num_row_all;
	unsigned int num_field;
	virtual bool init(void) = 0;
	virtual void clear(void) = 0;
public:
	DB(void);
	virtual ~DB(void) = 0;
	virtual void* connect(const char *host, const char *user, const char *passwd, unsigned int port, const char *db = "") = 0;
	virtual bool  commit(void) = 0;
	virtual bool  rollback(void) = 0;
	virtual void  disconnect(void) = 0;
	virtual Row*  select(const char *sql, const char **bind = NULL) = 0;
	virtual unsigned int dml(const char *sql, const char **bind = NULL) = 0; // insert, update, delete
	virtual unsigned int rowCount(void) = 0;
	virtual unsigned int fieldCount(void) = 0;
	virtual int errorCode(void) = 0;
	virtual char* errorMessage(void) = 0;
};

#endif
