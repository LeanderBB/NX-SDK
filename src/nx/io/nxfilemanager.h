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
#ifndef __NX_FILEMANAGER_H__
#define __NX_FILEMANAGER_H__

namespace nx
{

class NXVirtualfs;
class NXIOBase;


enum FileManagerOpenHint
{
    kFileManagerOpenHintMemoryFileBit = NX_BIT(31) // use this to request memory based file mapping
};

/// File Manger for NX, has the possibiltiy of managing several sandboxed
/// directories as well as compresssed archives.
///
/// If no tag is specified, all directories will be mounted in the root virtual
/// file system, otherwise they will be directly mapped into a virtual file
/// system. A tag begins with $ and ends with $(e.g.: $USER$).
class NXFileManager
{
public:

    NXFileManager();

    ~NXFileManager();

    bool init();

    void shutdown();

    NXIOBase* open(const char* path,
                   const int flags);

    bool remove(const char* path);

    bool exists(const char* path) const;

    void list(StringList_t& out,
              const char* path,
              const bool recursive);

    bool mount(const char* path,
               const char* tag,
               const bool readOnly);

    bool mountArchive(const char* path,
                      const char* tag);

    void unmount(const char* path);

    void clear();

protected:

    NXVirtualfs* decodeTag(const char* path,
                           const char *&correctedPath) const;

protected:
    typedef std::unordered_map<NXString, NXVirtualfs*> TagMap_t;
    typedef NXCntrPtr<NXVirtualfs> VirtualfsPtr_t;
    typedef std::vector<VirtualfsPtr_t> VirtualfsVec_t;

    VirtualfsVec_t _vfs;
    TagMap_t _tags;

};

}
#endif
