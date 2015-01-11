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
#ifndef __NX_IOMEMORYMAPPEDFILE_H__
#define __NX_IOMEMORYMAPPEDFILE_H__

#include "nxiomemory.h"

namespace nx
{

class NXIOMemoryMappedFile : public NXIOMemory
{
public:

    static NXIOMemoryMappedFile* open(const char* path,
                                      const int flags);

    virtual ~NXIOMemoryMappedFile();

    virtual void close();

protected:
    NXIOMemoryMappedFile(const nx_oshdl_t hdl,
                         void* ptr,
                         const size_t size,
                         const nx_u32 flags);

private:
    NX_CPP_NO_COPY(NXIOMemoryMappedFile);

private:
    nx_oshdl_t _fhdl;

};

}
#endif
