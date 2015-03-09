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
#include "nx/media/nximage.h"
#include "nx/ogl/nxogl.h"
#include "nx/ogl/nxoglinternal.h"
#include "nx/ogl/nxogltexture.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"
namespace nx
{

struct NXKTXHeader
{
    nx_u8 identifier[12];
    nx_u32 endianness;
    nx_u32 glType;
    nx_u32 glTypeSize;
    nx_u32 glFormat;
    nx_u32 glInternalFormat;
    nx_u32 glBaseInternalFormat;
    nx_u32 pixelWidth;
    nx_u32 pixelHeight;
    nx_u32 pixelDepth;
    nx_u32 numberOfArrayElements;
    nx_u32 numberOfFaces;
    nx_u32 numberOfMipmapLevels;
    nx_u32 bytesOfKeyValueData;
};

static const nx_u8 gKTXIdentifier[] =
{
    0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
};


static GPUTextureFormat
NXGLFormatToGPUTextureFormat(const nx_u32 format,
                             const nx_u32 internal,
                             const bool compressed,
                             const nx_u32 type)
{
    GPUTextureFormat result = kGPUTextureFormatTotal;
    // We use sized values internal, convert to adjusted value
    nx_u32 internal_adjusted = internal;
    if (internal == GL_RGB)
    {
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            internal_adjusted = GL_RGB8;
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
            internal_adjusted = GL_RGB565;
            break;
        default:
            //invalid type return
            NX_ASSERT(false);
            return result;
        }
    }
    if (internal == GL_RGBA)
    {
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            internal_adjusted = GL_RGBA8;
            break;
        case GL_UNSIGNED_SHORT_5_5_5_1:
            internal_adjusted = GL_RGB5_A1;
            break;
        case GL_UNSIGNED_SHORT_4_4_4_4:
            internal_adjusted = GL_RGBA4;
            break;
        default:
            //invalid type return
            NX_ASSERT(false);
            return result;
        }
    }

    nx_u64 combined = ((nx_u64)format) << 32 | internal_adjusted;
    nx_u64 cur_combined;

    const NXOGLTextureDescription * p_descriptions = nxOGLTextureDescriptions();

    for (int i = 0; i < kGPUTextureFormatTotal; ++i)
    {
        const NXOGLTextureDescription* p_native = &p_descriptions[i];
        cur_combined = ((nx_u64)p_native->format) << 32 | p_native->internal;
        if ( cur_combined == combined)
        {
            const NXGPUTetureFormatDescription* p_info = NXGPUTextureFormatGetDescription(static_cast<GPUTextureFormat>(i));
            if (p_info)
            {
                if (compressed == p_info->compressed)
                {
                    return p_info->format;
                }
            }
        }
    }
    return result;
}

static bool
NXKTXValidateHeader(NXIOFile* file,
                    NXKTXHeader& hdr)
{
    memset(&hdr, 0, sizeof(hdr));

    // read header
    if (file->read(&hdr, sizeof(hdr)) != sizeof(hdr))
    {
        NXLogError("Failed to read KTX Header");
        return false;
    }

    // validate header
    if (memcmp(&hdr.identifier,&gKTXIdentifier, sizeof(gKTXIdentifier)) != 0)
    {
        NXLogError("Invalid KTX header identifier");
        return false;
    }

    if (hdr.endianness == 0x01020304)
    {
        NXLogError("Big Endian KTX files are not supported");
        return false;
    }

    if (hdr.numberOfFaces > 1)
    {
        NXLogError("CubeMaps are currently not supported");
        return false;
    }

    // print header
     NXLog("GLType               : 0x%04x", hdr.glType);
     NXLog("GLTypeSize           : %d", hdr.glTypeSize);
     NXLog("GLFormat             : 0x%04x", hdr.glFormat);
     NXLog("GLInternalFormat     : 0x%04x", hdr.glInternalFormat);
     NXLog("GLBaseInternalFormat : 0x%04x", hdr.glBaseInternalFormat);
     NXLog("Width                : %d", hdr.pixelWidth);
     NXLog("Height               : %d", hdr.pixelHeight);
     NXLog("Depth                : %d", hdr.pixelDepth);
     NXLog("nArrayElements       : %d", hdr.numberOfArrayElements);
     NXLog("nFaces               : %d", hdr.numberOfFaces);
     NXLog("nMipMapLevels        : %d", hdr.numberOfMipmapLevels);
     NXLog("nBytesOfKeyValueData : %d<", hdr.bytesOfKeyValueData);

    return true;
}

static bool
NXKTXConvert(const char* input,
             const char* output)
{
    (void) input;
    (void) output;

    std::unique_ptr<NXIOFile> finput(NXIOFile::open(input, kIOAccessModeReadBit));

    if (!finput)
    {
        return false;
    }

    NXKTXHeader ktx_hdr;

    if (!NXKTXValidateHeader(finput.get(), ktx_hdr))
    {
        return false;
    }

    // header okay, check formats
    bool is_compressed = ktx_hdr.glFormat == 0;

    GPUTextureFormat format = NXGLFormatToGPUTextureFormat((!is_compressed) ? ktx_hdr.glFormat: ktx_hdr.glBaseInternalFormat,
                                                           ktx_hdr.glInternalFormat,
                                                           is_compressed,
                                                           ktx_hdr.glType);
    if (format == kGPUTextureFormatTotal)
    {
        NXLogError("Format conversion failed");
        return false;
    }

    // open output file
    std::unique_ptr<NXIOFile> foutput(NXIOFile::open(output, kIOAccessModeWriteBit));
    if (!foutput)
    {
        return false;
    }

    // perform conversion ---

    nx_u32 max_width, max_height, max_depth;
    if (ktx_hdr.pixelHeight == 0)
    {
        max_height = 1;
    }
    else
    {
        max_height = ktx_hdr.pixelHeight;
    }
    if (ktx_hdr.pixelDepth == 0)
    {
        max_depth = 1;
    }
    else
    {
        max_depth = ktx_hdr.pixelDepth;
    }
    max_width = ktx_hdr.pixelWidth;

    // guess type
    GPUTextureType tex_type;

    if (ktx_hdr.pixelHeight == 0)
    {
        if (ktx_hdr.numberOfArrayElements == 0)
        {
            // emulate 1D tex as 2D tex with 1pix height
            tex_type= kGPUTextureType2D;
        }
        else
        {
            tex_type = kGPUTextureType2DArray;
        }
    }
    else if (ktx_hdr.pixelDepth == 0)
    {
        if (ktx_hdr.numberOfArrayElements == 0)
        {
            if (ktx_hdr.numberOfFaces == 1)
            {
                tex_type = kGPUTextureType2D;
            }
            else
            {
                //tex_type = ETextureType::kTexCubeMap;
                NXLogError("CubeMaps are currently not supported");
                return false;
            }
        }
        else
        {
            if (ktx_hdr.numberOfFaces <= 1)
            {
                tex_type = kGPUTextureType2DArray;
            }
            else
            {
                // Cube Map Array
                NXLogError("CubeMaps Arrays are currently not supported");
                return false;
            }
        }
    }
    else
    {
        tex_type = kGPUTextureType3D;
    }


    NXImageHeader img_hdr;
    memcpy(&img_hdr, &NXImage::sMagic[0], sizeof(NXImage::sMagic));
    img_hdr.format = format;
    img_hdr.originalFormat = format;
    img_hdr.type = tex_type;
    img_hdr.nArrayElem = (!ktx_hdr.numberOfArrayElements) ? 1 : ktx_hdr.numberOfArrayElements;
    img_hdr.nMipMaps = (!ktx_hdr.numberOfMipmapLevels) ? 1 : ktx_hdr.numberOfMipmapLevels;
    img_hdr.nAttr = 0;
    img_hdr.depth = max_depth;
    img_hdr.width = max_width;
    img_hdr.height = max_height;

    // write header

    if(foutput->write(&img_hdr, sizeof(img_hdr)) != sizeof(img_hdr))
    {
        NXLogError("Failed to write header to '%s'", output);
        return false;
    }

    // skip attributes

    finput->seek(ktx_hdr.bytesOfKeyValueData, kIOSeekOpCur);

    // allocate storage for the rest of the image
    size_t offset = finput->tell();
    size_t buffer_size = finput->size() - offset;
    NXScopedFree scoped_buffer(NXMalloc(buffer_size));
    const char* buffer = static_cast<const char*>(scoped_buffer.get());
    if (finput->read(scoped_buffer.get(), buffer_size) != buffer_size)
    {
        NXLogError("Failed to read ktx image into memory", output);
        return false;
    }

    // reset offset pointer to the beginning of the allocated buffer
    offset = 0;

    std::vector<NXImageMipMapHeader> mipmaps;
    std::vector<const void*> mipmaps_ptrs;
    mipmaps_ptrs.reserve(img_hdr.nMipMaps);
    mipmaps.reserve(img_hdr.nMipMaps);

    size_t img_offset = 0;
    // process each mipmap level & write mip map header
    for (nx_u32 level = 0; level < img_hdr.nMipMaps; ++level)
    {
        NX_ASSERT(offset < buffer_size);
        NXImageMipMapHeader& mipmap_hdr = mipmaps[level];
        mipmap_hdr.size = *reinterpret_cast<const nx_u32*>(&buffer[offset]);
        mipmap_hdr.width = std::max(1u, max_width >> level);
        mipmap_hdr.height = std::max(1u, max_height >> level);
        mipmap_hdr.depth = std::max(1u, max_depth >> level);

        mipmap_hdr.offset = img_offset;

        // write mipmap header
        if(foutput->write(&mipmap_hdr, sizeof(mipmap_hdr)) != sizeof(mipmap_hdr))
        {
            NXLogError("Failed to write mipmap header (%d) to '%s'", level, output);
            return false;
        }

        // update offset
        offset += sizeof(nx_u32);

        // set img offset

        mipmaps_ptrs[level] = buffer + offset;

        // calculate padding
        nx_u32 padding_size = 3 - ((mipmap_hdr.size + 3) % 4);

        // update size
        offset += mipmap_hdr.size + padding_size;
        img_offset = mipmap_hdr.size + padding_size;
    }

    // write each mipmap level
    for (nx_u32 level = 0; level < img_hdr.nMipMaps; ++level)
    {
        static const char s_padding[4] = {0,0,0,0};
        // write mipmap
        if (foutput->write(mipmaps_ptrs[level], mipmaps[level].size) != mipmaps[level].size)
        {
            NXLogError("Failed to write mipmap (%d) to '%s'", level, output);
            return false;
        }

        // calculate padding
        nx_u32 padding_size = 3 - ((mipmaps[level].size + 3) % 4);
        if (padding_size && foutput->write(s_padding, padding_size) != padding_size)
        {
            NXLogError("Failed to write mipmap padding (%d) to '%s'", level, output);
            return false;
        }
    }

    // finished
    return true;
}

}

int main(const int argc,
         const char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr,"Usage: %s  ktx_file nximg_output\n", argv[0]);
        return EXIT_FAILURE;
    }

    bool result = nx::NXKTXConvert(argv[1], argv[2]);
    nx::NXLogFlush();
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
