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
#include "nx/util/nxtime.h"

#if defined(NX_SYSTEM_SDL2)
#include <SDL2/SDL_timer.h>
#elif defined(NX_SYSTEM_ANDROID)

#endif

namespace nx
{

nx_u64
NXGetPerformanceCounter()
{
#if defined(NX_SYSTEM_SDL2)
    return SDL_GetPerformanceCounter();
#else
    NXLogError("NXGetPerformanceCounter not yet implemented");
    return 0;
#endif
}

nx_u64
NXGetPerformanceFrequency()
{
 #if defined(NX_SYSTEM_SDL2)
    return SDL_GetPerformanceFrequency();   
#else
    NXLogError("NXGetPerformanceFrequency not yet implemented");
    return 0;
#endif
}

}
