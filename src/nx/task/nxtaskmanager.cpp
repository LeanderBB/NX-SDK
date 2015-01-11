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
#include "nx/task/nxtaskmanager.h"
#include "nx/task/nxtaskexecutor.h"
#include "nx/task/nxtask.h"

namespace nx
{

NXTaskManager::NXTaskManager()
{

}

NXTaskManager::~NXTaskManager()
{
    NX_ASSERT(_executors.size() == 0);
}

bool
NXTaskManager::init()
{

    bool result = true;

    // Todo: Get cpu cores
    const int nCpu = 2;

    for (int i = 0; i < nCpu && result; ++i)
    {
        NXString threadName = NXStrWithFormat("NXTSKE%02d", i);
        NXTaskExecutor* p_exec = new NXTaskExecutor(threadName.c_str(), this);
        if (p_exec->start())
        {
            _executors.push_back(NXCntrPtr<NXTaskExecutor>(p_exec));
        }
        else
        {
            NXLogError("TaskManager: Failed to start task executor (%02d)", i);
            result = false;

            // delete previous created threads
            term();
        }
    }
    return result;
}

void
NXTaskManager::term()
{
    for (auto it = _executors.begin(); it != _executors.end(); ++it)
    {
        (*it)->stop();
        (*it)->join();
    }
    _executors.clear();
}

void
NXTaskManager::queueTask(NXTask* pTask)
{
    // Add task to queue
    time_t t = time(nullptr);
    pTask->_timeSubmitted = t;
    _tasks.queue(pTask);
}

}
