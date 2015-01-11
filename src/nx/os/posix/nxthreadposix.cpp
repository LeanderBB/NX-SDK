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
#include "nx/os/posix/nxthreadposix.h"
#include "nx/os/nxthread.h"
namespace nx
{

struct NXPThreadData
{
    int (*func)(void*);
    void* data;
};

NX_STATIC_ASSERT(sizeof(nx_threadId_t) == sizeof(pthread_t),
                 "Type sizes for threadId do not match")

NXThreadImp::NXThreadImp()
{

}

NXThreadImp::~NXThreadImp()
{

}


nx_threadId_t
NXThreadImp::currentThreadIdImp()
{
    return (nx_threadId_t)pthread_self();
}

nx_threadId_t
NXThreadImp::threadIdImp() const
{
    return (nx_threadId_t) _thread;
}

bool
NXThreadImp::startImp(const char*,
                      int (*func)(void*))
{
    bool result = false;

    struct NXPThreadData* p_data = static_cast<struct NXPThreadData*>(NXMalloc(sizeof(p_data)));

    p_data->func = func;
    p_data->data = this;

    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    //pthread_attr_setstacksize(&thread_attr, this->stackSize);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    if ( pthread_create(&_thread, &thread_attr, NXThreadImp::ThreadFunc, p_data) == 0)
    {
        result = true;
    }
    else
    {
        NXFree(p_data);
    }

    pthread_attr_destroy(&thread_attr);
    return result;
}

bool
NXThreadImp::setPriorityImp(const ThreadPriority priority)
{
    (void) priority;
    NXLogWarning("NXThreadPosix Thread priority not yet implemented");
    return true;
}

bool
NXThreadImp::joinImp()
{
    void* ptr;
    return pthread_join(_thread, &ptr) == 0;
}

void*
NXThreadImp::ThreadFunc(void* pData)
{
    struct NXPThreadData* p_data = static_cast<struct NXPThreadData*>(pData);
    p_data->func(p_data->data);
    NXFree(p_data);
    return nullptr;
}

void
NXThreadImp::setNameImp(const char* name)
{
#if defined(NX_OS_APPLE)
    pthread_setname_np(name);
#else
    pthread_setname_np(pthread_self(), name);
#endif
}

}
#endif // NX_OS_UNIX && !NX_SYSTEN_SDL2
