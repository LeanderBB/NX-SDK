//
// This file is part of the NX Project
//
// Copyright (c) 2014 Leander Beernaert
//
// NX Project is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// NX Project is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with NX. If not, see <http://www.gnu.org/licenses/>.
//
#include "nx/nxcore.h"

#if defined(NX_SYSTEM_SDL2)
#include "nx/sys/sdl/nxsystemsdl.h"
#include "nx/sys/nxsysevents.h"
#include "nx/sys/nxsystem.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxwindow.h"


namespace nx
{

NXSystemImp::NXSystemImp()
{

}

NXSystemImp::~NXSystemImp()
{

}

bool
NXSystemImp::initImp(const int argc,
                     const char** argv)
{
    (void) argc;
    (void) argv;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 )
    {
        NXLogError("NXSystem failed to initialize SDL: %s",SDL_GetError());
        return false;
    }

    // register custom system event filter
    SDL_SetEventFilter(systemEventHandler, this);

    return true;
}

void
NXSystemImp::termImp()
{
    // shutdown sdl
    SDL_Quit();
}


void
NXSystemImp::tickImp()
{
    SDL_PumpEvents();
}

int
NXSystemImp::systemEventHandler(void *pUserData,
                                SDL_Event * pEvent)
{
    NXSystem* p_sys = static_cast<NXSystem*>(pUserData);
    switch(pEvent->type)
    {
    case SDL_QUIT:
        p_sys->setQuit();
        return 0;
    case SDL_WINDOWEVENT:
    {
        switch (pEvent->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        {
            NXSysEvtWinResize evt(pEvent->window.data1, pEvent->window.data2);
            p_sys->eventManager()->trigger(&evt);
            p_sys->window()->_height = pEvent->window.data2;
            p_sys->window()->_width = pEvent->window.data1;
            return 0;
        }
        case SDL_WINDOWEVENT_MINIMIZED:
            p_sys->setPaused(true);
            return 0;
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_EXPOSED:
            p_sys->setPaused(false);
            return 0;
        default:
            return 0;
        }
    }
    default:
        return 1;
    }

}
}

#endif // defined(NX_SYSTEM_SDL2)
