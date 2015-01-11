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


#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if defined (__GNUC__)
#pragma GCC diagnostic pop
#endif


namespace nx
{

typedef struct
{
    nx_u8* ptr;
    size_t size_input = 0;
    size_t size_output = 0;
    nx_u32 w; // width
    nx_u32 h; // # height
    nx_u32 d; // # depth
    nx_u32 n; // # components
    GPUTextureFormat format;
    nx_u32 row_stride;
    nx_u32 padding;
} NXImgFile;


static NXImgFile*
NXImgFileLoad(const char* path)
{
    int width, height, n_components;

    if (stbi_is_hdr(path))
    {
        NXLogError("Input '%s' is an HDR format!", path);
        return nullptr;
    }

    // load image file
    nx_u8* p_data = stbi_load(path, &width, &height, &n_components, 0);

    if (!p_data)
    {
        NXLogError("Failed to load '%s': %s", path, stbi_failure_reason());
        return nullptr;
    }

    NXImgFile* p_result = (NXImgFile*)NXMalloc(sizeof(NXImgFile));
    p_result->w = width;
    p_result->h = height;
    p_result->d = 1;
    p_result->n = n_components;
    p_result->ptr = p_data;
    switch(n_components)
    {
    case 1:
        p_result->format = kGPUTextureFormatR8;
        break;
    case 2:
        p_result->format = kGPUTextureFormatRG8;
        break;
    case 3:
        p_result->format = kGPUTextureFormatRGB8;
        break;
    case 4:
        p_result->format = kGPUTextureFormatRGBA8;

        break;
    default:
        stbi_image_free(p_data);
        NXFree(p_result);
        NXLogError("Invalid Image Components number: %d", n_components);
        return nullptr;
    }

    const NXGPUTetureFormatDescription* desc = NXGPUTextureFormatGetDescription(p_result->format);
    if (!desc)
    {
        stbi_image_free(p_data);
        NXFree(p_result);
        NXLogError("Could not find texture description for %x", p_result->format);
        return false;
    }

    const size_t row_stride = NXGPUTextureRowStride(desc, width);
    p_result->padding = 3 - ((row_stride + 4-1) % 4);
    p_result->row_stride = row_stride;
    p_result->size_output = (row_stride + p_result->padding) * p_result->h * p_result->d;
    p_result->size_input = row_stride * p_result->h * p_result->d;

    return p_result;
}

#define TMP_BUFFER_SIZE (64 * 1024)

static bool
NXWriteImgFileToOuput(NXIOFile* pOutput,
                      NXImgFile* pInput)
{

    nx_u8 * ptr = nullptr;
    size_t bytes_written = 0;
    size_t offset = 0;

    const nx_u8 padding[4] = {0};


    while (offset < pInput->size_input)
    {
        ptr = pInput->ptr + offset;

        bytes_written = pOutput->write(ptr, pInput->row_stride);
        if (bytes_written != pInput->row_stride)
        {
            NXLogError("Error writing mipmap contents to ouput");
            return false;
        }

        if (pInput->padding)
        {
            if(pOutput->write(padding, pInput->padding) != pInput->padding)
            {
                NXLogError("Error writing mipmap padding to ouput");
                return false;
            }
        }
        offset += pInput->row_stride;
    }

    return true;
}

static bool
NXGenerateImg(const int argc,
              const char** argv)
{

    bool result = false;
    std::vector<NXImgFile*> input_vec;
    std::vector<NXImageMipMapHeader> mipmaps;
    NXImageHeader nxhdr;
    NXIOFile* p_output = nullptr;

    size_t offset = 0;
    // open all input files, assuming they have already been sorted
    for (int i = 2; i < argc; ++i)
    {
        NXImgFile* astc_file = NXImgFileLoad(argv[i]);
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
        mipmaphdr.size = astc_file->size_output;

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
    nxhdr.format = input_vec[0]->format;
    nxhdr.originalFormat = kGPUTextureFormatTotal;
    nxhdr.depth = input_vec[0]->d;
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
        NXImgFile*& p_file = input_vec[i];

        NXImageMipMapHeader& mipmaphdr = mipmaps[i];

        if(!NXWriteImgFileToOuput(p_output, p_file))
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
        if (input->ptr)
        {
           stbi_image_free(input->ptr);
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
    if (argc < 3)
    {
        fprintf(stderr,"Usage: %s <output> <input 0> <input 1> ...<input n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    bool result = nx::NXGenerateImg(argc, argv);
    nx::NXLogFlush();
    return (result) ? EXIT_SUCCESS : EXIT_FAILURE;
}
