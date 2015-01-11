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
#include "nx/os/nxsharedobj.h"

#if defined(NX_SYSTEM_SDL2)
#include <SDL2/SDL_loadso.h>
#elif defined(NX_OS_UNIX)
#include <dlfcn.h>
#else
#error "Missing shared library loading implementation"
#endif
namespace nx
{

const char* sExtension;

NXSharedObj::NXSharedObj():
    _obj(nullptr)
{
}

NXSharedObj::~NXSharedObj()
{
    this->unload();
}

bool
NXSharedObj::load(const char* path)
{
    // unload previous if any
    this->unload();

#if defined(NX_SYSTEM_SDL2)
    _obj = SDL_LoadObject(path);
#elif defined(NX_OS_UNIX)
    _obj = dlopen(path, RTLD_LAZY);
#else
#error Missing implementation
#endif
    if (!_obj)
    {
        NXLogError("Failed to load shared object '%s': %s", path,
           #if defined(NX_SYSTEM_SDL2)
                   SDL_GetError()
           #elif defined(NX_OS_UNIX)
                   dlerror()
           #else
                   "Unknown"
           #endif
                   );
    }

    return _obj != nullptr;
}

void*
NXSharedObj::locateSymbol(const char* symbol)
{
#if defined(NX_SYSTEM_SDL2)
    return (_obj) ? SDL_LoadFunction(_obj, symbol) : nullptr;
#elif defined(NX_OS_UNIX)
    return (_obj) ? dlsym(_obj, symbol) : nullptr;
#else
#error Missing implementation
#endif

}

void
NXSharedObj::unload()
{
    if (_obj)
    {
#if defined(NX_SYSTEM_SDL2)
        SDL_UnloadObject(_obj);
#elif defined(NX_OS_UNIX)
        dlclose(_obj);
#else
#error Missing implementation
#endif

        _obj = nullptr;
    }
}

}
