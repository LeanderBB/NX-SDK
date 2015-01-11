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
#ifndef __NX_ATOMIC_H__
#define __NX_ATOMIC_H__

#if defined(NX_OS_APPLE)
#include <libkern/OSAtomic.h>
#endif

namespace nx
{
// ----------------------------------------------------------------------------
// Collection of atomic operations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
NX_INLINE nx_i32
NXAtomicInc32(volatile nx_i32* ptr)
{
#if defined(NX_OS_APPLE)
return OSAtomicIncrement32(ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_add_and_fetch(ptr, 1);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE nx_i32
NXAtomicDec32(volatile nx_i32* ptr)
{
#if defined(NX_OS_APPLE)
return OSAtomicDecrement32(ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_sub_and_fetch(ptr, 1);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE nx_i64
NXAtomicInc64(volatile nx_i64* ptr)
{
#if defined(NX_OS_APPLE)
return OSAtomicIncrement64(ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_add_and_fetch(ptr, 1);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE nx_i64
NXAtomicDec64(volatile nx_i64* ptr)
{
#if defined(NX_OS_APPLE)
return OSAtomicDecrement64(ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_sub_and_fetch(ptr, 1);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE nx_i32
NXAtomicAdd32(volatile nx_i32* ptr, const nx_i32 v)
{
#if defined(NX_OS_APPLE)
return OSAtomicAdd32(v, ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_add_and_fetch(ptr, v);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE nx_i64
NXAtomicAdd64(volatile nx_i64* ptr, const nx_i64 v)
{
#if defined(NX_OS_APPLE)
return OSAtomicAdd64(v, ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_add_and_fetch(ptr, v);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE bool
NXAtomicCAS32(volatile nx_i32* ptr, const nx_i32 oldVal, const nx_i32 newVal)
{
#if defined(NX_OS_APPLE)
return OSAtomicCompareAndSwap32(oldVal, newVal, ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_bool_compare_and_swap(ptr, oldVal, newVal);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE bool
NXAtomicCAS64(volatile nx_i64* ptr, const nx_i64 oldVal, const nx_i64 newVal)
{
#if defined(NX_OS_APPLE)
return OSAtomicCompareAndSwap64(oldVal, newVal, ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_bool_compare_and_swap(ptr, oldVal, newVal);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE bool
NXAtomicCASPtr(void * volatile* ptr, void* oldVal, void* newVal)
{
#if defined(NX_OS_APPLE)
return OSAtomicCompareAndSwapPtr(oldVal, newVal, ptr);
#elif defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)
return __sync_bool_compare_and_swap(ptr, oldVal, newVal);
#else
#error "Atomic operation not defined on this platform!"
#endif
}
// ----------------------------------------------------------------------------
NX_INLINE void
NXAtomicSet32(nx_atomic32_t* ptr, nx_i32 newVal)
{
volatile nx_i32 cur_val;
do {
cur_val = *ptr;
} while (!NXAtomicCAS32(ptr, cur_val, newVal));
}
// ----------------------------------------------------------------------------
NX_INLINE void
NXAtomicSet64(nx_atomic64_t* ptr, nx_i64 newVal)
{
volatile nx_i64 cur_val;
do {
cur_val = *ptr;
} while (!NXAtomicCAS64(ptr, cur_val, newVal));
}
// ----------------------------------------------------------------------------
NX_INLINE void
NXAtomicSetPtr(nx_atomicptr_t* ptr, void* newVal)
{
volatile void* cur_val;
do {
cur_val = ptr;
} while (!NXAtomicCASPtr(ptr, (void*)cur_val, newVal));
}
// ----------------------------------------------------------------------------
#if defined(NX_CPU_64BIT)
#define NXAtomicCASSize(ptr, oldVal, newVal) NXAtomicAdd64(ptr, oldVal, newVal)
#define NXAtomicAddSize(ptr, val) NXAtomicAdd64(ptr, val)
#define NXAtomicSetSize(ptr, newVal) NXAtomicSet64(ptr, newVal)
#else
#define NXAtomicCASSize(ptr, oldVal, newVal) NXAtomicAdd32(ptr, oldVal, newVal)
#define NXAtomicAddSize(ptr, val) NXAtomicAdd32(ptr, val)
#define NXAtomicSetSize(ptr, newVal) NXAtomicSet32(ptr, newVal)
#endif
// ----------------------------------------------------------------------------


}

#endif
