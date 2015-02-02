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
#ifndef __NX_GPUTEXTURERESOURCE_H__
#define __NX_GPUTEXTURERESOURCE_H__

#include "nx/resource/nxresource.h"
#include "nx/gpu/nxgputexture.h"
#include "nx/resource/nximageresource.h"

namespace nx
{
class NXGPUTexture;
class NXGPUTextureResource: public NXResource
{
public:

    static const nxresourcetype_t sType;

    static NXResourcePtr_t create(const char* name,
                                  const char* path);

    NXGPUTextureResource(const char* name,
                         const char* path);

    ~NXGPUTextureResource();

    void load(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    void unload(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    const NXGPUTexture* texture() const;

protected:
    NXHdl _imgHdl;
    NXGPUTexture _texture;
    NXImageResourcePtr_t _imgPtr;
};
typedef NXTLSharedPtr<NXGPUTextureResource> NXGPUTextureResourcePtr_t;
}
#endif
