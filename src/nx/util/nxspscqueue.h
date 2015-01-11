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
#ifndef __NX_SPSCQUEUE_H__
#define __NX_SPSCQUEUE_H__

namespace nx
{

/// Lockfree single producer single consumer queue. Based on the article
/// http://kjellkod.wordpress.com/2012/11/28/c-debt-paid-in-full-wait-free-lock-free-queue/
template <class T, nx_u32 queue_size>
class NXSPSCQueue
{
public:
    typedef T value_type;
    typedef T* value_ptr;

    explicit NXSPSCQueue():
        _size(queue_size),
        _entries(),
        _readIdx(0),
        _writeIdx(0)
    {
        memset(_entries, 0, sizeof(value_ptr) * queue_size);
    }


    ~NXSPSCQueue()
    {
        int readIdx = _readIdx;
        int endIdx = _writeIdx;

        while (readIdx != endIdx)
        {
            delete _entries[readIdx];
            readIdx = (readIdx + 1) % _size;
        }
    }

    value_ptr dequeue()
    {
        value_ptr entry = nullptr;
        auto const curReadIdx = _readIdx.load(std::memory_order_relaxed);
        if( curReadIdx != _writeIdx.load(std::memory_order_acquire))
        {
            auto nextIdx = (curReadIdx + 1) % _size;
            entry = _entries[curReadIdx];
            _entries[curReadIdx] = nullptr;
            _readIdx.store(nextIdx, std::memory_order_release);
        }
        return entry;

    }

    bool queue(const value_ptr entry)
    {
        auto const curWriteIdx = _writeIdx.load(std::memory_order_relaxed);
        auto nextIdx = (curWriteIdx + 1) % _size;

        if (nextIdx != _readIdx.load(std::memory_order_acquire))
        {
            _entries[curWriteIdx] = entry;
            _writeIdx.store(nextIdx, std::memory_order_release);
            return true;
        }

        return false;
    }

    bool empty() const
    {
        return _readIdx.load(std::memory_order_consume) ==
                _writeIdx.load(std::memory_order_consume);
    }

private:
    NX_CPP_NO_COPY(NXSPSCQueue);

private:
    const nx_u32 _size;
    const T* _entries[queue_size];
    std::atomic<int> _readIdx;
    std::atomic<int> _writeIdx;
};

}

#endif
