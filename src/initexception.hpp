#pragma once

#ifdef _WIN32
#include <stdafx.h>
#endif

class InitException : std::exception
{
public:
	InitException(std::string message, std::string reason);
	~InitException();
private:
};
