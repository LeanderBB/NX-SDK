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
#include "nx/resource/nx3dmodelresource.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxfilemanager.h"
#include "nx/media/nx3dmodel.h"
namespace nx
{

const nxresourcetype_t NX3DModelResource::sType = 1;


NXResourcePtr_t
NX3DModelResource::create(const char* name,
                          const char* path)
{
    return nxMakeTLShared<NX3DModelResource>(name, path);
}

NX3DModelResource::NX3DModelResource(const char *name,
                                     const char *path):
    NXResource(name, path, sType, 0 ),
    _pModel(nullptr)
{

}

NX3DModelResource::~NX3DModelResource()
{
    NX_ASSERT(!_pModel);
}

void
NX3DModelResource::load(NXResourceManager& resourceManager)
{
    if(isLoaded())
        return;

    setState(kResourceStateLoading);
    NXFileManager& fm = resourceManager.fileManager();
    auto io = std::unique_ptr<NXIOBase>(fm.open(_path.c_str(), kIOAccessModeReadBit));
    if (io)
    {
        _pModel = NX3DModel::load(io.get());
    }
    if(!_pModel)
    {
        NXLogError("NX3DModelResource: Could not load '%s'", path());
        setState(kResourceStateError);
    }
    else
    {
        setSize(_pModel->memorySize());
        setState(kResourceStateLoaded);
    }
}

void
NX3DModelResource::unload(NXResourceManager& resourceManager)
{
    if(isUnloaded())
        return;

    setState(kResourceStateUnloaded);
    (void) resourceManager;
    NX_SAFE_DELETE(_pModel);
}

}

