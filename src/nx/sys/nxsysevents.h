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
#ifndef __NX_SYSEVENTS_H__
#define __NX_SYSEVENTS_H__

#include "nx/event/nxeventdata.h"

namespace nx
{

enum SystemEvent
{
    kSystemEventWinHide = 0x9a902217,
    kSystemEventWinShow = 0xbf6d0d9f,
    kSystemEventWinResize = 0xde26a4d3,
    kSystemEventLowMem = 0xe96e04b
};

/// Event thrown in case of a window resize
class NXSysEvtWinResize : public NXEventData
{
public:

    static const nx_u32 sEvtType;

    NXSysEvtWinResize(const nx_i32 width,
                      const nx_i32 height):
        NXEventData(sEvtType),
        w(width),
        h(height)
    {
    }

    ~NXSysEvtWinResize(){}

    const nx_i32 w;
    const nx_i32 h;
};

/// Event thrown in case of a low memory response by the os
class NXSysEvtLowMem: public NXEventData
{
public:

    static const nx_u32 sEvtType;

    NXSysEvtLowMem():
        NXEventData(sEvtType)
    {
    }

    ~NXSysEvtLowMem(){}
};


/// Event triggered when window is hidden
class NXSysEvtWinHide: public NXEventData
{
public:

    static const nx_u32 sEvtType;

    NXSysEvtWinHide():
        NXEventData(sEvtType)
    {
    }

    ~NXSysEvtWinHide(){}
};

/// Event triggered when window is shown
class NXSysEvtWinShow: public NXEventData
{
public:

    static const nx_u32 sEvtType;

    NXSysEvtWinShow():
        NXEventData(sEvtType)
    {
    }

    ~NXSysEvtWinShow(){}
};
}

#endif
