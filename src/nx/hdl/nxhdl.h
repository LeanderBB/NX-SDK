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
#ifndef __NX_HDL_H__
#define __NX_HDL_H__

#define NX_HDL_INVALID (nx_u32(0))

namespace nx
{

class NXHdl
{
public:
    NXHdl():
        idx(0),
        counter(0),
        type(0)
    {
    }

    NXHdl(const nx_u32 idx,
             const nx_u32 counter,
             const nx_u32 magic):
        idx(idx),
        counter(counter),
        type(magic)
    {
    }

    operator nx_u32() const
    {
        return type << 27 | counter << 12 | idx;
    }

    void reset()
    {
        idx = 0;
        counter = 0;
        type = 0;
    }

    bool valid() const
    {
        return *this != NX_HDL_INVALID;
    }

    unsigned idx: 12;
    unsigned counter:15;
    unsigned type: 5;
};
}

#endif
