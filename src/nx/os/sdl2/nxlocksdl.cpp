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
#include "nxlocksdl.h"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
namespace nx
{
NXLockImp::NXLockImp()
{
    _pMutex = SDL_CreateMutex();
    if (!_pMutex)
    {
        nx::NXLogError("Failed to create Lock: %s", SDL_GetError());
    }
}

NXLockImp::~NXLockImp()
{
    if (_pMutex)
    {
        SDL_DestroyMutex(_pMutex);
    }
}

void
NXLockImp::lockImp()
{
    if (SDL_LockMutex(_pMutex) != 0)
    {
        nx::NXLogError("Failed to acquire Lock: %s", SDL_GetError());
    }
}

void
NXLockImp::unlockImp()
{
    if (SDL_UnlockMutex(_pMutex) != 0)
    {
        nx::NXLogError("Failed to release Lock: %s", SDL_GetError());
    }
}

// ----------------------------------------------------------------------------

NXConditionImp::NXConditionImp()
{
    _pCondition = SDL_CreateCond();
    if (!_pCondition)
    {
        nx::NXLogError("Failed to create Condition: %s", SDL_GetError());
    }
}

NXConditionImp::~NXConditionImp()
{
    if (_pCondition)
    {
        SDL_DestroyCond(_pCondition);
    }
}

void
NXConditionImp::waitImp(NXLockImp& lock)
{
    if (SDL_CondWait(_pCondition, lock._pMutex) != 0)
    {
        nx::NXLogError("Failed to waint on condition: %s", SDL_GetError());
    }
}

void
NXConditionImp::signalOneImp()
{
    if (SDL_CondSignal(_pCondition) != 0)
    {
        nx::NXLogError("Failed to signal condition: %s", SDL_GetError());
    }
}

void
NXConditionImp::signallAllImp()
{
    if (SDL_CondBroadcast(_pCondition) != 0)
    {
        nx::NXLogError("Failed to signall all condition: %s", SDL_GetError());
    }
}
}

#endif // #defined(NX_SYSTEM_SDL2)
