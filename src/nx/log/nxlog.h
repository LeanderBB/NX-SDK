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

#ifndef __NX_LOG_H__
#define __NX_LOG_H__

namespace nx
{

void NXLogError(const char* format, ...);

void NXLogWarning(const char* format, ...);

#if defined(NX_DEBUG)
void NXLogDebug(const char* format, ...);
#else
NX_INLINE void NXLogDebug(const char*, ...) {}
#endif

void NXLogFatal(const char* format, ...);

void NXLog(const char* format, ...);

void NXLogFlush();

}

#endif
