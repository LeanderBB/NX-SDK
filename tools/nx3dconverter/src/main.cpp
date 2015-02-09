//
// This file is part of the NX Project
//
// Copyright (c) 2015 Leander Beernaert
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

#include <nx/nxcore.h>
#include <nx/media/nx3dmodel.h>
#include <nx/io/nxiobase.h>
#include <nx/io/nxiofile.h>
#include <nx/os/nxargs.h>
#include "nxassimpconverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "nx3dmodelbuilder.h"
#include "nx3dmodelpostprocess.h"

using namespace nx;

int main(const int argc, const char** argv)
{

    NXArgs args;

    args.add(kOptionInterleavedBuffers, 'i',"interleaved", "Interleave components as to use a minumum number of buffers possible", 0);
    args.add(kOptionOutput, 'o', "output", "Output file", kArgFlagRequired | kArgFlagSingleValue);

    int res = args.parse(argc, argv);

    if (res <= 0 || args.isSet(0))
    {
        args.printHelp("nx3dconverter - Convert 3D Models into the nx3d format.");
        return (res < 0) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    int arg_size = 0;
    const char* output_file = args.get(kOptionOutput,arg_size)[0];


    unsigned int convert_flags = 0;


    if (args.isSet(kOptionInterleavedBuffers))
    {
        convert_flags |= kConverterPackComponentsBit;
    }

    NXInputStateVec_t input_state;

    if (!NXAssimpConverter::convert(input_state, argv[res], convert_flags))
    {
        NXLogError("Failed to open/load '%s'", argv[res]);
        return EXIT_FAILURE;
    }

    NX3DModelPostProcess post_process(input_state);

    if (!post_process.buildOutputData())
    {
        return EXIT_FAILURE;
    }

    auto& output_state = post_process.output();

    NX3DModelBuilderHelper helper;

    if (!helper.build(output_state))
    {
        return EXIT_FAILURE;
    }

    if (!helper.write(output_file))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
