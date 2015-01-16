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
#ifndef __NX_EVENTDATA_H__
#define __NX_EVENTDATA_H__

#include "nx/util/nxtime.h"

namespace nx
{

class NXEventData
{
public:
    virtual ~NXEventData(){}

     const nx_u32 type;
     const nx_u32 timestamp;

protected:

    NXEventData(const nx_u32 evttype):
        type(evttype),
        timestamp(static_cast<float>(nxGetTicks()))
    {
    }

    NXEventData(const nx_u32 evttype,
                const nx_u32 evttimestamp):
        type(evttype),
        timestamp(evttimestamp)
    {
    }


private:

    NX_CPP_NO_COPY(NXEventData);
};


}

#endif
