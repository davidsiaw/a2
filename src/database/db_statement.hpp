#pragma once

#include "db_result.hpp"

class DbStatement
{
	std::shared_ptr<sqlite3_stmt> stmt;
	std::string last_err;

public:
	DbStatement(std::shared_ptr<sqlite3> db, const std::string& sql) : last_err("SQLITE_NONE")
	{
		sqlite3_stmt* stmtptr = nullptr;
		const char *tail;
		sqlite3_prepare_v2(db.get(), sql.c_str(), sql.length(), &stmtptr, &tail);
		stmt = std::shared_ptr<sqlite3_stmt>(stmtptr, sqlite3_finalize);
	}

	void bind_null(const std::string& name)
	{
		sqlite3_bind_null(stmt.get(), param_index(name));
	}

	void bind_int(const std::string& name, int64_t value)
	{
		sqlite3_bind_int64(stmt.get(), param_index(name), value);
	}

	void bind_double(const std::string& name, double value)
	{
		sqlite3_bind_double(stmt.get(), param_index(name), value);
	}

	void bind_text(const std::string& name, const std::string& value)
	{
		sqlite3_bind_text(stmt.get(), param_index(name), value.c_str(), value.length(), SQLITE_TRANSIENT);
	}

	int param_index(const std::string& name)
	{
		return sqlite3_bind_parameter_index(stmt.get(), name.c_str());
	}

	int param_count() const
	{
		return sqlite3_bind_parameter_count(stmt.get());
	}

	const std::string param_name(int index) const
	{
		return sqlite3_bind_parameter_name(stmt.get(), index);
	}

	const std::string last_status() const
	{
		return last_err;
	}

	mruby::NativeObject<DbResult> step()
	{
		int result = sqlite3_step(stmt.get());

#define	CASE(x) case x: \
			last_err = #x; \
			break;

		switch (result)
		{
			CASE(SQLITE_OK)
				CASE(SQLITE_ROW)
				CASE(SQLITE_DONE)
				CASE(SQLITE_ERROR)
				CASE(SQLITE_MISUSE)
		default:
			last_err = "OTHER_SQLITE_ERROR";
		}

		if (result == SQLITE_ROW)
		{
			auto ptr = std::make_shared<DbResult>(stmt);
			return mruby::NativeObject<DbResult>("DbResult", ptr);
		}
		return mruby::NativeObject<DbResult>("DbResult", nullptr);
	}

	void reset()
	{
		sqlite3_reset(stmt.get());
	}
};
