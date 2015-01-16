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
#ifndef __NX_CPP_H__
#define __NX_CPP_H__

#include <vector>
#include <list>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <limits>
#include <stack>
#include <set>
#include <atomic>
#include <string>

#define NX_CPP_OVERRIDE override


#define NX_CPP_NO_COPY(C) \
    C& operator = (const C&);\
    C (const C&); \
    C& operator = (C&&); \
    C (C&&)

#define NX_CPP_NO_COPY_TEMPLATE(C, T) \
    T& operator = (const T&);\
    C (const T&); \
    T& operator = (T&&); \
    C (T&&)

#define NX_SAFE_DELETE(p) delete p; p = nullptr;

#define NX_STATIC_ASSERT(exp, str) static_assert((exp), str);


#define NX_INTERFACE_BEGIN(cl) \
class cl { \
    public: \
        virtual ~cl() {} \
    protected: \
        cl(){} \
    public: \

#define NX_INTERFACE_END() };


namespace nx
{
// Use std::string as the current string implementation
typedef std::string NXString;
typedef std::forward_list<NXString> StringList_t;

/// NXCntrPtr is a class to automatically delete ptrs contained within a
/// container.
/// @Note: do not use this class outside containers
template<class T>
class NXCntrPtr
{
public:
    typedef T value_type;
    typedef T* value_ptr;
    typedef const T* const_value_ptr;
    typedef NXCntrPtr<T> class_type;
    typedef T& reference;

    NXCntrPtr():
        _val(nullptr)
    {

    }

    NXCntrPtr(value_ptr val):
        _val(val)
    {

    }

    NXCntrPtr(value_ptr&& val):
        _val(val)
    {
        val = nullptr;
    }

    ~NXCntrPtr()
    {
        if(_val) delete _val;
    }


    NXCntrPtr(const class_type& cl):
        _val(cl._val)
    {
    }

    NXCntrPtr(class_type&& cl):
        _val(cl._val)
    {
        cl._val = nullptr;
    }

    class_type& operator = (const class_type& cl)
    {
        _val = cl._val;
        return *this;
    }

    class_type& operator = (class_type&& cl)
    {
        _val = cl._val;
        cl._val = nullptr;
        return *this;
    }

    class_type& operator = (const value_ptr val)
    {
        _val = val;
        return *this;
    }

    class_type& operator = (value_ptr&& val)
    {
        _val = val;
        val = nullptr;
        return *this;
    }

    reference operator *()
    {
        return _val;
    }

    const reference operator *() const
    {
        return *_val;
    }

    value_ptr operator ->()
    {
        return _val;
    }

    const_value_ptr operator ->() const
    {
        return _val;
    }

    operator bool() const
    {
        return _val != nullptr;
    }

    bool operator == (const class_type& cl) const
    {
        return _val == cl._val;
    }

    bool operator != (const class_type& cl) const
    {
        return _val != cl._val;
    }

    value_ptr get()
    {
        return _val;
    }

    const_value_ptr get() const
    {
        return _val;
    }


    value_ptr _val;
};

}

// new operators
void* operator new ( size_t s );
void* operator new[] ( size_t s );

// delete operators
#if defined(_LIBCPP_VERSION)
#define NX_CPP_DELETE_NOEXCEPT _NOEXCEPT
#elif defined(__GLIBCXX__)
#define NX_CPP_DELETE_NOEXCEPT _GLIBCXX_USE_NOEXCEPT
#else
#define NX_CPP_DELETE_NOEXCEPT
#endif
void operator delete ( void* p ) NX_CPP_DELETE_NOEXCEPT;
void operator delete[] ( void* p ) NX_CPP_DELETE_NOEXCEPT;

#endif
