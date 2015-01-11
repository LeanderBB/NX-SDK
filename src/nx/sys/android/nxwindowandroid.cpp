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


namespace nx
{
NXWindowImp::NXWindowImp()
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
    (void) width;
    (void) height;
}

void
NXWindowImp::dimensionsImp(nx_i32 &width,
                           nx_i32 &height)
{
    (void) width;
    (void) height;
}

bool
NXWindowImp::createImp(struct NXWindowDesc& desc)
{
    (void) desc;

    return false;
}

void
NXWindowImp::destroyImp()
{

}

bool
NXWindowImp::makeCurrentImp(void *pWin,
                            void *pCtx)
{
    (void) pWin;
    (void) pCtx;
    return false;
}

bool
NXWindowImp::bindImp()
{
    return false;
}

bool
NXWindowImp::unbindImp()
{
    return false;
}

void
NXWindowImp::setFullscreenImp(const bool fullscreen)
{
    (void) fullscreen;
}

void
NXWindowImp::hideImp()
{
}

void
NXWindowImp::showImp()
{

}

void
NXWindowImp::swapBuffersImp()
{

}

void
NXWindowImp::setCaptureInputImp(const bool b)
{
    (void)b;
}

}

#endif // defined(NX_SYSTEM_ANDROID)
