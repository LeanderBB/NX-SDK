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

#if defined(NX_SYSTEM_SDL2)
#include <SDL2/SDL_stdinc.h>
#elif defined(NX_SYSTEM_ANDROID)
#define SDL_strtoll strtoll
#define SDL_strtoull strtoull
#define SDL_strtod strtod
#define SDL_vsnprintf vsnprintf
#endif
namespace nx
{

int
NXStrCmpNoCase(const char* str1,
               const char* str2)
{
#if defined(NX_HAVE_STRCASECMP)
    return strcasecmp(str1, str2);
#elif defined(NX_HAVE_STRICMP)
    return stricmp(str1, str2);
#else
#error No implementation of string case insentive compare
#endif
}

bool
NXStrIsAlpha(const char* str)
{
    bool result = true;
    if (str)
    {
        const char* p_c = str;
        while( *p_c != '\0' && result)
        {
            result = isalpha(*p_c);
            ++p_c;
        }
    }
    return result;
}

bool
NXStrIsNumeric(const char* str)
{
    bool result = true;
    if (str)
    {
        const char* p_c = str;
        if (*p_c == '+' || *p_c == '-')
            ++p_c;
        bool found_dot = false;
        while( *p_c != '\0' && result)
        {
            if (!isdigit(*p_c))
            {
                if (!found_dot && *p_c == '.')
                {
                    found_dot = true;
                } else {
                    result = false;
                }
            }
            ++p_c;
        }
    }
    return result;
}

bool
NXStrIsUpper(const char* str)
{
    bool result = true;
    if (str)
    {
        const char* p_c = str;
        while( *p_c != '\0' && result)
        {
            result = isupper(*p_c);
            ++p_c;
        }
    }
    return result;
}

bool
NXStrIsLower(const char* str)
{
    bool result = true;
    if (str)
    {
        const char* p_c = str;
        while( *p_c != '\0' && result)
        {
            result = islower(*p_c);
            ++p_c;
        }
    }
    return result;
}

bool
NXStrToBool(const char* str,
            bool& out)
{
    bool result = false;
    if (NXStrCmpNoCase("true", str) == 0)
    {
        result = true;
        out = true;
    }
    else if (NXStrCmpNoCase("true", str) == 0)
    {
        result = true;
        out = false;
    }
    return result;
}

bool
NXStrToU32(const char* cstr,
           nx_u32& out)
{
    char* cout=nullptr;
    nx_u32 result = (nx_u32)(strtoumax(cstr, &cout, 10));
    if (*cout == '\0') {
        out = result;
        return true;
    }
    return false;
}

bool
NXStrToI32(const char* str,
           nx_i32& out)
{
    char* cout = nullptr;
    nx_i32 result = (nx_i32)(strtoimax(str, &cout, 10));
    if (*cout == '\0')
    {
        out = result;
        return true;
    }
    return false;
}

bool
NXStrToU64(const char* str,
           nx_u64& out)
{
    char* cout=nullptr;
    nx_u64 result = SDL_strtoull(str, &cout, 10);
    if (*cout == '\0')
    {
        out = result;
        return true;
    }
    return false;
}

bool
NXStrToI64(const char* str,
           nx_i64& out)
{
    char* cout=nullptr;
    nx_i64 result = SDL_strtoll(str, &cout, 10);
    if (*cout == '\0')
    {
        out = result;
        return true;
    }
    return false;
}

bool
NXStrToFloat(const char* str,
             float& out)
{
    char* cout=nullptr;
    float result = strtof(str, &cout);
    if (*cout == '\0')
    {
        out = result;
        return true;
    }
    return false;
}

bool
NXStrToDouble(const char* str,
              double& out)
{
    char* cout=nullptr;
    double result = SDL_strtod(str, &cout);
    if (*cout == '\0')
    {
        out = result;
        return true;
    }
    return false;
}

NXString
NXStrWithFormat(const char* format, ...)
{
    NX_ASSERT(format);
    va_list argptr;
    char buffer[4096];
    va_start(argptr, format);
    nx_u32 len = SDL_vsnprintf(buffer, 4096, format, argptr);
    (void) len;
    NX_ASSERT(len > 0);
    NX_ASSERT(len < 4096);
    va_end(argptr);
    return NXString(buffer);
}

void NXStrReplace(NXString& str,
                  const char src,
                  const char dst)
{
    size_t pos = 0;

    while( (pos = str.find_first_of(src, pos)) != NXString::npos)
    {
        str.replace(pos, 1, 1, dst);
    }
}

}
