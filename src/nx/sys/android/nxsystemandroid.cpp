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

#if defined(NX_SYSTEM_ANDROID)
#include "nx/sys/android/nxsystemandroid.h"
#include "nx/sys/nxsysevents.h"
#include "nx/sys/nxsystem.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxwindow.h"
namespace nx
{

NXSystemImp::NXSystemImp()
{

}

NXSystemImp::~NXSystemImp()
{

}

bool
NXSystemImp::initImp(const int argc,
                     const char** argv)
{
    (void) argc;
    (void) argv;


    return true;
}

void
NXSystemImp::termImp()
{

}


void
NXSystemImp::tickImp()
{

}


}


#endif // defined(NX_SYSTEM_ANDROID)
