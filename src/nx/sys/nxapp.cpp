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

    if(!_system.init(argc, argv, &_options))
    {
        NXLogError("NXApp: Failed to init system");
        return false;
    }

    if (!appInit(argc, argv))
    {
         NXLogError("NXApp: Failed to init app");
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

void
NXApp::run()
{

    while (!_system.quit())
    {
         _system.tick();
        if (!_system.paused())
        {
            appRun();
            _system.window()->swapBuffers();
        }

    }
}

void
NXApp::term()
{
    NXLog("NXApp: Terminating...");
    appTerm();
    _system.term();

    NXLog("NXApp: Term complete");
}

}
