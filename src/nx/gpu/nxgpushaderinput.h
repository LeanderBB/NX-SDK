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
#ifndef __NX_GPUSHADERINPUT_H__
#define __NX_GPUSHADERINPUT_H__

#include "nx/gpu/nxgpu.h"
#include "nx/gpu/nxgpubuffer.h"
namespace nx
{

struct NXGPUShaderInputDesc
{
    nx_u8 binding_idx = 0;
    nx_u8 data_type = kGPUDataTypeUnknown;
    nx_u8 data_idx = 1;
    nx_u8 data_count = 1;
    nx_u16 data_offset = 0;
    nx_u16 enabled = 0;
};

struct NXShaderInputBufferDesc
{
    NXGPUBufferHdl hdl;
    nx_u16 stride = 0;
    nx_u8 binding_idx = 0;
    nx_u8 enabled = 0;
};

typedef std::vector<NXGPUShaderInputDesc> NXGPUShaderInputDescVec_t;
typedef std::vector<NXShaderInputBufferDesc> NXShaderInputBufferDescVec_t;

nx_u32 nxGPUBufferLayoutStride(const GPUBufferLayout layout);

nx_u32 nxGPUBUfferLayoutNumComponents(const GPUBufferLayout layout);

bool nxGPUBufferLayoutComponentInfo(NXGPUShaderInputDesc& info,
                                    const GPUBufferLayout layout,
                                    const nx_u32 idx);

nx_u32 nxGPUBufferStride(const GPUDataType type,
                         const nx_u32 count);

const char* nxGPUShaderInputIdxToStr(const GPUShaderInputIdx idx);

class NXGPUShaderInput
{
public:
    NXGPUShaderInput();

    ~NXGPUShaderInput();

    bool addInput(const NXGPUShaderInputDesc& desc);

    bool addBuffer(const NXGPUBufferHdl& buffer,
                   const nx_u32 bindingIdx);


    const NXGPUShaderInputDescVec_t& inputs() const
    {
        return _inputs;
    }

    const NXShaderInputBufferDescVec_t& bufferBindings() const
    {
        return _bufferBindings;
    }

    void setIndexBuffer(const NXGPUBufferHdl& hdl)
    {
        _indexBuffer = hdl;
    }

    const NXGPUBufferHdl& indexBuffer() const
    {
        return _indexBuffer;
    }

private:
    NX_CPP_NO_COPY(NXGPUShaderInput);

protected:
    NXGPUShaderInputDescVec_t _inputs;
    NXShaderInputBufferDescVec_t _bufferBindings;
    NXGPUBufferHdl _indexBuffer;
};

}
#endif
