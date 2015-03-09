//
// This file is part of the NX Project
//
// Copyright (c) 2014-2015 Leander Beernaert
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
#ifndef __NX_GPURENDERTARGET_H__
#define __NX_GPURENDERTARGET_H__

#include "nx/gpu/nxgpu.h"
#include "nx/gpu/nxgputexture.h"
namespace nx
{

struct NXGPURTAttachmentDesc
{
    nx_u16 width = 0;
    nx_u16 height = 0;
    GPUTextureFormat format = kGPUTextureFormatTotal;
};

struct NXGPURenderTargetDesc
{
    NXGPURTAttachmentDesc attachments[kGPURTAttachmentColorMax];
};


class NXGPURenderTarget
{
public:
    NXGPURenderTarget(const NXGPURenderTargetDesc& desc);

    const NXGPURTAttachmentDesc* attachment(const GPURTAttachment idx) const;

    NXHdl gpuHdl() const
    {
        return _gpuHdl;
    }

    void setGPUHdl(const NXHdl hdl)
    {
        _gpuHdl = hdl;
    }

private:
    NXGPURenderTargetDesc _desc;
    NXHdl _gpuHdl;
};

}
#endif
