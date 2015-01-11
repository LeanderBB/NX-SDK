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

#ifndef __NX_IOMEMORY_H__
#define __NX_IOMEMORY_H__

namespace nx
{
class NXIOBase;

class NXIOMemory : public NXIOBase
{
public:

    NXIOMemory(void *buffer,
               const size_t size,
               const nx_u32 state);

    NXIOMemory(const void *buffer,
               const size_t size,
               const nx_u32 state);

    NXIOMemory(const size_t size,
               const nx_u32 state);

    virtual ~NXIOMemory();

    virtual size_t read(void* buffer,
                const size_t size);

    virtual size_t write(const void* buffer,
                 const size_t size);

    virtual bool seek(const size_t offset,
              const IOSeekOp seekOp);

    virtual size_t tell() const;

    virtual void close();

    void* buffer()
    {
        return _buffer.write;
    }

    const void* buffer() const
    {
        return _buffer.read;
    }

protected:


protected:
    union
    {
        char* write;
        const char* read;
    } _buffer;
    size_t _offset;
};

}
#endif
