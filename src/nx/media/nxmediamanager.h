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
#ifndef __NX_MEDIAMANAGER_H__
#define __NX_MEDIAMANAGER_H__

#include "nx/hdl/nxhdlmanager.h"
#include "nx/media/nxmediaitem.h"
namespace nx
{

enum MediaType
{
    kMediaTypeUnknown = 0,
    kMediaTypeImage,
    kMediaTypeModel,
    kMediaTypeSound,
    kMediaTypeProgram,
    kMediaTypeMax,
    kMediaTypeMask = 0x1F
};

class NXFileManager;
class NXIOBase;
class NXMediaManager
{
public:

    static bool validType(const nx_u32 type);

    NXMediaManager(NXFileManager& fileManager);

    ~NXMediaManager();

    NXHdl create(const char* path,
                    const nx_u32 type);

    bool isLoaded(const NXHdl hdl) const;

    bool isUnloaded(const NXHdl hdl) const;

    bool isError(const NXHdl hdl) const;

    NXMediaItem* get(const NXHdl hdl) const;

    NXMediaItem* getWithPath(const char* path) const;

    void unload(const NXHdl hdl);

    void remove(const NXHdl hdl);

    void incRef(const NXHdl hdl);

    void decRef(const NXHdl hdl);

    nx_u32 count() const;

    void clear();


private:
    NX_CPP_NO_COPY(NXMediaManager);

private:

    enum MediaItemState
    {
        kMediaItemStateUnloaded,
        kMediaItemStateLoaded,
        kMediaItemStateError,
        kMediaItemStateLoading,
    };

    class NXMediaItemInfo
    {
    public:

        NXMediaItemInfo():
            refs(0),
            type(kMediaTypeMax),
            state(kMediaItemStateUnloaded),
            ptr(nullptr),
            path()
        {
        }

        ~NXMediaItemInfo()
        {
            if (ptr)
            {
                ptr->unload();
                delete ptr;
            }
        }

        nx_atomic32_t refs;
        nx_u32 type;
        volatile MediaItemState state;
        NXMediaItem* ptr;
        NXString path;
    };

    void load(NXMediaItemInfo* pItem,
              const bool async);

    void unload(NXMediaItemInfo* pItem);

    void remove(const NXHdl hdl,
                 NXMediaItemInfo* pItem);



private:
    typedef std::unordered_map<NXString, NXHdl> HdlMap_t;


    NXHandleManager<NXMediaItemInfo> _handles;
    HdlMap_t _hdlByName;
    NXFileManager& _fileManager;
};

}
#endif // NXMEDIAMANAGER_H
