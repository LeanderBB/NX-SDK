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
#include "nx/ogl/nxogltexture.h"
#include "nx/media/nximage.h"
namespace nx
{
// -- NXOGLTexutre Binder-------------------------------------------------------
// According to OpenGL Spec
#define NX_MAX_ACTIVE_TEX_UNITS 32
#if !defined(NX_OS_ANDROID)
static __thread nx_u32 sActiveTexureList[NX_MAX_ACTIVE_TEX_UNITS][kGPUTextureTypeTotal] = {{0}};
static __thread nx_u32 sActiveIdx = 0;
#else
static nx_u32 sActiveTexureList[NX_MAX_ACTIVE_TEX_UNITS][kGPUTextureTypeTotal] = {{0}};
static nx_u32 sActiveIdx = 0;
#endif
static const GLenum sGLTextureTypes[] =
{
    GL_TEXTURE_2D,
    GL_TEXTURE_3D,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_CUBE_MAP
};





static void
NXBindTextureSafe(const nx_u32 hdl,
                  const nx_u32 type,
                  const nx_u32 idx)
{
    if (sActiveIdx != idx)
    {
        glActiveTexture(idx);
        sActiveIdx = idx;
    }
    if (sActiveTexureList[idx][type] != hdl)
    {
        glBindTexture(sGLTextureTypes[type], hdl);
        sActiveTexureList[idx][type] = hdl;
    }
}


static void
NXUnbindTextureSafe(const nx_u32 hdl,
                    const nx_u32 type,
                    const nx_u32 idx)
{

    if (sActiveTexureList[idx][type] != hdl)
    {
        glBindTexture(sGLTextureTypes[type], hdl);
        sActiveTexureList[idx][type] = hdl;
    }

    if (sActiveIdx != idx)
    {
        glActiveTexture(idx);
        sActiveIdx = idx;
    }
}

static void
NXBindTexture(const nx_u32 hdl,
              const nx_u32 type,
              const nx_u32 idx)
{
    NX_ASSERT(type < kGPUTextureTypeTotal);
    NX_ASSERT(idx < NX_MAX_ACTIVE_TEX_UNITS);

    glActiveTexture(idx);
    sActiveIdx = idx;
    glBindTexture(sGLTextureTypes[type], hdl);
    sActiveTexureList[idx][type] = hdl;
}

// -- NXOGLTexutre Formats------------------------------------------------------

#define NX_OGLTEXINFO(tex,format, internal, datatype, compressed) \
{ format, internal, datatype, compressed}
#define NX_OGLTEXINFO_UNCOMPRESSED(tex, format, internal, datatype) \
    NX_OGLTEXINFO(tex, format, internal, datatype, 0)
#define NX_OGLTEXINFO_COMPRESSED(tex, format, internal) \
    NX_OGLTEXINFO(tex, format, internal, GL_INVALID_ENUM, 1)

static const NXOGLTextureDescription gOGLTextureDescriptions [] =
{
    // Tex format internal datatype
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR8, GL_RED, GL_R8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR16, GL_RED_INTEGER, GL_R16UI, GL_UNSIGNED_SHORT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR32, GL_RED_INTEGER, GL_R32UI, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR16F, GL_RED, GL_R16F, GL_HALF_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR32F, GL_RED, GL_R32F, GL_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG8, GL_RG, GL_RG8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG16, GL_RG_INTEGER, GL_RG16UI, GL_UNSIGNED_SHORT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG32, GL_RG_INTEGER, GL_RG32UI, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG16F, GL_RG, GL_RG16F, GL_HALF_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG32F, GL_RG, GL_RG32F, GL_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB8, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatBGR8, GL_BGR, GL_RGB8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA8, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatBGRA8, GL_BGRA, GL_RGBA8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB16, GL_RGB_INTEGER, GL_RGB16UI, GL_UNSIGNED_SHORT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA16, GL_RGBA_INTEGER, GL_RGBA16UI, GL_UNSIGNED_SHORT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB32, GL_RGB_INTEGER, GL_RGB32UI, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA32, GL_RGBA_INTEGER, GL_RGBA32UI, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB16F, GL_RGB, GL_RGB16F, GL_HALF_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA16F, GL_RGBA, GL_RGBA16F, GL_HALF_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB32F, GL_RGB, GL_RGB32F, GL_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA32F, GL_RGBA, GL_RGBA32F, GL_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR5G6B5, GL_RGB, GL_RGB565, GL_UNSIGNED_SHORT_5_6_5),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB5A1, GL_RGBA, GL_RGB5_A1, GL_UNSIGNED_SHORT_5_5_5_1),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA4, GL_RGBA, GL_RGBA4, GL_UNSIGNED_SHORT_4_4_4_4),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatSRGB8, GL_RGB, GL_SRGB8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatSRGBA8, GL_RGBA, GL_SRGB8_ALPHA8, GL_UNSIGNED_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatR8_SNORM, GL_RED, GL_R8_SNORM, GL_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRG8_SNORM, GL_RG, GL_RG8_SNORM, GL_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB8_SNORM, GL_RGB, GL_RGB8_SNORM, GL_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA8_SNORM, GL_RGBA, GL_RGBA8_SNORM, GL_BYTE),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth16, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_UNSIGNED_SHORT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth24, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth32, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_UNSIGNED_INT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth32F, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F, GL_FLOAT),
    NX_OGLTEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth24Stencil8, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8),
    // Compressed
    // Tex format internal
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGB8_ETC2, GL_RGB, GL_COMPRESSED_RGB8_ETC2 ),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA8_ETC2, GL_RGBA, GL_COMPRESSED_RGBA8_ETC2_EAC),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGB8_ETC2, GL_SRGB, GL_COMPRESSED_SRGB8_ETC2),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ETC2, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGB8_PTAETC2, GL_RGBA, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGB8_PTAETC2, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatR11_EAC, GL_RED, GL_COMPRESSED_R11_EAC),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRG11_EAC, GL_RG, GL_COMPRESSED_RG11_EAC),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatR11S_EAC, GL_RED, GL_COMPRESSED_SIGNED_R11_EAC),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRG11S_EAC, GL_RG, GL_COMPRESSED_SIGNED_RG11_EAC),

    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_4x4, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_4x4_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_5x4, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_5x4_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_5x5, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_5x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_6x5, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_6x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_6x6, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_6x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_8x5, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_8x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_8x6, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_6x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_8x8, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_8x8_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_10x5, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_10x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_10x6, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_10x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_10x8, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_10x8_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_10x10, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_10x10_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_12x10, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_12x10_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatRGBA_ASTC_12x12, GL_RGBA, GL_COMPRESSED_RGBA_ASTC_12x10_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_4x4, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_5x4, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_5x5, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_6x5, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_6x6, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_8x5, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_8x6, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_8x8, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_10x5, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_10x6, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_10x8, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_10x10, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_12x10, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR),
    NX_OGLTEXINFO_COMPRESSED(kGPUTextureFormatSRGBA8_ASTC_12x12, GL_SRGB8_ALPHA8, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR),
};


nx_u32
NXOGLTextureType(const GPUTextureType type)
{
    switch(type)
    {
    case kGPUTextureType2D:
        return GL_TEXTURE_2D;
    case kGPUTextureType3D:
        return GL_TEXTURE_3D;
    case kGPUTextureType2DArray:
        return GL_TEXTURE_2D_ARRAY;
    case kGPUTextureTypeCubeMap:
        return GL_TEXTURE_CUBE_MAP;
    default:
        NX_ASSERT_UNREACHABLE();
        return GL_INVALID_ENUM;
    }
}




const NXOGLTextureDescription*
NXOGLTextureDescriptionForGPUTextureFormat(const GPUTextureFormat format)
{
    return (format < kGPUTextureFormatTotal) ? &gOGLTextureDescriptions[format] : nullptr;
}

const NXOGLTextureDescription* NXOGLTextureDescriptions()
{
    return &gOGLTextureDescriptions[0];
}

// -- NXOGLTexutre ------------------------------------------------------------

NXOGLTexture*
NXOGLTexture::create(const NXGPUTextureDesc& desc)
{
    NXOGLTexture* p_tex = new NXOGLTexture(desc);

    if (!p_tex->allocateStorage())
    {
        NX_SAFE_DELETE(p_tex);
    }
    return p_tex;
}

NXOGLTexture*
NXOGLTexture::create(const NXImage& img)
{
    NXOGLTexture* p_result = nullptr;

    NXGPUTextureDesc desc;
    NXImage::ImageHeaderToGPUTextureDescription(img.header(), desc);

    p_result = NXOGLTexture::create(desc);

    if (!p_result)
    {
        return nullptr;
    }


    for (nx_u32 i = 0; i < desc.nMipMap; ++i)
    {

        const NXImageMipMapHeader* p_mipi = img.mipMapInfo(i);
        const void* p_mip_data = img.mipMapData(i);
        if (!p_mipi || !p_mip_data)
        {
            NXLogError("OGLTexture: Mip map level %d not available for texture, max mip map level = %d",
                       i, desc.nMipMap);
            NX_SAFE_DELETE(p_result);
            break;
        }

        bool upload_result = false;
        upload_result = p_result->upload(p_mip_data, p_mipi->size, p_mipi->width,
                                         p_mipi->height, p_mipi->depth,
                                         i, 4);

        if (!upload_result)
        {
            NXLogError("OGLTexture: Failed to upload map level %d", i);
            NX_SAFE_DELETE(p_result);
        }
    }

    return p_result;
}

NXOGLTexture::~NXOGLTexture()
{
    if (oglHdlValid())
    {
        glDeleteTextures(1, &_oglhdl);
        oglHdlInvalidate();
    }
}

void
NXOGLTexture::use(const nx_u32 unit) const
{
    NXBindTexture(oglHdl(), _desc.type, unit);
}

bool
NXOGLTexture::upload(const void* pData,
                     const size_t size,
                     const nx_u32 width,
                     const nx_u32 height,
                     const nx_u32 depth,
                     const nx_u32 mipLvl,
                     const int unpackAlignment)
{

    NXOGLTextureBinder binder(this);

    GLint previousUnpack;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousUnpack);
    if (unpackAlignment != previousUnpack)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);
    }
    if (oglHdlValid())
    {
        // TODO: Use DSA extensions
        const NXOGLTextureDescription* p_info = NXOGLTextureDescriptionForGPUTextureFormat(_desc.format);
        NX_ASSERT(p_info);
        switch(_oglType)
        {
        case GL_TEXTURE_2D:
            if (p_info->compressed)
            {
                glCompressedTexSubImage2D(_oglType, mipLvl, 0, 0, width, height,
                                          p_info->internal, size, pData);
            }
            else
            {
                glTexSubImage2D(_oglType, mipLvl, 0, 0, width, height,
                                p_info->format,p_info->dataType, pData);
            }
            break;
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
            if (p_info->compressed)
            {
                glCompressedTexSubImage3D(_oglType, mipLvl, 0, 0, 0, width, height,
                                          depth, p_info->internal, size, pData);
            }
            else
            {
                glTexSubImage3D(_oglType, mipLvl, 0, 0, 0, width, height, depth,
                                p_info->format,p_info->dataType, pData);
            }
            break;
        case GL_TEXTURE_CUBE_MAP:
        {
            if (p_info->compressed)
            {
                NXLogError("OGLTexture: Compressed cubmaps not supported at the moment");
                return false;
            }

            const NXGPUTetureFormatDescription* p_tex_info = NXGPUTextureFormatGetDescription(_desc.format);
            size_t face_size = p_tex_info->nBytesPerPixel * width + height;
            for (int i = 0; i < 6; ++i)
            {
                glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipLvl, 0, 0, width,
                                height, p_info->format, p_info->dataType,
                                (const char*)pData + face_size);
            }
        }
            break;
        default:
            NX_ASSERT_UNREACHABLE();
            break;
        }
    }
    return true;

}

NXOGLTexture::NXOGLTexture(const NXGPUTextureDesc& desc):
    NXOGLObj(),
    _desc(desc),
    _oglType(NXOGLTextureType(desc.type))
{

}

bool
NXOGLTexture::allocateStorage()
{
    const NXOGLTextureDescription* p_info = NXOGLTextureDescriptionForGPUTextureFormat(_desc.format);
    if (!p_info)
    {
        const NXGPUTetureFormatDescription* p_texi = NXGPUTextureFormatGetDescription(_desc.format);
        NXLogError("OGLTexture: No native texture information for format %0x%x (%s)",
                   _desc.format, (p_texi) ? p_texi->formatString : nullptr);
        return false;
    }

    glGenTextures(1, &_oglhdl);

    NXOGLTextureBinder binder(this);

    switch(_oglType)
    {
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_2D:
        glTexStorage2D(_oglType, _desc.nMipMap, p_info->internal, _desc.width,
                       _desc.height);
        break;
    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
        glTexStorage3D(_oglType, _desc.nMipMap, p_info->internal, _desc.width,
                       _desc.height, _oglType == GL_TEXTURE_3D ? _desc.depth : _desc.nArray);
        break;
    default:
        NX_ASSERT_UNREACHABLE();
        return false;
    }

    // set filter params
    if (_desc.nMipMap > 1)
    {
        glTexParameteri(_oglType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    }
    else
    {
        glTexParameteri(_oglType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    // default magnification filter
    glTexParameteri(_oglType, GL_TEXTURE_MAG_FILTER , GL_LINEAR);


    // set default wrap mode
    glTexParameteri(_oglType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(_oglType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(_oglType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    return true;
}

NXOGLTextureBinder::NXOGLTextureBinder(const NXOGLTexture* pTex):
    _pTex(pTex),
    _prevIdx(sActiveIdx),
    _prevHdl(0)

{
    NX_ASSERT(_pTex->desc().type < kGPUTextureTypeTotal);
    NX_ASSERT(sActiveIdx < NX_MAX_ACTIVE_TEX_UNITS);

    _prevHdl = sActiveTexureList[sActiveIdx][_pTex->desc().type];
    NXBindTextureSafe(pTex->oglHdl(), _pTex->desc().type, sActiveIdx);
}

NXOGLTextureBinder::NXOGLTextureBinder(const NXOGLTexture* pTex,
                                       const nx_u32 idx):
    _pTex(pTex),
    _prevIdx(sActiveIdx),
    _prevHdl(0)

{
    NX_ASSERT(_pTex->desc().type < kGPUTextureTypeTotal);
    NX_ASSERT(idx < NX_MAX_ACTIVE_TEX_UNITS);

    _prevHdl = sActiveTexureList[sActiveIdx][_pTex->desc().type];
    NXBindTextureSafe(pTex->oglHdl(), _pTex->desc().type, idx);
}

NXOGLTextureBinder::~NXOGLTextureBinder()
{
    NXUnbindTextureSafe(_prevHdl, _pTex->desc().type, _prevIdx);
}


}
