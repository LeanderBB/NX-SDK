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


#ifndef __NX_CONFIG_H__
#define __NX_CONFIG_H__

// Config file generated by CMake, do not modify.

// version info

#define NX_CORE_VERSION_MAJ 0
#define NX_CORE_VERSION_MIN 1
#define NX_CORE_VERSION_REV 0


// Have the strings.h include file
#cmakedefine NX_HAVE_STRINGS_H @NX_HAVE_STRINGS_H@

// have the strcasecmp function
#cmakedefine NX_HAVE_STRCASECMP @NX_HAVE_STRCASECMP@

// have the stricmp function
#cmakedefine NX_HAVE_STRICMP @NX_HAVE_STRICMP@

// have the dirent function
#cmakedefine NX_HAVE_DIRENT_H @NX_HAVE_DIRENT_H@

// have the inttype type
#cmakedefine NX_HAVE_INTTYPES_H @NX_HAVE_INTTYPES_H@

// sse flags
#cmakedefine NX_HAVE_SSE2 @NX_HAVE_SSE2@
#cmakedefine NX_HAVE_SSE2 @NX_HAVE_SSE4@
#cmakedefine NX_HAVE_SSE2 @NX_HAVE_SSE41@
#cmakedefine NX_HAVE_SSE2 @NX_HAVE_SSE42@

// neon flags
#cmakedefine NX_HAVE_NEON @NX_HAVE_NEON@

// system interface
#cmakedefine NX_SYSTEM_SDL2 @NX_SYSTEM_SDL2@
#cmakedefine NX_SYSTEM_ANDROID @NX_SYSTEM_ANDROID@

// whether to track memory allocations
#cmakedefine NX_CONFIG_MEMORY_TRACK_ALLOCATIONS @NX_CONFIG_MEMORY_TRACK_ALLOCATIONS@

#endif