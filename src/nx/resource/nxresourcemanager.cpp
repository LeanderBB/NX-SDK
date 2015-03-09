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
#include "nx/resource/nxresourcemanager.h"
#include "nx/resource/nxresource.h"
#include "nx/os/nxpath.h"

#include "nx/resource/nx3dmodelresource.h"
#include "nx/resource/nxgputextureresource.h"
#include "nx/resource/nxgpuprogramresource.h"
#include "nx/resource/nxgpumeshresource.h"
namespace nx
{

NXResourceManager::NXResourceManager(NXFileManager& fileManager,
                                     NXResourceManager* pParentManager):
    _fileManager(fileManager),
    _pGPUInterface(nullptr),
    _pParentManager(pParentManager),
    _handles(),
    _hdlByName(),
    _hdlByPath()
{
    registerCreator("nxprog", NXGPUProgramResource::create);
    registerCreator("nximg", NXGPUTextureResource::create);
    registerCreator("nx3d", NXGPUMeshResource::create);
}

NXResourceManager::~NXResourceManager()
{
    clear();
}


bool
NXResourceManager::registerCreator(const char* extension,
                                   fnCreate_t fn)
{
    auto it = _creators.find(extension);
    if (it == _creators.end())
    {
        _creators.insert(CreatorMap_t::value_type(extension, fn));
        return true;
    }
    return false;
}

NXHdl
NXResourceManager::create(const char* name,
                          const char* path)
{
    NXHdl hdl;
    const char* extension = NXPath::fileExtension(path);
    if (extension)
    {
        auto it = _creators.find(extension);
        if (it != _creators.end())
        {
            auto ptr = it->second(name, path);
            hdl = registerResource(ptr);
            if (!hdl)
            {
                NXLogError("NXResourceManager::create Failed to register '%s'",
                           name);
            }
        }
        else
        {
            NXLogError("NXResourceManager::create No creator found for extension '%s' for '%s'",
                       extension, name);
        }
    }
    else
    {
        NXLogError("NXResourceManager::create No extension found in '%s' for '%s'",
                   path, name);
    }

    return hdl;
}

NXHdl
NXResourceManager::createAndLoad(const char* name,
                                 const char* path)
{
    NXHdl hdl = create(name, path);
    load(hdl);
    return hdl;
}

NXHdl
NXResourceManager::registerResource(const NXResourcePtr_t &resource)
{
    NXHdl hdl;
    if(resource)
    {
        // check name
        const char* res_name = resource->name();
        const char* res_path = resource->path();

        bool register_path = !(resource->resourceFlags() & kResourceFlagSkipPathRegistrationBit);

        auto it = _hdlByName.find(res_name);
        if (it != _hdlByName.end())
        {
            NXLogError("NXResourceManager::register Resource with name '%s' already exists",
                       res_name);
            return hdl;
        }



        if(resource->isReloadable() && register_path)
        {
            auto path_it = _hdlByPath.find(res_path);
            if (path_it != _hdlByPath.end())
            {
                NXLogError("NXResourceManager::register Another resource has \
                           already registered the path '%s' for '%s'",
                           res_path,
                           res_name);
                return hdl;
            }
        }

        hdl = _handles.add((nx_u32)resource->resourceType(), resource);

        if (hdl)
        {
            _hdlByName.insert(std::make_pair(res_name, hdl));
            if (register_path)
            {
                _hdlByPath.insert(std::make_pair(res_path, hdl));
            }
        }
    }
    else
    {
        NXLogError("NXResourceManager::register Invalid pointer");
    }
    return hdl;
}

void
NXResourceManager::unregisterResource(const NXHdl hdl)
{
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        _hdlByName.erase(ptr->name());
        if (ptr->isReloadable())
        {
            _hdlByPath.erase(ptr->path());
        }
        _handles.remove(hdl);
    }
}

void
NXResourceManager::load(const NXHdl hdl)
{
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        ptr->load(*this);
    }
}

void
NXResourceManager::unload(const NXHdl hdl)
{
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        ptr->unload(*this);
    }void setGPUInterface(NXGPUInterface* pGPUInterface);
}

bool
NXResourceManager::reload(const NXHdl hdl)
{
    bool ret = false;
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        if (ptr->isReloadable())
        {
            ptr->unload(*this);
            ptr->load(*this);
            ret = true;
        }
    }
    return ret;
}

bool
NXResourceManager::isLoaded(const NXHdl hdl) const
{
    bool ret = false;
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        ret = ptr->isLoaded();
    }
    return ret;
}

bool
NXResourceManager::isRegistered(const NXHdl hdl) const
{
    return _handles.contains(hdl);
}

bool
NXResourceManager::isError(const NXHdl hdl) const
{
    bool ret = false;
    NXResourcePtr_t ptr;
    if(_handles.get(hdl, ptr))
    {
        ret = ptr->isError();
    }
    return ret;
}

NXTLSharedPtr<NXResource>
NXResourceManager::get(const NXHdl hdl) const
{
    NXResourcePtr_t ptr;
    (void)_handles.get(hdl, ptr);
    return ptr;
}

NXHdl
NXResourceManager::getByName(const char* name) const
{
    auto it = _hdlByName.find(name);
    if (it != _hdlByName.end())
    {
        return it->second;
    }
    return NXHdl();
}

NXHdl
NXResourceManager::getByPath(const char* path) const
{
    auto it = _hdlByPath.find(path);
    if (it != _hdlByPath.end())
    {
        return it->second;
    }
    return NXHdl();
}

void
NXResourceManager::clear()
{
    NXResourcePtr_t ptr;

    for(auto& res : _hdlByName)
    {
        if(_handles.get(res.second, ptr))
        {
            ptr->unload(*this);
        }
    }

    _hdlByName.clear();
    _hdlByPath.clear();
    _handles.reset();
}

void
NXResourceManager::setGPUInterface(NXGPUInterface* pGPUInterface)
{
    _pGPUInterface = pGPUInterface;
}

}
