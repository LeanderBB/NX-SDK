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
#ifndef __NX_IOBASE_H__
#define __NX_IOBASE_H__

#include "nxio.h"

namespace nx
{

class NXIOBase
{
public:

    virtual ~NXIOBase()
    {

    }

    virtual size_t read(void* buffer,
                        const size_t size) = 0;

    virtual size_t write(const void* buffer,
                         const size_t size) = 0;

    virtual bool seek(const size_t offset,
                      const IOSeekOp op) = 0;

    virtual size_t tell() const = 0;

    virtual void close() = 0;

    bool isOpen() const
    {
        return _state & kIOStateOpenBit;
    }

    bool isReadable() const
    {
        return _state & kIOAccessModeReadBit;
    }

    bool isWritable() const
    {
        return _state & (kIOAccessModeWriteBit | kIOAccessModeAppendBit);
    }

    bool isEOF() const
    {
        return _state & kIOStateEOFBit;
    }

    bool isError() const
    {
        return _state & kIOStateErrorBit;
    }

    virtual void clearError()
    {
        _state &= ~(kIOStateEOFBit | kIOStateErrorBit);
    }

    size_t size() const
    {
        return _size;
    }

protected:
    NXIOBase(const size_t size,
             const nx_u32 state) :
        _size(size),
        _state(state)
    {

    }

    void setClosed()
    {
        NX_FLAG_REM(_state, kIOStateOpenBit);
    }

    void setError()
    {
        NX_FLAG_ADD(_state, kIOStateErrorBit);
    }

    void setEOF()
    {
        NX_FLAG_ADD(_state, kIOStateEOFBit);
    }

private:
    NX_CPP_NO_COPY(NXIOBase);

protected:
    size_t _size;
    nx_u32 _state;
};


}

#endif
