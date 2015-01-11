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
#ifndef __NX_LOCK_H__
#define __NX_LOCK_H__


#if defined(NX_SYSTEM_SDL2)
#include "nx/os/sdl2/nxlocksdl.h"
#elif defined(NX_OS_UNIX)
#include "nx/os/posix/nxlockposix.h"
#else
#error "Missing implementation for locks/conditions"
#endif

namespace nx
{

class NXLock : public NXLockImp
{
public:

    NXLock(){}

    ~NXLock(){}

    void lock()
    {
        NXLockImp::lockImp();
    }

    void unlock()
    {
        NXLockImp::unlockImp();
    }

private:
    NX_CPP_NO_COPY(NXLock);

};


class NXScopedLock
{
public:

    NXScopedLock(NXLock& lock) :
        _lock(lock)
    {
        _lock.lock();
    }

    ~NXScopedLock()
    {
        _lock.unlock();
    }
private:
    NX_CPP_NO_COPY(NXScopedLock);
private:
    NXLock& _lock;
};


class NXCondition : public NXConditionImp
{
    NXCondition() {}

    ~NXCondition() {}

    void wait(NXLock& lock)
    {
        NXConditionImp::waitImp(lock);
    }

    void signalOne()
    {
        NXConditionImp::signalOneImp();
    }

    void signallAll()
    {
        NXConditionImp::signallAllImp();
    }

private:
    NX_CPP_NO_COPY(NXCondition);

};

}

#endif // NXLOCK_H
