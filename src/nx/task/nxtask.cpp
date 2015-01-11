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
#include "nx/task/nxtask.h"

namespace nx
{

NXTask::NXTask(const char *name, const TaskPriority prio):
    _name(name),
    _prio(prio),
    _timeSubmitted(),
    _timeStart(),
    _timeFinish(),
    _started(false),
    _finished(false)
{
}


void
NXTask::wait()
{
    while(!_finished)
    {
        NXSleepMicro(3);
    }
}

void
NXTask::execute()
{
    if (!_started)
    {
        _timeStart = time(nullptr);
        _started = true;
        this->run();
        _timeFinish = time(nullptr);
        _finished = true;
    }
    else
    {
        NXLogError("Task '%s' has already been started", _name.c_str());
    }
}


}
