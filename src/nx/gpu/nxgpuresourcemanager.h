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
#ifndef __NX_GPUMANAGER_H__
#define __NX_GPUMANAGER_H__

#include "nx/hdl/nxhdlmanager.h"
#include "nx/gpu/nxgpuresource.h"
namespace nx
{

enum GPUResourceType
{
    kGPUResourceTypeUnknown = 0,
    kGPUResourceTypeTexutre,
    kGPUResourceTypeProgram,
    kGPUResourceTypeBuffer,
    kGPUResourceTypeFrameBuffer
};

class NXImage;
class NXMediaManager;
class NXGPUInterface;
class NXGPUResourceManager
{
public:
    NXGPUResourceManager(NXMediaManager& mediaManager,
                         NXGPUInterface* pGPUInterface);

    ~NXGPUResourceManager();

    NXHdl create(const char* path,
               const GPUResourceType type);

    NXHdl add(NXGPUResource* pResource,
              const GPUResourceType type);

    NXGPUResource* get(const NXHdl hdl);

    bool isLoaded(const NXHdl hdl) const;

    bool isError(const NXHdl hdl) const;

    void remove(const NXHdl hdl);

    void incRef(const NXHdl hdl);

    void decRef(const NXHdl hdl);

    nx_u32 count() const;

    void clear();

protected:

    class ItemInfo
    {
    public:

        ItemInfo():
            p_res(nullptr),
            type(0),
            refs(0),
            mediaItem()
        {
        }

        ~ItemInfo()
        {
            delete p_res;
        }

        mutable NXGPUResource* p_res;
        nx_u32 type;
        nx_atomic32_t refs;
        NXHdl mediaItem;
    };

   void load(ItemInfo* pInfo);

   void loadTexture(ItemInfo* pInfo);

   void loadProgram(ItemInfo* pInfo);

private:
    NX_CPP_NO_COPY(NXGPUResourceManager);

protected:
    NXMediaManager& _mediaManager;
    NXGPUInterface* _pGPUInterface;
    NXHandleManager<ItemInfo> _handles;
};

}
#endif
