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
#ifndef __NX_THREADSDL_H__
#define __NX_THREADSDL_H__

#if defined(NX_SYSTEM_SDL2)
struct SDL_Thread;
namespace nx
{

class NXThreadImp
{
public:
    NXThreadImp();

    virtual ~NXThreadImp();
protected:

    static nx_threadId_t currentThreadIdImp();

    nx_threadId_t threadIdImp() const;

    bool startImp(const char* threadName,
                 int (*func)(void*));

    bool setPriorityImp(const ThreadPriority prio);

    bool joinImp();

    void setNameImp(const char*){}

private:
    NX_CPP_NO_COPY(NXThreadImp);

private:
    struct SDL_Thread* _pThread;
};

}
#endif // NX_SYSTEM_SDL2

#endif
