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

static fnMalloc_t g_fnmalloc = malloc;
static fnFree_t g_fnfree = free;
static fnCalloc_t g_fncalloc = calloc;
static fnRealloc_t g_fnrealloc = realloc;


void*
NXMalloc(size_t size)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.malloc(size);
#else
    return g_fnmalloc(size);
#endif
}

void
NXFree(void* ptr)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    gTracker.free(ptr);
#else
    g_fnfree(ptr);
#endif
}

void*
NXCalloc(size_t s,
         size_t size)
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.calloc(s * size);
#else
    return g_fncalloc(s, size);
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
    return g_fnrealloc(src, size);
#endif
}

size_t
NXAllocatedMemory()
{
#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
    return gTracker.bytesAllocated();
#else
    return 0;
#endif
}


NXScopedAllocator::~NXScopedAllocator()
{
    g_fnmalloc = _prevMalloc;
    g_fnfree = _prevFree;
    g_fncalloc = _prevCalloc;
    g_fnrealloc = _prevRealloc;
}

NXScopedAllocator::NXScopedAllocator(fnMalloc_t fnmalloc,
                                     fnFree_t fnfree,
                                     fnCalloc_t fncalloc,
                                     fnRealloc_t fnrealloc):
    _prevMalloc(g_fnmalloc),
    _prevFree(g_fnfree),
    _prevCalloc(g_fncalloc),
    _prevRealloc(g_fnrealloc)
{
    g_fnmalloc = fnmalloc;
    g_fnfree = fnfree;
    g_fncalloc = fncalloc;
    g_fnrealloc = fnrealloc;
}

NXScopedAllocatorTracker* NXScopedAllocatorTracker::spTracker = nullptr;
NXScopedAllocatorTracker::NXScopedAllocatorTracker(const char* const name):
    NXScopedAllocator(NXScopedAllocatorTracker::malloc,
                      NXScopedAllocatorTracker::free,
                      NXScopedAllocatorTracker::calloc,
                      NXScopedAllocatorTracker::realloc),
    _name(name),
    _bytes(0)
{
    if (spTracker)
    {
        //TODO: Multiple instances??
        NXLogFatal("An instance of scoped tracker is already instanced");
    }
    spTracker = this;
}

NXScopedAllocatorTracker::~NXScopedAllocatorTracker()
{
    if (_bytes !=0)
    {
        NXLogError("NXScopedAllocatorTracker(%s): Did not release all memory in scope: %" NX_PRIsize " bytes left\n",
                   _name, _bytes);
    }
}

void*
NXScopedAllocatorTracker::malloc(const size_t size)
{
    void* ptr = spTracker->_prevMalloc(size + sizeof(size_t));
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        sptr[0] = size;
        NXAtomicAddSize((volatile nx_ssize_t*)&spTracker->_bytes, (nx_ssize_t)size);
        return (char*)ptr + sizeof(size_t);
    }
    return NULL;
}

void*
NXScopedAllocatorTracker::calloc(const size_t s,
                                 const size_t size)
{
    void* ptr = spTracker->_prevCalloc(s, size + sizeof(size_t));
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        sptr[0] = size;
        NXAtomicAddSize((volatile nx_ssize_t*)&spTracker->_bytes, (nx_ssize_t)size);
        return (char*)ptr + sizeof(size_t);
    }
    return NULL;
}

void
NXScopedAllocatorTracker::free(void *ptr)
{
    if (ptr)
    {
        size_t* sptr = (size_t*)ptr;
        --sptr;
        size_t size = *sptr;
        NXAtomicAddSize((volatile nx_ssize_t*)&spTracker->_bytes, -((const nx_ssize_t)size));
        spTracker->_prevFree(sptr);
    }
}

void*
NXScopedAllocatorTracker::realloc(void *ptr,
                                  const size_t size)
{
    if (!ptr)
    {
        return NXScopedAllocatorTracker::malloc(size);
    }
    else
    {
        size_t* sptr = (size_t*)ptr;
        --sptr;
        size_t cur_size = *sptr;
        nx_ssize_t isize = (nx_ssize_t)size;
        nx_ssize_t icur_size = (nx_ssize_t)cur_size;
        NXAtomicAddSize((volatile nx_ssize_t*)&spTracker->_bytes, (isize - icur_size));
        sptr = (size_t*) spTracker->_prevRealloc(sptr, size + sizeof(size_t));
        sptr[0] = size;
        return ++sptr;
    }
}

}

void* operator new ( size_t s ) { return nx::NXMalloc(s); }
void* operator new[] ( size_t s ) { return nx::NXMalloc(s); }

void operator delete ( void* p ) NX_CPP_DELETE_NOEXCEPT { nx::NXFree(p); }
void operator delete[] ( void* p ) NX_CPP_DELETE_NOEXCEPT { nx::NXFree(p); }
