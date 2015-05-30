#pragma once

#include <DatabaseConnection.h>
#include <iostream>

DatabaseConnection::DatabaseConnection(const char * _databaseFilename) :
	db(nullptr)
{
	int rc;
	
	/*if( argc!=3 ){
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", _sql);
		return(1);
	}*/
	rc = sqlite3_open(_databaseFilename, &db);
	if(rc != SQLITE_OK){
		std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		db = nullptr;
	}
}

DatabaseConnection::~DatabaseConnection(){
	if(db != nullptr){
		sqlite3_close(db);
		db = nullptr;
	}
}

void DatabaseConnection::queryDb(std::string _sql, int (*_callback)(void *, int, char **, char **)){
	int rc;
	char * zErrMsg = nullptr;
	rc = sqlite3_exec(db, _sql.c_str(), _callback, 0, &zErrMsg);
	if(rc != SQLITE_OK){
		std::cout << "SQL error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		zErrMsg = nullptr;
	}
}
std::thread * DatabaseConnection::queryDbAsync(std::string _sql, int (*_callback)(void *, int, char **, char **)){
	std::thread * t = new std::thread([_sql, _callback](DatabaseConnection * _db){
		_db->queryDb(_sql.c_str(), _callback);
	}, this);
	t->detach();
	return t;
}