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
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"
#include <flatbuffers/flatbuffers.h>
#include "nxgpuprogramsource_generated.h"
#include "nx/os/nxpath.h"
#include "nx/os/nxargs.h"


/*
 * GPUProgram Input,
 * // comment
 * nn [name]            - program name
 * ve [file name]       - vertex shader
 * fr [file name]       - fragment shader
 * ge [file name]       - geometry shader
 * te [file name]       - tess evaluation shader
 * tc [file name]       - tess control shader
 * co [file name]       - compute shader
 * tp [packed|separate] - transform feedback result
 * tf [variable]        - transform feedback variable
 */

namespace nx
{

typedef std::vector<flatbuffers::Offset<flatbuffers::String>> fbstrvec_t;

static bool NXReadFileIntoString(fbstrvec_t& out,
                                 const char* filename,
                                 const char* shaderPath,
                                 flatbuffers::FlatBufferBuilder& builder)
{
    std::string str;
    std::string path = NXPath::join(shaderPath,filename);
    NXIOFile* pFile = NXIOFile::open(path.c_str(), kIOAccessModeReadBit);
    if (!pFile)
    {
        fprintf(stderr, "Failed to read shader file '%s'\n", filename);
        return false;
    }

    size_t file_size = pFile->size();
    size_t bytes_read = 0;
    str.resize(0, file_size + 1);

    bytes_read = pFile->read(&str[0], file_size);
    delete pFile;

    if (bytes_read == file_size)
    {
        auto fbb_str = builder.CreateString(str.c_str(), file_size);
        out.push_back(fbb_str);
        return true;
    }
    else
    {
        fprintf(stderr, "Failed to read contents of file '%s'\n", filename);
        return false;
    }
}


static bool NXParseProgram(FILE* input,
                           const char* shaderPath,
                           flatbuffers::FlatBufferBuilder& builder)
{
    char line_str[8192];

    fbstrvec_t vshs, fshs, gshs, cshs, teshs, tcshs, tfns;
    nx::FeedbackDataLayout layout;
    std::string name;
    int scan_result = 0;
    int line = 0;
    char c1, c2;
    bool feedback_layout_set = false;

    while(!feof(input))
    {
        scan_result = fscanf(input, "%c%c ",&c1, &c2);
        // check for error
        if (scan_result == 0)
        {
            fprintf(stderr, "Failed to parse type on line %d\n", line);
            return false;
        }
        // check for eof
        else if (scan_result == EOF)
        {
            break;
        }

        // read line

        if (!fgets(line_str, 8192, input))
        {
            printf(">>%s<<\n", line_str);

            fprintf(stderr, "Failed to read line %d\n", line);
            return false;
        }
        size_t line_len = strlen(line_str);
        if(line_str[line_len - 1] == '\n')
        {
            line_str[line_len - 1] = '\0';
        }

        switch(c1)
        {
        case '/':
        {
            if (c2 != '/')
            {
                fprintf(stderr, "Unknown type '%c%c'' on line %d\n",c1, c2, line);
                return false;
            }
            break;
        }
        case 'n':
        {
            name = line_str;
            break;
        }
        case 'v':
        {
            if (!NXReadFileIntoString(vshs, line_str, shaderPath, builder))
            {
                return false;
            }
            break;
        }
        case 'f':
        {
            if (!NXReadFileIntoString(fshs, line_str, shaderPath, builder))
            {
                return false;
            }
            break;
        }
        case 'g':
        {
            if (!NXReadFileIntoString(gshs, line_str, shaderPath, builder))
            {
                return false;
            }
            break;
        }
        case 't':
        {
            switch(c2)
            {
            case 'e':
                if (!NXReadFileIntoString(teshs, line_str, shaderPath, builder))
                {
                    return false;
                }
                break;

            case 'c':
                if (!NXReadFileIntoString(tcshs, line_str, shaderPath, builder))
                {
                    return false;
                }
                break;
            case 'f':
                tfns.push_back(builder.CreateString(line_str));
                break;
            case 'p':
                if (strcmp(line_str, "packed") == 0)
                {
                    layout = FeedbackDataLayout_Packed;
                }
                else if (strcmp(line_str,"separate") == 0)
                {
                    layout = FeedbackDataLayout_Separate;
                }
                else
                {
                    fprintf(stderr, "Unknown tansform feedback pack type '%s' on line %d\n", line_str, line);
                    return false;
                }
                feedback_layout_set = true;
                break;
            default:
                fprintf(stderr, "Unknown type '%c%c on line %d\n",c1, c2, line);
                return false;
            }

            break;
        }
        case 'c':
        {
            if (!NXReadFileIntoString(cshs, line_str, shaderPath, builder))
            {
                return false;
            }
            break;
        }
        default:
            fprintf(stderr, "Unknown type %c%c on line %d\n",c1, c2, line);
            return false;
        }

        ++line;
    }


    // validate data
    if (!name.length())
    {
        fprintf(stderr, "No name for program\n");
        return false;
    }

    if ((vshs.size() == 0 || fshs.size() == 0) && !cshs.size())
    {
        fprintf(stderr, "Program requires at least one vertex-fragment shader or at least one compute shader\n");
        return false;
    }

    if (feedback_layout_set && !tfns.size())
    {
        fprintf(stderr, "Transform feedback requires variables names, none found\n");
        return false;
    }


    auto fbb_name = builder.CreateString(name);
    auto fbb_tf_vec = builder.CreateVector(tfns);
    auto fbb_vs_vec = builder.CreateVector(vshs);
    auto fbb_fs_vec = builder.CreateVector(fshs);
    auto fbb_ge_vec = builder.CreateVector(gshs);
    auto fbb_te_vec = builder.CreateVector(teshs);
    auto fbb_tc_vec = builder.CreateVector(tcshs);
    auto fbb_co_vec = builder.CreateVector(cshs);

    nx::ProgramSrcBuilder prog(builder);
    prog.add_name(fbb_name);

    prog.add_feedback(feedback_layout_set);
    prog.add_feedbackLayout(layout);

    prog.add_feedbackNames(fbb_tf_vec);
    prog.add_vertex(fbb_vs_vec);
    prog.add_fragment(fbb_fs_vec);
    prog.add_geometry(fbb_ge_vec);
    prog.add_tessEval(fbb_te_vec);

    prog.add_tessCont(fbb_tc_vec);
    prog.add_compute(fbb_co_vec);

    auto mloc = prog.Finish();
    FinishProgramSrcBuffer(builder, mloc);

    return true;
}


}


enum Option
{
    kOptionDesc = 1,
    kOptionShaderPath,
    kOptionOutput
};

static const char s_description[] = "nxprogrambuilder - Utility to create binary nxprog files used by NX";

int main(const int argc,
         const char** argv)
{
    nx::NXArgs args;

    args.add(kOptionDesc, 'd', "description", "GPUProgram description file.", nx::kArgFlagRequired | nx::kArgFlagSingleValue);
    args.add(kOptionShaderPath, 's', "shader_path", "Folder containing all the shader sources mentioned in the description file.", nx::kArgFlagRequired | nx::kArgFlagSingleValue);
    args.add(kOptionOutput, 'o', "output", "Output name of the generated nxprogram file (without extension).", nx::kArgFlagRequired | nx::kArgFlagSingleValue);

    int res = args.parse(argc, argv);

    if (res < 0)
    {
        args.printHelp(s_description);
        return EXIT_FAILURE;
    }

    if (res == 0 || args.isSet(0))
    {
        args.printHelp(s_description);
        return EXIT_SUCCESS;
    }

    int arg_size;
    const char** arg_val;

    arg_val = args.get(kOptionDesc, arg_size);
    NX_ASSERT(arg_size >= 1);

    // open file
    FILE* input = fopen(arg_val[0],"r");
    if (!input)
    {
        fprintf(stderr, "Could not open for reading '%s'\n", arg_val[0]);
        return EXIT_FAILURE;
    }

    arg_val = args.get(kOptionShaderPath, arg_size);
    NX_ASSERT(arg_size >= 1);

    // parse file
    flatbuffers::FlatBufferBuilder fbb;
    bool result = nx::NXParseProgram(input, arg_val[0], fbb);
    fclose(input);

    if (!result)
    {
        return EXIT_FAILURE;
    }

    arg_val = args.get(kOptionOutput, arg_size);
    NX_ASSERT(arg_size >= 1);

    FILE* output = fopen(arg_val[0], "wb");
    if (!output)
    {
        fprintf(stderr, "Could not open for writing '%s'\n", arg_val[0]);
        return EXIT_FAILURE;
    }

    auto ptr = fbb.GetBufferPointer();
    auto size = fbb.GetSize();
    size_t bytes_written = fwrite(ptr, 1, size, output);
    fclose(output);
    if (bytes_written != size)
    {
        fprintf(stderr, "Failed to write into '%s'\n", arg_val[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
