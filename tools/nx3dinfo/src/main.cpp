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
#include "nx/gpu/nxgpubuffer.h"
#include "nx/media/nx3dmodel.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"

namespace nx
{

static void
NX3DModelLogInfo(const char* input)
{
    std::unique_ptr<NXIOFile> finput(NXIOFile::open(input, kIOAccessModeReadBit));

    if (!finput)
    {
        return;
    }

    NX3DModel* p_model = NX3DModel::load(finput.get());
    if (!p_model)
    {
        NXLogError("Could not load '%s'", input);
    }
    else
    {
        p_model->logInfo();
        delete p_model;
    }
}

}

int main(const int argc,
         const char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr,"Usage: %s  nx3d0 ... nx3dn\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i)
    {
        nx::NX3DModelLogInfo(argv[i]);
    }
    nx::NXLogFlush();
    return EXIT_SUCCESS;
}
