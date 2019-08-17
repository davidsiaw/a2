#pragma once
#include "top.h"

class Music
{
	std::shared_ptr<Mix_Music> music;
public:
	Music(const std::string& filename)
	{
		music = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filename.c_str()), Mix_FreeMusic);
		if (!music)
		{
			throw new InitException("Mix_LoadMUS", Mix_GetError());
		}
	}

	~Music()
	{

	}

	void play()
	{
		Mix_PlayMusic(music.get(), 0);
		Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
	}
};
