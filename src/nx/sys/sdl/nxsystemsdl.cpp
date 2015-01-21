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
#include "nx/sys/sdl/nxsdlinput.h"
#include "nx/sys/nxapp.h"

namespace nx
{

NXSystemImp::NXSystemImp():
    _pWindow(nullptr)
{

}

NXSystemImp::~NXSystemImp()
{

}

bool
NXSystemImp::initImp(const int argc,
                     const char** argv,
                     const struct NXAppOptions* pOptions)
{
    (void) pOptions;
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

    NXSetupSDLKeys();

    return true;
}

bool
NXSystemImp::createWindowImp(const NXAppOptions *pOptions,
                             NXEventManager *pEvtManager)
{
    NX_ASSERT(pEvtManager);
    // create window
    _pWindow = new NXWindow("NX");

    bool depth = false, stencil = false, dbgctx = false, resizable = false;
    if (pOptions)
    {
        _pWindow->setDimensions(pOptions->width, pOptions->height);
        _pWindow->setFullscreen(pOptions->fullscreen);
        depth = pOptions->depth;
        stencil = pOptions->stencil;
        dbgctx = pOptions->dbgctx;
        resizable = pOptions->resizable;
    }

    if (!_pWindow->create(depth, stencil, resizable, dbgctx))
    {
        return false;
    }

    NXSysEvtWinCreated evt_created;
    pEvtManager->trigger(&evt_created);
    return true;
}

void
NXSystemImp::termImp(NXEventManager* pEvtManager)
{
    NX_ASSERT(pEvtManager);
    NXSysEvtWinDestroy evt_destroy;
    if (_pWindow)
    {
        pEvtManager->trigger(&evt_destroy);
        _pWindow->destroy();
        NX_SAFE_DELETE(_pWindow);
    }

    // shutdown sdl
    SDL_Quit();
}


bool
NXSystemImp::tickImp(NXEventManager*,
                     NXInputManager* pInputManager)
{
    SDL_PumpEvents();
    if (pInputManager)
    {
        NXProcessSDLInput(pInputManager);
    }

    return true;
}

int
NXSystemImp::systemEventHandler(void *pUserData,
                                SDL_Event * pEvent)
{
    NXSystem* p_sys = static_cast<NXSystem*>(pUserData);
    switch(pEvent->type)
    {
    case SDL_QUIT:
        p_sys->signalQuit();
        return 0;
    case SDL_WINDOWEVENT:
    {
        switch (pEvent->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        {
            p_sys->window()->_height = pEvent->window.data2;
            p_sys->window()->_width = pEvent->window.data1;
            NXSysEvtWinResize evt(pEvent->window.data1, pEvent->window.data2);
            p_sys->eventManager()->trigger(&evt);
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
