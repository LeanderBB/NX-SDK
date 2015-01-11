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

class NXApp
{
public:
    virtual ~NXApp();

    bool init(const int argc, const char** argv);

    void run();

    void quit();

    void term();

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

    virtual void setAppOptions(const int ,
                               const char**,
                               NXAppOptions&) {}

    virtual bool appInit(const int ,
                         const char**) { return true; }

    virtual void appRun() = 0;

    virtual void appTerm() {}

private:
    const NXString _name;
    NXSystem _system;
    NXInputManager _inputMan;
    NXAppOptions _options;
};


#define NX_APP(CLASS) \
    int main(const int argc, const char** argv)\
{ \
{ \
    CLASS app; \
    if (app.init(argc, argv)) \
{ \
    app.run(); \
} \
    app.term(); \
} \
    return EXIT_SUCCESS; \
}

}

#endif
