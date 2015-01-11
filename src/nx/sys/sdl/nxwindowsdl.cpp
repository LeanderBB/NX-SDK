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
#include "nx/sys/sdl/nxwindowsdl.h"
#include "nx/sys/gl/core44/flextGL.h"
#include "nx/sys/nxwindow.h"
namespace nx
{
NXWindowImp::NXWindowImp():
    _pWindow(nullptr),
    _pCtx(nullptr)
{
}

NXWindowImp::~NXWindowImp()
{
    NX_ASSERT(!_pWindow);
    NX_ASSERT(!_pCtx);
}

void
NXWindowImp::setTitleImp(const char* title)
{
    if (_pWindow)
    {
        SDL_SetWindowTitle(_pWindow,title);
    }
}

void
NXWindowImp::setDimensionsImp(const nx_u32 width,
                              const nx_u32 height)
{
    if (_pWindow)
    {
        SDL_SetWindowSize(_pWindow, width, height);
    }
}

void
NXWindowImp::dimensionsImp(nx_i32 &width,
                           nx_i32 &height)
{
    if(_pWindow)
    {
        SDL_GetWindowSize(_pWindow, &width, &height);
    }
    else
    {
        width = height = -1;
    }
}

// return 0 on error;
#define NX_SET_SDL_GL_ATTR(attr, val) \
    res = SDL_GL_SetAttribute(attr, val); \
    if (res != 0) \
{ \
    NXLogError("NXWindow::create Failed to set SDL attribute " #attr ": %s", SDL_GetError()); \
    return false; \
}

static bool setSDLFlags(const bool depth,
                        const bool stencil,
                        const bool dbg)
{
    int res;
    // enable context sharing if necessary
    //NX_SET_SDL_GL_ATTR(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    // setup default framebuffer
    NX_SET_SDL_GL_ATTR(SDL_GL_RED_SIZE, 8);
    NX_SET_SDL_GL_ATTR(SDL_GL_BLUE_SIZE, 8);
    NX_SET_SDL_GL_ATTR(SDL_GL_GREEN_SIZE, 8);
    NX_SET_SDL_GL_ATTR(SDL_GL_ALPHA_SIZE, 0);
    // double buffered
    NX_SET_SDL_GL_ATTR(SDL_GL_DOUBLEBUFFER, 1);
    // multi-sample
    NX_SET_SDL_GL_ATTR(SDL_GL_MULTISAMPLEBUFFERS, 0);
    NX_SET_SDL_GL_ATTR(SDL_GL_MULTISAMPLESAMPLES, 0);
    // depth stencil
    NX_SET_SDL_GL_ATTR(SDL_GL_DEPTH_SIZE, depth ? 24 : 0);
    NX_SET_SDL_GL_ATTR(SDL_GL_STENCIL_SIZE, stencil ? 8 : 0);
    // Setup OpenGL Version
    int context_major = 0;
    int context_minor = 0;
#if defined(NX_TARGET_IOS) || defined(NX_TARGET_ANDROID)
    NX_SET_SDL_GL_ATTR(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_EGL);
    context_major = 3;
    context_minor = 0;
#else
    NX_SET_SDL_GL_ATTR(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    context_major = 4;
    context_minor = 4;
#endif
    NX_SET_SDL_GL_ATTR(SDL_GL_CONTEXT_MAJOR_VERSION, context_major);
    NX_SET_SDL_GL_ATTR(SDL_GL_CONTEXT_MINOR_VERSION, context_minor);
    if (dbg)
    {
        NX_SET_SDL_GL_ATTR(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }
    return true;
}

#undef NX_SET_SDL_GL_ATTR


bool
NXWindowImp::createImp(struct NXWindowDesc& desc)
{
    if (!_pWindow)
    {
        if(!setSDLFlags(desc.depth, desc.stencil, desc.debug))
        {
            return false;
        }

        nx_u32 flags = SDL_WINDOW_OPENGL | (desc.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

        if (desc.resizable)
        {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        _pWindow = SDL_CreateWindow(desc.title, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,desc.w, desc.h, flags);

        if (!_pWindow)
        {
            NXLogError("NXWindow::create Failed to create window: %s", SDL_GetError());
            return false;
        }

        _pCtx = SDL_GL_CreateContext(_pWindow);
        if (!_pCtx)
        {
            NXLogError("NXWindow::create Failed to create ctx: %s", SDL_GetError());
            return false;
        }

        // init opengl

        int result = flextInit();
        if (result != GL_TRUE)
        {
            NXLogError("NXWindow::create Failed to load all required OpenGL function. \
                       Please make sure you have an OpenGL 4.4 capable device");
                       return false;
        }

    }
    return true;
}

void
NXWindowImp::destroyImp()
{
    if (_pCtx)
    {
        unbindImp();
        SDL_GL_DeleteContext(_pCtx);
        _pCtx = nullptr;
    }

    if (_pWindow)
    {
        SDL_DestroyWindow(_pWindow);
        _pWindow = nullptr;
    }
}

bool
NXWindowImp::makeCurrentImp(void *pWin,
                            void *pCtx)
{
    SDL_GLContext ctx = static_cast<SDL_GLContext>(pCtx);
    SDL_Window* p_win = static_cast<SDL_Window*>(pWin);

    int res = SDL_GL_MakeCurrent(p_win, ctx);
    if (res != 0)
    {
        NXLogError("NXWindow: Failed to make current: %s", SDL_GetError());
    }
    return res == 0;
}

bool
NXWindowImp::bindImp()
{
    return makeCurrentImp(_pWindow, _pCtx);
}

bool
NXWindowImp::unbindImp()
{
    return makeCurrentImp(nullptr, nullptr);
}

void
NXWindowImp::setFullscreenImp(const bool fullscreen)
{
    if (_pWindow)
    {
        SDL_SetWindowFullscreen(_pWindow, (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
    }
}

void
NXWindowImp::hideImp()
{
    if (_pWindow)
    {
        SDL_HideWindow(_pWindow);
    }
}

void
NXWindowImp::showImp()
{
    if (_pWindow)
    {
        SDL_ShowWindow(_pWindow);
    }
}

void
NXWindowImp::swapBuffersImp()
{
    SDL_GL_SwapWindow(_pWindow);
}

void
NXWindowImp::setCaptureInputImp(const bool b)
{
    SDL_SetWindowGrab((_pWindow), (b) ? SDL_TRUE : SDL_FALSE);
    SDL_SetRelativeMouseMode((b) ? SDL_TRUE : SDL_FALSE);
}

}

#endif // defined(NX_SYSTEM_SDL2)
