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
#ifndef __NX_SANDBOXFS_H__
#define __NX_SANDBOXFS_H__

#include "nxvirtualfs.h"

namespace nx
{

class NXSandboxfs : public NXVirtualfs
{
public:

    NXSandboxfs(const char* path,
                const char* tag,
                const bool readOnly);

    ~NXSandboxfs();

    virtual NXIOBase* open(const char* path,
                           const int flags);

    virtual bool remove(const char* path);

    virtual bool exists(const char* path) const;

    virtual void list(StringList_t& out,
                      const char* path,
                      const bool recursive);

    virtual bool fileInfo(NXVirtualfsFileInfo& info,
                          const char* path);

protected:

    bool validPath(const char* path,
                   NXString* out = nullptr) const;

};

}

#endif // NXSANDBOXFS_H
