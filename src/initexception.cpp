#include "initexception.hpp"

InitException::InitException(std::string message, std::string reason) : std::exception((message + ":" + reason).c_str())
{
	SDL_Log(this->what());
}

InitException::~InitException()
{

}
