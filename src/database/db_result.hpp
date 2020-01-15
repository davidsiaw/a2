#pragma once

#include "sqlite3.h"

#include <string>
#include <memory>
#include <map>

class DbResult
{
	std::map<int, std::string> column_names;
	std::map<std::string, std::shared_ptr<sqlite3_value>> values;

public:
	DbResult(std::shared_ptr<sqlite3_stmt> stmt)
	{
		int count = sqlite3_column_count(stmt.get());

		for (int i = 0; i < count; i++)
		{
			std::string name = sqlite3_column_origin_name(stmt.get(), i);
			sqlite3_value* unprotected_val = sqlite3_column_value(stmt.get(), i);
			std::shared_ptr<sqlite3_value> value(sqlite3_value_dup(unprotected_val), sqlite3_value_free);

			column_names[i] = name;
			values[name] = value;
		}
	}

	size_t column_count() const
	{
		return values.size();
	}

	std::string column_name(int idx) const
	{
		return column_names.find(idx)->second;
	}

	int64_t int_result(const std::string& name)
	{
		return sqlite3_value_int64(values[name].get());
	}

	double double_result(const std::string& name)
	{
		return sqlite3_value_double(values[name].get());
	}

	std::string text_result(const std::string& name)
	{
		return (const char*)sqlite3_value_text(values[name].get());
	}
};
