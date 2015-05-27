#pragma once

#include <node\Node.h>
#include <sqlite\sqlite3.h>
#include <thread>

class DatabaseConnection : public Node{
private:
	sqlite3 * db;
public:
	DatabaseConnection(const char * _databaseFilename);
	~DatabaseConnection();
	
	void queryDb(std::string _sql, int (*_callback)(void *, int, char **, char **));
	std::thread * queryDbAsync(std::string _sql, int (*_callback)(void *, int, char **, char **));
};