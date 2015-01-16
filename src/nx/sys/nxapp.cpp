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
#include "nx/sys/nxapp.h"
#include "nx/sys/nxwindow.h"
#include "nx/event/nxeventdata.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxsysevents.h"

namespace nx
{

NXApp::NXApp(const char *name):
    _name(name)
{
    _options.width = 1280;
    _options.height = 720;
    _options.depth = false;
    _options.stencil = false;
    _options.fullscreen = false;
    _options.resizable = true;
#if defined(NX_DEBUG)
    _options.dbgctx = true;
#else
    _options.dbgctx = false;
#endif
}

NXApp::~NXApp()
{

}

bool
NXApp::init(const int argc, const char** argv)
{
    NXLog("NXApp: Intializing %s...", _name.c_str());

    setAppOptions(argc, argv, _options);

    _system.eventManager()->addListener(NXSysEvtWinDestroy::sEvtType, this);
    _system.eventManager()->addListener(NXSysEvtWinCreated::sEvtType,this);
    _system.eventManager()->addListener(NXSysEvtLowMem::sEvtType, this);

    if(!_system.init(argc, argv, &_options))
    {
        NXLogError("NXApp: Failed to init system");
        return false;
    }

    _system.setInputManager(&_inputMan);

    if (!onAppInit(argc, argv))
    {
        NXLogError("NXApp: Failed to init app");
        return false;
    }

    if (!_system.createWindow(&_options))
    {
        NXLogError("NXApp: Failed to create window");
        return false;
    }

    NXLog("NXApp: Init complete");
    return true;
}

void
NXApp::quit()
{
    _system.setQuit();
}

int NXApp::run(const int argc,
               const char **argv)
{

    if (!init(argc, argv))
    {
        return EXIT_FAILURE;
    }


    while (!_system.quit())
    {
        _system.tick();

        if (!_system.paused())
        {
            _system.beginFrame();
            appRun();
            _system.endFrame();
        }
    }

    term();

    return EXIT_SUCCESS;
}

void
NXApp::term()
{
    NXLog("NXApp: Terminating...");

    onAppWillTerm();
    _system.term();

    NXLog("NXApp: Term complete");
}

bool
NXApp::handleEvent(const NXEventData* pEvtData)
{
    switch (pEvtData->type)
    {
    case kSystemEventWinCreated:
        onWindowCreated();
        return true;
    case kSystemEventWinDestroy:
        onWindowWillBeDestroyed();
        return true;
    case kSystemEventLowMem:
        onSystemLowMemory();
        return false;
    default:
        NXLogWarning("NXApp::hanldeEvent: Unknown event recieved: %x", pEvtData->type);
        break;
    }
    return false;
}

}
