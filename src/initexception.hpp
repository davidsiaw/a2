#pragma once
#include "top.h"

class InitException : std::runtime_error
{
public:
	InitException(std::string message, std::string reason);
	~InitException();
private:
};
