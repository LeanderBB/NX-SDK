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
#ifndef __NX_MEMORY_H__
#define __NX_MEMORY_H__

namespace nx
{

typedef void* (*fnMalloc_t)(size_t);
typedef void  (*fnFree_t)(void*);
typedef void* (*fnCalloc_t)(size_t, size_t);
typedef void* (*fnRealloc_t)(void*, size_t);


void*
NXMalloc(size_t size);

void
NXFree(void* ptr);

void*
NXCalloc(size_t s,
         size_t size);

void*
NXMemMove(void* dst,
          const void* src,
          size_t size);

void*
NXRealloc(void* src,
          size_t size);

/**
 * If NX_MEMORY_TRACK_ALLOCATIONS is not defined, will return max value of size_t
 */
size_t
NXAllocatedMemory();

class NXScopedFree
{
public:

    NXScopedFree(void* ptr):
        _ptr(ptr)
    {

    }

    ~NXScopedFree()
    {
        if(_ptr)
        {
            NXFree(_ptr);
        }
    }

    void* get()
    {
        return _ptr;
    }

private:
    NX_CPP_NO_COPY(NXScopedFree);
    void *_ptr;
};

class NXScopedAllocator
{
public:
    virtual ~NXScopedAllocator();
protected:
    NXScopedAllocator(fnMalloc_t fnmalloc,
                      fnFree_t fnfree,
                      fnCalloc_t fncalloc,
                      fnRealloc_t fnrealloc);
protected:
    fnMalloc_t const  _prevMalloc;
    fnFree_t const    _prevFree;
    fnCalloc_t const  _prevCalloc;
    fnRealloc_t const _prevRealloc;
};


class NXScopedAllocatorTracker : protected NXScopedAllocator
{
public:

    NXScopedAllocatorTracker(const char* const name);

    ~NXScopedAllocatorTracker();

private:
    static void* malloc(const size_t size);

    static void* calloc(const size_t s,
                        const size_t size);

    static void free(void *ptr);

    static void* realloc(void *ptr,
                         const size_t size);

private:
    const char* const _name;
    size_t _bytes;
    static NXScopedAllocatorTracker* spTracker;
};

}
#endif
