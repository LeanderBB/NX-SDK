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
#include "nx/gpu/nxgpushaderinput.h"
#include "nx/gpu/nxgpuinterface.h"

namespace nx
{

nx_u32
nxGPUBufferLayoutStride(const GPUBufferLayout layout)
{
    switch(layout)
    {
    case kGPUBufferLayout3V3N2T:
        return (8 * sizeof(float));
    case kGPUBufferLayout3V3N:
        return (6 * sizeof(float));
    case kGPUBufferLayout3B3T:
        return (6 * sizeof(float));
    case kGPUBufferLayout3V3N2T3C:
        return (11 * sizeof(float));
    case kGPUBufferLayout3FLT:
        return (3 * sizeof(float));
    case kGPUBufferLayout2FLT:
        return (2 * sizeof(float));
    case kGPUBufferLayout1UI:
        return (1 * sizeof(nx_u32));
    case kGPUBufferLayout1US:
        return (1 * sizeof(nx_u16));
    default:
        NX_ASSERT_UNREACHABLE();
        return 0;
    }
}

nx_u32
nxGPUBUfferLayoutNumComponents(const GPUBufferLayout layout)
{
    switch(layout)
    {
    case kGPUBufferLayout3V3N2T:
        return 3;
    case kGPUBufferLayout3V3N:
    case kGPUBufferLayout3B3T:
        return 2;
    case kGPUBufferLayout3V3N2T3C:
        return 4;
    case kGPUBufferLayout3FLT:
    case kGPUBufferLayout2FLT:
    case kGPUBufferLayout1UI:
    case kGPUBufferLayout1US:
        return 1;
    default:
        NX_ASSERT_UNREACHABLE();
        return 0;
    }
}

bool
nxGPUBufferLayoutComponentInfo(NXGPUShaderInputDesc& info,
                               const GPUBufferLayout layout,
                               const nx_u32 idx)
{
    switch(layout)
    {
    case kGPUBufferLayout3V3N2T:
        switch(idx)
        {
        case 0:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxVertices;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
            break;
        case 1:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxVertices;
            info.data_offset = (3 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        case 2:
            info.data_count = 2;
            info.data_idx = kGPUShaderInputIdxTexCoord0;
            info.data_offset = (6 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        default:
            return false;
        }
        break;
    case kGPUBufferLayout3V3N:
        switch(idx)
        {
        case 0:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxVertices;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
            break;
        case 1:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxNormals;
            info.data_offset = (3 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        default:
            return false;
        }
        break;
    case kGPUBufferLayout3B3T:
        switch(idx)
        {
        case 0:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxBinormal;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
            break;
        case 1:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxTangent;
            info.data_offset = (3 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        default:
            return false;
        }
        break;
    case kGPUBufferLayout3V3N2T3C:
        switch(idx)
        {
        case 0:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxVertices;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
            break;
        case 1:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxVertices;
            info.data_offset = (3 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        case 2:
            info.data_count = 2;
            info.data_idx = kGPUShaderInputIdxTexCoord0;
            info.data_offset = (6 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        case 3:
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxColor;
            info.data_offset = (8 * sizeof(float));
            info.data_type = kGPUDataTypeFloat;
            break;
        default:
            return false;
        }
        break;
    case kGPUBufferLayout3FLT:
        if (idx == 0)
        {
            info.data_count = 3;
            info.data_idx = kGPUShaderInputIdxOther;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
        }
        else
        {
            return false;
        }
        break;
    case kGPUBufferLayout2FLT:
        if (idx == 0)
        {
            info.data_count = 2;
            info.data_idx = kGPUShaderInputIdxOther;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
        }
        else
        {
            return false;
        }
    case kGPUBufferLayout1FLT:
        if (idx == 0)
        {
            info.data_count = 1;
            info.data_idx = kGPUShaderInputIdxOther;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeFloat;
        }
        else
        {
            return false;
        }
    case kGPUBufferLayout1UI:
        if (idx == 0)
        {
            info.data_count = 1;
            info.data_idx = kGPUShaderInputIdxOther;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeUInt;
        }
        else
        {
            return false;
        }
    case kGPUBufferLayout1US:
        if (idx == 0)
        {
            info.data_count = 1;
            info.data_idx = kGPUShaderInputIdxOther;
            info.data_offset = 0;
            info.data_type = kGPUDataTypeUShort;
        }
        else
        {
            return false;
        }
    default:
        NX_ASSERT_UNREACHABLE();
        return false;
    }
    return true;
}

nx_u32
nxGPUBufferStride(const GPUDataType type,
                  const nx_u32 count)
{
    nx_u32 data_size = 0;
    switch(type)
    {
    case kGPUDataTypeFloat:
    case kGPUDataTypeUInt:
    case kGPUDataTypeInt:
        data_size = 4;
        break;
    case kGPUDataTypeUShort:
    case kGPUDataTypeShort:
        data_size = 2;
        break;
    case kGPUDataTypeUByte:
    case kGPUDataTypeByte:
        data_size = 1;
        break;
    default:
        NX_ASSERT_UNREACHABLE();
    }

    return count * data_size;
}


const char*
nxGPUShaderInputIdxToStr(const GPUShaderInputIdx idx)
{
    static const char* strings[] =
    {
        "kGPUShaderInputIdxVertices",
        "kGPUShaderInputIdxNormals",
        "kGPUShaderInputIdxTexCoord0",
        "kGPUShaderInputIdxTexCoord1",
        "kGPUShaderInputIdxBinormal",
        "kGPUShaderInputIdxTangent",
        "kGPUShaderInputIdxColor",
        "kGPUShaderInputIdxOther0",
        "kGPUShaderInputIdxOther1",
        "kGPUShaderInputIdxOther2",
        "kGPUShaderInputIdxOther3",
        "kGPUShaderInputIdxOther4",
        "kGPUShaderInputIdxOther5",
        "kGPUShaderInputIdxOther6",
        "kGPUShaderInputIdxOther7",
        "kGPUShaderInputIdxOther8",
        "kGPUShaderInputIdxOther9",
        "kGPUShaderInputIdxOther10"
    };
    return (idx < kGPUShaderInputIdxMax) ? strings[idx] : "Uknown Idx";
}

NXGPUShaderInput::NXGPUShaderInput():
    _inputs(NX_GPU_DEFAULT_MAX_SHADER_INPUT),
    _bufferBindings(NX_GPU_DEFAULT_MAX_SHADER_INPUT)
{

}

NXGPUShaderInput::~NXGPUShaderInput()
{

}

bool
NXGPUShaderInput::addInput(const NXGPUShaderInputDesc& desc)
{
    // checl valid range
    if (desc.data_idx >= kGPUShaderInputIdxMax)
    {
        NXLogError("NXGPUShaderInput::addInput: data_idx exceed the maxium supported input index");
        return false;
    }

    if (desc.binding_idx >= kGPUShaderInputIdxMax)
    {
        NXLogError("NXGPUShaderInput::addInput: data_idx exceed the maxium supported input index");
        return false;
    }


    // check if already set
    auto& input = _inputs[desc.data_idx];
    if (input.enabled)
    {
        NXLogError("NXGPUShaderInput::addInput: Input index (%d) is alread set", desc.data_idx);
        return false;
    }

    // update binding information
    _bufferBindings[desc.binding_idx].stride += nxGPUBufferStride(static_cast<GPUDataType>(desc.data_type), desc.data_count);

    // copy values
    input = desc;
    input.enabled = 1;

    return true;
}

bool
NXGPUShaderInput::addBuffer(const NXGPUBufferHdl& buffer,
                            const nx_u32 bindingIdx)
{
    if (bindingIdx > kGPUShaderInputIdxMax)
    {
        NXLogError("NXGPUShaderInput::addBuffer: binding index exceeds current available bindings");
        return false;
    }

    NXShaderInputBufferDesc& desc = _bufferBindings[bindingIdx];

    if (desc.enabled)
    {
        NXLogError("NXGPUShaderInput::addBuffer: binding index (%d) already has a buffer assigned",
                   bindingIdx);
        return false;
    }

    desc.hdl = buffer;
    desc.enabled = 1;
    desc.binding_idx = bindingIdx; // FIXME: Is this really necessary?

    return true;
}

}
