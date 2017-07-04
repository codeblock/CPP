#ifndef __POOL_H__
#define __POOL_H__

#include <ctime>
#include <vector>
#include <functional>
#include <mutex>
#include "DB/DB.h"
#include "DB/MySQL/MySQL.h"
#include "Logger.h"

#define DB_HOST "${DB_HOST}"
#define DB_USER "${DB_USER}"
#define DB_PASS "${DB_PASS}"
#define DB_PORT 3306
#define DB_NAME "${DB_NAME}"

struct tagPoolStruct
{
	DB* body;
	int timeout;
	time_t past;
	short use;
};
typedef struct tagPoolStruct PoolStruct;

class Pool
{
public:
	static void set(int n);
	static DB* get(void);
	static void release(DB* db);
	static void clear(void);
private:
	Pool(void);
	Pool(const Pool& rhw);
	Pool& operator=(const Pool& rhw);
	~Pool(void);
private:
	static int m_size;
	static int m_timeout;
	static std::vector<PoolStruct> m_vec;
	static std::hash<DB *> m_hash;
};

#endif