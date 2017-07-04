#include "../../myUTIL.h"
#include "MySQL.h"

// https://dev.mysql.com/doc/refman/5.0/en/c-api-functions.html

MySQL::MySQL(void)
{
	this->conn = NULL;
	this->row = NULL;
	this->error_code = 0;
	//this->error_message;
	this->num_row = 0;
	this->num_row_all = 0;
	this->num_field = 0;
}

MySQL::~MySQL(void)
{
	this->disconnect();
}

bool MySQL::init(void)
{
	bool rtn = true;

	if (this->conn == NULL)
	{
		this->conn = mysql_init(NULL);
		if (this->conn == NULL)
		{
			this->error_code = -1;
			myUTIL::strCopy(this->error_message, "init error");
			rtn = false;
		}
		else
		{
			mysql_options(this->conn, MYSQL_SET_CHARSET_NAME, MYSQL_CHARSET);
			mysql_options(this->conn, MYSQL_INIT_COMMAND, MYSQL_COMMAND);
			// mysql_options(this->conn, MYSQL_INIT_COMMAND, "SET wait_timeout=10"); // timeout test
		}
	}

	return rtn;
}

void* MySQL::connect(const char *host, const char *user, const char *passwd, unsigned int port, const char *db)
{
	this->init();
	if (!mysql_real_connect(this->conn, host, user, passwd, db, port, NULL, 0))
	{
		this->error_code = -1;
		myUTIL::strCopy(this->error_message, "connection error");
	}
	return this->conn;
}

bool MySQL::commit(void)
{
	bool rtn = true;

	if (!mysql_commit(this->conn))
	{
		this->error_code = -1;
		myUTIL::strCopy(this->error_message, "commit error");
		rtn = false;
	}

	return rtn;
}

bool MySQL::rollback(void)
{
	bool rtn = true;

	if (!mysql_rollback(this->conn))
	{
		this->error_code = -1;
		myUTIL::strCopy(this->error_message, "rollback error");
		rtn = false;
	}

	return rtn;
}

void MySQL::disconnect(void)
{
	this->clear();

	if (this->conn != NULL)
	{
		mysql_close(this->conn);
		this->conn = NULL;
	}
}

Row* MySQL::select(const char *sql, const char **bind)
{
	this->clear();

	mysql_query(this->conn, sql);

	myUTIL::strCopy(this->error_message, mysql_error(this->conn));
	this->error_code = mysql_errno(this->conn);

	if (this->error_code)
	{
		return NULL;
	}

	MYSQL_RES *result = mysql_store_result(this->conn);

	if (result->row_count > 0)
	{
		MYSQL_ROW dbrow;
		unsigned int i, j;
		this->num_row = result->row_count;
		this->num_field = result->field_count;
		this->num_row_all = this->num_row; // <------------------------------------------- if all fetched...
		this->row = new Row[this->num_row];
		for (i = 0; i < this->num_row; i++)
		{
			dbrow = mysql_fetch_row(result);
			this->row[i].data = new Field[this->num_field];
			this->row[i].next = NULL;

			for (j = 0; j < this->num_field; j++)
			{
				this->row[i].data[j].data = new char[myUTIL::strLen(dbrow[j]) + 1];
				myUTIL::strCopy(this->row[i].data[j].data, static_cast<char *>(dbrow[j]));
				this->row[i].data[j].next = NULL;
				if (j > 0)
				{
					this->row[i].data[j - 1].next = &this->row[i].data[j]; // add link prev->now
				}
			}

			if (i > 0)
			{
				this->row[i - 1].next = &this->row[i];
			}
		}
	}
	mysql_free_result(result);

	return this->row;
}

unsigned int MySQL::dml(const char *sql, const char **bind)
{
	unsigned int rtn = 0;

	mysql_query(this->conn, sql);

	myUTIL::strCopy(this->error_message, mysql_error(this->conn));
	this->error_code = mysql_errno(this->conn);

	if (this->error_code)
	{
		return NULL;
	}
	
	if (myUTIL::startsWithi("INSERT ", sql))
	{
		// insert
		rtn = mysql_insert_id(this->conn);
	}
	else
	{
		// update, delete
		rtn = mysql_affected_rows(this->conn);
	}

	return rtn;
}

unsigned int MySQL::rowCount(void)
{
	return this->num_row;
}

unsigned int MySQL::fieldCount(void)
{
	return this->num_field;
}

void MySQL::clear(void)
{
	Row *rows, *rows_prev;
	Field *fields, *fields_prev;

	if (this->row != NULL)
	{
		rows = this->row;
		while (rows != NULL)
		{
			fields = rows->data;
			while (fields != NULL)
			{
				fields_prev = fields; // bakcup field
				fields = fields->next; // walking through the next field
				delete [] fields_prev->data; // delete backup field data (char[])
			}
			rows_prev = rows;
			rows = rows->next;
			delete [] rows_prev->data; // delete array of field (Field[])
		}
		delete [] this->row; // delete array of record (Row[])
		
		this->row = NULL;
		this->error_code = 0;
		myUTIL::strCopy(this->error_message, "");
		this->num_row = 0;
		this->num_row_all = 0;
		this->num_field = 0;
	}
}

int MySQL::errorCode(void)
{
	return this->error_code;
}

char* MySQL::errorMessage(void)
{
	return this->error_message;
}
