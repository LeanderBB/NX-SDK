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
#ifndef __NX_IMAGE_H__
#define __NX_IMAGE_H__

#include "nx/gpu/nxgputexture.h"
#include "nx/media/nxmediaitem.h"

namespace nx
{
/*
 * NXImage Layout
 *
 * NXImageHeader
 * for each attribute
 *    NXImageAttributeHeader
 *    Attribute Data
 *    padding
 * end
 * for each mipmap
 *    NXImageMipMapHeader
 * end
 * for each mipmap
 *      for each array
 *         for each depth
 *             for each row
 *                 for each pixel in column
 *                     pixel
 *                 end
 *                 padding[3 - ((row + 3) % 4)]
 *             end
 *         end
 *      end
 *      padding[3 - ((imageSize + 3) % 4)]
 * end
 */

struct NXImageHeader
{
    nx_u8  magic[12];
    nx_u32 format; // see GPUTextureFormat
    nx_u32 originalFormat;
    nx_u32 type; // set GPUTextureType;
    nx_u32 nArrayElem;
    nx_u32 nMipMaps;
    nx_u32 nAttr;
    nx_u32 width;
    nx_u32 height;
    nx_u32 depth;
};

struct NXImageMipMapHeader
{
    nx_u32 width = 0;
    nx_u32 height = 0;
    nx_u32 depth = 0;
    nx_u32 size = 0;
    nx_u32 offset = 0;
};

struct NXImageAttributeHeader
{
    nx_u32 attrId;
    nx_u32 size;
};


class NXIOBase;
class NXImage : public NXMediaItem
{
public:
    static const nx_u8 sMagic[12];

    static NXImage* load(NXIOBase* pIo);

    static void ImageHeaderToGPUTextureDescription(const NXImageHeader& hdr,
                                                   NXGPUTextureDesc& desc);

    static void ImageInfo(const NXImageHeader& hdr);

    ~NXImage();

    nx_u32 width() const;

    nx_u32 height() const;

    nx_u32 depth() const;

    nx_u32 nMipMaps() const;

    nx_u32 nArrayElements() const;

    GPUTextureFormat textureFormat() const;

    GPUTextureFormat originalTextureFormat() const;

    GPUTextureType textureType() const;

    const NXImageMipMapHeader* mipMapInfo(const nx_u32 idx) const;

    const void* mipMapData(const nx_u32 idx) const;

    const NXImageHeader& header() const
    {
        return _hdr;
    }

    virtual void unload();

protected:

    typedef std::vector<NXImageMipMapHeader> MipMapInfoVec_t;

    NXImage(const NXImageHeader& hdr,
            void* pData,
            const size_t dataSize,
            MipMapInfoVec_t& mipMapInfo);

private:
    NX_CPP_NO_COPY(NXImage);

protected:

    NXImageHeader _hdr;
    void* _pData;
    size_t _dataSize;
    MipMapInfoVec_t _mipMapInfo;

};


}

#endif
