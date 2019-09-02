#pragma once

#include "db_statement.hpp"

class Database
{
	std::shared_ptr<sqlite3> db;

public:
	Database(const std::string& filename)
	{
		sqlite3 *dbptr;
		sqlite3_open(filename.c_str(), &dbptr);
		db = std::shared_ptr<sqlite3>(dbptr, sqlite3_close);
	}

	mruby::NativeObject<DbStatement> prepare(const std::string& sql)
	{
		auto ptr = std::make_shared<DbStatement>(db, sql);
		return mruby::NativeObject<DbStatement>("DbStatement", ptr);
	}
};
