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
#ifndef __NX_GPUINTERFACEOGL_H__
#define __NX_GPUINTERFACEOGL_H__


#include "nx/gpu/nxgpuinterface.h"
#include "nx/hdl/nxhdlmanager.h"
#include "nx/ogl/nxoglobj.h"
#include "nx/ogl/nxogltexture.h"
#include "nx/ogl/nxoglprogram.h"
#include "nx/ogl/nxoglbuffer.h"
#include "nx/ogl/nxoglshaderinput.h"

namespace nx
{
class NXGPUInterfaceOGL : public NXGPUInterface
{
public:
    NXGPUInterfaceOGL();

    virtual ~NXGPUInterfaceOGL();

    bool init() NX_CPP_OVERRIDE;

    void shutdown() NX_CPP_OVERRIDE;

    NXHdl allocBuffer(const NXGPUBufferDesc& desc,
                      const void *pData = nullptr) NX_CPP_OVERRIDE;

    void releaseBuffer(const NXHdl& hdl) NX_CPP_OVERRIDE;

    NXHdl allocTexture(const NXGPUTextureDesc& desc) NX_CPP_OVERRIDE;

    NXHdl allocTexture(const NXImage& image) NX_CPP_OVERRIDE;

    void releaseTexture(const NXHdl& hdl) NX_CPP_OVERRIDE;

    NXHdl allocProgram(const NXGPUProgramSourceBase& source) NX_CPP_OVERRIDE;

    void releaseProgram(const NXHdl& hdl) NX_CPP_OVERRIDE;

    NXHdl allocShaderInput(const NXGPUShaderInput& input) NX_CPP_OVERRIDE;

    void releaseShaderInput(const NXHdl& hdl) NX_CPP_OVERRIDE;

    void bindProgram(const NXHdl& hdl) NX_CPP_OVERRIDE;

    void bindTexture(const NXHdl& hdl,
                     const nx_u32 idx) NX_CPP_OVERRIDE;

    void bindShaderInput(const NXHdl& hdl) NX_CPP_OVERRIDE;

    bool isValidTextureHdl(const NXHdl& hdl) const NX_CPP_OVERRIDE;

    bool isValidBufferHdl(const NXHdl& hdl) const NX_CPP_OVERRIDE;

    bool isValidProgramHdl(const NXHdl& hdl) const NX_CPP_OVERRIDE;

    bool isValidShaderInput(const NXHdl& hdl) const NX_CPP_OVERRIDE;

    int uniformLocation(const NXHdl& proghdl,
                        const char* uniform) const NX_CPP_OVERRIDE;


    NXOGLProgramPtr_t getProgram(const NXHdl& hdl) const;

    NXOGLBufferPtr_t getBuffer(const NXHdl& hdl) const;

    NXOGLTexturePtr_t getTexture(const NXHdl& hdl) const;

    NXOGLShaderInputPtr_t getShaderInput(const NXHdl& hdl) const;


protected:
    NXHandleManager<NXOGLBufferPtr_t, 256> _hdlsBuffer;
    NXHandleManager<NXOGLProgramPtr_t, 256> _hdlsProgram;
    NXHandleManager<NXOGLTexturePtr_t, 256> _hdlsTexture;
    NXHandleManager<NXOGLShaderInputPtr_t, 256> _hdlsVAO;

};
}
#endif
