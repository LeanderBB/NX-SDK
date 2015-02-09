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
#include "nx/nxcore.h"
#include "nx/resource/nxgpumeshresource.h"
#include "nx/resource/nx3dmodelresource.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/gpu/nxgpuinterface.h"
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"
namespace nx
{


const nxresourcetype_t NXGPUMeshResource::sType = 5;

NXResourcePtr_t
NXGPUMeshResource::create(const char* name,
                          const char* path)
{
    return nxMakeTLShared<NXGPUMeshResource>(name, path);
}

NXGPUMeshResource::NXGPUMeshResource(const char* name,
                                     const char* path):
    NXResource(name,path,sType, kResourceFlagRequireGPUInterfaceBit),
    _mesh()
{

}

NXGPUMeshResource::~NXGPUMeshResource()
{

}

void
NXGPUMeshResource::load(NXResourceManager& resourceManager)
{
    if(isLoaded())
        return;

    NXString model_name(name());
    model_name += "-model";

    NXGPUInterface* gpuInterface = resourceManager.gpuInterface();
    setState(kResourceStateLoading);


    // get or load 3d model
    NX3DModelResourcePtr_t model_ptr;
    // check wether it was loaded before
    if(!_modelHdl)
    {
        _modelHdl = resourceManager.getByName(model_name.c_str());
        model_ptr = resourceManager.get(_modelHdl);
    }

    // if not loaded, register the resource
    if (!model_ptr)
    {
        model_ptr = NX3DModelResource::create(model_name.c_str(), path());
        _modelHdl = resourceManager.registerResource(model_ptr);


        if(!_modelHdl)
        {
            NXLogError("NXGPUMeshResource(%s): Could not register NX3DModel '%s' with resource manager",
                       _name.c_str(), path());
            setState(kResourceStateError);
            return;
        }
    }

    // load the resource
    model_ptr->load(resourceManager);
    if (!model_ptr->isLoaded())
    {
        NXLogError("NXGPUMeshResource(%s): Could not load NX3DModel '%s'",
                   _name.c_str(), path());
        setState(kResourceStateError);
        return;
    }

    NX_ASSERT(model_ptr->model());

    if (!gpuInterface)
    {
        NXLogError("NXGPUMeshResource(%s): Valid GPU Interface required",
                   name());
        setState(kResourceStateError);
        return;
    }

    // create mesh
    const NX3DModel* p_3dmodel = model_ptr->model();
    _mesh  = NXGPUMesh::create(*p_3dmodel, *gpuInterface);
    if (!_mesh)
    {
        NXLogError("NXGPUMeshResource:(%s): Could not create to NXGPUMesh",
                   _name.c_str());
        setState(kResourceStateError);
        return;
    }

    setSize(_mesh->memorySize());
    setState(kResourceStateLoaded);
}

void
NXGPUMeshResource::unload(NXResourceManager& resourceManager)
{
    (void) resourceManager;

    if(isUnloaded())
        return;

    _mesh.reset();

    setState(kResourceStateUnloaded);
}
}

