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
#ifndef __NX_RESOURCEMANAGER_H__
#define __NX_RESOURCEMANAGER_H__

#include "nx/util/nxtlsharedptr.h"
#include "nx/hdl/nxhdlmanager.h"
#include "nx/resource/nxresource.h"
namespace nx
{

class NXFileManager;
class NXGPUInterface;
class NXResourceManager
{
public:

    typedef NXResourcePtr_t(*fnCreate_t)(const char*, const char*);

public:
    NXResourceManager(NXFileManager& fileManager,
                      NXResourceManager* pParentManager = nullptr);

    ~NXResourceManager();

    bool registerCreator(const char* extension,
                         fnCreate_t fn);

    NXHdl create(const char* name,
                 const char* path);

    NXHdl createAndLoad(const char* name,
                        const char* path);

    NXHdl registerResource(const NXResourcePtr_t& resource);

    void unregisterResource(const NXHdl hdl);

    void load(const NXHdl hdl);

    void unload(const NXHdl hdl);

    bool reload(const NXHdl hdl);

    bool isLoaded(const NXHdl hdl) const;

    bool isRegistered(const NXHdl hdl) const;

    bool isError(const NXHdl hdl) const;

    NXTLSharedPtr<NXResource> get(const NXHdl hdl) const;

    NXHdl getByName(const char* name) const;

    NXHdl getByPath(const char* path) const;

    void clear();

    void unloadUnreferenced();

    NXFileManager& fileManager()
    {
        return _fileManager;
    }

    NXGPUInterface* gpuInterface()
    {
        return _pGPUInterface;
    }

    NXResourceManager* parentManager() const
    {
        return _pParentManager;
    }

    void setGPUInterface(NXGPUInterface* pGPUInterface);

private:
    typedef std::unordered_map<const char*, NXHdl> HandleMap_t;
    typedef std::unordered_map<NXString, fnCreate_t> CreatorMap_t;

    NXFileManager& _fileManager;
    NXGPUInterface* _pGPUInterface;
    NXResourceManager* _pParentManager;
    NXHandleManager<NXTLSharedPtr<NXResource>> _handles;
    HandleMap_t _hdlByName;
    HandleMap_t _hdlByPath;
    CreatorMap_t _creators;
};

}
#endif
