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
    _evtMan(),
    _quit(false),
    _pause(true)
{
    // register event types
    _evtMan.addEvent(NXSysEvtLowMem::sEvtType);
    _evtMan.addEvent(NXSysEvtWinHide::sEvtType);
    _evtMan.addEvent(NXSysEvtWinShow::sEvtType);
    _evtMan.addEvent(NXSysEvtWinResize::sEvtType);
    _evtMan.addEvent(NXSysEvtWinCreated::sEvtType);
    _evtMan.addEvent(NXSysEvtWinDestroy::sEvtType);
}

bool
NXSystem::init(const int argc,
               const char** argv,
               const NXAppOptions *pOptions)
{

    if (!NXSystemImp::initImp(argc, argv, pOptions))
    {
        return false;
    }

    return true;
}

void
NXSystem::term()
{
    NXSystemImp::termImp(&_evtMan);
}

bool
NXSystem::createWindow(const NXAppOptions* pOptions)
{
    return NXSystemImp::createWindowImp(pOptions, &_evtMan);
}

NXWindow*
NXSystem::window()
{
   return NXSystemImp::windowImp();
}

void
NXSystem::tick()
{
    NXSystemImp::tickImp(_pInputMan);
}

void
NXSystem::beginFrame()
{
    _evtMan.update();
}

void
NXSystem::endFrame()
{
    NXWindow* p_window = NXSystemImp::windowImp();
    if (p_window)
    {
        p_window->swapBuffers();
    }
}

}
