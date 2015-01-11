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
#include "nx/allocator/nxalloctracker.h"

namespace nx
{
NXAllocTracker::NXAllocTracker():
    _bytesAllocated(0)
{

}

NXAllocTracker::~NXAllocTracker()
{
    if (_bytesAllocated != 0)
    {
        fprintf(stderr,"!!! NXAllocTracker: Did not release all memory: %" NX_PRIsize " bytes left\n",
                _bytesAllocated);
        fflush(stderr);
    }
}

void*
NXAllocTracker::malloc(const size_t size)
{
    void* ptr = ::malloc(size + sizeof(size_t));
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        sptr[0] = size;
        NXAtomicAddSize((volatile nx_ssize_t*)&_bytesAllocated, (nx_ssize_t)size);
        return (char*)ptr + sizeof(size_t);
    }
    return NULL;
}

void*
NXAllocTracker::calloc(const size_t size)
{
    void* ptr = ::calloc(1, size + sizeof(size_t));
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        sptr[0] = size;
        NXAtomicAddSize((volatile nx_ssize_t*)&_bytesAllocated, (nx_ssize_t)size);
        return (char*)ptr + sizeof(size_t);
    }
    return NULL;
}

void
NXAllocTracker::free(void *ptr)
{
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        --sptr;
        size_t size = *sptr;
        NXAtomicAddSize((volatile nx_ssize_t*)&_bytesAllocated, -((const nx_ssize_t)size));
        ::free(sptr);
    }
}

void*
NXAllocTracker::realloc(void* ptr, const size_t size)
{
    if (!ptr)
    {
        return this->malloc(size);
    }
    else
    {
        size_t* sptr = (size_t*)ptr;
        --sptr;
        size_t cur_size = *sptr;
        nx_ssize_t isize = (nx_ssize_t)size;
        nx_ssize_t icur_size = (nx_ssize_t)cur_size;
        NXAtomicAddSize((volatile nx_ssize_t*)&_bytesAllocated, (isize - icur_size));
        sptr = (size_t*) ::realloc(sptr, size + sizeof(size_t));
        sptr[0] = size;
        return ++sptr;
    }
}

}
