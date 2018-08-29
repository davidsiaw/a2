#pragma once

#include "printlog.hpp"
#include "inputstate.hpp"
#include "map.hpp"

namespace bomberman {

class ScriptAPI
{
    

public:
	ScriptAPI(const std::string &script);
	InputState Resume(int x, int y, int mx, int my, const MapConstPtr &iPresentMap) const;
};

}
