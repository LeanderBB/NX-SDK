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

namespace tlref
{
template <class T>
struct NXTLReference
{
#if defined(NX_ENABLE_TLSHAREDPTR_CHECKS)
    nx_threadId_t threadid;
#endif
    nx_i32 refs = 0;
    nx_i32 refsWeak = 0;
    T* ptr = nullptr;
    char type[sizeof(T)];
};
}

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
    typedef value_type* pointer;
    typedef NXTLSharedPtr<T> class_type;
    typedef tlref::NXTLReference<T> Data;

    NXTLSharedPtr():
        _ptr(nullptr),
        _pData(nullptr)
    {

    }

    ~NXTLSharedPtr()
    {
        doReleaseStrong();
    }

    explicit NXTLSharedPtr(Data* pData):
        _ptr(nullptr),
        _pData(pData)
    {
        if(pData)
        {
            NX_TLSHAREDPTR_THREAD_CHECK(_pData);
            _ptr = pData->ptr;
            ++pData->refs;
        }
    }

    NXTLSharedPtr(const class_type& other):
        _ptr(other._ptr),
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
        _ptr(static_cast<pointer>(other.get())),
        _pData(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
        if(_pData)
        {
            ++_pData->refs;
        }
    }

    NXTLSharedPtr(class_type&& other):
        _ptr(other._ptr),
        _pData(other._pData)
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
        other._pData = nullptr;
    }

    template<class U>
    NXTLSharedPtr(NXTLSharedPtr<U>&& other):
        _ptr(static_cast<pointer>(other.get())),
        _pData(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
        other.__setNull();
    }

    NXTLSharedPtr& operator = (const class_type& other)
    {
        doReleaseStrong();
        _ptr = other._ptr;
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
        doReleaseStrong();
        _ptr = other._ptr;
        _pData = other._pData;
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
        other.__setNull();
        return *this;
    }

    template<class U>
    NXTLSharedPtr& operator = (const NXTLSharedPtr<U>& other)
    {
        doReleaseStrong();
        _ptr = static_cast<pointer>(other.get());
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
        doReleaseStrong();
        _pData = static_cast<Data*>(other.__getData());
        _ptr = static_cast<pointer>(other.get());
        NX_TLSHAREDPTR_THREAD_CHECK(_pData);
        other.__setNull();
        return *this;
    }

    nx_i32 use_count() const
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

    pointer get() const
    {
        return _ptr;
    }

    pointer operator ->() const
    {
        return _ptr;
    }

    operator bool () const
    {
        return _ptr != nullptr;
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
        doReleaseStrong();
        _ptr = nullptr;
        _pData = nullptr;
    }

private:
    void doReleaseStrong()
    {
        if (_pData)
        {
            NX_TLSHAREDPTR_THREAD_CHECK(_pData);
            nx_i32 ref = --_pData->refs;
            nx_i32 wref = _pData->refsWeak;
            if(ref == 0)
            {
                _ptr->~T();
                _ptr = nullptr;
                _pData->ptr = nullptr;
                if(!wref)
                {
                    NXFree(_pData);
                    _ptr = nullptr;
                    _pData = nullptr;
                }
            }
        }
    }

private:
    pointer _ptr;
    Data* _pData;
};

template <class T>
class NXTLWeakPtr
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef NXTLWeakPtr<T> class_type;
    typedef NXTLSharedPtr<T> shared_type;
    typedef tlref::NXTLReference<T> Data;


    NXTLWeakPtr():
        _data(nullptr)
    {
    }

    ~NXTLWeakPtr()
    {
        doReleaseWeak();
    }


    NXTLWeakPtr(const class_type& other):
        _data(other._data)
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if(_data)
        {
            ++_data->refsWeak;
        }
    }

    template<class U>
    NXTLWeakPtr(const NXTLWeakPtr<U>& other):
        _data(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if(_data)
        {
            ++_data->refsWeak;
        }
    }

    NXTLWeakPtr(class_type&& other):
        _data(_data)
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        other._data = nullptr;
    }

    template<class U>
    NXTLWeakPtr(const NXTLSharedPtr<U>& other):
        _data(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if (_data)
        {
            ++_data->refsWeak;
        }
    }

    template<class U>
    NXTLWeakPtr(NXTLWeakPtr<U>&& other):
        _data(static_cast<Data*>(other.__getData()))
    {
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        other.__setNull();
    }

    NXTLWeakPtr& operator = (const class_type& other)
    {
        doReleaseWeak();
        _data = other._data;
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if(_data)
        {
            ++_data->refsWeak;
        }
        return *this;
    }

    NXTLWeakPtr& operator = (class_type&& other)
    {
        doReleaseWeak();
        _data = other._data;
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        other.__setNull();
        return *this;
    }

    template<class U>
    NXTLWeakPtr& operator = (const NXTLWeakPtr<U>& other)
    {
        doReleaseWeak();
        _data = static_cast<Data*>(other.__getData());
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if(_data)
        {
            ++_data->refsWeak;
        }
        return *this;
    }

    template<class U>
    NXTLWeakPtr& operator = (const NXTLSharedPtr<U>& other)
    {
        doReleaseWeak();
        _data = static_cast<Data*>(other.__getData());
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        if(_data)
        {
            ++_data->refsWeak;
        }
        return *this;
    }


    template<class U>
    NXTLWeakPtr& operator = (NXTLWeakPtr<U>&& other)
    {
        doReleaseWeak();
        _data = static_cast<Data*>(other.__getData());
        NX_TLSHAREDPTR_THREAD_CHECK(_data);
        other.__setNull();
        return *this;
    }

    shared_type lock() const
    {
        return shared_type(_data);
    }

    bool expired() const
    {
        return !_data->refs;
    }

    nx_i32 use_count() const
    {
        return _data->refsWeak;
    }

    void* __getData() const
    {
        return _data;
    }

    void __setNull()
    {
        _data = nullptr;
    }

private:
    void doReleaseWeak()
    {
        if(_data)
        {
            NX_TLSHAREDPTR_THREAD_CHECK(_data);
            nx_i32 ref = _data->refs;
            nx_i32 wref = --_data->refsWeak;
            if(wref == 0)
            {
                if(!ref)
                {
                    NXFree(_data);
                    _data = nullptr;
                }
            }
        }
    }

private:
    Data* _data;
};


template<class T, class... Args>
NXTLSharedPtr<T> nxMakeTLShared(Args&&... args)
{
    typename NXTLSharedPtr<T>::Data *ptr = static_cast<typename NXTLSharedPtr<T>::Data*>(NXMalloc(sizeof(typename NXTLSharedPtr<T>::Data)));
    new ((void*)&ptr->type) T(std::forward<Args>(args)...);
    ptr->ptr = reinterpret_cast<T*>(&ptr->type);
    ptr->refs = 0;
    ptr->refsWeak = 0;
#if defined(NX_ENABLE_TLSHAREDPTR_CHECKS)
    ptr->threadid = nx::NXThread::currentThreadId();
#endif
    return NXTLSharedPtr<T>(ptr);
}


}

#endif
