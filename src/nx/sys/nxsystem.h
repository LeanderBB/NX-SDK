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
#ifndef __NX_SYSTEM_H__
#define __NX_SYSTEM_H__


#if defined(NX_SYSTEM_SDL2)
#include "nx/sys/sdl/nxsystemsdl.h"
#elif defined(NX_SYSTEM_ANDROID)
#include "nx/sys/android/nxsystemandroid.h"
#else
#error "No implementatinn for target system"
#endif

namespace nx
{

struct NXAppOptions;
class NXInputManager;
class NXEventManager;
class NXWindow;
class NXSystem : NXSystemImp
{
public:

    NXSystem();

    bool init(const int argc,
              const char** argv,
              const struct NXAppOptions* pOptions);

    void term();

    void tick();

    NXEventManager* eventManager()
    {
        return _pEvtMan;
    }

    NXWindow* window()
    {
        return _pWindow;
    }

    bool quit() const
    {
        return _quit;
    }

    bool paused() const
    {
        return _pause;
    }

    void setPaused(const bool b)
    {
        _pause = b;
    }

    void setQuit()
    {
        _quit = true;
    }


protected:
    NXEventManager* _pEvtMan;
    NXWindow* _pWindow;
    volatile bool _quit;
    volatile bool _pause;

};

}

#endif
