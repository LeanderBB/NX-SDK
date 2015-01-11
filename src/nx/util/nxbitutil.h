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
#ifndef __NX_BITUTIL_H__
#define __NX_BITUTIL_H__

namespace nx
{

NX_INLINE bool NXIsPowOf2(const nx_i32 n)
{
    return n && !(n & (n - 1));
}

NX_INLINE nx_u32 NXPow2Next(const nx_u32 n)
{
    nx_u32 v = n;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

#define NX_FLAG_ADD(x,f) (x |= f)
#define NX_FLAG_REM(x,f) (x &= (~f))
#define NX_BIT(i) (1 << i)



NX_FORCE_INLINE float
NX_BSWAPFLT(const float v)
{
    union
    {
        float f;
        nx_u32 i;
    } uftoi;
    uftoi.f = v;
    uftoi.i = NX_BSWAP32(uftoi.i);
    return uftoi.f;
}

NX_FORCE_INLINE double
NX_BSWAPDBL(const double v)
{
    union
    {
        double d;
        nx_u64 i;
    } udtoi;
    udtoi.d = v;
    udtoi.i = NX_BSWAP64(udtoi.i);
    return udtoi.d;
}

size_t
NXHexDecode(const char* input,
            const size_t inLen,
            void* output,
            const size_t outLen);

size_t
NXHexEncode(const void* input,
            const size_t inLen,
            char* output,
            const size_t outLen);

}
#endif
