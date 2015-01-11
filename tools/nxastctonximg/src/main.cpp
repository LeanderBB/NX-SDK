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
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"
#include "nx/util/nxstrutil.h"
namespace nx
{

typedef struct
{
    nx_u8 magic[4];
    nx_u8 blockdim_x;
    nx_u8 blockdim_y;
    nx_u8 blockdim_z;
    nx_u8 xsize[3];   /* x-size = xsize[0] + xsize[1] + xsize[2] */
    nx_u8 ysize[3];   /* x-size, y-size and z-size are given in texels */
    nx_u8 zsize[3];   /* block count is inferred */
} NXASTCHdr;

static const nx_u8 sASTCMagic[4] = { 0x13, 0xAB, 0xA1, 0x5C};

typedef struct
{
    FILE* file = nullptr;
    size_t size = 0;
    nx_u32 w;
    nx_u32 h;
    nx_u32 d;
} NXASTCFile;


typedef struct
{
    GPUTextureFormat format = kGPUTextureFormatTotal;
    bool srgb = false;
} NXASTCExtraInfo;


static bool
NXASTCValidate(const NXASTCHdr& hdr)
{
    return (memcmp(&hdr.magic, &sASTCMagic, sizeof(sASTCMagic)) == 0);
}

#define NX_INFO_OFFSET_SRGB 3
#define NX_INFO_OFFSET_BLOCK_W 5
#define NX_INFO_OFFSET_BLOCK_H 8
#define NX_INFO_OFFSET_BLOCK_X 7

static bool
NXASTCExtraInfoParse(NXASTCExtraInfo& info, const char* str)
{
    // Expected format dd:s:bbxbb
    // d 8, 16, 32, channel depth
    // s srgb 1, else 0
    // b encoding block size

    if (strlen(str) != strlen("dd:s:bbxbb"))
    {
        NXLogError("Invalid info '%s'", str);
        return false;
    }

    if (str[NX_INFO_OFFSET_SRGB] == '1')
    {
        info.srgb = true;
    }

    NXString sbw(str + NX_INFO_OFFSET_BLOCK_W,2);
    NXString sbh(str + NX_INFO_OFFSET_BLOCK_H,2);

    nx_u32 bw = 0, bh = 0;
    if (!NXStrToU32(sbw.c_str(), bw) ||!NXStrToU32(sbh.c_str(), bh))
    {
        NXLogError("Invalid block info, values are not integers '%s'", str);
        return false;
    }

    switch(bw)
    {
    case 4:
        if (bh == 4)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_4x4 : kGPUTextureFormatSRGBA8_ASTC_4x4;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    case 5:
        if (bh == 4)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_5x4 : kGPUTextureFormatSRGBA8_ASTC_5x4;
        }
        else if (bh == 5)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_5x5 : kGPUTextureFormatSRGBA8_ASTC_5x5;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    case 6:
        if (bh == 5)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_6x5 : kGPUTextureFormatSRGBA8_ASTC_6x5;
        }
        else if (bh == 6)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_6x6 : kGPUTextureFormatSRGBA8_ASTC_6x6;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    case 8:
        if (bh == 5)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_8x5 : kGPUTextureFormatSRGBA8_ASTC_8x5;
        }
        else if (bh == 6)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_8x6 : kGPUTextureFormatSRGBA8_ASTC_8x6;
        }
        else if (bh == 8)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_8x8 : kGPUTextureFormatSRGBA8_ASTC_8x8;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    case 10:
        if (bh == 5)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_10x5 : kGPUTextureFormatSRGBA8_ASTC_10x5;
        }
        else if (bh == 6)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_10x6 : kGPUTextureFormatSRGBA8_ASTC_10x6;
        }
        else if (bh == 8)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_10x8 : kGPUTextureFormatSRGBA8_ASTC_10x8;
        }
        else if(bh == 10)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_10x10 : kGPUTextureFormatSRGBA8_ASTC_10x10;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    case 12:
        if (bh == 10)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_12x10 : kGPUTextureFormatSRGBA8_ASTC_12x10;
        }
        else if(bh == 12)
        {
            info.format = !info.srgb ? kGPUTextureFormatRGBA_ASTC_12x12 : kGPUTextureFormatSRGBA8_ASTC_12x12;
        }
        else
        {
            NXLogError("Invalid block height '%s'", str);
            return false;
        }
        break;
    default:
        NXLogError("Invalid block width '%s'", str);
        return false;
    }

    return true;
}

static void
NXASTCGetSize(const NXASTCHdr& hdr,
              NXASTCFile* pInfo)
{
    // Taken from the Mali OpenGL ES SDK
    /* Merge x,y,z-sizes from 3 chars into one integer value. */
    nx_u32 xsize = hdr.xsize[0] + (hdr.xsize[1] << 8) + (hdr.xsize[2] << 16);
    nx_u32 ysize = hdr.ysize[0] + (hdr.ysize[1] << 8) + (hdr.ysize[2] << 16);
    nx_u32 zsize = hdr.zsize[0] + (hdr.zsize[1] << 8) + (hdr.zsize[2] << 16);

    /* Compute number of blocks in each direction. */
    nx_u32 xblocks = (xsize + hdr.blockdim_x - 1) / hdr.blockdim_x;
    nx_u32 yblocks = (ysize + hdr.blockdim_y - 1) / hdr.blockdim_y;
    nx_u32 zblocks = (zsize + hdr.blockdim_z - 1) / hdr.blockdim_z;

    /* Each block is encoded on 16 bytes, so calculate total compressed image data size. */
    pInfo->size = xblocks * yblocks * zblocks << 4;
    pInfo->w = xsize;
    pInfo->h = ysize;
    pInfo->d = zsize;
}

static NXASTCFile*
NXASTCOpen(const char* path)
{
    FILE* file = fopen(path, "rb");

    if (!file)
    {
        NXLogError("Failed to open '%s'", path);
        return nullptr;
    }

    NXASTCHdr hdr;

    if (fread(&hdr, 1, sizeof(NXASTCHdr), file) != sizeof(NXASTCHdr))
    {
        fclose(file);
        NXLogError("Failed to read astc header for '%s'", path);
        return nullptr;
    }

    if (!NXASTCValidate(hdr))
    {
        fclose(file);
        NXLogError("Invalid astc header for '%s'", path);
        return nullptr;
    }

    NXASTCFile* p_result = (NXASTCFile*)NXMalloc(sizeof(NXASTCFile));
    NXASTCGetSize(hdr, p_result);
    p_result->file = file;
    return p_result;
}

#define TMP_BUFFER_SIZE (64 * 1024)

static bool
NXWriteASTCToOuput(NXIOFile* pOutput,
                   NXASTCFile* pInput)
{
    char buffer[TMP_BUFFER_SIZE];
    size_t bytes_read = 0;
    size_t bytes_written = 0;
    while (!feof(pInput->file))
    {
        bytes_read = fread(buffer, 1, TMP_BUFFER_SIZE, pInput->file);
        if (!bytes_read)
        {
            if (ferror(pInput->file))
            {
                NXLogError("Error reading mipmap contents");
                return false;
            }
        }
        else
        {
            bytes_written = pOutput->write(buffer, bytes_read);
            if (bytes_written != bytes_read)
            {
                NXLogError("Error writing mipmap contents to ouput");
                return false;
            }
        }
    }

    return true;
}

static bool
NXGenerateImg(const int argc,
              const char** argv)
{

    bool result = false;
    std::vector<NXASTCFile*> input_vec;
    std::vector<NXImageMipMapHeader> mipmaps;
    NXImageHeader nxhdr;
    NXIOFile* p_output = nullptr;
    NXASTCExtraInfo extra_info;

    if (!NXASTCExtraInfoParse(extra_info, argv[2]))
    {
        return false;
    }

    size_t offset = 0;
    // open all input files, assuming they have already been sorted
    for (int i = 3; i < argc; ++i)
    {
        NXASTCFile* astc_file = NXASTCOpen(argv[i]);
        if(!astc_file)
        {
            goto cleanup;
        }

        // fill mipmap info
        NXImageMipMapHeader mipmaphdr;

        mipmaphdr.depth = astc_file->d;
        mipmaphdr.width = astc_file->w;
        mipmaphdr.height = astc_file->h;
        mipmaphdr.offset = offset;
        mipmaphdr.size = astc_file->size;

        // calculate padding
        nx_u32 padding_size = 3 - ((mipmaphdr.size + 3) % 4);

        // update size
        offset += mipmaphdr.size + padding_size;

        mipmaps.push_back(mipmaphdr);
        input_vec.push_back(astc_file);
    }

    // fill the header
    //TODO: Support more types!!!
    memcpy(&nxhdr.magic, &NXImage::sMagic, sizeof(nxhdr.magic));
    nxhdr.type = kGPUTextureType2D;
    nxhdr.format = extra_info.format;
    nxhdr.originalFormat = kGPUTextureFormatTotal;
    nxhdr.depth = 1;
    nxhdr.nArrayElem = 1;
    nxhdr.width = input_vec[0]->w;
    nxhdr.height = input_vec[0]->h;
    nxhdr.nMipMaps = input_vec.size();
    nxhdr.nAttr = 0;

    // open output file
    p_output = NXIOFile::open(argv[1], kIOAccessModeWriteBit);
    if (!p_output)
    {
        NXLogError("Failed to open '%s'",argv[1]);
        goto cleanup;
    }

    // writhe nximg hdr
    if (p_output->write(&nxhdr, sizeof(nxhdr)) != sizeof(nxhdr))
    {
        NXLogError("Failed to write header to '%s'",argv[1]);
        goto cleanup;
    }

    // write each mipmap hdr
    for (nx_u32 i = 0; i < nxhdr.nMipMaps; ++i)
    {
        NXImageMipMapHeader& mipmaphdr = mipmaps[i];

        if(p_output->write(&mipmaphdr, sizeof(mipmaphdr)) != sizeof(mipmaphdr))
        {
            NXLogError("Failed to write mipmap header (%d) to '%s'",i,argv[1]);
            goto cleanup;
        }
    }

    // write each mipmap into the file

    for (nx_u32 i = 0; i < nxhdr.nMipMaps; ++i)
    {
        NXASTCFile*& p_file = input_vec[i];

        NXImageMipMapHeader& mipmaphdr = mipmaps[i];

        if(!NXWriteASTCToOuput(p_output, p_file))
        {
            NXLogError("Failed to write mipmap (%d) to '%s'",i,argv[1]);
            goto cleanup;
        }
        // calculate padding
        nx_u32 padding_size = 3 - ((mipmaphdr.size + 3) % 4);
        static const nx_u8 padding[4] = {0};

        if (padding_size)
        {
            if (p_output->write(padding, padding_size) != padding_size)
            {
                NXLogError("Failed to write padding for mipmap (%d) to '%s'",i,argv[1]);
            }
        }
    }

    // finished
    result = true;
cleanup:
    for(auto input : input_vec)
    {
        if (input->file)
        {
            fclose(input->file);
        }
        NXFree(input);
    }

    if (p_output)
    {
        delete p_output;
    }

    return result;
}



}

int main(const int argc,
         const char** argv)
{
    if (argc < 4)
    {
        fprintf(stderr,"Usage: %s  <output> <info> <input 0> <input 1> ...<input n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    bool result = nx::NXGenerateImg(argc, argv);
    nx::NXLogFlush();
    return (result) ? EXIT_SUCCESS : EXIT_FAILURE;
}
