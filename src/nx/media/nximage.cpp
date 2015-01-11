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
#include "nx/nxcore.h"
#include "nx/media/nximage.h"
#include "nx/gpu/nxgputexture.h"
#include "nx/io/nxiobase.h"

namespace nx
{

const nx_u8 NXImage::sMagic[12] =
{
    0xAB, 'N', 'X', 'I', 'M', 'G', '1', '0', 0xBB, 0x0A, 0x1A, 0x0A
};



NXImage*
NXImage::load(NXIOBase* pIo)
{
    NXImageHeader hdr;
    NXImage* p_img = nullptr;
    if (pIo->read(&hdr, sizeof(hdr)) != sizeof(hdr))
    {
        NXLogError("NXImage: Failed to read NXImage header");
        return p_img;
    }

    if (memcmp(&hdr.magic, NXImage::sMagic, sizeof(NXImage::sMagic)) != 0)
    {
        NXLogError("NXImage: Input is not an NXImage file");
        return p_img;
    }


    // skip over attributes
    for (nx_u32 i = 0; i < hdr.nAttr; ++i)
    {
        NXImageAttributeHeader attr_hdr;
        if (pIo->read(&hdr, sizeof(hdr)) != sizeof(hdr))
        {
            NXLogError("NXImage: Failed to read NXImage Attribute header (%d)", i);
            return p_img;
        }

        pIo->seek(attr_hdr.size, kIOSeekOpCur);
    }


    // Read mipmap data
    MipMapInfoVec_t mipMapInfo;
    mipMapInfo.resize(hdr.nMipMaps);

    if (pIo->read(&mipMapInfo[0], sizeof(NXImageMipMapHeader) * hdr.nMipMaps)
            != sizeof(NXImageMipMapHeader) * hdr.nMipMaps)
    {
        NXLogError("Failed to read mip map info");
        return p_img;
    }


    // read all the data into memory
    size_t offset = pIo->tell();
    size_t data_size = pIo->size() - offset;
    void* ptr = NXMalloc(data_size);

    if (pIo->read(ptr, data_size) != data_size)
    {
        NXLogError("NXImage: Failed to read image data into memory");
        NXFree(ptr);
        return p_img;
    }


    p_img = new NXImage(hdr, ptr, data_size, mipMapInfo);
    return p_img;
}


void
NXImage::ImageHeaderToGPUTextureDescription(const NXImageHeader& hdr,
                                            NXGPUTextureDesc& desc)
{

    desc.depth = hdr.depth;
    desc.width = hdr.width;
    desc.height = hdr.height;
    desc.nArray = hdr.nArrayElem;
    desc.nMipMap = hdr.nMipMaps;
    desc.type = static_cast<GPUTextureType>(hdr.type);
    desc.format = static_cast<GPUTextureFormat>(hdr.format);
}

void
NXImage::ImageInfo(const NXImageHeader &hdr)
{
    const NXGPUTetureFormatDescription* p_format = NXGPUTextureFormatGetDescription(static_cast<GPUTextureFormat>(hdr.format));
    const NXGPUTetureFormatDescription* p_orig_format = NXGPUTextureFormatGetDescription(static_cast<GPUTextureFormat>(hdr.originalFormat));
    NXLog("  Width           : %d", hdr.width);
    NXLog("  Height          : %d", hdr.height);
    NXLog("  Depth           : %d", hdr.depth);
    NXLog("  Array Elements  : %d", hdr.nArrayElem);
    NXLog("  Mip Maps        : %d", hdr.nMipMaps);
    NXLog("  Format          : %s", (p_format) ? p_format->formatString : "Unknown");
    NXLog("  Format (Orig)   : %s", (p_orig_format) ? p_orig_format->formatString : "Unknown");
    NXLog("  Type            : %s", NXGPUTextureTypeToStr(static_cast<GPUTextureType>(hdr.type)));

    if (p_format)
    {
        NXLog("    Normalized    : %d", p_format->normalized);
        NXLog("    Compressed    : %d", p_format->compressed);
        NXLog("    SRGB          : %d", p_format->normalized);
        NXLog("    Red Bytes     : %d", p_format->nBitsRed);
        NXLog("    Green Bits    : %d", p_format->nBitsGreen);
        NXLog("    Blue Bits     : %d", p_format->nBitsBlue);
        NXLog("    Alpha Bits    : %d", p_format->nBitsAlpha);
        NXLog("    Depth Bits    : %d", p_format->nBitsDepth);
        NXLog("    Stencil Bits  : %d", p_format->nBitsStencil);
        if (p_format->compressed)
        {
            NXLog("    Block Size    : %d", p_format->blockSize);
            NXLog("    Block Width   : %d", p_format->blockWidth);
            NXLog("    Block Height  : %d", p_format->blockHeight);
            NXLog("    Block Depth   : %d", p_format->blockDepth);
        }
        else
        {
            NXLog("    Bytes / Pixel : %d", p_format->nBytesPerPixel);
        }
    }
}

NXImage::~NXImage()
{
    NX_ASSERT(_pData == nullptr);
}

nx_u32
NXImage::width() const
{
    return _hdr.width;
}

nx_u32
NXImage::height() const
{
    return _hdr.height;
}

nx_u32
NXImage::depth() const
{
    return _hdr.depth;
}

nx_u32
NXImage::nMipMaps() const
{
    return _hdr.nMipMaps;
}

nx_u32
NXImage::nArrayElements() const
{
    return _hdr.nArrayElem;
}

GPUTextureFormat
NXImage::textureFormat() const
{
    return static_cast<GPUTextureFormat>(_hdr.format);
}

GPUTextureFormat
NXImage::originalTextureFormat() const
{
    return static_cast<GPUTextureFormat>(_hdr.originalFormat);
}

GPUTextureType
NXImage::textureType() const
{
    return static_cast<GPUTextureType>(_hdr.type);
}

const NXImageMipMapHeader*
NXImage::mipMapInfo(const nx_u32 idx) const
{
    return (idx < _mipMapInfo.size()) ? &_mipMapInfo[idx] : nullptr;
}

const void*
NXImage::mipMapData(const nx_u32 idx) const
{
    return (idx < _mipMapInfo.size()) ? (const char*) _pData + _mipMapInfo[idx].offset : nullptr;
}

void
NXImage::unload()
{
    if(_pData)
    {
        NXFree(_pData);
        _pData = nullptr;
        _dataSize = 0;
        _mipMapInfo.clear();
    }
}

NXImage::NXImage(const NXImageHeader& hdr,
                 void* pData,
                 const size_t dataSize,
                 MipMapInfoVec_t& mipMapInfo):
    NXMediaItem(),
    _hdr(hdr),
    _pData(pData),
    _dataSize(dataSize),
    _mipMapInfo(std::move(mipMapInfo))
{

}


}
