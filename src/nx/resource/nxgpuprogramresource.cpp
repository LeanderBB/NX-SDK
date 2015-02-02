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
#include "nx/resource/nxgpuprogramresource.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/gpu/nxgpuinterface.h"
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"
namespace nx
{

const nxresourcetype_t NXGPUProgramResource::sType = 2;

NXResourcePtr_t
NXGPUProgramResource::create(const char* name,
                             const char* path)
{
    return nxMakeTLShared<NXGPUProgramResource>(name, path);
}

NXGPUProgramResource::NXGPUProgramResource(const char* name,
                                           const char* path):
    NXResource(name,path,sType, kResourceFlagRequireGPUInterfaceBit),
    _gpuHdl(),
    _pSource(nullptr)
{

}

NXGPUProgramResource::~NXGPUProgramResource()
{
    NX_SAFE_DELETE(_pSource);
}

void
NXGPUProgramResource::load(NXResourceManager& resourceManager)
{
    if(isLoaded())
        return;

    NXGPUInterface* gpuInterface = resourceManager.gpuInterface();
    setState(kResourceStateLoading);

    if (!_pSource && isReloadable())
    {
        NXFileManager& fm = resourceManager.fileManager();
        auto io = std::unique_ptr<NXIOBase>(fm.open(_path.c_str(), kIOAccessModeReadBit));
        if (io)
        {
            _pSource = NXGPUProgramSource::load(io.get());

        }

        if (!_pSource)
        {
            NXLogError("NXGPUProgramResource: Could not load '%s'", path());
            setState(kResourceStateError);
            return;
        }
    }

    if (!gpuInterface)
    {
        NXLogError("NXGPUProgramResource: Valid GPU Interface required");
        setState(kResourceStateError);
        return;
    }

    // release previous program (if any)
    gpuInterface->releaseProgram(_gpuHdl);

    // create gpu program
    _gpuHdl = gpuInterface->allocProgram(*_pSource);
    if (!_gpuHdl.valid())
    {
        NXLogError("NXGPUProgramResource:(%s): Could not upload to gpu",
                   _name.c_str());
        setState(kResourceStateError);
        return;
    }

    setSize(_pSource->memorySize());
    setState(kResourceStateLoaded);
}

void
NXGPUProgramResource::unload(NXResourceManager& resourceManager)
{
    if(isUnloaded())
        return;

    NXGPUInterface* gpuInterface = resourceManager.gpuInterface();

    // release gpu data
    if(gpuInterface)
    {
        gpuInterface->releaseProgram(_gpuHdl);
    }

    setState(kResourceStateUnloaded);
}

void
NXGPUProgramResource::setSource(NXGPUProgramSourceBase* pSource)
{
    if (_pSource)
    {
        NX_SAFE_DELETE(_pSource);
    }
    _pSource = pSource;
}

const NXGPUProgramSourceBase*
NXGPUProgramResource::source() const
{
    return _pSource;
}

}
