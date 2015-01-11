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
#include "nx/gpu/nxgpuresourcemanager.h"
#include "nx/media/nxmediamanager.h"
#include "nx/ogl/nxogltexture.h"
#include "nx/ogl/nxoglprogram.h"
namespace nx
{

NXGPUResourceManager::NXGPUResourceManager(NXMediaManager &mediaManager):
    _mediaManager(mediaManager)
{

}

NXGPUResourceManager::~NXGPUResourceManager()
{
    clear();
}

NXHdl
NXGPUResourceManager::create(const char* path,
                     const GPUResourceType type)
{
    MediaType media_type = kMediaTypeUnknown;
    NXHdl hdl;
    NXHdl hdl_media;
    switch(type)
    {

    case kGPUResourceTypeTexutre:
        media_type = kMediaTypeImage;
        break;
    case kGPUResourceTypeProgram:
        media_type = kMediaTypeProgram;
        break;
    default:
        NXLogError("NXGPUManager::load Unknown GPUResourceType %x", type);
        return hdl;
    }

    hdl_media = _mediaManager.create(path, media_type);

    if (_mediaManager.isError(hdl_media))
    {
        NXLogError("NXGPUManager::load Failed to load media '%s'", path);
        return hdl;
    }

    if (hdl_media.valid())
    {
        ItemInfo* p_info = nullptr;
        hdl = _handles.add(type, p_info);
        if (hdl.valid())
        {
            _mediaManager.incRef(hdl_media);
            p_info->mediaItem = hdl_media;
            p_info->type = type;
            load(p_info);
        }
    }

    return hdl;
}

NXHdl
NXGPUResourceManager::add(NXGPUResource* pResource,
                  const GPUResourceType type)
{
    NXHdl hdl;
    ItemInfo* p_info = nullptr;

    hdl = _handles.add(type, p_info);
    if (hdl.valid())
    {
        p_info->type = type;
        p_info->p_res = pResource;
    }
    return hdl;
}

NXGPUResource*
NXGPUResourceManager::get(const NXHdl hdl)
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        return p_info->p_res;
    }
    return nullptr;
}

bool
NXGPUResourceManager::isLoaded(const NXHdl hdl) const
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        if (!p_info->mediaItem.valid())
        {
            return p_info->p_res;
        }
        else
        {
            return (_mediaManager.isLoaded(p_info->mediaItem) && p_info->p_res);
        }
    }
    return false;
}

bool
NXGPUResourceManager::isError(const NXHdl hdl) const
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        if (!p_info->mediaItem.valid())
        {
            return !p_info->p_res;
        }
        else
        {
            return (_mediaManager.isError(p_info->mediaItem) || !p_info->p_res);
        }
    }
    return false;
}

void
NXGPUResourceManager::remove(const NXHdl hdl)
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        _mediaManager.decRef(p_info->mediaItem);
        _handles.remove(hdl);
    }
}

void
NXGPUResourceManager::incRef(const NXHdl hdl)
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        NXAtomicInc32(&p_info->refs);
    }
}

void
NXGPUResourceManager::decRef(const NXHdl hdl)
{
    ItemInfo* p_info = nullptr;
    if (_handles.get(hdl, p_info))
    {
        if (NXAtomicDec32(&p_info->refs) < 0)
        {
            remove(hdl);
        }
    }
}

nx_u32
NXGPUResourceManager::count() const
{
    return _handles.count();
}

void
NXGPUResourceManager::clear()
{
    _handles.reset();
}

void
NXGPUResourceManager::load(ItemInfo* pInfo)
{
    switch(pInfo->type)
    {
    case kGPUResourceTypeTexutre:
        loadTexture(pInfo);
        break;
    case kGPUResourceTypeProgram:
        loadProgram(pInfo);
        break;
    default:
        NXLogError("GPUManager::Load Uknown type %x", pInfo->type);
        break;
    }
}

void
NXGPUResourceManager::loadTexture(ItemInfo* pInfo)
{
    NXImage* p_image = reinterpret_cast<NXImage*>(_mediaManager.get(pInfo->mediaItem));
    if (!p_image)
    {
        NXLogError("GPUManager::loadTexture: No image present");
        return;
    }

    pInfo->p_res = NXOGLTexture::create(*p_image);
}

void
NXGPUResourceManager::loadProgram(ItemInfo* pInfo)
{
    NXGPUProgramSource* p_source = static_cast<NXGPUProgramSource*>(_mediaManager.get(pInfo->mediaItem));
    if (!p_source)
    {
        NXLogError("GPUManager::loadProgram: No program source present");
        return;
    }

    pInfo->p_res = NXOGLProgram::create(p_source);
}

}
