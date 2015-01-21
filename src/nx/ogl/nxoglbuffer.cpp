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
#include "nxoglinternal.h"
#include "nxoglbuffer.h"

namespace nx
{

nx_u32
NXOGLBufferType(const GPUBufferType type)
{
    switch(type)
    {
    case kGPUBufferTypeData:
        return GL_ARRAY_BUFFER;
    case kGPUBufferTypeIndex:
        return GL_ELEMENT_ARRAY_BUFFER;
    case kGPUBufferTypeUniform:
        return GL_UNIFORM_BUFFER;
    case kGPUBufferTypeFeedback:
        return GL_TRANSFORM_FEEDBACK_BUFFER;
    case kGPUBufferTypeCompute:
        return GL_SHADER_STORAGE_BUFFER;
    default:
        NX_ASSERT_UNREACHABLE();
        return GL_INVALID_ENUM;
    }
}

NXOGLBuffer*
NXOGLBuffer::create(const NXGPUBufferDesc& desc)
{
    NXOGLBuffer* p_buffer = new NXOGLBuffer(desc);
    p_buffer->allocateStorage();
    return p_buffer;
}

NXOGLBuffer*
NXOGLBuffer::create(const NXGPUBufferDesc& desc,
                    const void* pData)
{
    NXOGLBuffer* p_buffer = new NXOGLBuffer(desc);
    p_buffer->allocateStorage(pData);
    return p_buffer;
}

NXOGLBuffer::~NXOGLBuffer()
{
    glDeleteBuffers(1, &_oglhdl);
    oglHdlInvalidate();
}

GPUBufferType
NXOGLBuffer::type() const
{
    return _desc.type;
}

nx_u32
NXOGLBuffer::oglType() const
{
    return _oglType;
}

nx_u32
NXOGLBuffer::size() const
{
    return _desc.size;
}

NXOGLBuffer::NXOGLBuffer(const NXGPUBufferDesc& desc):
    _desc(desc),
    _oglType(NXOGLBufferType(desc.type)),
    _mapped(false)
{
    glCreateBuffers(1, &_oglhdl);
}

void
NXOGLBuffer::allocateStorage(const void* ptr)
{
    GLbitfield glflags = 0;

    if (_desc.mode & kGPUBufferModeCoherentBit)
    {
        glflags |= GL_MAP_COHERENT_BIT;
    }

    if (_desc.mode & kGPUBufferModePresistentBit)
    {
        glflags |= GL_MAP_PERSISTENT_BIT;
    }

    if (_desc.mode & kGPUBufferModeDynamicStorageBit)
    {
        glflags |= GL_DYNAMIC_STORAGE_BIT;
    }

    if (_desc.mode & kGPUBufferModeWriteBit)
    {
        glflags |= GL_MAP_READ_BIT;
    }

    if (_desc.mode & kGPUBufferModeReadBit)
    {
        glflags |= GL_MAP_WRITE_BIT;
    }

    glNamedBufferStorage(oglHdl(), _desc.size, ptr, glflags);
}

}
