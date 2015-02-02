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
#include "nx/resource/nximageresource.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxfilemanager.h"
#include "nx/media/nximage.h"
namespace nx
{

const nxresourcetype_t NXImageResource::sType = 4;

NXResourcePtr_t
NXImageResource::create(const char* name,
                                  const char* path)
{
    return nxMakeTLShared<NXImageResource>(name, path);
}

NXImageResource::NXImageResource(const char *name,
                                 const char *path,
                                 const bool skipPathReg):
    NXResource(name, path, sType, (skipPathReg ? (nx_u32)kResourceFlagSkipPathRegistrationBit : 0)),
    _pImage(nullptr)
{

}

NXImageResource::~NXImageResource()
{
    NX_ASSERT(!_pImage);
}

void
NXImageResource::load(NXResourceManager& resourceManager)
{
    if(isLoaded())
        return;

    setState(kResourceStateLoading);
    NXFileManager& fm = resourceManager.fileManager();
    auto io = std::unique_ptr<NXIOBase>(fm.open(_path.c_str(), kIOAccessModeReadBit));
    if (io)
    {
        _pImage = NXImage::load(io.get());
    }
    if(!_pImage)
    {
        NXLogError("NXImageResource: Could not load '%s'", path());
        setState(kResourceStateError);
    }
    else
    {
        setSize(_pImage->memorySize());
        setState(kResourceStateLoaded);
    }
}

void
NXImageResource::unload(NXResourceManager& resourceManager)
{
    if(isUnloaded())
        return;

    setState(kResourceStateUnloaded);
    (void) resourceManager;
    NX_SAFE_DELETE(_pImage);
}

}

