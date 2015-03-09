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
#include "nxgpuinterfaceogl.h"
#include "nx/gpu/nxgpu.h"
#include "nx/ogl/nxogl.h"
#include "nx/ogl/nxoglinternal.h"

#if defined(NX_SYSTEM_GLFW)
#include <GLFW/glfw3.h>
#endif

namespace nx
{

static void APIENTRY
NXWindowGLDbgCallback(GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar *message,
                      const void *userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    const char* chr_severity;
    const char* error_type;
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        chr_severity = "notification";
        break;
    default:
        chr_severity = "unknown";
    }

    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR:
        error_type = "error";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        error_type = "deperecated behaviour";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        error_type = "undefined behaviour";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        error_type = "portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        error_type = "perforance";
        NXLogWarning("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    default:
        error_type = "Unknown";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    }
}

NXGPUInterface*
NXGPUInterface::create()
{
    return new NXGPUInterfaceOGL();
}

NXGPUInterfaceOGL::NXGPUInterfaceOGL():
    _hdlsBuffer(),
    _hdlsProgram(),
    _hdlsTexture(),
    _hdlsVAO(),
    _bufferManager(this)
{
}

NXGPUInterfaceOGL::~NXGPUInterfaceOGL()
{

}

bool
NXGPUInterfaceOGL::init()
{

    // init opengl
#if defined(NX_SYSTEM_GLFW)
    int result = flextInit(glfwGetCurrentContext());
#else
    int result = flextInit();
#endif
    if (result != GL_TRUE)
    {
        NXLogError("NXGPUInterfaceOGL::init Failed to load all required OpenGL function. \
                   Please make sure you have an OpenGL 4.4 capable device with \
                   GL_ARB_direct_state_access support");
                   return false;
    }

#if defined(NX_DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(NXWindowGLDbgCallback, nullptr);
#endif

    NXOGL::logGLInfo();
    return true;
}

void
NXGPUInterfaceOGL::shutdown()
{
    _hdlsBuffer.reset();
    _hdlsProgram.reset();
    _hdlsTexture.reset();
}

NXHdl
NXGPUInterfaceOGL::allocBuffer(const NXGPUBufferDesc& desc)
{
    NXHdl hdl;
    auto ptr = NXOGLBuffer::create(desc);
    if(!ptr)
    {
        return hdl;
    }
    hdl = _hdlsBuffer.add(kGPUResourceTypeBuffer, ptr);
    return hdl;
}

void
NXGPUInterfaceOGL::releaseBuffer(const NXHdl& hdl)
{
    _hdlsBuffer.remove(hdl);
}

NXHdl
NXGPUInterfaceOGL::allocTexture(const NXGPUTextureDesc& desc)
{
    NXHdl hdl;
    auto ptr =  NXOGLTexture::create(desc);
    if(!ptr)
    {
        return hdl;
    }
    hdl = _hdlsTexture.add(kGPUResourceTypeTexture, ptr);
    return hdl;
}

NXHdl
NXGPUInterfaceOGL::allocTexture(const NXImage& image)
{
    NXHdl hdl;
    auto ptr =  NXOGLTexture::create(image);
    if(!ptr)
    {
        return hdl;
    }
    hdl = _hdlsTexture.add(kGPUResourceTypeTexture, ptr);
    return hdl;
}

void
NXGPUInterfaceOGL::releaseTexture(const NXHdl& hdl)
{
    _hdlsTexture.remove(hdl);
}

NXHdl
NXGPUInterfaceOGL::allocProgram(const NXGPUProgramSourceBase& source)
{
    NXHdl hdl;
    auto ptr =  NXOGLProgram::create(&source);
    if(!ptr)
    {
        return hdl;
    }
    hdl = _hdlsProgram.add(kGPUResourceTypeProgram, ptr);
    return hdl;
}

void
NXGPUInterfaceOGL::releaseProgram(const NXHdl& hdl)
{
    _hdlsProgram.remove(hdl);
}


NXHdl
NXGPUInterfaceOGL::allocShaderInput(const NXGPUShaderInput& input)
{
    NXHdl hdl;
    NXOGLShaderInputBufferDescVec_t ogl_bindings;
    ogl_bindings.reserve(NX_GPU_DEFAULT_MAX_SHADER_INPUT);
    nx_u32 ogl_idx_name = NX_OGL_HDL_INVALID;

    // translate buffer bindings;
    const NXShaderInputBufferDescVec_t& buffer_bindings = input.bufferBindings();
    for(auto it = buffer_bindings.begin(); it != buffer_bindings.end(); ++it)
    {
        // skip disabled entries
        if (!it->enabled)
        {
            continue;
        }

        NXOGLShaderInputBufferDesc ogl_desc;
        NXOGLBufferPtr_t ogl_buffer = getBuffer(it->hdl.gpuhdl);

        if (!ogl_buffer)
        {
            NXLogError("NXGPUInterfaceOGL::allocShaderInput: Invalid GPU buffer for binding %d",
                       it->binding_idx);
            return hdl;
        }

        ogl_desc.hdl = ogl_buffer->oglHdl();
        ogl_desc.offset = it->hdl.offset;
        ogl_desc.stride = it->stride;
        ogl_desc.binding_idx = it->binding_idx;
        ogl_bindings.push_back(ogl_desc);
    }

    // check index buffer
    const NXGPUBufferHdl& idx_buffer = input.indexBuffer();
    NXOGLBufferPtr_t ogl_idxbuffer = getBuffer(idx_buffer.gpuhdl);

    if (ogl_idxbuffer)
    {
        ogl_idx_name = ogl_idxbuffer->oglHdl();
    }


    // create ogl shader input
    NXOGLShaderInputPtr_t shader_input = NXOGLShaderInput::create(input.inputs(),
                                                                  ogl_bindings,
                                                                  ogl_idx_name);

    hdl = _hdlsVAO.add(kGPUResourceTypeShaderInput, shader_input);
    return hdl;
}

void
NXGPUInterfaceOGL::releaseShaderInput(const NXHdl& hdl)
{
    _hdlsVAO.remove(hdl);
}

void
NXGPUInterfaceOGL::bindProgram(const NXHdl& hdl)
{
    NXOGLProgramPtr_t prog;
    if (_hdlsProgram.get(hdl, prog))
    {
        glUseProgram(prog->oglHdl());
    }
    else
    {
        NXLogError("NXGPUInterfaceOGL::bindProgram: Invalid program hdl %x", (nx_u32)hdl);
    }
}

void
NXGPUInterfaceOGL::bindTexture(const NXHdl& hdl,
                               const nx_u32 idx)
{
    NXOGLTexturePtr_t tex;
    if (_hdlsTexture.get(hdl, tex))
    {
        tex->bind(idx);
    }
    else
    {
        NXLogError("NXGPUInterfaceOGL::bindTexture: Invalid program hdl %x", (nx_u32)hdl);
    }
}

void
NXGPUInterfaceOGL::bindShaderInput(const NXHdl& hdl)
{
    NXOGLShaderInputPtr_t vao;
    if(_hdlsVAO.get(hdl, vao))
    {
        vao->bind();
    }
    else
    {
        NXLogError("NXGPUInterfaceOGL::bindShaderInput: Invalid hdl %x", (nx_u32) hdl);
    }
}

bool
NXGPUInterfaceOGL::isValidTextureHdl(const NXHdl& hdl) const
{
    return _hdlsTexture.contains(hdl);
}

bool
NXGPUInterfaceOGL::isValidBufferHdl(const NXHdl& hdl) const
{
    return _hdlsBuffer.contains(hdl);
}

bool
NXGPUInterfaceOGL::isValidProgramHdl(const NXHdl& hdl) const
{
    return _hdlsProgram.contains(hdl);
}

bool
NXGPUInterfaceOGL::isValidShaderInput(const NXHdl& hdl) const
{
    return _hdlsVAO.contains(hdl);
}

int
NXGPUInterfaceOGL::uniformLocation(const NXHdl& proghdl,
                                   const char* uniform) const
{
    int result = -1;
    NXOGLProgramPtr_t prog;
    _hdlsProgram.get(proghdl, prog);
    if (prog)
    {
        result = glGetUniformLocation(prog->oglHdl(), uniform);
    }
    return result;
}

NXGPUBufferManagerInterface&
NXGPUInterfaceOGL::gpuBufferManager()
{
    return _bufferManager;
}

NXOGLProgramPtr_t
NXGPUInterfaceOGL::getProgram(const NXHdl& hdl) const
{
    NXOGLProgramPtr_t prog;
    _hdlsProgram.get(hdl, prog);
    return prog;
}

NXOGLBufferPtr_t
NXGPUInterfaceOGL::getBuffer(const NXHdl& hdl) const
{
    NXOGLBufferPtr_t buff;
    _hdlsBuffer.get(hdl, buff);
    return buff;
}

NXOGLTexturePtr_t
NXGPUInterfaceOGL::getTexture(const NXHdl& hdl) const
{
    NXOGLTexturePtr_t tex;
    _hdlsTexture.get(hdl, tex);
    return tex;
}

NXOGLShaderInputPtr_t
NXGPUInterfaceOGL::getShaderInput(const NXHdl& hdl) const
{
    NXOGLShaderInputPtr_t vao;
    _hdlsVAO.get(hdl, vao);
    return vao;
}

}
