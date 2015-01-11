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
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
static NXAllocTracker gTracker;
#endif

void*
NXMalloc(size_t size)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.malloc(size);
#else
    return malloc(size);
#endif
}

void
NXFree(void* ptr)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    gTracker.free(ptr);
#else
    free(ptr);
#endif
}

void*
NXCalloc(size_t s,
         size_t size)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.calloc(s * size);
#else
    return calloc(s, size);
#endif
}

void*
NXMemMove(void* dst,
          const void* src,
          size_t size)
{
    return memmove(dst, src, size);
}

void*
NXRealloc(void* src,
          size_t size)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.realloc(src, size);
#else
    return realloc(src, size);
#endif
}

size_t
NXAllocatedMemory()
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.bytesAllocated();
#else
    return NX_SIZE_T_MAX;
#endif
}


}

void* operator new ( size_t s ) { return nx::NXMalloc(s); }
void* operator new[] ( size_t s ) { return nx::NXMalloc(s); }

void operator delete ( void* p ) NX_CPP_DELETE_NOEXCEPT { nx::NXFree(p); }
void operator delete[] ( void* p ) NX_CPP_DELETE_NOEXCEPT { nx::NXFree(p); }
