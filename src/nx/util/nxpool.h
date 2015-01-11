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
#ifndef __NX_POOL_H__
#define __NX_POOL_H__


namespace nx
{

template<class T>
class NXPool
{
public:

   enum
   {
       kMaxElements = 0xFFFE,
       kIndexInvalid = 0xFFFF
   };

   typedef T value_type;
   typedef T* value_ptr;
   typedef NXPool<T> class_type;
   typedef nx_u16 size_type;

protected:

    struct PoolItem
    {
        value_type item;
        size_type idx;
        size_type nxtFree;
    };

public:


   explicit NXPool(const size_type intialSize):
       _items(nullptr),
       _capacity(0),
       _count(0),
       _nextFreeIdx(0)
   {
        init(intialSize);
   }

    ~NXPool()
    {
       NXFree(_items);
    }

    value_ptr alloc()
    {
        // realloc if we need more storage
        if (!growIfNecessary())
        {
            return nullptr;
        }
        // get next free slot and update the index
        PoolItem& item = _items[_nextFreeIdx];
        _nextFreeIdx = item.nxtFree;
        ++_count;
        return &item.item;
    }

    void release(value_ptr pItem)
    {
        PoolItem* p_item = reinterpret_cast<PoolItem*>(pItem);
        // set next free to current released item
        p_item->nxtFree = _nextFreeIdx;
        _nextFreeIdx = p_item->idx;
        --_count;
    }

    size_type count() const
    {
        return _count;
    }

    size_type capacity() const
    {
        return _capacity;
    }

    void clear()
    {
        _count = 0;
        _nextFreeIdx = 0;

        // reset all next free indices
        size_type i;
        for(i = 0; i < _capacity - 1; ++i)
        {
            PoolItem& item = _items[i];
            item.idx = i;
            item.nxtFree = i + 1;
        }

        PoolItem& last_item = _items[i];
        last_item.idx = i;
        last_item.nxtFree = kIndexInvalid;
    }

protected:

    bool growIfNecessary()
    {
        if (_count == _capacity)
        {
            NXLogWarning("NXPool: Not enough memory to store all entries, reallocing with factor of 1.25");
            size_type new_capacity = _capacity * 1.25;

            // check for maximum cacpity or overflows
            if (new_capacity >= kMaxElements || new_capacity < _capacity)
            {
                NXLogWarning("NXPool: Reached maximum capacity");
                return false;
            }
            _items = static_cast<PoolItem*>(NXRealloc(_items, sizeof(PoolItem) * _capacity));
        }
        return true;
    }

    void init(const size_type size)
    {
        _capacity = size;
        _items = static_cast<PoolItem*>(NXMalloc(sizeof(PoolItem) * _capacity));
    }

private:
    NX_CPP_NO_COPY_TEMPLATE(NXPool, class_type);

protected:

    PoolItem* _items;
    size_type _capacity;
    size_type _count;
    size_type _nextFreeIdx;
};

}

#endif // NXPOOL_H
