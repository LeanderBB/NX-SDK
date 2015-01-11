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
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiomemory.h"
#include "nx/os/nxpath.h"




int main(const int argc, const char** argv)
{
    if (argc < 3)
    {
        nx::NXLogError("Usage: %s [archive] [file]", argv[0]);
        return EXIT_FAILURE;
    }

    nx::NXFileManager fs;

    if (!fs.init())
    {
        nx::NXLogError("Could not init file system");
        return EXIT_FAILURE;
    }

    if (!fs.mountArchive(argv[1], ""))
    {
        return EXIT_FAILURE;
    }

    nx::NXIOBase* p_io = fs.open(argv[2], nx::kIOAccessModeReadBit);

    if (p_io)
    {
        while(!p_io->isEOF())
        {
            static const size_t buffer_size = 64*1024;
            static char buffer[64*1024];
            size_t bytes_read = p_io->read(buffer,buffer_size - 1);
            if (bytes_read)
            {
                buffer[(bytes_read == buffer_size -1) ? (bytes_read) : (bytes_read -1)] = '\0';
                printf("%s", buffer);
            }
        }
        puts("");
        delete p_io;
    }
    else
    {
        nx::NXLogError("Failed to open '%s'", argv[2]);
    }
    fs.shutdown();
    return EXIT_SUCCESS;
}
