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
#include "nx/sys/nxapp.h"
#include "nx/nx3d.h"
#include "nx/gpu/nxgpuinterface.h"

class NXGLInfoApp : public nx::NXApp
{
public:

    NXGLInfoApp():
        nx::NXApp("GLInfo")
    {

    }

    virtual void setAppOptions(const int ,
                               const char**,
                               nx::NXAppOptions& options)
    {
        options.width = 64;
        options.height = 64;
    }

    virtual void appRun()
    {

        quit();
    }

    virtual void onWindowCreated()
    {
        _gpu = nx::NXGPUInterface::create();
        if (!_gpu->init())
        {
            nx::NXLogError("Failed to initialize GPU Interface");
            quit();
        }
        nx::NX3D::log3DInfo();
    }

    virtual void onWindowWillBeDestroyed()
    {
        _gpu->shutdown();
        NX_SAFE_DELETE(_gpu);
    }

private:
    nx::NXGPUInterface* _gpu;

};

NX_APP(NXGLInfoApp)
