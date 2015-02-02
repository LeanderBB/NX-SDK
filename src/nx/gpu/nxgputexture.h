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
#ifndef __NX_GPUTEXTURE_H__
#define __NX_GPUTEXTURE_H__

#include "nx/hdl/nxhdl.h"

namespace nx
{

enum GPUTextureType
{
    kGPUTextureType2D,
    kGPUTextureType3D,
    kGPUTextureType2DArray,
    kGPUTextureTypeCubeMap,
    // place holder, do not use
    kGPUTextureTypeTotal
};


enum GPUTextureFormat
{
    kGPUTextureFormatR8 = 0,
    kGPUTextureFormatR16,
    kGPUTextureFormatR32,
    kGPUTextureFormatR16F,
    kGPUTextureFormatR32F,
    kGPUTextureFormatRG8,
    kGPUTextureFormatRG16,
    kGPUTextureFormatRG32,
    kGPUTextureFormatRG16F,
    kGPUTextureFormatRG32F,
    kGPUTextureFormatRGB8,
    kGPUTextureFormatBGR8,
    kGPUTextureFormatRGBA8,
    kGPUTextureFormatBGRA8,
    kGPUTextureFormatRGB16,
    kGPUTextureFormatRGBA16,
    kGPUTextureFormatRGB32,
    kGPUTextureFormatRGBA32,
    kGPUTextureFormatRGB16F,
    kGPUTextureFormatRGBA16F,
    kGPUTextureFormatRGB32F,
    kGPUTextureFormatRGBA32F,
    kGPUTextureFormatR5G6B5,
    kGPUTextureFormatRGB5A1,
    kGPUTextureFormatRGBA4,
    kGPUTextureFormatSRGB8,
    kGPUTextureFormatSRGBA8,
    kGPUTextureFormatR8_SNORM,
    kGPUTextureFormatRG8_SNORM,
    kGPUTextureFormatRGB8_SNORM,
    kGPUTextureFormatRGBA8_SNORM,
    kGPUTextureFormatDepth16,
    kGPUTextureFormatDepth24,
    kGPUTextureFormatDepth32,
    kGPUTextureFormatDepth32F,
    kGPUTextureFormatDepth24Stencil8,
    // Compressed formats
    // ETC2 Compression
    kGPUTextureFormatRGB8_ETC2,
    kGPUTextureFormatRGBA8_ETC2,
    kGPUTextureFormatSRGB8_ETC2,
    kGPUTextureFormatSRGBA8_ETC2,
    kGPUTextureFormatRGB8_PTAETC2,
    kGPUTextureFormatSRGB8_PTAETC2,
    kGPUTextureFormatR11_EAC,
    kGPUTextureFormatRG11_EAC,
    kGPUTextureFormatR11S_EAC,
    kGPUTextureFormatRG11S_EAC,
    // ASTC Compression
    // note: ASTC is not tied to any texture size or number of channels.
    kGPUTextureFormatRGBA_ASTC_4x4,
    kGPUTextureFormatRGBA_ASTC_5x4,
    kGPUTextureFormatRGBA_ASTC_5x5,
    kGPUTextureFormatRGBA_ASTC_6x5,
    kGPUTextureFormatRGBA_ASTC_6x6,
    kGPUTextureFormatRGBA_ASTC_8x5,
    kGPUTextureFormatRGBA_ASTC_8x6,
    kGPUTextureFormatRGBA_ASTC_8x8,
    kGPUTextureFormatRGBA_ASTC_10x5,
    kGPUTextureFormatRGBA_ASTC_10x6,
    kGPUTextureFormatRGBA_ASTC_10x8,
    kGPUTextureFormatRGBA_ASTC_10x10,
    kGPUTextureFormatRGBA_ASTC_12x10,
    kGPUTextureFormatRGBA_ASTC_12x12,
    kGPUTextureFormatSRGBA8_ASTC_4x4,
    kGPUTextureFormatSRGBA8_ASTC_5x4,
    kGPUTextureFormatSRGBA8_ASTC_5x5,
    kGPUTextureFormatSRGBA8_ASTC_6x5,
    kGPUTextureFormatSRGBA8_ASTC_6x6,
    kGPUTextureFormatSRGBA8_ASTC_8x5,
    kGPUTextureFormatSRGBA8_ASTC_8x6,
    kGPUTextureFormatSRGBA8_ASTC_8x8,
    kGPUTextureFormatSRGBA8_ASTC_10x5,
    kGPUTextureFormatSRGBA8_ASTC_10x6,
    kGPUTextureFormatSRGBA8_ASTC_10x8,
    kGPUTextureFormatSRGBA8_ASTC_10x10,
    kGPUTextureFormatSRGBA8_ASTC_12x10,
    kGPUTextureFormatSRGBA8_ASTC_12x12,
    // Do not use, placeholder only
    kGPUTextureFormatTotal
};

enum GPUTexturePixelDataType
{
    kGPUTexturePixelDataTypeUnused,
    kGPUTexturePixelDataTypeByte,
    kGPUTexturePixelDataTypeUByte,
    kGPUTexturePixelDataTypeShort,
    kGPUTexturePixelDataTypeUShort,
    kGPUTexturePixelDataTypeInt,
    kGPUTexturePixelDataTypeUInt,
    kGPUTexturePixelDataTypeHFloat,
    kGPUTexturePixelDataTypeFloat
};


// Contains texture format info for a specific texture format
struct NXGPUTetureFormatDescription
{
    GPUTextureFormat format;
    unsigned normalized : 1; // is texture normalized between (-1,1)
    unsigned compressed : 1; // is texture compressed
    unsigned srgb : 1; // is texture of type srgb
    unsigned nBytesPerPixel: 5; // number of bytes per pixel
    unsigned nBitsRed : 8; // number of red bits
    unsigned nBitsGreen : 8; // number of green bits
    unsigned nBitsBlue : 8; // number of blue bits
    unsigned nBitsAlpha : 8; // number of alpha bits
    unsigned nBitsDepth : 8; // number of depth bits
    unsigned nBitsStencil : 8; // number of stencil bits
    unsigned blockSize : 8; // compressed block size
    unsigned blockWidth : 8; // compressed block width
    unsigned blockHeight : 8; // compressed block height
    unsigned blockDepth : 8; // compressed block depth
    GPUTexturePixelDataType colorType; // pixel data type for color channel
    GPUTexturePixelDataType depthType; // pixel data type for depth
    GPUTexturePixelDataType stencilType; // pixel data type for stencil
    const char* formatString;
};

NX_INLINE bool NXGPUTextureFormatBitsUnknown(const unsigned bits)
{
    return (bits & 0xFF) == 0xFF;
}

const NXGPUTetureFormatDescription* NXGPUTextureFormatGetDescription(const GPUTextureFormat format);

const NXGPUTetureFormatDescription *NXGPUTextureFormatGetDescriptions();

NX_INLINE int
NXGPUTextureRowStride(const struct NXGPUTetureFormatDescription* pInfo,
                      const int width)
{
    NX_ASSERT(pInfo);
    if (pInfo)
    {
        return pInfo->nBytesPerPixel * width;
    }
    return 0;
}

const char* NXGPUTextureTypeToStr(const GPUTextureType type);

struct NXGPUTextureDesc
{
    GPUTextureType type = kGPUTextureType2D;
    GPUTextureFormat format = kGPUTextureFormatRGB8;
    nx_u32 width = 64;
    nx_u32 height = 64;
    nx_u32 depth = 1;
    nx_u32 nArray = 0;
    nx_u32 nMipMap = 1;
};

class NXGPUTexture
{
public:


    NXGPUTexture():
        _desc(),
        _gpuHdl()
    {

    }

    nx_u32 width() const
    {
        return _desc.width;
    }

    nx_u32 height() const
    {
        return _desc.height;
    }

    nx_u32 depth() const
    {
        return _desc.depth;
    }

    nx_u32 nArraySlices() const
    {
        return _desc.nArray;
    }

    nx_u32 nMimpMaps() const
    {
        return _desc.nMipMap;
    }

    GPUTextureFormat format() const
    {
        return _desc.format;
    }

    GPUTextureType type() const
    {
        return _desc.type;
    }

    NXGPUTextureDesc& desc()
    {
        return _desc;
    }

    const NXGPUTextureDesc& desc() const
    {
        return _desc;
    }

    void setGpuHdl(const NXHdl hdl)
    {
        _gpuHdl = hdl;
    }

    NXHdl gpuHdl()
    {
        return _gpuHdl;
    }

protected:
    NXGPUTextureDesc _desc;
    NXHdl _gpuHdl; // handle to GPU resource
};

}

#endif

