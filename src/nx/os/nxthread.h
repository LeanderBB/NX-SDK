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
#ifndef __NX_THREAD_H__
#define __NX_THREAD_H__



namespace nx
{

enum ThreadPriority
{
    kThreadPriorityHigh,
    kThreadPriorityLow,
    kThreadPriotityNormal
};

}

#if defined(NX_SYSTEM_SDL2)
#include "nx/os/sdl2/nxthreadsdl.h"
#elif defined(NX_OS_UNIX)
#include "nx/os/posix/nxthreadposix.h"
#else
#error "No Thread Implementation available for the current system"
#endif


namespace nx
{



class NXThread : public NXThreadImp
{
public:

    static nx_threadId_t currentThreadId();

    virtual ~NXThread();

    bool start();

    void stop();

    void join();

    const char* name() const
    {
        return _name.c_str();
    }

    bool isRunning() const
    {
        return _isRunning;
    }

    nx_threadId_t threadId() const;

    ThreadPriority priority() const
    {
        return _priority;
    }

protected:

    NXThread(const char* name,
           const ThreadPriority priority);

    virtual int run() = 0;

    bool shouldExit() const
    {
        return _shouldExit;
    }

private:
    NX_CPP_NO_COPY(NXThread);

    static int ThreadEntryFunc(void *ptr);

protected:
    ThreadPriority _priority;
    const NXString _name;
    volatile bool _isRunning;
    volatile bool _shouldExit;
};

}
#endif // NXTHREAD_H
