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
#include "nx/util/nxtlsharedptr.h"


class TestClass
{
public:
    TestClass(const char* message):
        msg(message)
    {
        nx::NXLog("Class created: %s", message);
    }
    ~TestClass()
    {
        nx::NXLog("Class destroyed: %s", msg);
    }

    const char* msg;
};


int main()
{
    auto ptr = nx::nxMakeTLShared<TestClass>("ptr1");
    {
        auto ptr2 = nx::nxMakeTLShared<TestClass>("ptr2");

        ptr = ptr2;
    }
    return EXIT_SUCCESS;
}
