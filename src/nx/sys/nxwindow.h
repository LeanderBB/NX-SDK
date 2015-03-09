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
#ifndef __NX_WINDOW_H__
#define __NX_WINDOW_H__


namespace nx
{

struct NXWindowDesc
{
    nx_i32 w;
    nx_i32 h;
    const char* title;
    bool depth;
    bool stencil;
    bool resizable;
    bool fullscreen;
    bool debug;
};

}

#if defined(NX_SYSTEM_SDL2)
#include "nx/sys/sdl/nxwindowsdl.h"
#elif defined(NX_SYSTEM_GLFW)
#include "nx/sys/glfw/nxwindowglfw.h"
#elif defined(NX_SYSTEM_ANDROID)
#include "nx/sys/android/nxwindowandroid.h"
#else
#error "Missing implemenetation for the current system"
#endif

namespace nx
{

class NXWindow : public NXWindowImp
{
public:

    NXWindow(const char* title);

    ~NXWindow();

    void setTile(const char* title);

    void setDimensions(const nx_u32 width,
                       const nx_u32 height);

    void dimensions(nx_i32& width,
                    nx_i32 &height) const;

    bool create(const bool depth,
                const bool stencil,
                const bool resizable,
                const bool dgbctx);

    void destroy();

    bool bind();

    bool unbind();

    void setFullscreen(const bool fullscreen);

    bool fullscreen() const;

    void hide();

    void show();

    void swapBuffers();

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    void setCaptureInput(const bool b);

protected:
    friend class NXSystem;

private:
    NX_CPP_NO_COPY(NXWindow);

    friend class NXSystemImp;

protected:
    nx_i32 _width;
    nx_i32 _height;
    NXString _title;
    bool _fullscreen;
};


}

#endif
