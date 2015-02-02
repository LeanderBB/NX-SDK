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

#include "nx/ogl/nxoglinternal.h"
#include "nx/ogl/nxoglshaderinput.h"
#include "nx/gpu/nxgpushaderinput.h"
#include "nx/gpu/nxgpubuffer.h"
#include "nx/ogl/nxgpuinterfaceogl.h"

namespace nx
{

static void
nxOGLVAOSetup(const nx_u32 hdl,
              const NXGPUShaderInputDesc& desc)
{

    glVertexArrayAttribBinding(hdl, desc.data_idx, desc.binding_idx);
    switch(desc.data_type)
    {
    case kGPUDataTypeFloat:
        glVertexArrayAttribFormat(hdl, desc.data_idx, desc.data_count, GL_FLOAT, GL_FALSE, desc.data_offset);
        break;
    case kGPUDataTypeUInt:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_UNSIGNED_INT, desc.data_offset);
        break;
    case kGPUDataTypeInt:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_INT, desc.data_offset);
        break;
    case kGPUDataTypeUShort:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_UNSIGNED_SHORT, desc.data_offset);
        break;
    case kGPUDataTypeShort:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_SHORT, desc.data_offset);
        break;
    case kGPUDataTypeUByte:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_UNSIGNED_BYTE, desc.data_offset);
        break;
    case kGPUDataTypeByte:
        glVertexArrayAttribIFormat(hdl, desc.data_idx, desc.data_count, GL_BYTE, desc.data_offset);
        break;

    default:
        NX_ASSERT_UNREACHABLE();
    }
    glEnableVertexArrayAttrib(hdl, desc.data_idx);
}

/*
void
nxGPUShaderInputDescToOGLShaderInputDesc(NXGPUShaderInputDescVec_t &out,
                                         const NXGPUShaderInputDescVec_t& inputs,
                                         const NXGPUBufferHdlVec_t& buffers,
                                         NXGPUInterfaceOGL& gpuInterface)
{
    out.resize(inputs.size());

    int idx = 0;
    for(auto& val : inputs)
    {
        NXOGLShaderInputDesc& desc = out[idx++];

        // sanity checks
        NX_ASSERT(val.buffer_idx < buffers.size());

        // get the buffer
        const NXGPUBufferHdl& buf_hdl = buffers[val.buffer_idx];
        NXOGLBufferPtr_t buffer = gpuInterface.getBuffer(buf_hdl.gpuhdl);
        NX_ASSERT(buffer);

        // fill in values
        desc.binding_idx = val.buffer_idx;
        desc.buffer = buffer->oglHdl();
        desc.buffer_offset = buf_hdl.offset;
        desc.buffer_stride = 0;
        desc.data_count = val.data_count;
        desc.data_idx = val.data_idx;
        desc.data_type = val.data_type;
        desc.data_offset =val.data_stride;

        // continue
    }
}

*/
NXTLSharedPtr<NXOGLShaderInput>
NXOGLShaderInput::create(const NXGPUShaderInputDescVec_t& inputs,
                         const NXOGLShaderInputBufferDescVec_t& buffers,
                         const nx_u32 indexBufferName)
{
    NXOGLShaderInputPtr_t ptr = nxMakeTLShared<NXOGLShaderInput>();

    nx_u32 hdl = ptr->oglHdl();
    // setup vertex array attribs
    for(auto& val : inputs)
    {
        if (val.enabled)
        {
            nxOGLVAOSetup(hdl, val);
        }
    }

    for (auto& buf : buffers)
    {
        glVertexArrayVertexBuffer(hdl, buf.binding_idx, buf.hdl, buf.offset, buf.stride);
    }

    // bind index buffer (if any)

    if (indexBufferName != NX_OGL_HDL_INVALID)
    {
        glVertexArrayElementBuffer(hdl, indexBufferName);
    }
    return ptr;
}


NXOGLShaderInput::NXOGLShaderInput()
{
    glCreateVertexArrays(1, &_oglhdl);
}

NXOGLShaderInput::~NXOGLShaderInput()
{
    glDeleteVertexArrays(1, &_oglhdl);
    oglHdlInvalidate();
}

void
NXOGLShaderInput::bind()
{
    glBindVertexArray(_oglhdl);
}

}
