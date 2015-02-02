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
#ifndef __NX_HDLMANAGER_H__
#define __NX_HDLMANAGER_H__


#include "nx/hdl/nxhdl.h"

namespace nx
{

template<typename T, nx_u32 MAX_ENTRIES = 2048>
class NXHandleManager
{
public:

    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef NXHandleManager< T,MAX_ENTRIES> class_type;

    NX_STATIC_ASSERT((MAX_ENTRIES <= 4096), "Number of entries exceeds addressable limit")

    public:

        enum
    {
        kMaxEntries = MAX_ENTRIES
    };

    NXHandleManager(const char* name = "HandleManager"):
        _entries(),
        _nActive(0),
        _firstFreeIdx(0),
        _name(name)
    {
        reset();
    }

    ~NXHandleManager()
    {

    }

    void reset()
    {
        _nActive = 0;
        _firstFreeIdx = 0;

        int i;
        for ( i = 0; i < kMaxEntries; ++i)
        {
            NXHandleEntry& entry = _entries[i];
            entry.nextFreeIdx = i + 1;
            if (entry.active)
            {
                (&entry.value)->~T();
                new (&entry.value) T();
            }
            entry.active = 0;
        }
        _entries[i-1].eol = 1;
    }

    NXHdl add(const nx_u32 magic,
              const_reference entry)
    {
        NX_ASSERT(magic > 0 && magic <= 31);
        // check if capacity has been exhausted
        if (_nActive >= kMaxEntries - 1)
        {
            NXLogError("HandleManager::add (%s) : Exhausted all available handles",
                       _name.c_str());
            return NXHdl();
        }

        const nx_u32 new_idx = _firstFreeIdx;
        NX_ASSERT(new_idx < kMaxEntries);

        NXHandleEntry& hdle = _entries[new_idx];

        // do some checks
        NX_ASSERT(!hdle.active);
        NX_ASSERT(!hdle.eol);

        _firstFreeIdx = hdle.nextFreeIdx;
        hdle.nextFreeIdx = 0;
        ++hdle.counter;

        // check for the overflow, if it happens, make sure the counter starts at 1
        if (hdle.counter == 0)
        {
            hdle.counter = 1;
        }
        hdle.active = 1;
        ++_nActive;

        hdle.value = entry;
        return NXHdl(new_idx, hdle.counter, magic);
    }

    bool update(const NXHdl hdl,
                const_reference entry)
    {
        bool result = false;
        NXHandleEntry& hdle = _entries[hdl.idx];
        if (hdle.counter == hdl.counter && hdle.active)
        {
            hdle.value = entry;
            result = true;
        }
        return result;
    }

    bool remove(const NXHdl hdl)
    {
        bool result = false;
        NXHandleEntry& hdle = _entries[hdl.idx];
        if (hdle.counter == hdl.counter && hdle.active)
        {
            hdle.nextFreeIdx = _firstFreeIdx;
            hdle.active = 0;
            _firstFreeIdx = hdl.idx;
            --_nActive;
            result = true;
        }
        return result;
    }

    bool get(const NXHdl hdl,
             reference entry) const
    {
        const NXHandleEntry& hdle = _entries[hdl.idx];
        if (hdle.counter == hdl.counter && hdle.active)
        {
            entry = hdle.value;
            return true;
        }
        return false;
    }

    bool contains(const NXHdl hdl) const
    {
        const NXHandleEntry& hdle = _entries[hdl.idx];
        return (hdle.counter == hdl.counter && hdle.active);
    }

    nx_u32 count() const
    {
        return _nActive;
    }

    nx_u32 maxCount() const
    {
        return MAX_ENTRIES;
    }

private:
    NX_CPP_NO_COPY_TEMPLATE(NXHandleManager, class_type);

protected:
    class NXHandleEntry
    {
    public:
        NXHandleEntry():
            value(),
            nextFreeIdx(0),
            counter(1),
            active(0),
            eol(0)
        {

        }

        explicit NXHandleEntry(const nx_u32 nxtFreeIdx):
            value(),
            nextFreeIdx(nxtFreeIdx),
            counter(1),
            active(0),
            eol(0)
        {

        }

        mutable value_type value;
        unsigned nextFreeIdx:12;
        unsigned counter: 15;
        unsigned active:1;
        unsigned eol: 1; // end of list
    };

protected:
    NXHandleEntry _entries[kMaxEntries];
    nx_u32 _nActive;
    nx_u32 _firstFreeIdx;
    const NXString _name;
};



}
#endif
