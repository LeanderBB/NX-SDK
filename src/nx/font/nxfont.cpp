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
#include "nx/nxcore.h"
#include "nx/font/nxfont.h"
#include "nx/io/nxiobase.h"

namespace nx
{

const nx_u8 NXFont::sMagic[12] =
{
    0xAB, 'N', 'X', 'F', 'N', 'T', '1', '0', 0xBB, 0x0A, 0x1A, 0x0A
};


NXFont::NXFont():
    _hdr(),
    _metaData(),
    _imgBuffer(nullptr),
    _imgBufferSize(0)
{

}

NXFont::~NXFont()
{
    freeImgBuffer();
}

bool
NXFont::load(NXIOBase* pIO)
{
    // read header
    if (pIO->read(&_hdr, sizeof(_hdr)) != sizeof(_hdr))
    {
        NXLogError("NXFont::load: Failed to read font header");
        return false;
    }

    // validate header
    if (memcmp(_hdr.magic, sMagic, sizeof(sMagic) != 0))
    {
        NXLogError("NXFont::load: Input is not a valid font format");
        return false;
    }

    // read font data
    NXFontCharData data[kFontMaxMetaData];
    if (pIO->read(data, sizeof(data)) != sizeof(data))
    {
        NXLogError("NXFont::load: Failed to read font data");
        return false;
    }

    // calculate img size
    _imgBufferSize = _hdr.atlasDimensionsPx * _hdr.atlasDimensionsPx * 4;

    // alloc memory
    _imgBuffer = NXMalloc(_imgBufferSize);

    // read img
    if (pIO->read(_imgBuffer, _imgBufferSize) != _imgBufferSize)
    {
        NXLogError("NXFont::load: Failed to read font image");
        return false;
    }

    // convert data
    charToMetaData(_metaData, data, _hdr);



    return true;
}

const NXFontMetaData*
NXFont::metaData(const nx_u32 chr) const
{
    return (chr >= 32 && chr < 256) ? &_metaData[chr - 32] : nullptr;
    }

    void
    NXFont::freeImgBuffer()
    {
    if (_imgBuffer)
    {
    NXFree(_imgBuffer);
    _imgBuffer = nullptr;
    _imgBufferSize = 0;
}
}

void
NXFont::gpuTextureDesc(NXGPUTextureDesc& desc) const
{
    desc.depth = 1;
    desc.format = kGPUTextureFormatBGRA8;
    desc.height = _hdr.atlasDimensionsPx;
    desc.width = _hdr.atlasDimensionsPx;
    desc.nArray = 1;
    desc.type = kGPUTextureType2D;
    desc.nMipMap = 1;
}

void
NXFont::charToMetaData(NXFontMetaData metaData[kFontMaxMetaData],
                       const NXFontCharData chrData[kFontMaxMetaData],
                       const NXFontHdr hdr)
{
    for (nx_u32 i = 0; i < kFontMaxMetaData; ++i)
    {
        const NXFontCharData& d = chrData[i];
        if (d.chr > 32 && d.chr < 256)
        {
            int order = d.chr - 32;
            auto& meta = metaData[order];
            int col = order % hdr.atlasColumns;
            int row = order / hdr.atlasColumns;
            float x_min = (float)(col * hdr.slotGlyphSize) / (float)hdr.atlasDimensionsPx;
            float y_min = (float)(row * hdr.slotGlyphSize) / (float)hdr.atlasDimensionsPx;

            meta.xMin = x_min;
            meta.yMin = y_min;
            meta.width = (float)(d.glyphWidth + hdr.paddingPx) / (float)hdr.atlasGlyphPx;
            meta.height = (float)(d.glyphRow + hdr.paddingPx)  / (float)hdr.atlasGlyphPx;
            meta.yOffset = -((float)hdr.paddingPx - (float)d.glyphYMin) / (float)hdr.atlasGlyphPx;
        }
        else
        {
            NXLogWarning("NXFont::charToMetaData: Invalid chr %d, skipping...", d.chr);
        }
    }

    // add data for space
    auto& meta = metaData[0];
    meta.xMin = 0.0f;
    meta.width = 0.5f;
    meta.yMin = 0.0f;
    meta.height = 1.0f;
    meta.yOffset = 0.0f;
}

}
