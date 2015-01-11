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
#include "nx/io/nxarchivefs.h"
#include "nx/io/nxfilemanager.h"
#include "nx/os/nxpath.h"
#include "nx/io/nxiobase.h"

#include <YAAF/YAAF.h>
namespace nx
{

class NXYAAFFile : public NXIOBase
{
public:
    NXYAAFFile(YAAF_File* file):
        NXIOBase(YAAF_FileSize(file), kIOAccessModeReadBit),
        _file(file)
    {

    }

    ~NXYAAFFile()
    {
        close();
    }

    size_t read(void* buffer,
                const size_t size)
    {
        if (size > NX_U32_MAX)
        {
            NXLogError("ArchiveFile: File read limits 4GB");
            return 0;
        }
        size_t bytes_read = YAAF_FileRead(_file, buffer, (uint32_t) size);
        if (YAAF_FileEOF(_file))
        {
            this->setEOF();
        }
        return bytes_read;
    }

    size_t write(const void*,
                 const size_t)
    {
        this->setError();
        return 0;
    }

    bool seek(const size_t offset,
              const IOSeekOp seekOp)
    {
        if (offset > NX_I32_MAX)
        {
            NXLogError("ArchiveFile: offset limits 2GB");
            return false;
        }

        int flags = 0;
        switch(seekOp)
        {
            case kIOSeekOpCur:
            flags = SEEK_CUR;
            break;
        case kIOSeekOpEnd:
            flags = SEEK_END;
            break;
        case kIOSeekOpSet:
            flags = SEEK_SET;
            break;
        default:
            NXLogError("ArchiveFile: Unknown seek operation %x", seekOp);
            return false;
        }

        this->clearError();
        return YAAF_FileSeek(_file, static_cast<int32_t>(offset), flags) == YAAF_SUCCESS;
    }

    size_t tell() const
    {
        return YAAF_FileTell(_file);
    }

    void close()
    {
        if (_file)
        {
            YAAF_FileDestroy(_file);
            _file = nullptr;
            this->setClosed();
        }
    }

protected:
    YAAF_File* _file;
};

NXArchiveFS::NXArchiveFS(const char* path,
                         const char* tag)
    : NXVirtualfs(path, tag, true),
      _pArchive(nullptr)
{

}

NXArchiveFS::~NXArchiveFS()
{
    if (_pArchive)
    {
        YAAF_ArchiveClose(static_cast<YAAF_Archive*>(_pArchive));
    }
}

bool
NXArchiveFS::init()
{
    YAAF_Archive* p_archive = YAAF_ArchiveOpen(path().c_str());
    if (!p_archive)
    {
        NXLogError("ArchiveFS: Failed to open archive '%s': %s",
                   path().c_str(), YAAF_GetError());
        return false;
    }
    _pArchive = p_archive;
    return true;
}

NXIOBase*
NXArchiveFS::open(const char* path,
                  const int flags)
{
    if (flags & (kIOAccessModeAppendBit|kIOAccessModeWriteBit))
    {
        NXLogError("ArchiveFS: Failed to open '%s', read only operations supported", path);
        return nullptr;
    }

    YAAF_File* p_file = YAAF_FileOpen(static_cast<YAAF_Archive*>(_pArchive), path);
    if (!p_file)
    {
        NXLogDebug("ArchiveFS: Couldn't open '%s': %s", path, YAAF_GetError());
        return nullptr;
    }
    else
    {
        return new NXYAAFFile(p_file);
    }
}

bool
NXArchiveFS::remove(const char* path)
{
    NXLogError("ArchiveFS: Cannot remove '%s' from read-only file systen", path);
    return false;
}

bool
NXArchiveFS::exists(const char* path) const
{
    return YAAF_ArchiveContains(static_cast<const YAAF_Archive*>(_pArchive),
                                path) == YAAF_SUCCESS;
}

void
NXArchiveFS::list(StringList_t& out,
                  const char* path,
                  const bool recursive)
{
    if (recursive)
    {
        NXLogWarning("ArchiveFS: Recursive listing not supported");
    }

    const char** list = YAAF_ArchiveListDir(static_cast<YAAF_Archive*>(_pArchive), path);
    const char** aux = list;
    while(*aux)
    {
        out.push_front(*aux);
        ++aux;
    }
    YAAF_ArchiveFreeList(list);
}

bool
NXArchiveFS::fileInfo(NXVirtualfsFileInfo& info,
                      const char* path)
{
    YAAF_FileInfo yaaf_info;
    if(YAAF_ArchiveFileInfo(static_cast<YAAF_Archive*>(_pArchive),
                            path, &yaaf_info) == YAAF_SUCCESS)
    {
        info.lastModification = yaaf_info.lastModification;
        info.sizeCompressed = yaaf_info.sizeCompressed;
        info.lastModification = yaaf_info.sizeUncompressed;
        info.pFileSystem = this;
        return true;
    }
    return false;
}

}
