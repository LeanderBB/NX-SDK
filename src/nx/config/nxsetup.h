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

#ifndef __NX_SETUP_H__
#define __NX_SETUP_H__

#include <nx/nxconfig.h>

#define NX_VERSION (NX_VERSION_MAJOR * 1000) + (NX_VERSION_MINOR * 10) + \
  NX_VERSION_PATCH


// ----------------------------------------------------------------------------
// HOST OS
// ----------------------------------------------------------------------------

#if defined(__ANDROID__) || defined(ANDROID)
  #define NX_OS_UNIX
  #define NX_OS_ANDROID
  #define NX_TARGET_ANDROID
#elif defined(__linux) || defined(__linux__)
  #define NX_OS_UNIX
  #define NX_OS_LINUX
  #define NX_TARGET_LINUX
#elif defined(__APPLE__)
  #define NX_OS_UNIX
  #define NX_OS_APPLE
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
  #if defined(__IPHONE_OS__)
    #define NX_TARGET_IOS
    #if defined(TARGET_OS_IPHONE)
      #define NX_IOS_DEVICE
    #else
      #define NX_IOS_SIMULATOR
    #endif
  #else
    #define NX_TARGET_MAC
  #endif
#elif defined(WIN32) || defined(_WIN32)
  #define NX_OS_WIN
  #define NX_TARGET_WIN
#else
  #error Unknown Operating System
#endif


// ----------------------------------------------------------------------------
// Compiler
// ----------------------------------------------------------------------------

#if defined(__clang__)
  #define NX_COMPILER_CLANG
  #if defined(NX_OS_APPLE)
    #define NX_COMPILER_NAME "Apple Clang/LLVM"
  #else
    #define NX_COMPILER_NAME "Clang/LLVM"
  #endif
  #define NX_COMPILER_VERSION (((__clang_major__)*100) + \
(__clang_minor__*10))
  #define NX_COMPILER_VERSION_MAJOR __clang_major__
  #define NX_COMPILER_VERSION_MINOR __clang_minor__
#elif defined(__GNUC__)
  #define NX_COMPILER_GNUC
  #define NX_COMPILER_NAME "GNUC"
  #define NX_COMPILER_VERSION (__GNUC__ * 100 + __GNUC_MINOR__ * 10)
  #define NX_COMPILER_VERSION_MAJOR __GNUC__
  #define NX_COMPILER_VERSION_MINOR __GNUC_MINOR__
#elif defined(_MSC_VER)
  #define NX_COMPILER_MSC
  #define NX_COMPILER_NAME "MSC"
  #define NX_COMPILER_VERSION _MSC_VER
  #define NX_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
  #define NX_COMPILER_VERSION_MINOR (_MSC_VER % 100)
#else
  #error "Unknown compiler detected!"
#endif

#if defined(NX_COMPILER_CLANG) || defined(NX_COMPILER_GNUC)
  #define NX_CALL
  #define NX_EXPORT __attribute__ ((visibility("default")))
  #define NX_INLINE __inline__
  #define NX_FORCE_INLINE __attribute__((always_inline)) static __inline__
#elif defined(NX_COMPILER_MSC)
  #define NX_CALL __cdecl
  #define NX_EXPORT __declspec(dllexport)
  #define NX_INLINE __inline
  #define NX_FORCE_INLINE __forceinline
#else
  #error Missing calling convetions for current compiler
#endif

// ----------------------------------------------------------------------------
// CPU ARCH
// ----------------------------------------------------------------------------

#if !defined(NX_CPU_BE)
#define NX_CPU_LE
#endif

#if defined(NX_CPU_BE)
#define NX_REQUIRE_BSWAP
#endif

/* use builtin swapping functions when possilbe */
#if __GNUC__ >=4 && __GNUC_MINOR__ >= 1
  #define NX_BSWAP16 __builtin_bswap16
  #define NX_BSWAP32 __builtin_bswap32
  #define NX_BSWAP64 __builtin_bswap64
#elif defined(_MSC_VER)
  #define NX_BSWAP16 _byteswap_ushort
  #define NX_BSWAP32 _byteswap_ulong
  #define NX_BSWAP64 _byteswap_uint64
#else
static NX_INLINE NX_BSWAP16(uint16_t x)
{
  return (( v & 0x00ff) << 8) | ((v & 0xff00) >> 8);
}

static NX_INLINE NX_BSWAP32(uint32_t x)
{
  return ((x << 24) & 0xff000000 ) |
      ((x <<  8) & 0x00ff0000 ) |
      ((x >>  8) & 0x0000ff00 ) |
      ((x >> 24) & 0x000000ff );
}

static NX_INLINE NX_BSWAP64(uint64_t v)
{
  return (((v & 0xff00000000000000ull) >> 56) |
          ((v & 0x00ff000000000000ull) >> 40) |
          ((v & 0x0000ff0000000000ull /* __NX_SETUP_H__ */) >> 24) |
          ((v & 0x000000ff00000000ull) >> 8) |
          ((v & 0x00000000ff000000ull) << 8) |
          ((v & 0x0000000000ff0000ull) << 24) |
          ((v & 0x000000000000ff00ull) << 40) |
          ((v & 0x00000000000000ffull) << 56));
}
#endif

#if defined(NX_REQUIRE_BSWAP)
#define NX_LITTLE_E16(x) NX_BSWAP16(x)
#define NX_LITTLE_E32(x) NX_BSWAP32(x)
#define NX_LITTLE_E64(x) NX_BSWAP64(x)
#else
#define NX_LITTLE_E16(x) x
#define NX_LITTLE_E32(x) x
#define NX_LITTLE_E64(x) x
#endif

#if defined(__x86_64) || defined(__amd64) || defined (__LP64__) || defined(_WIN64)
#define NX_CPU_X86
#define NX_CPU_64BIT
#elif defined(__i386__) || (__intel__) || defined(_WIN32)
#define NX_CPU_X86
#define NX_CPU_32BIT
#elif defined(__aarch64__) // arm 64
#define YAAF_CPU_ARM
#define NX_CPU_64BIT
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) \
    || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__) || defined(__ARM_ARCH_6__) \
    || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6ZK__) || defined(_M_ARM)
#define NX_CPU_ARM
#define NX_CPU_32BIT
#else
#error "Unknown Processor."
#endif

// ----------------------------------------------------------------------------
// Debug
// ----------------------------------------------------------------------------
#if defined(NX_DEBUG)
#define NX_ENABLE_ASSERT
#endif

#if !defined(NX_DEBUG_BREAK)
    #if (defined(NX_COMPILER_GNUC) || defined(NX_COMPILER_CLANG)) && \
        (defined(NX_CPU_X86) || defined(NX_CPU_X86_64))
        #define NX_DEBUG_BREAK() asm("int3")
    #else
        #define NX_DEBUG_BREAK() raise(SIGTRAP)
    #endif
#endif

#if defined(NX_ENABLE_ASSERT)
#include <assert.h>
#define NX_ASSERT(x) assert(x)
#else
#define NX_ASSERT(x)
#endif

#define NX_ASSERT_UNREACHABLE() NX_ASSERT(false && "This locations should not be reached")


#if defined(NX_CONFIG_MEMORY_TRACK_ALLOCATIONS)
#define NX_MEMORY_TRACK_ALLOCATIONS
#endif

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#if defined(NX_HAVE_MALLOC_H)
#include <malloc.h>
#endif
#include <string.h>
#include <limits.h>
#if defined(NX_HAVE_STRINGS_H)
#include <strings.h>
#endif

#if defined(NX_OS_UNIX)
#include <sys/time.h>
#include <sys/param.h>
#include <unistd.h>
#else
#include <time.h>
#endif

#if defined(NX_HAVE_INTTYPES_H)
#include <inttypes.h>
#else
typedef char int8_t;
typedef unsigned char uint8_t; /* __NX_SETUP_H__ */
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif


// ----------------------------------------------------------------------------
// Integer Types
// ----------------------------------------------------------------------------
typedef int8_t nx_i8;
typedef uint8_t nx_u8;
typedef int16_t nx_i16;
typedef uint16_t nx_u16;
typedef int32_t nx_i32;
typedef uint32_t nx_u32;
typedef int64_t nx_i64;
typedef uint64_t nx_u64;

#if defined(NX_CPU_64BIT)
typedef int64_t nx_ssize_t;
#else
typedef int32_t nx_ssize_t;
#endif

typedef nx_u16 nx_f16;

// ----------------------------------------------------------------------------
// Time tipes
// ----------------------------------------------------------------------------
typedef nx_u64 nx_timems;
typedef double nx_timesec;
typedef nx_u64 nx_timemic;

// ----------------------------------------------------------------------------
// Atomic types
// ----------------------------------------------------------------------------
typedef volatile nx_i32 nx_atomic32_t;
typedef volatile nx_i64 nx_atomic64_t;
typedef void* volatile nx_atomicptr_t;

#if defined(NX_CPU_64BIT)
typedef nx_atomic64_t nx_atomicsize_t;
#else
typedef nx_atomic32_t nx_atomicsize_t;
#endif

// ----------------------------------------------------------------------------
// Other types
// ----------------------------------------------------------------------------
typedef void* nx_ptr;
typedef unsigned long nx_threadId_t;
typedef unsigned int nx_tlsId_t;

// ----------------------------------------------------------------------------
// Limits
// ----------------------------------------------------------------------------

#define NX_PRIu8 PRIu8
#define NX_PRIi8 PRIi8
#define NX_PRIu16 PRIu16
#define NX_PRIi16 PRIi16
#define NX_PRIu32 PRIu32
#define NX_PRIi32 PRIi32
#define NX_PRIu64 PRIu64
#define NX_PRIi64 PRIi64
#if defined(NX_CPU_64BIT)
#define NX_PRIsize "lu"
#define NX_PRIisize "ld"
#else
#define NX_PRIsize PRIu32
#define NX_PRIisize PRIi32
#endif
#define NX_SIZE_T_MAX SIZE_MAX
#define NX_U64_MAX UINT64_MAX
#define NX_U32_MAX UINT32_MAX
#define NX_U16_MAX UINT16_MAX
#define NX_U8_MAX UINT8_MAX
#define NX_I64_MAX INT64_MAX
#define NX_I64_MIN INT64_MIN
#define NX_I32_MIN INT32_MIN
#define NX_I32_MAX INT32_MAX
#define NX_I16_MIN INT16_MIN
#define NX_I16_MAX INT16_MAX
#define NX_I8_MIN INT8_MIN
#define NX_I8_MAX INT8_MAX
#define NX_FLOAT_MIN FLT_MIN
#define NX_FLOAT_MAX FLT_MAX
#define NX_DOUBLE_MAX DBL_MAX
#define NX_DOUBLE_MIN DBL_MIN

// ----------------------------------------------------------------------------
// Index not found
// ----------------------------------------------------------------------------
#define NX_INDEX_INVALID32 UINT32_MAX
#define NX_INDEX_INVALID64 UINT64_MAX

// ----------------------------------------------------------------------------
// Templates
// ----------------------------------------------------------------------------
#define NX_TPL_CONCAT_AUX(A, B) A ## _ ## B
#define NX_TPL_CONCAT(A, B) NX_TPL_CONCAT_AUX(A, B)

#endif
