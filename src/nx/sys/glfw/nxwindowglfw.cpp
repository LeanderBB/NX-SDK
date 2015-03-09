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

#include "nx/sys/glfw/nxwindowglfw.h"
#include "nx/sys/nxwindow.h"
#include "nx/sys/nxsystem.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxsysevents.h"
#include "nx/sys/glfw/nxglfwinput.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace nx
{
NXWindowImp::NXWindowImp():
    _pWindow(nullptr)
{
}

NXWindowImp::~NXWindowImp()
{
    NX_ASSERT(!_pWindow);
}

void
NXWindowImp::setTitleImp(const char* title)
{
    if (_pWindow)
    {
        glfwSetWindowTitle(_pWindow, title);
    }
}

void
NXWindowImp::setDimensionsImp(const nx_u32 width,
                              const nx_u32 height)
{
    if (_pWindow)
    {
        glfwSetWindowSize(_pWindow, width, height);
    }
}

void
NXWindowImp::dimensionsImp(nx_i32 &width,
                           nx_i32 &height)
{
    if(_pWindow)
    {
        glfwGetFramebufferSize(_pWindow, &width, &height);
    }
    else
    {
        width = height = -1;
    }
}


static void
setWindowHint(const bool depth,
              const bool stencil,
              const bool dbg)
{

    // setup default framebuffer
    glfwWindowHint(GLFW_ALPHA_BITS, 0);
    // double buffered
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    // multi-sample
    glfwWindowHint(GLFW_SAMPLES, 0);

    // depth stencil
    glfwWindowHint(GLFW_DEPTH_BITS, depth ? 24 : 0);
    glfwWindowHint(GLFW_STENCIL_BITS, stencil ? 8 : 0);
    // Setup OpenGL Version
    int context_major = 0;
    int context_minor = 0;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    context_major = 4;
    context_minor = 5;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_minor);
    if (dbg)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    }
    else
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 0);
    }
}

bool
NXWindowImp::createImp(struct NXWindowDesc& desc)
{
    if (!_pWindow)
    {
        setWindowHint(desc.depth, desc.stencil, desc.debug);

        if (desc.resizable)
        {
            glfwWindowHint(GLFW_RESIZABLE, 1);
        }

        _pWindow = glfwCreateWindow(desc.w, desc.h, desc.title, desc.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

        if (!_pWindow)
        {
            NXLogError("NXWindow::create Failed to create window");
            return false;
        }

        glfwSetFramebufferSizeCallback(_pWindow, NXWindowImp::windowFBResize);
        glfwSetCursorPosCallback(_pWindow, nxGLFWMousePositionCallback);
        glfwSetKeyCallback(_pWindow, nxGLFWKeyboardCallback);
        glfwSetMouseButtonCallback(_pWindow, nxGLFWMouseCallback);
        glfwSetScrollCallback(_pWindow, nxGLFWScrollCallback);
    }
    return true;
}

void
NXWindowImp::destroyImp()
{
    if (_pWindow)
    {
        unbindImp();
        glfwDestroyWindow(_pWindow);
        _pWindow = nullptr;
    }
}

bool
NXWindowImp::bindImp()
{
    glfwMakeContextCurrent(_pWindow);
    return true;
}

bool
NXWindowImp::unbindImp()
{
    glfwMakeContextCurrent(NULL);
    return true;
}

void
NXWindowImp::setFullscreenImp(const bool fullscreen)
{
    (void) fullscreen;
    NXLogError("NXWindowGLFW: Cannot change between fullscreen and normal windows at the moment");
}

void
NXWindowImp::hideImp()
{
    if (_pWindow)
    {
        glfwHideWindow(_pWindow);
    }
}

void
NXWindowImp::showImp()
{
    if (_pWindow)
    {
        glfwShowWindow(_pWindow);
    }
}

void
NXWindowImp::swapBuffersImp()
{
    glfwSwapBuffers(_pWindow);
}

void
NXWindowImp::setCaptureInputImp(const bool b)
{
    if(_pWindow)
    {
        glfwSetInputMode(_pWindow, GLFW_CURSOR, (b) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}

bool
NXWindowImp::isFocused() const
{
    return (_pWindow) ? glfwGetWindowAttrib(_pWindow, GLFW_FOCUSED) : false;
}

void
NXWindowImp::windowFBResize(GLFWwindow* pWin,
                            int width,
                            int height)
{
    (void) pWin;

    NXSystem* p_sys = NXSystem::instance();
    NX_ASSERT(p_sys);
    NXEventManager* p_evtman =p_sys->eventManager();
    NXSysEvtWinResize evt(width, height);
    p_evtman->trigger(&evt);

}

}

#endif //defined(NX_SYSTEM_GLFW)
