#include "scriptapi.hpp"

#include "computer.hpp"
#include "propexplosion.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "softblock.hpp"
#include "block.hpp"
#include "explosion.hpp"
#include "umpire.hpp"

namespace bomberman {
enum EntityFlags
{
	BOMB = 0x1,
	PLAYER = 0x2,
	BLOCK = 0x4,
	SOFTBLOCK = 0x8,
	PROPBOMB = 0x10,
	EXPLOSION = 0x20,
	PROPEXPLOSION = 0x40,
	OUTSIDE = 0x80,

	PLAYER1 = 0x1002,
	PLAYER2 = 0x2002,
	PLAYER3 = 0x4002,
	PLAYER4 = 0x8002
};

ScriptAPI::ScriptAPI(const std::string &script)
{
}

InputState ScriptAPI::Resume(int x, int y, int mx, int my, const MapConstPtr &iPresentMap) const
{
	InputState input;
    

	/*printlog("inputa: %d %d\ %d %d\n", 
		input.GetButtonState(InputState::DOWN), 
		input.GetButtonState(InputState::UP),
		mx, my
		);*/
		
	return input;
}
}
