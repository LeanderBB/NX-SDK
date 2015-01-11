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
#ifndef __NX_TASK_H__
#define __NX_TASK_H__

namespace nx
{

enum TaskPriority
{
    kTaskPriorityHigh,
    kTaskPriorityNormal,
    kTaskPriorityLow
};

class NXTask
{
public:

    virtual ~NXTask()
    {
    }

    bool finished() const
    {
        return _finished;
    }

    bool started() const
    {
        return _started;
    }

    void wait();

    void execute();

protected:
    NXTask(const char* name,
           const TaskPriority prio);


    virtual void run() = 0;
private:
    NX_CPP_NO_COPY(NXTask);

protected:
    friend class NXTaskManager;
    const NXString _name;
    const TaskPriority _prio;
    time_t _timeSubmitted;
    time_t _timeStart;
    time_t _timeFinish;
    volatile bool _started;
    volatile bool _finished;

};

}
#endif
