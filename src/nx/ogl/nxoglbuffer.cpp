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
#include "nx/ogl/nxoglbuffer.h"

namespace nx
{

nx_u32
nxOGLBufferType(const GPUBufferType type)
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

nx_u32
nxOGLBufferAccess(const nx_u32 flags)
{
    const nx_u32 buffer_access = flags & (~kGPUBufferImmutableModeMask);
    switch(buffer_access)
    {

    case kGPUBufferAccessDynamicBit:
        return GL_DYNAMIC_DRAW;

    case kGPUBufferAccessStreamBit:
        return GL_STREAM_DRAW;

    case kGPUBufferAccessStaticBit:
        return GL_STATIC_DRAW;

    default:
        NX_ASSERT_UNREACHABLE();
        return GL_INVALID_ENUM;
    }
}

NXTLSharedPtr<NXOGLBuffer>
NXOGLBuffer::create(const NXGPUBufferDesc& desc)
{
    NXOGLBufferPtr_t buffer = nxMakeTLShared<NXOGLBuffer>(desc);
    buffer->allocateStorage(desc.data);
    return buffer;
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
    _oglType(nxOGLBufferType(desc.type)),
    _flags(0)
{
    glCreateBuffers(1, &_oglhdl);
    if (desc.flags & kGPUBufferImmutableModeMask)
    {
        _flags |=kOGLBufferFlagImmutable;
    }
}

bool
NXOGLBuffer::mapped() const
{
    return _flags & kOGLBufferFlagMappedBit;
}

bool
NXOGLBuffer::immutable() const
{
    return _flags & kOGLBufferFlagImmutable;
}

void
NXOGLBuffer::allocateStorage(const void* ptr)
{
    GLbitfield glflags = 0;

    if (_flags & kOGLBufferFlagImmutable)
    {
        if (_desc.mode & kGPUBufferImmutableModeCoherentBit)
        {
            glflags |= GL_MAP_COHERENT_BIT;
        }

        if (_desc.mode & kGPUBufferImmutableModePresistentBit)
        {
            glflags |= GL_MAP_PERSISTENT_BIT;
        }

        if (_desc.mode & kGPUBufferModeImmutableDynamicStorageBit)
        {
            glflags |= GL_DYNAMIC_STORAGE_BIT;
        }

        if (_desc.mode & kGPUBufferModeImmutableWriteBit)
        {
            glflags |= GL_MAP_READ_BIT;
        }

        if (_desc.mode & kGPUBufferModeImmutableReadBit)
        {
            glflags |= GL_MAP_WRITE_BIT;
        }
        glNamedBufferStorage(oglHdl(), _desc.size, ptr, glflags);
    }
    else
    {
        nx_u32 buffer_access = nxOGLBufferAccess(_desc.flags);
        glNamedBufferData(_oglhdl, _desc.size, ptr, buffer_access);
    }
}

}
