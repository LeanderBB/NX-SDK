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
#ifndef __NXSYSTEMGLFW_H__
#define __NXSYSTEMGLFW_H__

#if defined(NX_SYSTEM_GLFW)
#include "nx/sys/glfw/nxwindowglfw.h"

namespace nx
{
class NXInputManager;
class NXWindow;
class NXEventManager;
struct NXAppOptions;
class NXSystemImp
{
public:
    NXSystemImp();

    virtual ~NXSystemImp();

protected:
    bool initImp(const int argc,
                 const char** argv,
                 const struct NXAppOptions* pOptions);

    void termImp(NXEventManager* pEvtManager);

    bool tickImp(NXEventManager* pEvtManager,
                 NXInputManager* pInputManager);

    bool createWindowImp(const struct NXAppOptions* pOptions,
                         NXEventManager* pEvtManager);

    NXWindow* windowImp()
    {
        return _pWindow;
    }

    bool shouldQuitImp() const;

private:
    static void errorCallback(int error,
                              const char* description);

    NXWindow* _pWindow;
};
}
#endif // defined(NX_SYSTEM_GLFW)
#endif
