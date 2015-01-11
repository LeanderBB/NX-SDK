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
#ifndef __NX_EVENTMANAGER_H__
#define __NX_EVENTMANAGER_H__

namespace nx
{

class NXEventListener;
class NXEventData;

class NXEventManager
{

public:
    typedef NXEventListener* ListenerType_t;
    typedef nx_u32 EventType_t;
public:

    NXEventManager();
    ~NXEventManager();

    /// Process all events in the queue, optinally specify a max time in
    /// miliseconds to be spent processing these events. Events that are not
    /// processed within the alloted time are added to the next queue
    void update(const nx_u64 maxDurationMiliSec = NX_U64_MAX);

    /// Queue an event to be processed on the next update cycle
    /// The event data will later be deleted by the event manager when the
    /// event has been processed
    void queue(const NXEventData* pData);

    /// Remove an event from the active event queue
    void abort(const EventType_t evtType);

    /// Trigger an event imediatilly, the even manager will not delete
    /// the event data
    void trigger(const NXEventData* pData);

    /// Register a new event
    bool addEvent(const EventType_t evtType);

    /// Remove an event and all the listeners associated to the event
    bool removeEvent(const EventType_t evtType);

    bool addListener(const EventType_t evtType,
                     const ListenerType_t listener);
    bool removeListener(const EventType_t evtType,
                        const ListenerType_t listener);
    void removeListeners(const EventType_t evtType);

    void clear();
private:
    NX_CPP_NO_COPY(NXEventManager);

protected:
    enum
    {
        kNumEvtQueues = 2
    };

    typedef std::vector<ListenerType_t> ListenerVec_t;
    typedef std::unordered_map<EventType_t, ListenerVec_t*> EvtMap_t;
    typedef std::deque<const NXEventData*> EvtQueue_t;

    EvtMap_t _evtMap;
    EvtQueue_t _evtQueues[kNumEvtQueues];
    int _activeQueue;
};

}
#endif
