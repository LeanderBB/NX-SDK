//
// This file is part of the NX Project
//
// Copyright (c) 2015 Leander Beernaert
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
#if defined(NX_SYSTEM_GLFW)
#include "nx/sys/glfw/nxsystemglfw.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "nx/sys/nxsysevents.h"
#include "nx/sys/nxsystem.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxwindow.h"
#include "nx/sys/sdl/nxsdlinput.h"
#include "nx/sys/nxapp.h"
#include "nx/sys/glfw/nxglfwinput.h"
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

    // set error callback
    glfwSetErrorCallback(NXSystemImp::errorCallback);

    // Init GLFW
    if (!glfwInit())
    {
        NXLogError("NXSystem failed to initialize GLFW");
        return false;
    }

    nxSetupGLFWKeys();
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

    _pWindow->bind();

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

    // shutdown glfw
    glfwTerminate();
}


bool
NXSystemImp::tickImp(NXEventManager*,
                     NXInputManager* pInputManager)
{
    glfwPollEvents();
    if (pInputManager)
    {
        // TODO: process input events
    }

    return true;
}


bool
NXSystemImp::shouldQuitImp() const
{
    return (_pWindow) ? glfwWindowShouldClose(_pWindow->_pWindow) : false;
}


void
NXSystemImp::errorCallback(int error,
                           const char* description)
{
    NXLogError("GLFW: %d - %s", error, description);
}

}

#endif //defined(NX_SYSTEM_GLFW)

