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
#include "nx/io/nxiobase.h"
#include "nx/io/nxiomemory.h"

namespace nx
{

NXIOMemory::NXIOMemory(void *buffer,
                       const size_t size,
                       const nx_u32 state):
    NXIOBase(size, state),
    _buffer(),
    _offset(0)
{
    _buffer.write = static_cast<char*>(buffer);
}

NXIOMemory::NXIOMemory(const void *buffer,
                       const size_t size,
                       const nx_u32 state):
    NXIOBase(size, state),
    _buffer(),
    _offset(0)
{
    _buffer.read= static_cast<const char*>(buffer);
}

NXIOMemory::NXIOMemory(const size_t size,
                       const nx_u32 state):
    NXIOBase(size, state | kIOStateFreeBuffersOnCloseBit),
    _buffer(),
    _offset(0)
{
    _buffer.write = static_cast<char*>(NXMalloc(size));
}

NXIOMemory::~NXIOMemory()
{
    this->close();
}

size_t
NXIOMemory::read(void* buffer,
                 const size_t size)
{
    size_t bytes_read = 0;
    if (isReadable())
    {
        size_t size_to_read = (_offset + size >= _size) ? (_size - _offset) : size;
        if (size_to_read)
        {
            memcpy(buffer, _buffer.read + _offset, size_to_read);
            _offset += size_to_read;
            bytes_read = size_to_read;

            if (_offset >= _size)
            {
                setEOF();
            }
        }
    }
    return bytes_read;
}

size_t
NXIOMemory::write(const void* buffer,
                  const size_t size)
{
    size_t bytes_written = 0;
    if (isWritable())
    {
        size_t size_to_write = (_offset + size >= _size) ? (_size - _offset) : size;
        if (size_to_write)
        {
            memcpy(_buffer.write + _offset, buffer, size_to_write);
            _offset += size_to_write;
            bytes_written = size_to_write;

            if (_offset >= _size)
            {
                setEOF();
            }
        }
    }
    return bytes_written;
}

bool
NXIOMemory::seek(const size_t offset,
                 const IOSeekOp seekOp)
{
    clearError();
    switch(seekOp)
    {
    case kIOSeekOpCur:
        _offset += (_offset + offset >= _size) ? (_size - _offset) : offset;
    case kIOSeekOpSet:
        _offset = (offset > _size) ? _size : offset;
        break;
    case kIOSeekOpEnd:
        _offset = _size;
        break;
    default:
        NX_ASSERT(false && "Should not be reached");
        NXLogDebug("NXIOMemory::seek : Unknown seekOp %x", seekOp);
        return false;
    }

    if (_offset >= _size)
    {
        setEOF();
    }

    return true;
}

size_t
NXIOMemory::tell() const
{
    return _offset;
}

void
NXIOMemory::close()
{
    if (_buffer.write && (_state & kIOStateFreeBuffersOnCloseBit))
    {
        NXFree(_buffer.write);
    }
    _buffer.write = nullptr;
}

const void*
NXIOMemory::directMemoryAccessPtr(const size_t offset) const
{
    return _buffer.read + offset;
}

}
