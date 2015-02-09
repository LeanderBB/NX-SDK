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
#ifndef __NX_GPUINTERFACE_H__
#define __NX_GPUINTERFACE_H__


#include "nx/gpu/nxgpubuffer.h"
#include "nx/gpu/nxgputexture.h"
#include "nx/gpu/nxgpuprogramsource.h"
#include "nx/gpu/nxgpushaderinput.h"
#include "nx/gpu/nxgpubuffermanagerinterface.h"
namespace nx
{
class NXImage;
/**
 * Interface with GPU/Rendering API
 */
class NXGPUInterface
{
public:

    static NXGPUInterface* create();

    virtual bool init() = 0;

    virtual void shutdown() = 0;

    virtual ~NXGPUInterface(){}

    virtual NXHdl allocBuffer(const NXGPUBufferDesc& desc) = 0;

    virtual void releaseBuffer(const NXHdl& hdl) = 0;

    virtual NXHdl allocTexture(const NXGPUTextureDesc& desc) = 0;

    virtual NXHdl allocTexture(const NXImage& image) = 0;

    virtual void releaseTexture(const NXHdl& hdl) = 0;

    virtual NXHdl allocProgram(const NXGPUProgramSourceBase& source) = 0;

    virtual void releaseProgram(const NXHdl& hdl) = 0;

    virtual NXHdl allocShaderInput(const NXGPUShaderInput& input) = 0;

    virtual void releaseShaderInput(const NXHdl& hdl) = 0;

    virtual void bindProgram(const NXHdl& hdl) = 0;

    virtual void bindTexture(const NXHdl& hdl,
                             const nx_u32 idx) = 0;

    virtual void bindShaderInput(const NXHdl& hdl) = 0;

    virtual bool isValidTextureHdl(const NXHdl& hdl) const = 0;

    virtual bool isValidBufferHdl(const NXHdl& hdl) const = 0;

    virtual bool isValidProgramHdl(const NXHdl& hdl) const = 0;

    virtual bool isValidShaderInput(const NXHdl& hdl) const = 0;

    virtual int uniformLocation(const NXHdl& proghdl,
                                const char* uniform) const = 0;

    virtual NXGPUBufferManagerInterface& gpuBufferManager() = 0;

protected:
    NXGPUInterface() {}

private:
    NX_CPP_NO_COPY(NXGPUInterface);

};

}

#endif
