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
#include "nx/gpu/nxgpubuffermanagersimple.h"
#include "nx/gpu/nxgpuinterface.h"

namespace nx
{
NXGPUBufferPtr_t
NXGPUBufferManagerSimple::create(const NXGPUBufferDesc& desc)
{
    NXGPUBufferPtr_t ptr;
    NXHdl gpuhdl = _pGPUInterface->allocBuffer(desc);
    if (gpuhdl)
    {
        NXGPUBufferHdl buff_hdl;
        buff_hdl.gpuhdl = gpuhdl;
        buff_hdl.offset = 0;
        ptr = nxMakeTLShared<NXGPUBuffer>(desc, buff_hdl, *this);
    }
    return ptr;
}

void
NXGPUBufferManagerSimple::destroy(const NXGPUBuffer* pBuffer)
{
    _pGPUInterface->releaseBuffer(pBuffer->hdl().gpuhdl);
}

}


