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
#include "nxmediamanager.h"
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"


#include "nx/media/nx3dmodel.h"
#include "nx/media/nximage.h"
#include "nx/gpu/nxgpuprogramsource.h"

namespace nx
{

bool
NXMediaManager::validType(const nx_u32 type)
{
    if (type < kMediaTypeMax)
    {
        switch(type)
        {
        case kMediaTypeImage:
        case kMediaTypeModel:
        case kMediaTypeProgram:
            return true;
        default:
            return false;
        }
    }
    return false;
}


NXMediaManager::NXMediaManager(NXFileManager& fileManager):
    _handles(),
    _hdlByName(),
    _fileManager(fileManager)
{
}

NXMediaManager::~NXMediaManager()
{
    clear();
}

NXHdl
NXMediaManager::create(const char* path,
                       const nx_u32 type)
{
    // validate type

    NXHdl hdl;

    if (!validType(type))
    {
        NXLogError("MediaManager::Create Unknown type %x", type);
        return hdl;
    }

    // create and register handle



    // register handle
    NXMediaItemInfo* p_info = nullptr;
    hdl = _handles.add(type, p_info);

    // check if the handle is valid
    if (hdl.valid())
    {
        p_info->type = type;
        p_info->path = path;
        // perform loading operations
        load(p_info, false);
    }
    // return hdl
    return hdl;
}

bool
NXMediaManager::isLoaded(const NXHdl hdl) const
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        return p_item->state == kMediaItemStateLoaded;
    }
    return false;
}

bool
NXMediaManager::isUnloaded(const NXHdl hdl) const
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        return p_item->state == kMediaItemStateUnloaded;
    }
    return false;
}

bool
NXMediaManager::isError(const NXHdl hdl) const
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        return p_item->state == kMediaItemStateError;
    }
    return false;
}

NXMediaItem*
NXMediaManager::get(const NXHdl hdl) const
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        // Only return if the type matches the handle type
        return ((p_item->type & kMediaTypeMask) == hdl.type) ? p_item->ptr : nullptr;
    }
    return nullptr;
}

NXMediaItem*
NXMediaManager::getWithPath(const char* path) const
{
    // lookup from map
    auto it = _hdlByName.find(path);
    if (it != _hdlByName.end())
    {
        return get(it->second);
    }
    return nullptr;
}

void
NXMediaManager::unload(const NXHdl hdl)
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        unload(p_item);
    }
}

void
NXMediaManager::remove(const NXHdl hdl)
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        if (p_item->refs > 1)
        {
            NXLogWarning("MediaManager::remove : Handle %x still has active references\n", hdl);
        }
        remove(hdl,p_item);
    }
}

void
NXMediaManager::incRef(const NXHdl hdl)
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        NXAtomicInc32(&p_item->refs);
    }
}

void
NXMediaManager::decRef(const NXHdl hdl)
{
    NXMediaItemInfo* p_item = nullptr;
    if(_handles.get(hdl, p_item))
    {
        if(NXAtomicDec32(&p_item->refs) < 0)
        {
            remove(hdl, p_item);
        }
    }
}

nx_u32
NXMediaManager::count() const
{
    return _handles.count();
}

void
NXMediaManager::clear()
{
    _hdlByName.clear();
    _handles.reset();
}


void
NXMediaManager::load(NXMediaItemInfo* pItem,
                     const bool async)
{
    if(async)
    {
        NXLogWarning("MediaManager::load Async loading not yet supported");
    }

    // open io with path,
    NXIOBase* p_io = _fileManager.open(pItem->path.c_str(), kIOAccessModeReadBit);
    if (!p_io)
    {
        NXLogError("MediaManager::load Could not open '%s'", pItem->path.c_str());
        pItem->state = kMediaItemStateError;
    }

    // load resource
    pItem->state = kMediaItemStateLoading;

    switch(pItem->type)
    {
    case kMediaTypeImage:
        pItem->ptr = NXImage::load(p_io);
        break;
    case kMediaTypeModel:
        pItem->ptr = NX3DModel::load(p_io);
        break;
    case kMediaTypeProgram:
        pItem->ptr = NXGPUProgramSource::load(p_io);
        break;
    default:
        NXLogError("MediaManager::load No loading operation available for type '%x'", pItem->type);
    }
    delete p_io;

    if (!pItem->ptr)
    {
        pItem->state = kMediaItemStateError;
    }
    else
    {
        pItem->state = kMediaItemStateLoaded;
    }
}

void
NXMediaManager::unload(NXMediaItemInfo* pItem)
{
    pItem->ptr->unload();
    NX_SAFE_DELETE(pItem->ptr);
}

void
NXMediaManager::remove(const NXHdl hdl,
                       NXMediaItemInfo* pItem)
{
    _hdlByName.erase(pItem->path);
    unload(pItem);
    _handles.remove(hdl);
}

}
