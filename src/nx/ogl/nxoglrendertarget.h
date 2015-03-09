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
#ifndef __NX_OGLRENDERTARGET_H__
#define __NX_OGLRENDERTARGET_H__

#include "nx/ogl/nxoglobj.h"
#include "nx/gpu/nxgpurendertarget.h"
#include "nx/ogl/nxogltexture.h"
namespace nx
{


bool nxOGLRTValidDepthAttachment(const GPUTextureFormat format);

bool nxOGLRTValidStencilAttachment(const GPUTextureFormat format);

bool nxOGLRTValidColorAttachment(const GPUTextureFormat format);

class NXOGLRenderTarget;

struct NXOGLRTAttachment
{
    NXOGLTexturePtr_t tex;
    GPUTextureFormat format = kGPUTextureFormatTotal;
};

struct NXOGLRenderTargetDesc
{
    NXOGLRTAttachment attachments[kGPURTAttachmentColorMax];
};

class NXOGLRenderTarget  : public NXOGLObj
{
public:

    static NXTLSharedPtr<NXOGLRenderTarget> create(const NXGPURenderTargetDesc& desc);

    static NXTLSharedPtr<NXOGLRenderTarget> create(const NXOGLRenderTargetDesc& desc);

    explicit NXOGLRenderTarget(const NXGPURenderTargetDesc& desc);

    ~NXOGLRenderTarget();

    NXOGLTexturePtr_t attachment(const GPURTAttachment attachment) const;

    const NXGPURenderTargetDesc& desc() const
    {
        return _desc;
    }

    bool isComplete() const;

    void enableColorAttachments(const GPURTAttachment* pAttach,
                                const int count);

    void disableColorAttachments();

    void bindForRead() const;

    void bindForWrite() const;

    void bindForReadWrite() const;

private:
    NX_CPP_NO_COPY(NXOGLRenderTarget);

    void attachTextures();

protected:
    NXGPURenderTargetDesc _desc;
    NXOGLTexturePtr_t _attachments[kGPURTAttachmentColorMax];
};

typedef NXTLSharedPtr<NXOGLRenderTarget> NXOGLRenderTargetPtr_t;

}
#endif
