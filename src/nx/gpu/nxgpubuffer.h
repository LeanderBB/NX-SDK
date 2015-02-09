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
#ifndef __NX_GPUBUFFER_H__
#define __NX_GPUBUFFER_H__

#include "nx/hdl/nxhdl.h"
#include "nx/util/nxtlsharedptr.h"
#include "nx/gpu/nxgpu.h"

namespace nx
{

struct NXGPUBufferDesc
{
    GPUBufferType type = kGPUBufferTypeData;
    nx_u32 mode = 0;
    nx_u32 size = 0;
    nx_u32 flags = 0;
    const void* data = nullptr;
};

struct NXGPUBufferHdl
{
    NXHdl gpuhdl;
    nx_u32 offset = 0;
};

class NXGPUBufferManagerInterface;
class NXGPUBuffer
{
public:

    NXGPUBuffer(const NXGPUBufferDesc& desc,
                const NXGPUBufferHdl hdl,
                NXGPUBufferManagerInterface& manager);


    ~NXGPUBuffer();

    const NXGPUBufferDesc& desc() const;

    nx_u32 size() const;

    GPUBufferType type() const;

    nx_u32 mode() const;

    const NXGPUBufferHdl& hdl() const;

protected:
    NXGPUBufferDesc _desc;
    NXGPUBufferHdl _hdl;
    NXGPUBufferManagerInterface& _manager;
};

typedef NXTLSharedPtr<NXGPUBuffer> NXGPUBufferPtr_t;
typedef std::vector<NXGPUBufferPtr_t> NXGPUBufferPtrVec_t;
typedef std::vector<NXGPUBufferHdl> NXGPUBufferHdlVec_t;

}
#endif
