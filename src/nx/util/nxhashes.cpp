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

namespace nx
{

nx_u32
NXHash32OATData(const void* pData,
                const size_t len)
{
    nx_u32 hash = 0;
    const char* p_char = (const char*) pData;
    for(size_t i=0; i < len; ++i)
    {
        hash += p_char[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

nx_u32
NXHash32OATCStr(const char* str)
{
    if (!str) return 0;
    nx_u32 hash = 0;
    for(;*str; ++str)
    {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

nx_u32
NXHash32OATCStrNoCase(const char* str)
{
    if (!str) return 0;
    nx_u32 hash = 0;
    for(;*str; ++str)
    {
        hash += tolower(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
}

