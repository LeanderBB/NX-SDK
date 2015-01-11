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
#include "nx/io/nxiofile.h"
#include "nx/os/nxpath.h"
#include <errno.h>


namespace nx
{

NXIOFile*
NXIOFile::open(const char* path,
               const nx_u32 ioflags)
{
    NXIOFile* p_file = nullptr;
    size_t fsize;
    bool got_file_size = NXPath::fileSize(fsize, path);
    if (got_file_size || (!got_file_size && (ioflags & kIOAccessModeWriteBit)))
    {
        const char* flags;


        if ((ioflags & kIOAccessModeReadBit) &&
                (ioflags & kIOAccessModeWriteBit))
        {
            flags = (ioflags & kIOAccessModeOverwriteBit) ? "w+b" : "r+b";
        }
        else if ((ioflags & kIOAccessModeReadBit) &&
                 (ioflags & kIOAccessModeAppendBit))
        {
            flags = "a+b";
        }
        else
        {
            if (ioflags & kIOAccessModeReadBit)
            {
                flags = "rb";
            }
            if (ioflags & kIOAccessModeAppendBit)
            {
                flags = "ab";
            }
            if (ioflags & kIOAccessModeWriteBit)
            {
                flags = "wb";
            }
        }

        FILE* f = fopen(path, flags);
        if (f)
        {
            p_file = new NXIOFile(f, fsize, ioflags);
        }
        else
        {
            NXLogError("NXIOFile::open : Could not open '%s' - %s", path, strerror(errno));
        }
    }
    else
    {
        NXLogError("NXIOFile::open : '%s' is not a file or could not determine file size",path);
    }
    return p_file;
}

NXIOFile::~NXIOFile()
{
    this->close();
}

size_t
NXIOFile::read(void* buffer,
               const size_t size)
{
    size_t bytes_read = fread(buffer, 1, size, _file);
    if (feof(_file))
    {
        setEOF();
    }
    if (ferror(_file))
    {
        setError();
    }
    return bytes_read;
}

size_t
NXIOFile::write(const void* buffer,
                const size_t size)
{
    size_t bytes_written = fwrite(buffer, 1, size, _file);
    if (feof(_file))
    {
        setEOF();
    }
    if (ferror(_file))
    {
        setError();
    }
    return bytes_written;
}

bool
NXIOFile::seek(const size_t offset,
               const IOSeekOp seekOp)
{
    int op = 0;
    switch(seekOp)
    {
    case kIOSeekOpCur:
        op = SEEK_CUR;
        break;
    case kIOSeekOpSet:
        op = SEEK_SET;
        break;
    case kIOSeekOpEnd:
        op = SEEK_END;
        break;
    default:
        NX_ASSERT(false && "Should not be reached");
        NXLogDebug("NXIOFile::seek : Unknown seekOp %x", seekOp);
        return false;
    }

    int result = fseek(_file, offset, op);

    if (feof(_file))
    {
        setEOF();
    }
    if (ferror(_file))
    {
        setError();
    }

    return result;
}

size_t
NXIOFile::tell() const
{
    NX_ASSERT(_file);
    return ftell(_file);
}

void
NXIOFile::close()
{
    if (_file)
    {
        fclose(_file);
        _file = nullptr;
        setClosed();
    }
}

NXIOFile::NXIOFile(FILE* file,
                   const size_t size,
                   const nx_u32 state):
    NXIOBase(size, state | kIOStateOpenBit),
    _file(file)
{

}

}
