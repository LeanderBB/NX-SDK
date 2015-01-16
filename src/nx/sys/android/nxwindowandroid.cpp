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
#if defined(NX_SYSTEM_ANDROID)
#include "nx/sys/android/nxwindowandroid.h"
#include "nx/sys/gl/core44/flextGL.h"
#include "nx/sys/nxwindow.h"
#include "nx/sys/android/android_native_app_glue.h"


namespace nx
{

extern struct android_app* g_pAndroidApp;

NXWindowImp::NXWindowImp():
    _display(nullptr),
    _surface(nullptr),
    _context(nullptr)
{
}

NXWindowImp::~NXWindowImp()
{

}

void
NXWindowImp::setTitleImp(const char* title)
{
    (void) title;
}

void
NXWindowImp::setDimensionsImp(const nx_u32 width,
                              const nx_u32 height)
{
    NXLogWarning("NXWindowImp::setDimensionsImp Not available on Android");
    (void) width;
    (void) height;
}

void
NXWindowImp::dimensionsImp(nx_i32 &width,
                           nx_i32 &height)
{
    eglQuerySurface(_display, _surface, EGL_WIDTH, &width);
    eglQuerySurface(_display, _surface, EGL_WIDTH, &height);
}

bool
NXWindowImp::createImp(struct NXWindowDesc& desc)
{
    (void) desc;

    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, (desc.depth ? 24 : 0),
            EGL_STENCIL_SIZE, (desc.stencil ? 8 : 0),
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_NONE
    };

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (eglInitialize(_display, 0, 0) != EGL_TRUE)
    {
        NXLogError("NXWindowImp::createImp: Failed to initialize EGL");
        return false;
    }

    if(eglBindAPI(EGL_OPENGL_API) != EGL_TRUE)
    {
        NXLogError("NXWindowImp::createImp: Failed To Bind OpenGL API");
        return false;
    }
    EGLint format, num_configs;
    EGLConfig config;

    if(eglChooseConfig(_display, attribs, &config, 1, &num_configs) != EGL_TRUE)
    {
        NXLogError("NXWindowImp::createImp: Failed to select EGL config");
        return false;
    }

    if(eglGetConfigAttrib(_display, config, EGL_NATIVE_VISUAL_ID, &format) != EGL_TRUE)
    {
        NXLogError("NXWindowImp::createImp: Failed to get EGL config attrib");
        return false;
    }

    ANativeWindow_setBuffersGeometry(g_pAndroidApp->window, 0, 0, format);

    _surface = eglCreateWindowSurface(_display, config, g_pAndroidApp->window, NULL);
    _context = eglCreateContext(_display, config, NULL, NULL);

    if (!bindImp())
    {
        NXLogError("NXWindowImp::createImp: Failed to create EGL window/context");
        return false;
    }

    if (flextInit() != GL_TRUE)
    {
        NXLogError("NXWindow::create Failed to load all required OpenGL function. \
                   Please make sure you have an OpenGL 4.4 capable device");
                   return false;
    }

    return true;
}

void
NXWindowImp::destroyImp()
{
    if (_display != EGL_NO_DISPLAY)
    {
        unbindImp();

        if (_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
        _display = EGL_NO_DISPLAY;
        _context = EGL_NO_CONTEXT;
        _surface = EGL_NO_SURFACE;
    }
}


bool
NXWindowImp::bindImp()
{
    return EGL_TRUE == eglMakeCurrent(_display, _surface, _surface, _context);
}

bool
NXWindowImp::unbindImp()
{
    return EGL_TRUE == eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

void
NXWindowImp::setFullscreenImp(const bool fullscreen)
{
    (void) fullscreen;
    NXLogWarning("NXWindowImp::setFullscreenImp Not available on Android");
}

void
NXWindowImp::hideImp()
{
    NXLogWarning("NXWindowImp::hideImp Not available on Android");
}

void
NXWindowImp::showImp()
{
    NXLogWarning("NXWindowImp::showImp Not available on Android");
}

void
NXWindowImp::swapBuffersImp()
{
    eglSwapBuffers(_display, _surface);
}

void
NXWindowImp::setCaptureInputImp(const bool b)
{
    (void)b;
    NXLogWarning("NXWindowImp::setCaptureInputImp Not available on Android");
}

}

#endif // defined(NX_SYSTEM_ANDROID)
