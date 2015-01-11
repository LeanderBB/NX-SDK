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
#ifndef __NX_SHAREDOBJ_H__
#define __NX_SHAREDOBJ_H__

namespace nx
{

class NXSharedObj
{
public:

    static const char* sExtension;

    NXSharedObj();

    ~NXSharedObj();

    bool load(const char* path);

    void* locateSymbol(const char* symbol);

    void unload();

    bool isLoaded() const
    {
        return _obj != nullptr;
    }

private:
    void* _obj;
};

}
#endif

