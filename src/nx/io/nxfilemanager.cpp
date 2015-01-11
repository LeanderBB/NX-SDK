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
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxvirtualfs.h"
#include "nx/io/nxiobase.h"
#include "nx/os/nxpath.h"
#include "nx/io/nxsandboxfs.h"
#include "nx/io/nxarchivefs.h"
#include <YAAF/YAAF.h>

namespace nx
{

const char NXVirtualfs::sPathSepChr = '/';
const char* NXVirtualfs::sPathSepStr = "/";

NXFileManager::NXFileManager()
{

}

NXFileManager::~NXFileManager()
{
    shutdown();
}

bool
NXFileManager::init()
{
    YAAF_Allocator alloc;
    alloc.malloc = NXMalloc;
    alloc.free = NXFree;
    alloc.calloc = NXCalloc;

    if (YAAF_Init(&alloc) != YAAF_SUCCESS)
    {
        NXLogError("FileManager: Failed to init YAAF");
        return false;
    }
    return true;
}

void
NXFileManager::shutdown()
{
    clear();
    YAAF_Shutdown();
}

NXIOBase*
NXFileManager::open(const char* path,
                    const int flags)
{

    // check tags
    const char* corrected_path;
    NXVirtualfs* p_vfs = decodeTag(path, corrected_path);
    if (p_vfs)
    {
        return p_vfs->open(corrected_path, flags);
    }

    // check everything

    struct NXVirtualfsFileInfo cur_file;
    struct NXVirtualfsFileInfo final_file;

    // store the current information in cur_file and
    // the final result in final_file
    memset(&cur_file, 0, sizeof(cur_file));
    memset(&final_file, 0, sizeof(final_file));

    auto it = _vfs.begin();
    auto end = _vfs.end();
    for (;it != end; ++it)
    {
        if( (*it)-> fileInfo(cur_file, path))
        {
            if ((final_file.pFileSystem && cur_file.lastModification > final_file.lastModification) || !final_file.pFileSystem)
            {
                memcpy(&final_file, &cur_file, sizeof(final_file));
            }
        }
    }

    return (final_file.pFileSystem) ? final_file.pFileSystem->open(path, flags) : nullptr;
}

bool
NXFileManager::remove(const char* path)
{
    const char* corrected_path;
    NXVirtualfs* p_vfs = decodeTag(path, corrected_path);
    bool result = false;
    if (p_vfs)
    {
        if (!p_vfs->readOnly())
        {
            result = p_vfs->remove(corrected_path);
        }
    }
    else
    {
        auto it = _vfs.begin();
        auto end = _vfs.end();

        for (;it != end && !result; ++it)
        {
            if (!(*it)->readOnly())
            {
                result = (*it)->remove(path);
            }
        }
    }
    return result;
}

bool
NXFileManager::exists(const char* path) const
{
    const char* corrected_path;
    NXVirtualfs* p_vfs = decodeTag(path, corrected_path);
    bool result = false;
    if (p_vfs)
    {
        result = p_vfs->exists(corrected_path);
    }
    else
    {
        auto it = _vfs.begin();
        auto end = _vfs.end();

        for (;it != end && !result; ++it)
        {
            result = (*it)->exists(path);
        }
    }
    return result;
}

void
NXFileManager::list(StringList_t& out,
                    const char* path,
                    const bool recursive)
{
    const char* corrected_path;
    NXVirtualfs* p_vfs = decodeTag(path, corrected_path);
    if (p_vfs)
    {
        p_vfs->list(out, path, corrected_path);
    }
    else
    {
        auto it = _vfs.begin();
        auto end = _vfs.end();
        for (;it != end; ++it)
        {
            (*it)->list(out, path, recursive);
        }
    }
}

bool
NXFileManager::mount(const char* path,
                     const char* tag,
                     const bool readOnly)
{
    // check tag
    NXString vfs_tag = tag;
    if (vfs_tag.length())
    {
        auto tag_it = _tags.find(tag);
        if (tag_it != _tags.end())
        {
            NXLogError("FileManager: Tag '%s' already exists", tag);
            return false;
        }
    }

    // check if the file system has been mounted
    NXString full_path =  NXPath::realpath(path);

    // check it the path is a directory

    if (!NXPath::isDirectory(full_path.c_str()))
    {
        NXLogError("FileManger: Path '%s' is not a directory or does not exist",
                   path);
        return false;
    }

    auto it = _vfs.begin();
    auto end = _vfs.end();
    bool result = false;
    for (;it != end && !result; ++it)
    {
        if (NXStrCmpNoCase((*it)->path().c_str(), path) == 0)
        {
            result = true;
        }
    }

    if (result)
    {
        NXLogError("FileManger: Path '%s' already mounted", path);
        return false;
    }

    // add filesystem

    NXVirtualfs* p_vfs = new NXSandboxfs(full_path.c_str(), tag, readOnly);

    if (!p_vfs->init())
    {
        NXLogError("FileManager: Failed to init file system");
        delete p_vfs;
        return false;
    }

    if (vfs_tag.length())
    {
        _tags.insert(std::make_pair(vfs_tag, p_vfs));
    }
    _vfs.push_back(p_vfs);

    return true;
}

bool
NXFileManager::mountArchive(const char* path,
                  const char* tag)
{
    // check tag
    NXString vfs_tag = tag;
    if (vfs_tag.length())
    {
        auto tag_it = _tags.find(tag);
        if (tag_it != _tags.end())
        {
            NXLogError("FileManager: Tag '%s' already exists", tag);
            return false;
        }
    }

    // check if the file system has been mounted
    NXString full_path =  NXPath::realpath(path);

    // check it the path is a directory

    if (!NXPath::isFile(full_path.c_str()))
    {
        NXLogError("FileManger: Path '%s' is not a file or does not exist",
                   path);
        return false;
    }

    auto it = _vfs.begin();
    auto end = _vfs.end();
    bool result = false;
    for (;it != end && !result; ++it)
    {
        if (NXStrCmpNoCase((*it)->path().c_str(), path) == 0)
        {
            result = true;
        }
    }

    if (result)
    {
        NXLogError("FileManger: Path '%s' already mounted", path);
        return false;
    }

    // add filesystem

    NXVirtualfs* p_vfs = new NXArchiveFS(full_path.c_str(), tag);

    if (!p_vfs->init())
    {
        NXLogError("FileManager: Failed to mount archive '%s'", path);
        delete p_vfs;
        return false;
    }

    if (vfs_tag.length())
    {
        _tags.insert(std::make_pair(vfs_tag, p_vfs));
    }
    _vfs.push_back(p_vfs);

    return true;
}

void
NXFileManager::unmount(const char* path)
{
    auto it = _vfs.begin();
    auto end = _vfs.end();
    bool result = false;
    for (;it != end && !result; ++it)
    {
        if (NXStrCmpNoCase((*it)->path().c_str(), path) == 0)
        {
            if ((*it)->tag().length())
            {
                _tags.erase((*it)->tag());
            }
            _vfs.erase(it);
            result = true;
        }
    }
}

void
NXFileManager::clear()
{
    _vfs.clear();
    _tags.clear();
}

NXVirtualfs*
NXFileManager::decodeTag(const char* path,
                         const char*& correctedPath) const
{
    const char* chr = strchr(path, NXVirtualfs::sPathSepChr);
    correctedPath = nullptr;
    NXVirtualfs * p_vfs = nullptr;
    if (chr)
    {
        // check if it is a tag
        if (path[0] == '$' && *(chr - 1) == '$')
        {
            NXString tag(path + 1, (chr - 2) - path);
            auto it = _tags.find(tag);
            if (it != _tags.end())
            {
                p_vfs = it->second;
                correctedPath = chr + 1;
            }
        }
    }
    return p_vfs;
}


}
