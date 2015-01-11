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
#include "nx/gpu/nxgputexture.h"

namespace nx
{

#define NX_TEXINFO(format, norm, comp, srgb, bytesPixel, bitRed, bitGreen, bitBlue, bitAlpha, bitDepth, bitStencil, blockSize, blockWidth, blockHeight, blockDepth, color, depth, stencil) \
{ format, norm, comp, srgb, bytesPixel, bitRed, bitGreen, bitBlue, bitAlpha, bitDepth, bitStencil, blockSize, blockWidth, blockHeight, blockDepth, color, depth, stencil, #format }
#define NX_TEXINFO_UNCOMPRESSED(format, norm, srgb, bytesPixel, bitRed, bitGreen, bitBlue, bitAlpha, bitDepth, bitStencil, color, depth, stencil) \
    NX_TEXINFO(format, norm, 0, srgb, bytesPixel, bitRed, bitGreen, bitBlue, bitAlpha, bitDepth, bitStencil, 0, 0, 0, 0, color, depth, stencil)
static const struct NXGPUTetureFormatDescription sTextureFormatInfo[kGPUTextureFormatTotal] =
{
    // format norm? srgb? bytes bitR bitG bitB bitA bitD bitS color depth stencil
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR8, 0, 0, 1, 8, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR16, 0, 0, 2, 16, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR32, 0, 0, 4, 32, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR16F, 0, 0, 2, 16, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeHFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR32F, 0, 0, 4, 32, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG8, 0, 0, 2, 8, 8, 0, 0, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG16, 0, 0, 4, 16, 16, 0, 0, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG32, 0, 0, 8, 32, 32, 0, 0, 0, 0, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG16F, 0, 0, 4, 16, 16, 0, 0, 0, 0, kGPUTexturePixelDataTypeHFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG32F, 0, 0, 8, 32, 32, 0, 0, 0, 0, kGPUTexturePixelDataTypeFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB8, 0, 0, 3, 8, 8, 8, 0, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatBGR8, 0, 0, 3, 8, 8, 8, 0, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA8, 0, 0, 4, 8, 8, 8, 8, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatBGRA8, 0, 0, 4, 8, 8, 8, 8, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB16, 0, 0, 6, 16, 16, 16, 0, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA16, 0, 0, 8, 16, 16, 16, 16, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB32, 0, 0, 12, 32, 32, 32, 0, 0, 0, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA32, 0, 0, 16, 32, 32, 32, 32, 0, 0, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB16F, 0, 0, 6, 16, 16, 16, 0, 0, 0, kGPUTexturePixelDataTypeHFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA16F, 0, 0, 8, 16, 16, 16, 16, 0, 0, kGPUTexturePixelDataTypeHFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB32F, 0, 0, 12, 32, 32, 32, 0, 0, 0, kGPUTexturePixelDataTypeFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA32F, 0, 0, 16, 32, 32, 32, 32, 0, 0, kGPUTexturePixelDataTypeFloat, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR5G6B5, 0, 0, 2, 5, 6, 5, 0, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB5A1, 0, 0, 2, 5, 5, 5, 1, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA4, 0, 0, 2, 4, 4, 4, 4, 0, 0, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatSRGB8, 0, 1, 3, 8, 8, 8, 0, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatSRGBA8, 0, 1, 4, 8, 8, 8, 8, 0, 0, kGPUTexturePixelDataTypeUByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatR8_SNORM, 1, 0, 1, 8, 0, 0, 0, 0, 0, kGPUTexturePixelDataTypeByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRG8_SNORM, 1, 0, 2, 8, 8, 0, 0, 0, 0, kGPUTexturePixelDataTypeByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGB8_SNORM, 1, 0, 3, 8, 8, 8, 0, 0, 0, kGPUTexturePixelDataTypeByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatRGBA8_SNORM, 1, 0, 4, 8, 8, 8, 8, 0, 0, kGPUTexturePixelDataTypeByte, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth16, 0, 0, 2, 0, 0, 0, 0, 16, 0, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth24, 0, 0, 3, 0, 0, 0, 0, 24, 0, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth32, 0, 0, 4, 0, 0, 0, 0, 32, 0, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth32F, 0, 0, 4, 0, 0, 0, 0, 32, 0, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeFloat, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO_UNCOMPRESSED(kGPUTextureFormatDepth24Stencil8, 0, 0, 4, 0, 0, 0, 0, 24, 8, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUInt, kGPUTexturePixelDataTypeUByte),
    // Compressed
    // format                                        norm?  comp?   srgb?   bytes   bitR    bitG    bitB    bitA    bitD    bitS    blockS  blockW  blockH  blockD  color                           depth                           stencil
    NX_TEXINFO(kGPUTextureFormatRGB8_ETC2,           0,     1,      0,      0,      8,      8,      8,      0,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA8_ETC2,          0,     1,      0,      0,      8,      8,      8,      8,      0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGB8_ETC2,          0,     1,      1,      0,      8,      8,      8,      0,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ETC2,         0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGB8_PTAETC2,        0,     1,      0,      0,      8,      8,      8,      1,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGB8_PTAETC2,       0,     1,      1,      0,      8,      8,      8,      1,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatR11_EAC,             0,     1,      0,      0,      11,     0,      0,      0,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeUByte,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRG11_EAC,            0,     1,      0,      0,      11,     11,     0,      0,      0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatR11S_EAC,            0,     1,      0,      0,      11,     0,      0,      0,      0,      0,      8,      4,      4,      1,      kGPUTexturePixelDataTypeByte,   kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRG11S_EAC,           0,     1,      0,      0,      11,     11,     0,      0,      0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeShort,  kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_4x4,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_5x4,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     5,      4,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_5x5,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     5,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_6x5,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     6,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_6x6,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     6,      6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_8x5,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     8,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_8x6,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     8,      6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_8x8,       0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     8,      8,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_10x5,      0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     10,     5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_10x6,      0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     10,     6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_10x8,      0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     10,     8,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_10x10,     0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     10,     10,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_12x10,     0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     12,     10,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatRGBA_ASTC_12x12,     0,     1,      0,      0,      255,    255,    255,    255,    0,      0,      16,     12,     12,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_4x4,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     4,      4,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_5x4,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     5,      4,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_5x5,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     5,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_6x5,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     6,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_6x6,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     6,      6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_8x5,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     8,      5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_8x6,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     8,      6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_8x8,     0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     8,      8,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_10x5,    0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     10,     5,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_10x6,    0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     10,     6,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_10x8,    0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     10,     8,      1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_10x10,   0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     10,     10,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_12x10,   0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     12,     10,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused),
    NX_TEXINFO(kGPUTextureFormatSRGBA8_ASTC_12x12,   0,     1,      1,      0,      8,      8,      8,      8,      0,      0,      16,     12,     12,     1,      kGPUTexturePixelDataTypeUShort, kGPUTexturePixelDataTypeUnused, kGPUTexturePixelDataTypeUnused)

};

static const char* sTextureTypeStrings[] =
{
    "kGPUTextureType2D",
    "kGPUTextureType3D",
    "kGPUTextureType2DArray",
    "kGPUTextureTypeCubeMap"
};


const NXGPUTetureFormatDescription*
NXGPUTextureFormatGetDescriptions()
{
    return &sTextureFormatInfo[0];
}

const NXGPUTetureFormatDescription*
NXGPUTextureFormatGetDescription(const GPUTextureFormat format)
{
    const struct NXGPUTetureFormatDescription* p_result = (format < kGPUTextureFormatTotal) ? &sTextureFormatInfo[format] : nullptr;
#if defined(NX_DEBUG)
    if (p_result && p_result->format != format)
    {
        NXLogError("GPUTextureFormatDescription: Format requested is 0x%x, but returned format is 0x%x",
                   format, p_result->format);
    }
#endif
    return p_result;
}

const char*
NXGPUTextureTypeToStr(const GPUTextureType type)
{
    return (type < kGPUTextureTypeTotal) ? sTextureTypeStrings[type] : "Unknown";
}


}
