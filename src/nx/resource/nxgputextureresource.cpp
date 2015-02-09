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
#include "nx/resource/nxgputextureresource.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/gpu/nxgpuinterface.h"
#include "nx/media/nximage.h"
#include "nx/resource/nximageresource.h"
namespace nx
{

const nxresourcetype_t NXGPUTextureResource::sType = 3;

NXResourcePtr_t
NXGPUTextureResource::create(const char* name,
                             const char* path)
{
    return nxMakeTLShared<NXGPUTextureResource>(name, path);
}

NXGPUTextureResource::NXGPUTextureResource(const char* name,
                                           const char* path):
    NXResource(name,path,sType, kResourceFlagRequireGPUInterfaceBit),
    _imgHdl(),
    _texture()
{

}

NXGPUTextureResource::~NXGPUTextureResource()
{
}

void
NXGPUTextureResource::load(NXResourceManager& resourceManager)
{
    if(isLoaded())
        return;

    NXString image_name(name());
    image_name += "-image";

    NXGPUInterface* gpuInterface = resourceManager.gpuInterface();
    setState(kResourceStateLoading);

    NXImageResourcePtr_t img_ptr;

    if(!_imgHdl.valid())
    {
        _imgHdl = resourceManager.getByName(image_name.c_str());
        img_ptr = resourceManager.get(_imgHdl);
    }

    if (!img_ptr)
    {
        img_ptr = nxMakeTLShared<NXImageResource>(image_name.c_str(),
                                                  path(), true);
        _imgHdl = resourceManager.registerResource(img_ptr);

        if(!_imgHdl.valid())
        {
            NXLogError("NXGPUTextureResource(%s): Could not register NXImage '%s' with resource manager",
                       _name.c_str(), path());
            setState(kResourceStateError);
            return;
        }
    }

    // load image
    img_ptr->load(resourceManager);
    if(!img_ptr->isLoaded())
    {
        NXLogError("NXGPUTextureResource(%s): Could not load NXImage '%s'",
                   _name.c_str(), path());
        setState(kResourceStateError);
        return;
    }

    NX_ASSERT(img_ptr->image());

    if (!gpuInterface)
    {
        NXLogError("NXGPUTextureResource(%s): No gpu interface available",
                   _name.c_str());
        setState(kResourceStateError);
        return;
    }

    NXHdl gpuHdl = _texture.gpuHdl();
    if (!gpuInterface->isValidTextureHdl(gpuHdl))
    {
        // create gpu texture
        gpuHdl = gpuInterface->allocTexture(*img_ptr->image());
        if (!gpuHdl.valid())
        {
            NXLogError("NXGPUTextureResource(%s): Could not upload to gpu",
                       _name.c_str());
            setState(kResourceStateError);
            return;
        }
    }

    NXImage::ImageHeaderToGPUTextureDescription(img_ptr->image()->header(), _texture.desc());
    _texture.setGpuHdl(gpuHdl);

    setSize(img_ptr->size());
    setState(kResourceStateLoaded);
}

void
NXGPUTextureResource::unload(NXResourceManager& resourceManager)
{
    if(isUnloaded())
        return;

    NXGPUInterface* gpuInterface = resourceManager.gpuInterface();

    // release gpu data
    if(gpuInterface)
    {
        gpuInterface->releaseTexture(_texture.gpuHdl());
    }

    setState(kResourceStateUnloaded);
}

const NXGPUTexture*
NXGPUTextureResource::texture() const
{
    return isLoaded() ? &_texture : nullptr;
}

}
