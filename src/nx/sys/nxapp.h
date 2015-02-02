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
#ifndef __NX_APP_H__
#define __NX_APP_H__

#include "nx/sys/nxsystem.h"
#include "nx/sys/nxinputmanager.h"
#include "nx/allocator/nxmemory.h"
#include "nx/event/nxeventlistener.h"
#include "nx/allocator/nxmemory.h"

namespace nx
{

struct NXAppOptions
{
    int width;
    int height;
    bool fullscreen;
    bool depth;
    bool stencil;
    bool resizable;
    bool dbgctx;
};

class NXApp : public NXEventListener
{
public:
    virtual ~NXApp();

    int run(const int argc,
            const char** argv);

    void quit();

    const char* name() const
    {
        return _name.c_str();
    }

    NXSystem* system()
    {
        return &_system;
    }

    NXInputManager* inputManager()
    {
        return &_inputMan;
    }

protected:

    NXApp(const char* name);

    bool init(const int argc, const char** argv);


    virtual void setAppOptions(const int ,
                               const char**,
                               NXAppOptions&) {}

    void term();

    virtual bool handleEvent(const NXEventData* pEvtData);

    virtual bool onAppInit(const int,
                           const char**) { return true;}

    virtual void onAppWillTerm() {}

    virtual void onWindowCreated() = 0;

    virtual void onWindowWillBeDestroyed() = 0;

    virtual void onSystemLowMemory() {}

    virtual void appRun() = 0;

private:
    const NXString _name;
    NXSystem _system;
    NXInputManager _inputMan;
    NXAppOptions _options;
};


#if defined(NX_MEMORY_TRACK_ALLOCATIONS)
#define NX_APP_EXIT_CHECKS \
    if (nx::NXAllocatedMemory() != 0) \
    { \
        printf("NXApp Failed to release all memory, %" NX_PRIsize "b unreleased!!\n", \
        nx::NXAllocatedMemory()); \
    }\
    NX_ASSERT(nx::NXAllocatedMemory() == 0);
#else
#define NX_APP_EXIT_CHECKS
#endif

#if defined(NX_OS_ANDROID)
#define NX_MAIN_ENTRY nx_android_main
#else
#define NX_MAIN_ENTRY main
#endif
#define NX_APP(CLASS) \
    int NX_MAIN_ENTRY (const int argc, const char **argv); \
int NX_MAIN_ENTRY (const int argc, \
                   const char** argv)\
{ \
    int exit_code; \
    { \
        CLASS app; \
        exit_code = app.run(argc, argv); \
    } \
    NX_APP_EXIT_CHECKS \
    return exit_code;\
}

}

#endif
