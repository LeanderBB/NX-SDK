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

#if defined(NX_OS_UNIX) && !defined(NX_SYSTEM_SDL2)
#include "nx/os/posix/nxlockposix.h"

namespace nx
{
NXLockImp::NXLockImp()
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);

    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);

    int ret = pthread_mutex_init(&_mutex, &mutex_attr);
    if (ret != 0)
        nx::NXLogError("Failed to create Lock");

    pthread_mutexattr_destroy(&mutex_attr);
}

NXLockImp::~NXLockImp()
{
    pthread_mutex_destroy(&_mutex);
}

void
NXLockImp::lockImp()
{
    int ret = pthread_mutex_lock(&_mutex);
    if( ret != 0)
    {
        nx::NXLogError("Failed to acquire Lock");
    }
}

void
NXLockImp::unlockImp()
{
    int ret = pthread_mutex_unlock(&_mutex);
    if( ret != 0)
    {
        nx::NXLogError("Failed to release Lock");
    }
}

// ----------------------------------------------------------------------------

NXConditionImp::NXConditionImp()
{
    int ret = pthread_cond_init(&_condition, NULL);
    if (ret != 0)
    {
        nx::NXLogError("Failed to create Condition");
    }
}

NXConditionImp::~NXConditionImp()
{
    pthread_cond_destroy(&_condition);
}

void
NXConditionImp::waitImp(NXLockImp& lock)
{
    int ret = pthread_cond_wait(&_condition, &lock._mutex);
    if (ret != 0)
    {
        nx::NXLogError("Failed to waint on condition");
    }
}

void
NXConditionImp::signalOneImp()
{
    int ret = pthread_cond_signal(&_condition);
    if (ret != 0)
    {
        nx::NXLogError("Failed to signal condition");
    }
}

void
NXConditionImp::signallAllImp()
{
    int ret = pthread_cond_broadcast(&_condition);
    if (ret != 0)
    {
        nx::NXLogError("Failed to signall all condition");
    }
}
}


#endif // defined(NX_OS_UNIX) && !defined(NX_SYSTEM_SDL2)


