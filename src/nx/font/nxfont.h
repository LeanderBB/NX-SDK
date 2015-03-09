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
#ifndef __NX_FONT_H__
#define __NX_FONT_H__


#include "nx/gpu/nxgputexture.h"

namespace nx
{


struct NXFontHdr
{
    nx_u8 magic[12] = {0};
    nx_i32 atlasColumns = 0;
    nx_i32 atlasDimensionsPx = 0;
    nx_i32 atlasGlyphPx = 0;
    nx_i32 slotGlyphSize = 0;
    nx_i32 paddingPx = 0;
};

struct NXFontCharData
{
    nx_u32 chr;
    nx_i32 glyphWidth;
    nx_i32 glyphRow;
    nx_i32 glyphYMin;
};

struct NXFontMetaData
{
    float xMin = 0.0f;
    float width = 0.0f;
    float yMin = 0.0f;
    float height = 0.0f;
    float yOffset = 0.0f;
};

enum
{
    kFontMaxMetaData = 224
};

class NXIOBase;
class NXFont
{
public:

    static const nx_u8 sMagic[12];

    NXFont();

    ~NXFont();

    bool load(NXIOBase* pIO);

    const NXFontMetaData* metaData(const nx_u32 chr) const;

    void freeImgBuffer();

    void gpuTextureDesc(NXGPUTextureDesc& desc) const;

    const NXFontHdr& hdr() const
    {
        return _hdr;
    }

    size_t imgBufferSize() const
    {
        return _imgBufferSize;
    }

    const void* imgBuffer() const
    {
        return _imgBuffer;
    }


    static void charToMetaData(NXFontMetaData metaData[kFontMaxMetaData],
                               const NXFontCharData chrData[kFontMaxMetaData],
                               const NXFontHdr hdr);

private:
    NXFontHdr _hdr;
    NXFontMetaData _metaData[kFontMaxMetaData];
    void* _imgBuffer;
    size_t _imgBufferSize;
};

}

#endif
