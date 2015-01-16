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
#include "nx/event/nxeventmanager.h"
#include "nx/event/nxeventdata.h"
#include "nx/event/nxeventlistener.h"

namespace nx
{
NXEventManager::NXEventManager() :
    _evtMap(),
    _evtQueues(),
    _activeQueue(0)
{

}

NXEventManager::~NXEventManager()
{
    clear();
}


void
NXEventManager::update(const nx_u64 maxDurationMiliSec)
{
    nx_u64 cur_time = nxTimeInMiliSeconds();
    nx_u64 expired_time = (maxDurationMiliSec == NX_U64_MAX) ? NX_U64_MAX : (cur_time + maxDurationMiliSec);
    int cur_queue = _activeQueue;
    _activeQueue = (cur_queue + 1) % kNumEvtQueues;

    // trigger all events
    while(!_evtQueues[cur_queue].empty())
    {
        const NXEventData* p_evt = _evtQueues[cur_queue].front();
        this->trigger(p_evt);
        _evtQueues[cur_queue].pop_front();
        // cleanup memory
        delete p_evt;
        cur_time = nxTimeInMiliSeconds();
        if (cur_time > expired_time)
        {
            NXLogWarning("EvtentManager: Could not process all events in allotted time. Remaining events will be added to the queue");
            break;
        }
    }
    // add remaining events to the next queueu
    while (!_evtQueues[cur_queue].empty())
    {
        _evtQueues[_activeQueue].push_back(_evtQueues[cur_queue].front());
        _evtQueues[cur_queue].pop_front();
    }
}


void
NXEventManager::queue(const NXEventData* pData)
{
    _evtQueues[_activeQueue].push_back(pData);
}


void
NXEventManager::abort(const EventType_t evtType)
{
    auto it = _evtQueues[_activeQueue].begin();
    auto end = _evtQueues[_activeQueue].end();
    while(it != end)
    {
        if ((*it)->type == evtType)
        {
            // free memory
            delete *it;
            _evtQueues[_activeQueue].erase(it--);
        }
        else
        {
            ++it;
        }
    }
}

void
NXEventManager::trigger(const NXEventData* pData)
{
    if (pData)
    {
        auto it = _evtMap.find(pData->type);
        if (it != _evtMap.end())
        {
            ListenerVec_t* p_listeners = it->second;
            auto lit = p_listeners->begin();
            auto lit_end = p_listeners->end();
            bool quit_cycle = false;
            for (; lit != lit_end && !quit_cycle; ++lit)
            {
                quit_cycle = (*lit)->handleEvent(pData);
            }
        }
    }
}


bool
NXEventManager::addEvent(const EventType_t evtType)
{
    bool result = false;
    auto it = _evtMap.find(evtType);
    if (it == _evtMap.end())
    {
        _evtMap.insert(EvtMap_t::value_type(evtType, new ListenerVec_t()));
        result = true;
    }
    return result;
}


bool
NXEventManager::removeEvent(const EventType_t evtType)
{
    bool result = false;
    auto it = _evtMap.find(evtType);
    if (it != _evtMap.end())
    {
        delete it->second;
        _evtMap.erase(it);
        result = true;
    }
    return result;
}

bool
NXEventManager::addListener(const EventType_t evtType,
                            const ListenerType_t listener)
{
    bool result = false;
    auto it = _evtMap.find(evtType);
    if (it != _evtMap.end())
    {
        auto find_it = std::find(it->second->begin(),it->second->end(), listener);
        if (find_it == it->second->end())
        {
            it->second->push_back(listener);
            result = true;
        }
        else
        {
            NXLogWarning("EventManager: Adding the same listener twice for event %X", evtType);
        }
    }
    return result;
}

bool
NXEventManager::removeListener(const EventType_t evtType,
                               const ListenerType_t listener)
{
    bool result = false;
    auto it = _evtMap.find(evtType);
    if (it != _evtMap.end())
    {
        auto find_it = std::find(it->second->begin(),it->second->end(), listener);
        if (find_it != it->second->end())
        {
            it->second->erase(find_it);
            result = true;
        }
    }
    return result;
}

void
NXEventManager::removeListeners(const EventType_t evtType)
{
    auto it = _evtMap.find(evtType);
    if (it != _evtMap.end())
    {
        it->second->clear();
    }
}




void
NXEventManager::clear()
{

    struct
    {
        void operator()(EvtQueue_t::value_type& v)
        {
            delete v;
        }

        void operator()(EvtMap_t::value_type& v)
        {
            delete v.second;
        }
    } destroy;

    // Clear queues
    for (int i = 0; i < kNumEvtQueues ; ++i)
    {
        std::for_each(_evtQueues[i].begin(), _evtQueues[i].end(), destroy);
        _evtQueues[i].clear();
    }

    // clear subscrivers
    std::for_each(_evtMap.begin(), _evtMap.end(), destroy);
    _evtMap.clear();
}

}
