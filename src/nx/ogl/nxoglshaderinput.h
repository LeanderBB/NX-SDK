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
#ifndef __NX_OGLSHADERINPUT_H__
#define __NX_OGLSHADERINPUT_H__

#include "nx/ogl/nxoglobj.h"
#include "nx/gpu/nxgpu.h"
#include "nx/gpu/nxgpushaderinput.h"
namespace nx
{

class NXGPUInterfaceOGL;

struct NXOGLShaderInputBufferDesc
{
    nx_u32 hdl = 0;
    nx_u32 offset = 0;
    nx_u16 stride = 0;
    nx_u8 binding_idx = 0;
    nx_u8 __pading = 0;
};

typedef std::vector<NXOGLShaderInputBufferDesc> NXOGLShaderInputBufferDescVec_t;


class NXGPUShaderInput;
class NXOGLShaderInput : public NXOGLObj
{
public:

    static NXTLSharedPtr<NXOGLShaderInput> create(const NXGPUShaderInputDescVec_t& inputs,
                                                  const NXOGLShaderInputBufferDescVec_t& buffers,
                                                  const nx_u32 indexBufferName = NX_OGL_HDL_INVALID);

    NXOGLShaderInput();

    ~NXOGLShaderInput();

    void bind();

private:
    NX_CPP_NO_COPY(NXOGLShaderInput);
};

typedef NXTLSharedPtr<NXOGLShaderInput> NXOGLShaderInputPtr_t;

}
#endif
