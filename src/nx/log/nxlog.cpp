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
#include <stdarg.h>
#define NX_LOG_PREFIX_ERROR "ERR "
#define NX_LOG_PREFIX_INFO "INFO"
#define NX_LOG_PREFIX_WARN "WARN"
#define NX_LOG_PREFIX_DEBUG "DEBG"
#define NX_LOG_PREFIX_FATAL "!FATAL!"


namespace nx
{

static void
logInternal(const char* tag, const char* format, va_list& args)
{
    fprintf(stdout, "[%s] ", tag);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
}

void
NXLogError(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logInternal(NX_LOG_PREFIX_ERROR, format, argptr);
    va_end(argptr);
}

void
NXLogWarning(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logInternal(NX_LOG_PREFIX_WARN, format, argptr);
    va_end(argptr);
}

#if defined(NX_DEBUG)
void
NXLogDebug(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logInternal(NX_LOG_PREFIX_DEBUG, format, argptr);
    va_end(argptr);
}
#endif

void
NXLogFatal(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logInternal(NX_LOG_PREFIX_FATAL, format, argptr);
    va_end(argptr);
    exit(1);
}

void
NXLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logInternal(NX_LOG_PREFIX_INFO, format, argptr);
    va_end(argptr);
}

void
NXLogFlush()
{
    fflush(stdout);
}

}
