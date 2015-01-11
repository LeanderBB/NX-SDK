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
#ifndef __NX_VIRTUALFS_H__
#define __NX_VIRTUALFS_H__


namespace nx
{
class NXIOBase;
class NXVirtualfs;

struct NXVirtualfsFileInfo
{
    size_t size;
    size_t sizeCompressed;
    time_t lastModification;
    class NXVirtualfs * pFileSystem;
};

class NXVirtualfs
{
public:

    static const char sPathSepChr;
    static const char* sPathSepStr;

    NXVirtualfs(const char* path,
                const char* tag,
                const bool readOnly):
        _path(path),
        _tag(tag),
        _rdOnly(readOnly)
    {
    }

    virtual ~NXVirtualfs(){}

    virtual bool init()
    {
        return true;
    }

    virtual NXIOBase* open(const char* path,
                           const int flags) = 0;

    virtual bool remove(const char* path) = 0;

    virtual bool exists(const char* path) const = 0;

    virtual void list(StringList_t& out,
                      const char* path,
                      const bool recursive) = 0;

    virtual bool fileInfo(NXVirtualfsFileInfo& info,
                          const char* path) = 0;

    const NXString& path() const
    {
        return _path;
    }

    const NXString& tag() const
    {
        return _tag;
    }

    bool readOnly() const
    {
        return _rdOnly;
    }

protected:
    const NXString _path;
    const NXString _tag;
    const bool _rdOnly;
};

}

#endif
