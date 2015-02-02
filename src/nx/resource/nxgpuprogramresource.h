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
#ifndef __NX_GPUPROGRAMRESOURCE_H__
#define __NX_GPUPROGRAMRESOURCE_H__

#include "nx/resource/nxresource.h"
#include "nx/gpu/nxgpuprogramsource.h"
#include "nx/resource/nximageresource.h"

namespace nx
{
class NXGPUProgramResource: public NXResource
{
public:

    static const nxresourcetype_t sType;

    static NXResourcePtr_t create(const char* name,
                                  const char* path);

    NXGPUProgramResource(const char* name,
                         const char* path);

    virtual ~NXGPUProgramResource();

    void load(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    void unload(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    void setSource(NXGPUProgramSourceBase* pSource);

    const NXGPUProgramSourceBase *source() const;

    NXHdl gpuHdl() const
    {
        return _gpuHdl;
    }

protected:
    NXHdl _gpuHdl;
    NXGPUProgramSourceBase* _pSource;

};
typedef NXTLSharedPtr<NXGPUProgramResource> NXGPUProgramResourcePtr_t;
}
#endif
