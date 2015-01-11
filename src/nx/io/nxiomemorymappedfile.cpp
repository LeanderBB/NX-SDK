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
#include "nx/io/nxiomemorymappedfile.h"
#include "nx/os/nxpath.h"
#if defined(NX_OS_UNIX)
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#else
#error "Missing implementation for memory mapped file for current OS"
#endif

namespace nx
{

NXIOMemoryMappedFile*
NXIOMemoryMappedFile::open(const char* path,
                           const int flags)
{
    NXIOMemoryMappedFile* p_result = nullptr;
    size_t file_size;
    int oshdl = NX_OSHDL_INVALID;
    int osflags = 0;
    int mmap_flags = 0;
    void * ptr = nullptr;
    bool found_size = NXPath::fileSize(file_size, path);

    if (found_size)
    {
#if defined(NX_OS_UNIX)
        // convert flags
        if (flags & kIOAccessModeWriteBit)
        {
            osflags |= O_WRONLY;
            if (flags & kIOAccessModeOverwriteBit || !found_size)
            {
                osflags |= O_CREAT;
            }
            mmap_flags |= PROT_WRITE;
        }

        if (flags & kIOAccessModeAppendBit)
        {
            osflags |= O_APPEND | O_WRONLY;
            mmap_flags |= PROT_WRITE;
        }

        if (flags & kIOAccessModeReadBit)
        {
            if (flags & (kIOAccessModeAppendBit|kIOAccessModeWriteBit))
            {
                osflags &= ~(O_WRONLY);
                osflags |= O_RDWR;
            }
            else
            {
                osflags |= O_RDONLY;
            }
             mmap_flags |= PROT_READ;
        }

        // open descriptor
       oshdl = ::open(path, osflags);

        if (oshdl == -1)
        {
            NXLogError("IOMemoryMappedFile: Could not open '%s' - %s", path, strerror(errno));
        }
        else
        {
            // map memorry
            ptr = mmap(0, file_size, mmap_flags, MAP_SHARED, oshdl, 0);
            if (ptr == MAP_FAILED)
            {
                NXLogError("IOMemoryMappedFile: Failed to map memory for '%s' - %s", path, strerror(errno));
                ptr = nullptr;
            }
            else
            {
                p_result = new NXIOMemoryMappedFile(oshdl, ptr, file_size, flags);
            }
        }
#else
#error "Missing implementation for memory mapped file for current OS"
#endif
    }
    else
    {
        NXLogError("IOMemoryMappedFile: Could not get file size for '%s'", path);
    }

    // cleanup
    if (!p_result)
    {
#if defined(NX_OS_UNIX)
        if (ptr)
        {
            munmap(ptr, file_size);
        }
        if (oshdl != NX_OSHDL_INVALID)
        {
            ::close(oshdl);
        }
#else
#error "Missing implementation for memory mapped file for current OS"
#endif
    }
    return p_result;
}

NXIOMemoryMappedFile::~NXIOMemoryMappedFile()
{
    close();
}

void
NXIOMemoryMappedFile::close()
{
#if defined(NX_OS_UNIX)
        if (_buffer.write)
        {
            munmap(_buffer.write, size());
        }
        if (_fhdl != NX_OSHDL_INVALID)
        {
            ::close(_fhdl);
        }
#else
#error "Missing implementation for memory mapped file for current OS"
#endif
}

NXIOMemoryMappedFile::NXIOMemoryMappedFile(const nx_oshdl_t hdl,
                                           void* ptr,
                                           const size_t size,
                                           const nx_u32 flags):
    NXIOMemory(ptr, size, (flags & ~(kIOStateFreeBuffersOnCloseBit))),
    _fhdl(hdl)
{

}


}
