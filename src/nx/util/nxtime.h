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
#ifndef __NX_TIME_H__
#define __NX_TIME_H__


namespace nx
{

NX_INLINE nx_timems
NXTimeInMiliSeconds()
{
struct timeval now;
gettimeofday(&now, nullptr);
return (now.tv_sec * 1000) + (now.tv_usec / 1000);
}


NX_INLINE nx_timems
NXTimeInMicroSeconds()
{
struct timeval now;
gettimeofday(&now, nullptr);
return (now.tv_sec * 1000000) + (now.tv_usec);
}

NX_INLINE double
NXTimeInSeconds()
{
struct timeval now;
gettimeofday(&now, nullptr);
return (double)(now.tv_sec) + ((double)now.tv_usec / 1000000.0);
}

NX_INLINE void
NXTimeAsHMS(int& h, int& m, int& s)
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    h = now.tv_sec % 60;
    m = (now.tv_sec / 60) % 60;
    s = (now.tv_sec / (60 * 60)) % 24 + 1;
}

NX_INLINE void
NXSleep(nx_u32 seconds)
{
    ::sleep(seconds);
}

NX_INLINE void
NXSleepMicro(nx_u32 microseconds)
{
    ::usleep(microseconds);
}

nx_u64 NXGetPerformanceCounter();

nx_u64 NXGetPerformanceFrequency();

}

#endif
