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
#include "nx/os/sdl2/nxthreadsdl.h"
#include <SDL2/SDL_thread.h>

namespace nx
{

NX_STATIC_ASSERT(sizeof(nx_threadId_t) == sizeof(SDL_threadID),
                 "Type sizes for threadId do not match")

NXThreadImp::NXThreadImp():
    _pThread(nullptr)
{

}

NXThreadImp::~NXThreadImp()
{

}


nx_threadId_t
NXThreadImp::currentThreadIdImp()
{
    return SDL_ThreadID();
}

nx_threadId_t
NXThreadImp::threadIdImp() const
{
    return (_pThread) ? SDL_GetThreadID(_pThread) : 0;
}

bool
NXThreadImp::startImp(const char* threadName,
                      int (*func)(void*))
{
    bool result = false;
    if (!_pThread)
    {
        _pThread = SDL_CreateThread(func, threadName, this);
        if (!_pThread)
        {
            nx::NXLogError("Failed to start thread '%s': %s", threadName,
                         SDL_GetError());
        }
        else
        {
            result = true;
        }
    }
    return result;
}

bool
NXThreadImp::setPriorityImp(const ThreadPriority priority)
{
    SDL_ThreadPriority prio;
    switch(priority)
    {
    case kThreadPriorityHigh:
        prio = SDL_THREAD_PRIORITY_HIGH;
        break;
    case kThreadPriorityLow:
        prio = SDL_THREAD_PRIORITY_LOW;
        break;
    case kThreadPriotityNormal:
        prio = SDL_THREAD_PRIORITY_NORMAL;
        break;
    default:
        return false;
    }
    return SDL_SetThreadPriority(prio) == 0;
}

bool
NXThreadImp::joinImp()
{
    int status;
    SDL_WaitThread(_pThread, &status);
    return (status == 0);
}

/*
nx_tlsId_t
NXTlsCreate()
{
    return SDL_TLSCreate();
}

bool
NXTlsSet(const nx_tlsId_t id,
         void* pVal,
         void (*dtor)(void*))
{
    bool result = true;
    if (SDL_TLSSet(id, pVal, dtor) != 0)
    {
        nx::NXLogError("Failed to set TLS: %s", SDL_GetError());
        result = false;
    }
    return result;
}

void*
NXTlsGet(const nx_tlsId_t id)
{
    return SDL_TLSGet(id);
}

bool
NXTlsIdValid(const nx_tlsId_t id)
{
    return id != 0;
}
*/

}


#endif // NX_SYSTEM_SDL2
