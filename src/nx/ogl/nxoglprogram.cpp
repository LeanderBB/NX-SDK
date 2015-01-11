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
#include "nx/ogl/nxoglprogram.h"
#include "nx/gpu/nxgpuprogramsource.h"
namespace nx
{


static nx_u32
NXOGLCompileShader(const GPUShaderType type,
                   const char* src)
{
    GLenum shader_type;
    switch (type)
    {
    case kGPUShaderTypeVertex:
        shader_type = GL_VERTEX_SHADER;
        break;
    case kGPUShaderTypeFragment:
        shader_type = GL_FRAGMENT_SHADER;
        break;
    case kGPUShaderTypeGeometry:
        shader_type = GL_GEOMETRY_SHADER;
        break;
    case kGPUShaderTypeTessControl:
        shader_type = GL_TESS_CONTROL_SHADER;
        break;
    case kGPUShaderTypeTessEval:
        shader_type = GL_TESS_EVALUATION_SHADER;
        break;
    case kGPUShaderTypeCompute:
        shader_type = GL_COMPUTE_SHADER;
        break;
    default:
        NXLogError("OGLCompileShader: Unknown shader type %x ", type);
        return 0;
    }

    // set source
    nx_u32 hdl = glCreateShader(shader_type);
    glShaderSource(hdl, 1, &src, nullptr);

    // compile
    glCompileShader(hdl);

    // verify
    GLint compile_status = 0;
    glGetShaderiv(hdl, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_TRUE)
    {
        return hdl;
    }
    else
    {
        // failed to compile shader
        GLsizei length, written;
        glGetShaderiv(hdl, GL_INFO_LOG_LENGTH, &length);
        if (length > 0)
        {
            char* p_shader_log = (char*)NXMalloc(length + 1);
            glGetShaderInfoLog(hdl, length, &written, p_shader_log);
            p_shader_log[length] = '\0';
            NXLogError("OGLCompileShader: Failed to compile shader type %x:\n%s\n",
                       type, p_shader_log);
            NXFree(p_shader_log);
        }
        glDeleteShader(hdl);
        return 0;
    }
}

NXOGLProgram*
NXOGLProgram::create(const NXGPUProgramSourceBase *pSource)
{
    NXOGLProgram* p_prog = nullptr;

    nx_u32 shaders[kGPUShaderTypeTotal];
    bool success_build = true;

    // compile all sources
    for (int i = 0; i < kGPUShaderTypeTotal && success_build; ++i)
    {
        GPUShaderType shader_type = static_cast<GPUShaderType>(i);
        const char* shader_source = pSource->sourceForType(shader_type);
        if (shader_source)
        {
            shaders[i] = NXOGLCompileShader(shader_type, pSource->sourceForType(shader_type));
            if (shaders[i] == 0)
            {
                success_build = false;
            }
        }
        else
        {
            shaders[i] = 0;
        }
    }

    // create program only if all shaders compiled successfully
    if (success_build)
    {
        p_prog = new NXOGLProgram();

        // attach shaders
        for (int i = 0; i < kGPUShaderTypeTotal; ++i)
        {
            if (shaders[i])
            {
                glAttachShader(p_prog->oglHdl(), shaders[i]);
            }
        }

        // link
        glLinkProgram(p_prog->oglHdl());

        // check result
        GLint r = 0;
        glGetProgramiv(p_prog->oglHdl(), GL_LINK_STATUS, &r);
        if (!(r == GL_TRUE))
        {
            // get program log
            GLsizei len, written;
            glGetProgramiv(p_prog->oglHdl(), GL_INFO_LOG_LENGTH, &len);
            if (len > 0)
            {
                char* p_program_log = (char*)NXMalloc(len + 1);
                glGetProgramInfoLog(p_prog->oglHdl(), len, &written, p_program_log);
                p_program_log[len] = '\0';
                NXLogError("OGLProgram: Failed to link program:\n%s\n", p_program_log);
                NXFree(p_program_log);
            }
#if defined(NX_DEBUG) || defined(NX_GPUPROGRAM_LOG_EXTENSIVE_ON_ERROR)
            p_prog->logInfo();
#endif
            // delete program on failure
            NX_SAFE_DELETE(p_prog);
        }

    }

    // delete all shaders
    for (int i = 0; i < kGPUShaderTypeTotal; ++i)
    {
        if (shaders[i] != 0)
        {
            glDeleteShader(shaders[i]);
        }
    }

    return p_prog;
}

void
NXOGLProgram::logInfo() const
{
    int params = -1;
    int i;

    NXLog("--------------------\nOGLProgram %i info:", oglHdl());
    glGetProgramiv (oglHdl(), GL_LINK_STATUS, &params);
    NXLog("GL_LINK_STATUS = %i", params);

    glGetProgramiv (oglHdl(), GL_ATTACHED_SHADERS, &params);
    NXLog("GL_ATTACHED_SHADERS = %i", params);

    glGetProgramiv (oglHdl(), GL_ACTIVE_ATTRIBUTES, &params);
    NXLog("GL_ACTIVE_ATTRIBUTES = %i", params);

    for (i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib (oglHdl(), i, max_length, &actual_length, &size, &type, name);
        if (size > 1) {
            int j;
            for (j = 0; j < size; j++) {
                char long_name[64];
                int location;

                sprintf (long_name, "%s[%i]", name, j);
                location = glGetAttribLocation (oglHdl(), long_name);
                NXLog("  %i) type:%s name:%s location:%i",
                      i, NXOGLTypeToString(type), long_name, location);
            }
        } else {
            int location = glGetAttribLocation (oglHdl(), name);
            NXLog("  %i) type:%s name:%s location:%i",
                  i, NXOGLTypeToString(type), name, location);
        }
    }

    glGetProgramiv (oglHdl(), GL_ACTIVE_UNIFORMS, &params);
    NXLog("GL_ACTIVE_UNIFORMS = %i", params);
    for (i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform (oglHdl(), i, max_length, &actual_length, &size, &type, name);
        if (size > 1) {
            int j;
            for (j = 0; j < size; j++) {
                char long_name[64];
                int location;

                sprintf (long_name, "%s[%i]", name, j);
                location = glGetUniformLocation (oglHdl(), long_name);
                NXLog("  %i) type:%s name:%s location:%i",
                      i, NXOGLTypeToString(type), long_name, location);
            }
        } else {
            int location = glGetUniformLocation (oglHdl(), name);
            NXLog("  %i) type:%s name:%s location:%i",
                  i, NXOGLTypeToString(type), name, location);
        }
    }
}


NXOGLProgram::NXOGLProgram():
    NXOGLObj()
{
    _oglhdl = glCreateProgram();
    NX_ASSERT(_oglhdl);
}

NXOGLProgram::~NXOGLProgram()
{
    glDeleteProgram(_oglhdl);
    oglHdlInvalidate();
}

}

