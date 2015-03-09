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
#include <time.h>
#endif

namespace nx
{

#if defined(NX_OS_UNIX) && !defined(NX_SYSTEM_SDL2)

static struct timespec sTimeStart;
static bool sTimeStarted = false;
static void
nxAndroidInitTime()
{
    if (sTimeStarted)
    {
        return;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &sTimeStart) == 0)
    {
        sTimeStarted = true;
    }
    else
    {
        NXLogError("nxAndroidInitTime: Failed to init clock");
        sTimeStarted = false;
    }
}

#endif


nx_u64
nxGetPerformanceCounter()
{
#if defined(NX_SYSTEM_SDL2)
    return SDL_GetPerformanceCounter();
#elif defined(NX_OS_UNIX)
    nx_u64 ticks;
    if (!sTimeStarted)
    {
        nxAndroidInitTime();
    }

    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);
    ticks = now.tv_sec;
    ticks *= 1000000000;
    ticks += now.tv_nsec;

    return ticks;
#else
    NXLogError("nxGetPerformanceCounter not yet implemented");
    return 0;
#endif
}

nx_u64
nxGetPerformanceFrequency()
{
#if defined(NX_SYSTEM_SDL2)
    return SDL_GetPerformanceFrequency();
#elif defined(NX_OS_UNIX)
    return 1000000000;
#else
    NXLogError("nxGetPerformanceFrequency not yet implemented");
               return 0;
#endif
}

nx_u32 nxGetTicks()
{
#if defined(NX_SYSTEM_SDL2)
    return SDL_GetTicks();
#elif defined(NX_OS_UNIX)
    nx_u32 ticks;
    if (!sTimeStarted)
    {
        nxAndroidInitTime();
    }
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    ticks = (now.tv_sec - sTimeStart.tv_sec) * 1000 + (now.tv_nsec - sTimeStart.tv_nsec) / 1000000;

    return ticks;
#else
    NXLogError("nxGetTicks not yet implemented");
    return 0;
#endif
}

}
