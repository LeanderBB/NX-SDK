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
#ifndef __NX_ALLOCTRACKER_H__
#define __NX_ALLOCTRACKER_H__

namespace nx
{

class NXAllocTracker
{
public:

    NXAllocTracker();

    ~NXAllocTracker();

    void* malloc(const size_t size);

    void* calloc(const size_t size);

    void free(void *ptr);

    void* realloc(void *ptr, const size_t size);

    size_t bytesAllocated() const
    {
        return _bytesAllocated;
    }

protected:
    volatile size_t _bytesAllocated;
};

}

#endif
