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
#include "nx/sys/nxsystem.h"
#include "nx/sys/nxwindow.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxinputmanager.h"
#include "nx/sys/nxsysevents.h"
#include "nx/sys/sdl/nxsdlinput.h"
#include "nx/sys/nxapp.h"



namespace nx
{

NXSystem::NXSystem():
    _pEvtMan(nullptr),
    _pWindow(nullptr),
    _quit(false),
    _pause(false)
{

}

bool
NXSystem::init(const int argc,
               const char** argv,
               const NXAppOptions *pOptions)
{
    (void) argc;
    (void) argv;
    if (!_pWindow)
    {

        if (!NXSystemImp::initImp(argc, argv))
        {
            return false;
        }

        // create event and input manager
        _pEvtMan = new NXEventManager();

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

        // register event types
        _pEvtMan->addEvent(NXSysEvtLowMem::sEvtType);
        _pEvtMan->addEvent(NXSysEvtWinHide::sEvtType);
        _pEvtMan->addEvent(NXSysEvtWinShow::sEvtType);
        _pEvtMan->addEvent(NXSysEvtWinResize::sEvtType);

    }
    else
    {
        NXLogWarning("NXSystem init is called more than once!");
    }

    return true;
}

void
NXSystem::term()
{
    if (_pWindow)
    {
        // unregister events
        _pEvtMan->removeEvent(NXSysEvtWinResize::sEvtType);
        _pEvtMan->removeEvent(NXSysEvtWinShow::sEvtType);
        _pEvtMan->removeEvent(NXSysEvtWinHide::sEvtType);
        _pEvtMan->removeEvent(NXSysEvtLowMem::sEvtType);

        _pEvtMan->clear();
        NX_SAFE_DELETE(_pEvtMan)
        // destroy window
        _pWindow->destroy();
        NX_SAFE_DELETE(_pWindow);

        NXSystemImp::termImp();
    }
}

void
NXSystem::tick()
{
    NXSystemImp::tickImp();
}

}
