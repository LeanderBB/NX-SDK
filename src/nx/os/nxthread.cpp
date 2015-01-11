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


namespace nx
{



nx_threadId_t
NXThread::currentThreadId()
{
    return NXThreadImp::currentThreadIdImp();
}

NXThread::~NXThread()
{
    NX_ASSERT(!isRunning());
}

bool
NXThread::start()
{
    return NXThreadImp::startImp(_name.c_str(), NXThread::ThreadEntryFunc);
}

void
NXThread::stop()
{
    _shouldExit = true;
}

void
NXThread::join()
{
    if (!NXThreadImp::joinImp())
    {
        nx::NXLogError("Thread '%s' exited unsuccessfully", _name.c_str());
    }
}


nx_threadId_t
NXThread::threadId() const
{
    return NXThreadImp::threadIdImp();
}

NXThread::NXThread(const char* name,
               const ThreadPriority priority) :
    NXThreadImp(),
    _priority(priority),
    _name(name),
    _isRunning(false),
    _shouldExit(false)
{

}

int
NXThread::ThreadEntryFunc(void *ptr)
{
    int result;
    NXThread* p_thread = static_cast<NXThread*>(ptr);

    p_thread->setNameImp(p_thread->name());
    if (!p_thread->setPriorityImp(p_thread->priority()))
    {
        nx::NXLogError("Thread '%s': Failed to set thread priotiry",
                     p_thread->name());
    }
    nx::NXLogDebug("Thread '%s' launched", p_thread->name());
    p_thread->_isRunning = true;
    result = p_thread->run();
    p_thread->_isRunning = false;
    nx::NXLogDebug("Thread '%s' exiting", p_thread->name());
    return result;
}

}
