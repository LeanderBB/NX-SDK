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
#ifndef __NX_MPMCQUEUE_H__
#define __NX_MPMCQUEUE_H__

#define NX_CACHE_LINE_SIZE 64

namespace nx
{

/// Lockfree multi producer mutil consumer queue. Based on the article
/// http://www.drdobbs.com/parallel/writing-a-generalized-concurrent-queue/211601363?pgno=2
template <class T>
class NXMPMCQueue
{
public:
    typedef T* value_ptr;

    explicit NXMPMCQueue()
    {
        _pFirst = _pLast = new NXQueueNode(nullptr);
        _producerLock = _consumerLock = false;
    }


    ~NXMPMCQueue()
    {
        while( _pFirst != nullptr )
        {
            NXQueueNode* tmp = _pFirst;
            _pFirst = tmp->next;
            delete tmp->value;
            delete tmp;
        }
    }


    void queue(const value_ptr val)
    {
        NXQueueNode* tmp = new NXQueueNode( val );
        while (_producerLock.exchange(true)) {} // acquire exclusivity
        _pLast->next = tmp;       // publish to consumers
        _pLast = tmp;             // swing last forward
        _producerLock = false;     // release exclusivity
    }


    /// Returns nullptr if the queue was empty, value ptr needs to be release
    /// by the callee
    value_ptr dequeue()
    {
        value_ptr result = nullptr;
        while (_consumerLock.exchange(true)) { } // acquire exclusivity
        NXQueueNode* theFirst = _pFirst;
        NXQueueNode* theNext = _pFirst-> next;
        if( theNext != nullptr )
        {                                // if queue is nonempty
            value_ptr val = theNext->value;  // take it out
            theNext->value = nullptr;       // of the Node
            _pFirst = theNext;             // swing first forward
            _consumerLock = false;        // release exclusivity
            result = *val;               // now copy it back
            delete theFirst;             // and the old dummy
            return true;                 // and report success
        }
        _consumerLock = false;        // release exclusivity
        return result;
    }


private:
    NX_CPP_NO_COPY(NXMPMCQueue);

private:

    class NXQueueNode
    {
    public:
        NXQueueNode(value_ptr val):
            value(val), next(nullptr)
        {
        }
        value_ptr value;
        std::atomic<NXQueueNode*> next;
        char pad[NX_CACHE_LINE_SIZE - sizeof(value_ptr)- sizeof(std::atomic<NXQueueNode*>)];
    };

private:
    char pad0[NX_CACHE_LINE_SIZE];

    // for one consumer at a time
    NXQueueNode* _pFirst;

    char pad1[NX_CACHE_LINE_SIZE - sizeof(NXQueueNode*)];

    // shared among consumers
    std::atomic<bool> _consumerLock;

    char pad2[NX_CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];

    // for one producer at a time
    NXQueueNode* _pLast;

    char pad3[NX_CACHE_LINE_SIZE - sizeof(NXQueueNode*)];

    // shared among producers
    std::atomic<bool> _producerLock;

    char pad4[NX_CACHE_LINE_SIZE - sizeof(std::atomic<bool>)];
};

}

#endif

