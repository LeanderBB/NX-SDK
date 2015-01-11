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
#include "nx/os/nxargs.h"
#include <iostream>

enum
{
    kOptionHelp,
    kOptionBool,
    kOptionString,
    kOptionArray
};


int main(const int argc,
         const char** argv)
{
    nx::NXArgs args;

    args.add(kOptionBool, 'b', "bool", "Test for option being set", 0);

    args.add(kOptionString, 's', "string", "Test for option for a single argument",
            nx::kArgFlagSingleValue);

    args.add(kOptionArray, 'a', "array", "Test option for array based values",
             nx::kArgFlagMultiValue);
    int res = args.parse(argc, argv);
    if (res < 0)
    {
        return EXIT_FAILURE;
    }

    if (res == 0 || args.isSet(kOptionHelp))
    {
        args.printHelp("NXArgs test program.\nSimple test to check whether the arg parser works.");
        return EXIT_SUCCESS;
    }

    std::cout << std::endl;
    if (args.isSet(kOptionBool))
    {
        std::cout << "Option bool is set." << std::endl;
    }
    else
    {
        std::cout << "Option bool is not set." << std::endl;
    }

    int size =0;
    const char** values = args.get(kOptionString, size);

    if (size <= 0)
    {
        std::cout << "Option string is not set." << std::endl;
    }
    else
    {
        std::cout << "Option string is set: "<< values[0] << "." << std::endl;
    }


    values = args.get(kOptionArray, size);

    if (size <= 0)
    {
        std::cout << "Option array is not set." << std::endl;
    }
    else
    {
        std::cout << "Option array is set: "<< std::endl;
        for (int i = 0; i < size; ++i)
        {
            std::cout <<"    - " << values[i] << std::endl;
        }
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
