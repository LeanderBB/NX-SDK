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
#include "nx/ogl/nxoglinternal.h"
#include "nx/ogl/nxoglrendertarget.h"

namespace nx
{

bool
nxOGLRTValidDepthAttachment(const GPUTextureFormat format)
{

    const NXGPUTetureFormatDescription* tex_desc = NXGPUTextureFormatGetDescription(format);
    if (!tex_desc)
    {
        return false;
    }

    return tex_desc->nBitsDepth > 0;
}

bool
nxOGLRTValidStencilAttachment(const GPUTextureFormat format)
{
    const NXGPUTetureFormatDescription* tex_desc = NXGPUTextureFormatGetDescription(format);
    if (!tex_desc)
    {
        return false;
    }
    return tex_desc->nBitsStencil > 0;
}

bool
nxOGLRTValidColorAttachment(const GPUTextureFormat format)
{
    const NXGPUTetureFormatDescription* tex_desc = NXGPUTextureFormatGetDescription(format);
    if (!tex_desc)
    {
        return false;
    }

    if (tex_desc->nBitsStencil || tex_desc->nBitsDepth || tex_desc->compressed)
    {
        return false;
    }

    return true;
}


NXTLSharedPtr<NXOGLRenderTarget>
NXOGLRenderTarget::create(const NXGPURenderTargetDesc& desc)
{
    NXOGLRenderTargetPtr_t rt;
    // check depth
    if (desc.attachments[kGPURTAttachmentDepth].format != kGPUTextureFormatTotal)
    {
        if (!nxOGLRTValidDepthAttachment(desc.attachments[kGPURTAttachmentDepth].format))
        {
            NXLogError("NXOGLRenderTarget::create: Invalid depth format");
            return rt;
        }
    }

    // check stencil
    if (desc.attachments[kGPURTAttachmentStencil].format != kGPUTextureFormatTotal)
    {
        if (!nxOGLRTValidStencilAttachment(desc.attachments[kGPURTAttachmentStencil].format))
        {
            NXLogError("NXOGLRenderTarget::create: Invalid depth format");
            return rt;
        }
    }

    // check color
    for (unsigned i = kGPURTAttachmentColor0; i < kGPURTAttachmentColorMax; ++i)
    {
        if (desc.attachments[i].format != kGPUTextureFormatTotal)
        {
            if (!nxOGLRTValidColorAttachment(desc.attachments[i].format))
            {
                NXLogError("NXOGLRenderTarget::create: Invalid color format for color attachment %u)", i - kGPURTAttachmentColor0);
                return rt;
            }
        }
    }

    // create textures

    NXOGLTexturePtr_t textures[kGPURTAttachmentColorMax];

    for (unsigned i = 0; i < kGPURTAttachmentColorMax; ++i)
    {

        if (i == kGPURTAttachmentStencil &&
                desc.attachments[kGPURTAttachmentDepth].format == kGPUTextureFormatDepth24Stencil8)
        {
            // skip stencil texture creation since it already included
            // in the depth texture
            continue;
        }

        if (desc.attachments[i].format != kGPUTextureFormatTotal)
        {
            NXGPUTextureDesc tex_desc;
            tex_desc.depth = 1;
            tex_desc.nArray = 1;
            tex_desc.nMipMap = 1;
            tex_desc.width = desc.attachments[i].width;
            tex_desc.height = desc.attachments[i].height;
            tex_desc.format = desc.attachments[i].format;
            tex_desc.type = kGPUTextureType2D;

            textures[i] = NXOGLTexture::create(tex_desc);
            if (!textures[i])
            {
                NXLogError("Failed to create texture for attachment %u)", i);
                return rt;
            }
        }
    }

    rt = nxMakeTLShared<NXOGLRenderTarget>(desc);

    // copy textures
    for (int i = 0; i < kGPURTAttachmentColorMax; ++i)
    {
        rt->_attachments[i] = textures[i];
    }

    // attach textures and validate
    rt->attachTextures();

    if (!rt->isComplete())
    {
        rt.reset();
    }

    return rt;
}

NXTLSharedPtr<NXOGLRenderTarget>
NXOGLRenderTarget::create(const NXOGLRenderTargetDesc& desc)
{
    NXOGLRenderTargetPtr_t rt;
    NXGPURenderTargetDesc rtdesc;
    // check depth
    if (desc.attachments[kGPURTAttachmentDepth].format != kGPUTextureFormatTotal)
    {
        if (!nxOGLRTValidDepthAttachment(desc.attachments[kGPURTAttachmentDepth].format))
        {
            NXLogError("NXOGLRenderTarget::create: Invalid depth format");
            return rt;
        }

        NXGPURTAttachmentDesc& attch = rtdesc.attachments[kGPURTAttachmentDepth];
        attch.format = desc.attachments[kGPURTAttachmentDepth].format;
        attch.width = desc.attachments[kGPURTAttachmentDepth].tex->width();
        attch.height = desc.attachments[kGPURTAttachmentDepth].tex->height();
    }

    // check stencil
    if (desc.attachments[kGPURTAttachmentStencil].format != kGPUTextureFormatTotal)
    {
        if (!nxOGLRTValidStencilAttachment(desc.attachments[kGPURTAttachmentStencil].format))
        {
            NXLogError("NXOGLRenderTarget::create: Invalid depth format");
            return rt;
        }

        NXGPURTAttachmentDesc& attch = rtdesc.attachments[kGPURTAttachmentStencil];
        attch.format = desc.attachments[kGPURTAttachmentStencil].format;
        attch.width = desc.attachments[kGPURTAttachmentStencil].tex->width();
        attch.height = desc.attachments[kGPURTAttachmentStencil].tex->height();

    }

    // check color
    for (unsigned i = kGPURTAttachmentColor0; i < kGPURTAttachmentColorMax; ++i)
    {
        if (desc.attachments[i].format != kGPUTextureFormatTotal)
        {
            if (!nxOGLRTValidColorAttachment(desc.attachments[i].format))
            {
                NXLogError("NXOGLRenderTarget::create: Invalid color format for color attachment %u)", i - kGPURTAttachmentColor0);
                return rt;
            }

            NXGPURTAttachmentDesc& attch = rtdesc.attachments[i];
            attch.format = desc.attachments[i].format;
            attch.width = desc.attachments[i].tex->width();
            attch.height = desc.attachments[i].tex->height();
        }
    }

    rt = nxMakeTLShared<NXOGLRenderTarget>(rtdesc);

    // copy textures
    for (int i = 0; i < kGPURTAttachmentColorMax; ++i)
    {
        rt->_attachments[i] = desc.attachments[i].tex;
    }

    // attach textures and validate
    rt->attachTextures();

    if (!rt->isComplete())
    {
        rt.reset();
    }

    return rt;

}

NXOGLRenderTarget::NXOGLRenderTarget(const NXGPURenderTargetDesc& desc):
    NXOGLObj(),
    _desc(desc),
    _attachments()
{
    glCreateFramebuffers(1, &_oglhdl);
}

NXOGLRenderTarget::~NXOGLRenderTarget()
{
    glDeleteFramebuffers(1, &_oglhdl);
    oglHdlInvalidate();
}

NXOGLTexturePtr_t
NXOGLRenderTarget::attachment(const GPURTAttachment attachment) const
{
    return attachment < kGPURTAttachmentColorMax ? _attachments[attachment] : NXOGLTexturePtr_t();
}

bool
NXOGLRenderTarget::isComplete() const
{
    GLenum result = glCheckNamedFramebufferStatus(oglHdl(), GL_DRAW_FRAMEBUFFER);
    switch(result)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        NXLogError("NXOGLRenderTarget::isComplete: Framebuffer attachment points are framebuffer incomplete");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        NXLogError("NXOGLRenderTarget::isComplete: Framebuffer must have at least one color attachment");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        NXLogError("NXOGLRenderTarget::isComplete: GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        NXLogError("NXOGLRenderTarget::isComplete: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER");
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        NXLogError("NXOGLRenderTarget::isComplete: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        NXLogError("NXOGLRenderTarget::isComplete: Value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        NXLogError("NXOGLRenderTarget::isComplete: Framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target");
        return false;
    default:
        NXLogError("NXOGLRenderTarget::isComplete: Unknown framebuffer check status %x", result);
        return false;
    }
}

void
NXOGLRenderTarget::enableColorAttachments(const GPURTAttachment* pAttach,
                                          const int count)
{
    GLenum colors[kGPURTAttachmentColorMax - kGPURTAttachmentColor0] = {GL_NONE};
    int n = 0;
    for (int i = 0; i < count; ++i)
    {
        if (pAttach[i]> kGPURTAttachmentStencil && pAttach[i] < kGPURTAttachmentColorMax)
        {
            colors[n] = GL_COLOR_ATTACHMENT0 + (pAttach[i] - kGPURTAttachmentColor0);
            ++n;
        }
        glNamedFramebufferDrawBuffers(_oglhdl, n, colors);
    }
}

void
NXOGLRenderTarget::disableColorAttachments()
{
    const GLenum colors = GL_NONE;
    glNamedFramebufferDrawBuffers(_oglhdl, 1, &colors);
}


void
NXOGLRenderTarget::bindForRead() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _oglhdl);
}

void
NXOGLRenderTarget::bindForWrite() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _oglhdl);
}

void
NXOGLRenderTarget::bindForReadWrite() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _oglhdl);
}

void
NXOGLRenderTarget::attachTextures()
{

    if (_attachments[kGPURTAttachmentDepth])
    {
        GLenum depth_attachment = GL_DEPTH_ATTACHMENT;
        if(_desc.attachments[kGPURTAttachmentDepth].format == kGPUTextureFormatDepth24Stencil8)
        {
            depth_attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        }
        glNamedFramebufferTexture(_oglhdl, depth_attachment,
                                  _attachments[kGPURTAttachmentDepth]->oglHdl(), 0);
    }

    if (_attachments[kGPURTAttachmentStencil] &&
            _desc.attachments[kGPURTAttachmentDepth].format != kGPUTextureFormatDepth24Stencil8)
    {
        glNamedFramebufferTexture(_oglhdl, GL_STENCIL_ATTACHMENT,
                                  _attachments[kGPURTAttachmentStencil]->oglHdl(), 0);
    }

    for (unsigned i = kGPURTAttachmentColor0; i < kGPURTAttachmentColorMax; ++i)
    {
        if (_attachments[i])
        {
            glNamedFramebufferTexture(_oglhdl, GL_COLOR_ATTACHMENT0 + (i - kGPURTAttachmentColor0),
                                      _attachments[i]->oglHdl(), 0);
        }
    }
}

}
