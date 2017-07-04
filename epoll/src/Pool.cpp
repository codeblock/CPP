#include "Pool.h"

void Pool::set(int n)
{
	std::mutex muTex;
	short s = 0;
	
	if (m_size == 0)
	{
		muTex.lock();
		if (m_size == 0)
		{
			m_size = n;
			m_timeout = m_size << 1;
			m_vec.reserve(m_size);
			for (s = 0; s < m_size; ++s)
			{
				PoolStruct ps;
				ps.body = new MySQL();
				ps.body->connect(DB_HOST, DB_USER, DB_PASS, DB_PORT, DB_NAME);
				ps.timeout = 28800 - 1;
				ps.past = time(NULL);
				ps.use = 0;
				m_vec.push_back(ps);
				Logger::log(Logger::INFO, "Pool set #%hd", s + 1);
			}
		}
		else
		{
			//puts("pass! (inner zone)");
		}
		muTex.unlock();
	}
	else
	{
		//puts("pass! (outer zone)");
	}
}

DB* Pool::get(void)
{
	std::mutex muTex;
	short s = 0;
	DB* db = NULL;
	time_t past;
	
	if (m_size > 0)
	{
		past = time(NULL);
		while (db == NULL && time(NULL) - past < m_timeout)
		{
			s = 0;
			//printf("(%d / %d)\n", time(NULL) - past, m_timeout);
			//sleep(1);
			for (std::vector<PoolStruct>::iterator it = m_vec.begin(); it != m_vec.end(); ++it)
			{
				s++;
				if (it->use == 0)
				{
					muTex.lock();
					if (it->use == 0)
					{
						if (time(NULL) - it->past > it->timeout)
						{
							Logger::log(Logger::INFO, "Pool refresh #%hd for timeout (%d / %d sec).", s, time(NULL) - it->past, it->timeout);
							it->body->disconnect();
							it->body->connect(DB_HOST, DB_USER, DB_PASS, DB_PORT, DB_NAME);
						}
						db = it->body;
						it->past = time(NULL);
						it->use = 1;
						Logger::log(Logger::INFO, "Pool get #%hd.", s);
						break;
					}
					muTex.unlock();
				}
			}
		}
	}
	
	return db;
}

void Pool::release(DB* db)
{
	short s = 0;
	
	if (m_size > 0)
	{
		for (std::vector<PoolStruct>::iterator it = m_vec.begin(); it != m_vec.end(); ++it)
		{
			s++;
			if (it->use == 1 && m_hash(it->body) == m_hash(db))
			{
				it->use = 0;
				Logger::log(Logger::INFO, "Pool release #%hd.", s);
				break;
			}
		}
	}
}

void Pool::clear(void)
{
	std::mutex muTex;
	short s = 0;
	
	if (m_size > 0)
	{
		muTex.lock();
		if (m_size > 0)
		{
			for (std::vector<PoolStruct>::iterator it = m_vec.begin(); it != m_vec.end(); ++it)
			{
				s++;
				it->body->disconnect();
				delete it->body;
				// it->use = 0;
				Logger::log(Logger::INFO, "Pool delete #%hd.", s);
			}
			m_vec.clear();
			m_size = m_vec.size();
		}
		muTex.unlock();
	}
}

int Pool::m_size = 0;
int Pool::m_timeout = 0;
std::vector<PoolStruct> Pool::m_vec;
std::hash<DB *> Pool::m_hash;