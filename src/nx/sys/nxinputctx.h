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
#ifndef __NX_INPUTCTX_H__
#define __NX_INPUTCTX_H__

#include "nxinput.h"
namespace nx
{

class NXInputCtx
{
public:
    virtual ~NXInputCtx(){}

    /// Handle an input event, return true if the event should stop
    /// being propagated
    virtual bool handleInputEvent(const NXInputEvent& evt) = 0;

    const char* name() const
    {
        return _name.c_str();
    }

    nx_u32 priority() const
    {
        return _prio;
    }

protected:
    NXInputCtx(const char* name, const nx_u32 priotity):
        _name(name),
        _prio(priotity)
    {}


private:
    NX_CPP_NO_COPY(NXInputCtx);

protected:
    const NXString _name;
    const nx_u32 _prio;
};

}
#endif
