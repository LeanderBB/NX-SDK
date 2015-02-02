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
#include "nx/gpu/nxgpubuffer.h"
#include "nx/gpu/nxgpubuffermanagerinterface.h"

namespace nx
{

NXGPUBuffer::NXGPUBuffer(const NXGPUBufferDesc& desc,
            const NXGPUBufferHdl hdl,
            NXGPUBufferManagerInterface& manager):
    _desc(desc),
    _hdl(hdl),
    _manager(manager)
{

}

NXGPUBuffer::~NXGPUBuffer()
{
    _manager.destroy(this);
}

const NXGPUBufferDesc&
NXGPUBuffer::desc() const
{
    return _desc;
}

nx_u32
NXGPUBuffer::size() const
{
    return _desc.size;
}

GPUBufferType
NXGPUBuffer::type() const
{
    return _desc.type;
}

nx_u32
NXGPUBuffer::mode() const
{
    return _desc.mode;
}

const NXGPUBufferHdl&
NXGPUBuffer::hdl() const
{
    return _hdl;
}


}
