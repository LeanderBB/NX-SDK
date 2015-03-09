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
#include "nx/os/nxargs.h"
#include "nx/os/nxpath.h"
#include "nx/util/nxstrutil.h"
#include "nx/font/nxfont.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"

#include <ft2build.h>
#include <ftglyph.h>

#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#if defined (__GNUC__)
#pragma GCC diagnostic pop
#endif


enum Option
{
    kOptionInput = 1,
    kOptionOutput,
    kOptionDebugOuput,
};

class FreeTypeScope
{
public:

    FreeTypeScope(FT_Library& ft):
        _ft(ft)
    {

    }

    ~FreeTypeScope()
    {
        FT_Done_FreeType(_ft);
    }

private:
    FT_Library& _ft;
};

struct NXFontOptions
{
    int atlas_dimension_px = 1024;
    int atlas_columns = 16;
    int padding_px = 6;
    int slot_glyph_size = 64;
    int atlas_glyph_px = 64;
    int atlas_glyph_padded_px = 64 - 6;
};

static const char* gs_help_str = "nxfontconverter: Convert otf and ttf to engine readable format";


static bool
makeDirs(const char* output,
         const char* debug)
{
    if (!nx::NXPath::isDirectory(output))
    {
        if (!nx::NXPath::mkdir(output))
        {
            nx::NXLogError("Could not create output directory '%s'", output);
            return false;
        }
    }

    if (output != debug)
    {
        if (!nx::NXPath::isDirectory(debug))
        {
            if (!nx::NXPath::mkdir(debug))
            {
                nx::NXLogError("Could not create debug output directory '%s'", debug);
                return false;
            }
        }
    }

    return true;
}


int main(const int argc,
         const char** argv)
{
    // setup args and parse
    nx::NXArgs args;
    args.add(kOptionInput, 'i',"input","Input font file", nx::kArgFlagSingleValue | nx::kArgFlagRequired);
    args.add(kOptionOutput, 'o', "output", "Ouput directory to write data", nx::kArgFlagSingleValue | nx::kArgFlagRequired);
    args.add(kOptionDebugOuput, 'd', "debug-output", "Ouput directory to write debug data", nx::kArgFlagSingleValue);

    if (args.parse(argc, argv) < 0)
    {
        args.printHelp(gs_help_str);
        return EXIT_FAILURE;
    }

    if (args.isSet(nx::KArgsOptionHelp))
    {
        args.printHelp(gs_help_str);
        return EXIT_SUCCESS;
    }

    // input font
    const char* font_input = args.getSingleArg(kOptionInput);

    nx::NXString input_name;
    if (!nx::NXPath::basenameNoExt(input_name, font_input))
    {
        nx::NXLogWarning("Failed to acquire font base name, using 'atlas' as output name");
        input_name = "atlas";
    }

    // output files
    const char* output_dir = args.getSingleArg(kOptionOutput);
    const char* output_debug_dir = (args.isSet(kOptionDebugOuput) ? args.getSingleArg(kOptionDebugOuput) : output_dir);


    if (!makeDirs(output_dir, output_debug_dir))
    {
        return EXIT_FAILURE;
    }

    // output dirs
    nx::NXString output_png = nx::NXPath::join(output_debug_dir, input_name);
    output_png += ".png";
    nx::NXString output_meta = nx::NXPath::join(output_debug_dir, input_name);
    output_meta += ".meta";
    nx::NXString output_meta_gen = nx::NXPath::join(output_debug_dir, input_name);
    output_meta_gen += "_gen.meta";
    nx::NXString output_nxfont = nx::NXPath::join(output_dir, input_name);
    output_nxfont += ".nxfont";

    // init freetype
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        nx::NXLogError("Failed to init FreeType");
        return EXIT_FAILURE;
    }
    // scoped guard
    FreeTypeScope  ft_scope(ft);

    // open font
    FT_Face face;
    if (FT_New_Face(ft, font_input, 0, &face))
    {
        nx::NXLogError("Failed to load font '%s'", font_input);
        return EXIT_FAILURE;
    }

    // create storage
    NXFontOptions atlas_options;
    const size_t atlas_buffer_size = (atlas_options.atlas_dimension_px * atlas_options.atlas_dimension_px * 4);
    std::unique_ptr<nx_u8> atlas_buffer_ptr(new nx_u8[atlas_buffer_size]);
    nx_u8* atlas_buffer = atlas_buffer_ptr.get();
    size_t atlas_buffer_index = 0;

    int grows[256]; // glyph height in pixels
    int gwidth[256]; // glyph width in pixels
    int gpitch[256]; // bytes per row of pixels
    int gymin[256]; // offset for letters that dip below baseline like g and y
    unsigned char* glyph_buffer[256] = { NULL }; // stored glyph images

    // set height in pixels width 0 height 48 (48x48)
    FT_Set_Pixel_Sizes (face, 0, atlas_options.atlas_glyph_padded_px);

    // load all chars from 33 to 256
    for (int i = 33; i < 256; ++i)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            nx::NXLogError("Could not load character '%c' (%d)", i, i);
            return EXIT_FAILURE;
        }

        // draw glyph anti-aliased
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        grows[i] = face->glyph->bitmap.rows;
        gwidth[i] = face->glyph->bitmap.width;
        gpitch[i] = face->glyph->bitmap.pitch;

        // copy glyph data into memory because it seems to be overwritten/lost later
        glyph_buffer[i] = (unsigned char*)nx::NXMalloc(grows[i] * gpitch[i]);
        memcpy (glyph_buffer[i], face->glyph->bitmap.buffer,
                face->glyph->bitmap.rows * face->glyph->bitmap.pitch);

        // get y offeset for glyph baseline
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
        {
            nx::NXLogError("Could not get glyph handle for character '%c' (%d)", i, i);
            return EXIT_FAILURE;
        }

        // get the bbox in pixels
        FT_BBox bbox;
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
        gymin[i] = bbox.yMin;
    }

    // convert glyphs into image data
    for (int y = 0; y < atlas_options.atlas_dimension_px; y++)
    {
        for (int x = 0; x < atlas_options.atlas_dimension_px; x++)
        {
            int col = x / atlas_options.slot_glyph_size;
            int row = y / atlas_options.slot_glyph_size;
            int order = row * atlas_options.atlas_columns + col;
            int glyph_index = order + 32;

            // check if index is valid;
            if (glyph_index > 32 && glyph_index < 256)
            {
                // pixel indices with the padded glyh slot area
                int x_loc = x % atlas_options.slot_glyph_size - atlas_options.padding_px / 2;
                int y_loc = y % atlas_options.slot_glyph_size - atlas_options.padding_px / 2;

                // paint black image with unused blocks
                if ( x_loc < 0 || y_loc < 0 ||
                     x_loc >= gwidth[glyph_index] || y_loc >= grows[glyph_index])
                {
                    atlas_buffer[atlas_buffer_index++] = 0;
                    atlas_buffer[atlas_buffer_index++] = 0;
                    atlas_buffer[atlas_buffer_index++] = 0;
                    atlas_buffer[atlas_buffer_index++] = 0;
                }
                else
                {
                    int byte_order_in_glyph = y_loc * gwidth[glyph_index] + x_loc;
                    // print byte from glyph
                    atlas_buffer[atlas_buffer_index++] =
                            glyph_buffer[glyph_index][byte_order_in_glyph];
                    atlas_buffer[atlas_buffer_index++] =
                            glyph_buffer[glyph_index][byte_order_in_glyph];
                    atlas_buffer[atlas_buffer_index++] =
                            glyph_buffer[glyph_index][byte_order_in_glyph];
                    atlas_buffer[atlas_buffer_index++] =
                            glyph_buffer[glyph_index][byte_order_in_glyph];
                }
            }
            else
            {
                atlas_buffer[atlas_buffer_index++] = 0;
                atlas_buffer[atlas_buffer_index++] = 0;
                atlas_buffer[atlas_buffer_index++] = 0;
                atlas_buffer[atlas_buffer_index++] = 0;
            }
        }
    }

    // free that buffer of glyph info
    for (int i = 0; i < 256; i++) {
        if (NULL != glyph_buffer[i]) {
            free (glyph_buffer[i]);
        }
    }

    // write png output
    if (!stbi_write_png(output_png.c_str(),
                        atlas_options.atlas_dimension_px,
                        atlas_options.atlas_dimension_px,
                        4, atlas_buffer, 0))
    {
        nx::NXLogError("Could not write atlas output to '%s'", output_png.c_str());
        return EXIT_SUCCESS;
    }



    // flip image for Renderer
    const int width_in_bytes = atlas_options.atlas_dimension_px * 4;
    unsigned char *top = nullptr;
    unsigned char *bottom = nullptr;
    unsigned char temp = 0;
    const int half_height = atlas_options.atlas_dimension_px / 2;

    for (int row = 0; row < half_height; row++)
    {
        top = atlas_buffer + row * width_in_bytes;
        bottom = atlas_buffer + (atlas_options.atlas_dimension_px - row - 1) * width_in_bytes;
        for (int col = 0; col < width_in_bytes; col++)
        {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }

    // write meta debug output

    nx::NXFontCharData fnt_chr[nx::kFontMaxMetaData];
    nx::NXFontMetaData fnt_meta[nx::kFontMaxMetaData];
    memset(&fnt_chr, 0, sizeof(fnt_chr));

    FILE* fp = fopen (output_meta.c_str(), "w");
    if (!fp)
    {
        nx::NXLogError("Failed to open '%s' for writing", output_meta.c_str());
        return EXIT_FAILURE;
    }

    // comment
    fprintf (fp, "// ascii_code xMin width yMin height yOffset\n");

    // write a default space character, might not be used later
    fprintf (fp, "32 %f %f %f %f %f\n", 0.0f, 0.5f, 0.0f, 1.0f, 0.0f);

    // write a line for each regular character
    for (int i = 33; i < 256; i++)
    {
        // debug metadata
        int order = i - 32;
        int col = order % atlas_options.atlas_columns;
        int row = order / atlas_options.atlas_columns;
        float x_min = (float)(col * atlas_options.slot_glyph_size) / (float)atlas_options.atlas_dimension_px;
        float y_min = (float)(row * atlas_options.slot_glyph_size) / (float)atlas_options.atlas_dimension_px;
        fprintf (fp, "%i %f %f %f %f %f\n",
                 i,
                 x_min,
                 (float)(gwidth[i] + atlas_options.padding_px) / (float)atlas_options.atlas_glyph_px,
                 y_min,
                 (float)(grows[i] + atlas_options.padding_px)  / (float)atlas_options.atlas_glyph_px,
                 -((float)atlas_options.padding_px - (float)gymin[i]) / (float)atlas_options.atlas_glyph_px);

        // nx font char data
        nx::NXFontCharData& fntData = fnt_chr[i - 32];
        fntData.chr = i;
        fntData.glyphWidth= gwidth[i];
        fntData.glyphRow = grows[i];
        fntData.glyphYMin = gymin[i];
    }
    fclose (fp);

    // font hdr
    nx::NXFontHdr fnt_hdr;
    memcpy(fnt_hdr.magic, nx::NXFont::sMagic, sizeof(nx::NXFont::sMagic));
    fnt_hdr.atlasColumns = atlas_options.atlas_columns;
    fnt_hdr.atlasDimensionsPx = atlas_options.atlas_dimension_px;
    fnt_hdr.atlasGlyphPx = atlas_options.atlas_glyph_px;
    fnt_hdr.slotGlyphSize = atlas_options.slot_glyph_size;
    fnt_hdr.paddingPx = atlas_options.padding_px;


    // write second debug metadata file
    nx::NXFont::charToMetaData(fnt_meta, fnt_chr, fnt_hdr);
    fp = fopen (output_meta_gen.c_str(), "w");
    if (!fp)
    {
        nx::NXLogError("Failed to open '%s' for writing", output_meta.c_str());
        return EXIT_FAILURE;
    }

    // comment
    fprintf (fp, "// ascii_code xMin width yMin height yOffset\n");

    // write a line for each regular character
    for (int i = 32; i < 256; i++)
    {
        // debug metadata
        int order = i - 32;
        const nx::NXFontMetaData& meta = fnt_meta[order];
        fprintf (fp, "%i %f %f %f %f %f\n",
                 i, meta.xMin, meta.width, meta.yMin, meta.height, meta.yOffset);
    }
    fclose (fp);


    // write NXFont File
    std::unique_ptr<nx::NXIOFile> io(nx::NXIOFile::open(output_nxfont.c_str(),
                                                        nx::kIOAccessModeOverwriteBit | nx::kIOAccessModeWriteBit));

    if (!io)
    {
        nx::NXLogError("Failed to open '%s' for writing", output_nxfont.c_str());
        return EXIT_FAILURE;
    }

    // write header
    if (io->write(&fnt_hdr, sizeof(fnt_hdr)) != sizeof(fnt_hdr))
    {
        nx::NXLogError("Failed to write font header to '%s'", output_nxfont.c_str());
        return EXIT_FAILURE;
    }

    // write font data
    if (io->write(&fnt_chr, sizeof(fnt_chr)) != sizeof(fnt_chr))
    {
        nx::NXLogError("Failed to write font metadata to '%s'", output_nxfont.c_str());
        return EXIT_FAILURE;
    }

    // write image contents
    if (io->write(atlas_buffer, atlas_buffer_size) != atlas_buffer_size)
    {
        nx::NXLogError("Failed to write font image data to '%s'", output_nxfont.c_str());
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
