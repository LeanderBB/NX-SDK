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
#ifndef __NX_TLSHAREDPTR_H__
#define __NX_TLSHAREDPTR_H__

#include "nx/nxcore.h"
#include "nx/os/nxthread.h"

#if defined(NX_DEBUG) || defined(NX_CONFIG_ENABLE_TLSHAREDPTR_CHECKS)
#define NX_ENABLE_TLSHAREDPTR_CHECKS
#endif

#if defined(NX_ENABLE_TLSHAREDPTR_CHECKS)
#define NX_TLSHAREDPTR_THREAD_CHECK(data) \
    if(data) {\
    NX_ASSERT(data->threadid == NXThread::currentThreadId()); \
    if(data->threadid != NXThread::currentThreadId()) \
{ \
    NXLogFatal("NXTLSharedPtr: Sharing pointers between different threads!"); \
    }}
#else
#define NX_TLSHAREDPTR_THREAD_CHECK(data)
#endif


namespace nx
{


/**
 * Thread Local Shared Ptr implementation. It behaves exactly as std::shared_ptr
 * except that it does not include atomic syncronization primitives since it
 * it expected to be used only in the same thread.
 *
 * When NX_ENABLE_TLSHAREDPTR_CHECKS is enabled, additionall checks will be
 * made to ensure the shared ptr is called on the same thread it was created.
 */
template<class T>
class NXTLSharedPtr
{
public:
    typedef T value_type;
    typedef T& value_ref;
    typedef T* value_ptr;
    typedef NXTLSharedPtr<T> class_type;

    typedef struct
    {
#if defined(NX_ENABLE_TLSHAREDPTR_CHECKS)
        nx_threadId_t threadid;
#endif
        nx_i32 refs;
        char type[sizeof(value_type)];
    }Data;


    NXTLSharedPtr():
        _pData(nullptr)
    {

    }

    ~NXTLSharedPtr()
    {
        doRelease();
    }

    explicit NXTLSharedPtr(Data* pData):
        _pData(pData)
    {
        if(pData)
        {
            NX_TLSHAREDPTR_THREAD_CHECK(_pData);
            ++pData->refs;
        }
    }

    NXTLSharedPtr(const class_type& other):
        _pData(other._pData)
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                if(_pData)
        {
            ++_pData->refs;
        }
    }

    template<class U>
    NXTLSharedPtr(const NXTLSharedPtr<U>& other):
        _pData(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                if(_pData)
        {
            ++_pData->refs;
        }
    }

    NXTLSharedPtr(class_type&& other):
        _pData(other._pData)
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                other._pData = nullptr;
    }

    template<class U>
    NXTLSharedPtr(NXTLSharedPtr<U>&& other):
        _pData(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                other.__setNull();
    }

    NXTLSharedPtr& operator = (const class_type& other)
    {
        doRelease();
        _pData = other._pData;
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);

                if(_pData)
        {
            ++_pData->refs;
        }
        return *this;
    }

    NXTLSharedPtr& operator = (class_type&& other)
    {
        doRelease();
        _pData = other._pData;
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                other.__setNull();
        return *this;
    }

    template<class U>
    NXTLSharedPtr& operator = (const NXTLSharedPtr<U>& other)
    {
        doRelease();
        _pData = static_cast<Data*>(other.__getData());
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);

                if(_pData)
        {
            ++_pData->refs;
        }
        return *this;
    }

    template<class U>
    NXTLSharedPtr& operator = (NXTLSharedPtr<U>&& other)
    {
        doRelease();
        _pData = static_cast<Data*>(other.__getData());
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
                other.__setNull();
        return *this;
    }

    nx_i32 refs() const
    {
        return (_pData) ? _pData->refs : 0;
    }

    void* __getData() const
    {
        return _pData;
    }

    void __setNull()
    {
        _pData = nullptr;
    }

    value_ptr get() const
    {
        return (_pData) ? reinterpret_cast<value_ptr>(_pData->type) : nullptr;
    }

    value_ptr operator ->() const
    {
        return reinterpret_cast<value_ptr>(_pData->type);
    }

    operator bool () const
    {
        return _pData != nullptr;
    }

    bool operator == (const class_type& other) const
    {
        return (other._pData == _pData);
    }

    bool operator != (const class_type& other) const
    {
        return (other._pData != _pData);
    }

    void reset()
    {
        doRelease();
        _pData = nullptr;
    }

private:
    void doRelease()
    {

        if (_pData)
        {
            NX_TLSHAREDPTR_THREAD_CHECK(_pData);
            nx_i32 ref = _pData->refs;
            if(--ref == 0)
            {
                value_ptr ptr = get();
                ptr->~T();
                NXFree(_pData);
                _pData = nullptr;
            }
            else
            {
                _pData->refs = ref;
            }
        }
    }

private:
    Data* _pData;
};



template<class T, class... Args>
NXTLSharedPtr<T> nxMakeTLShared(Args&&... args)
{
    typename NXTLSharedPtr<T>::Data *ptr = static_cast<typename NXTLSharedPtr<T>::Data*>(NXMalloc(sizeof(typename NXTLSharedPtr<T>::Data)));
    ptr->refs = 0;
    new ((void*)&ptr->type) T(std::forward<Args>(args)...);
#if defined(NX_ENABLE_TLSHAREDPTR_CHECKS)
    ptr->threadid = nx::NXThread::currentThreadId();
#endif
    return NXTLSharedPtr<T>(ptr);
}


}

#endif
