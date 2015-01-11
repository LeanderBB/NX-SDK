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

#ifndef __NX_IO_H__
#define __NX_IO_H__


namespace nx
{

enum IOAccessMode
{
    kIOAccessModeReadBit = NX_BIT(0),
    kIOAccessModeWriteBit = NX_BIT(1),
    kIOAccessModeAppendBit = NX_BIT(2),
    kIOAccessModeOverwriteBit = NX_BIT(3),
};

enum IOState
{
    kIOStateOpenBit = NX_BIT(4),
    kIOStateEOFBit = NX_BIT(5),
    kIOStateErrorBit = NX_BIT(6),
    kIOStateStreamBit = NX_BIT(7), /// IO does not have a size, eg.: socket streams
    kIOStateFreeBuffersOnCloseBit = NX_BIT(8)
};

enum IOSeekOp
{
    kIOSeekOpEnd,
    kIOSeekOpCur,
    kIOSeekOpSet
};

}

#if defined(NX_OS_UNIX)
typedef int nx_oshdl_t;
#define NX_OSHDL_INVALID -1
#else
#error No typedef for OS handle type
#endif

#endif
