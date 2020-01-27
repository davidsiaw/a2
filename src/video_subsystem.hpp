//
//  video_subsystem.hpp
//  a2
//
//  Created by David Siaw on 2020/01/27.
//  Copyright © 2020 David Siaw. All rights reserved.
//

#ifndef VIDEO_SUBSYSTEM_H
#define VIDEO_SUBSYSTEM_H

class VideoSubsystem
{
    bool created;
public:
    VideoSubsystem() : created(false)
    {
        if (!SDL_WasInit(SDL_INIT_VIDEO))
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
            created = true;
        }
    }

    int get_display_count()
    {
        return SDL_GetNumVideoDisplays();
    }
    
    int get_video_width(int display)
    {
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(display, &current);
        return current.w;
    }
    
    int get_video_height(int display)
    {
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(display, &current);
        return current.h;
    }
    
    int get_video_refresh(int display)
    {
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(display, &current);
        return current.refresh_rate;
    }
    
    ~VideoSubsystem()
    {
        if (created)
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
    }
};

#endif /* VIDEO_SUBSYSTEM_H */
