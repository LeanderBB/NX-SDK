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
#include "nx/io/nxsandboxfs.h"
#include "nx/io/nxfilemanager.h"
#include "nx/os/nxpath.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"
#include "nx/io/nxiomemorymappedfile.h"
namespace nx
{


NXSandboxfs::NXSandboxfs(const char* path,
                         const char* tag,
                         const bool readOnly)
    : NXVirtualfs(path, tag, readOnly)
{

}

NXSandboxfs::~NXSandboxfs()
{

}

NXIOBase*
NXSandboxfs::open(const char* path,
                  const int flags)
{
    NXIOBase* p_file = nullptr;

    NXString real_path;
    if (validPath(path, &real_path))
    {
        if (flags & (kIOAccessModeWriteBit | kIOAccessModeAppendBit) && readOnly())
        {
            NXLogError("NXSandBoxfs: Cannot open '%s' in write mode, read only file system",
                       path);
        }
        else
        {
            if (flags & kFileManagerOpenHintMemoryFileBit)
            {
                p_file = NXIOMemoryMappedFile::open(real_path.c_str(), flags);
            }
            else
            {
                p_file = NXIOFile::open(real_path.c_str(), flags);
            }
            if (!p_file)
            {
                NXLogError("NXSandBoxfs: Could not open '%s'", path);
            }
        }
    }
    else
    {
        NXLogWarning("NXSandBoxfs: Attempting to open '%s' which is not a valid path",
                     path);
    }
    return p_file;
}

bool
NXSandboxfs::remove(const char* path)
{
    NX_ASSERT(!readOnly() && "Removing operation on readonly mode");
    NXString real_path;
    bool result = false;
    if (validPath(path, &real_path))
    {
        result = NXPath::rm(path);
    }
    return result;
}

bool
NXSandboxfs::exists(const char* path) const
{
    NXString real_path;
    bool result = false;
    if (validPath(path, &real_path))
    {
        result = NXPath::exists(real_path.c_str());
    }
    return result;
}

void
NXSandboxfs::list(StringList_t& out,
                  const char* path,
                  const bool recursive)
{
    NXString real_path;
    if (validPath(path, &real_path))
    {
        NXPath::list(out, real_path.c_str(), recursive);
    }
}

bool
NXSandboxfs::fileInfo(NXVirtualfsFileInfo& info,
                      const char* path)
{
    NXString real_path;
    bool result = false;
    if (validPath(path, &real_path))
    {
        size_t size;
        if (NXPath::fileSize(size, real_path.c_str()))
        {
            info.sizeCompressed = size;
            info.size = size;
            info.lastModification = 0;
            info.pFileSystem = this;
            result = true;
        }

    }
    return result;
}

bool
NXSandboxfs::validPath(const char* path,
                       NXString *out) const
{
    NXString joined_path = NXPath::join(_path.c_str(), path);
    if (NXPath::sPathSepChr != NXVirtualfs::sPathSepChr)
    {
        NXStrReplace(joined_path, NXPath::sPathSepChr, NXVirtualfs::sPathSepChr);
    }
    NXString real_path = NXPath::realpath(joined_path.c_str());

    if (real_path.find_first_of(_path) == NXString::npos)
    {
        return false;
    }
    else
    {
        if (out)
        {
            *out = std::move(real_path);
        }
        return true;
    }
}

}
