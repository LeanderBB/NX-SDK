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
#ifndef __NX_OGLTEXTURE_H__
#define __NX_OGLTEXTURE_H__

#include "nx/gpu/nxgputexture.h"
#include "nx/ogl/nxoglobj.h"
namespace nx
{

struct NXOGLTextureDescription
{
    nx_u32 format;
    nx_u32 internal;
    nx_u32 dataType;
    nx_u8  compressed;
    nx_u8  depthComponent;
    nx_u16 __padding;
};

const NXOGLTextureDescription* nxOGLTextureDescriptionForGPUTextureFormat(const GPUTextureFormat format);

const NXOGLTextureDescription* nxOGLTextureDescriptions();

nx_u32 NXOGLTextureType(const GPUTextureType type);

class NXImage;
class NXOGLTexture : public NXOGLObj
{

public:

    static NXTLSharedPtr<NXOGLTexture> create(const NXGPUTextureDesc& desc);

    static NXTLSharedPtr<NXOGLTexture> create(const NXImage& img);

    ~NXOGLTexture();

    const NXGPUTextureDesc& desc() const
    {
        return _desc;
    }

    nx_u32 oglType() const
    {
        return _oglType;
    }

    void bind(const nx_u32 unit) const;

    bool upload(const void* pData,
                const size_t size,
                const nx_u32 width,
                const nx_u32 height,
                const nx_u32 deptht,
                const nx_u32 mipLvl,
                const int unpackAlignment = 1);

    NXOGLTexture(const NXGPUTextureDesc& desc);

    nx_u32 width() const
    {
        return _desc.width;
    }

    nx_u32 height() const
    {
        return _desc.height;
    }

protected:

    bool allocateStorage();

private:
    NX_CPP_NO_COPY(NXOGLTexture);

protected:
    const NXGPUTextureDesc _desc;
    const nx_u32 _oglType;
};

typedef NXTLSharedPtr<NXOGLTexture> NXOGLTexturePtr_t;

/*
class NXOGLTextureBinder
{
public:

    explicit NXOGLTextureBinder(const NXOGLTexture* pTex);

    NXOGLTextureBinder(const NXOGLTexture* pTex,
                       const nx_u32 idx);

    ~NXOGLTextureBinder();

private:
    const NXOGLTexture* _pTex;
    nx_u32 _prevIdx;
    nx_u32 _prevHdl;
};
*/

}

#endif
