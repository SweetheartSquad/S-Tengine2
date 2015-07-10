#pragma once

#include <DatabaseConnection.h>
#include <Log.h>
#include <iostream>
#include <sstream>

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
		std::stringstream ss;
		ss << "Couldn't open database: " << _databaseFilename << "; Reason: " << sqlite3_errmsg(db);
		Log::error(ss.str());
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
		std::stringstream ss;
		ss << "SQL error: " << zErrMsg;
		Log::error(ss.str());
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