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
//
#ifndef __NX_RESOURCE_H__
#define __NX_RESOURCE_H__

#include "nx/util/nxtlsharedptr.h"

namespace nx
{

enum ResourceState
{
    kResourceStateUnloaded,
    kResourceStateLoaded,
    kResourceStateLoading,
    kResourceStateError
};


enum ResourceFlag
{
    kResourceFlagSkipPathRegistrationBit = NX_BIT(1 << 8),
    kResourceFlagRequireGPUInterfaceBit = NX_BIT(1 << 9),

    kResourceFlagMask = 0xFFFFFFF0
};


typedef nx_u32 nxresourcetype_t;

class NXResourceManager;
class NXResource
{
public:

    virtual ~NXResource();

    virtual void load(NXResourceManager& resourceManager) = 0;

    virtual void unload(NXResourceManager& resourceManager) = 0;

    ResourceState state() const;

    size_t size() const;

    const char* name() const;

    const char* path() const;

    bool isLoaded() const;

    bool isUnloaded() const;

    bool isError() const;

    bool isReloadable() const;

    nxresourcetype_t resourceType() const;

    nx_u32 resourceFlags() const;

protected:

    NXResource(const char* name,
               const char* path,
               const nxresourcetype_t type,
               const nx_u32 flags);

    void setState(const ResourceState state);

    void setSize(const size_t size);

private:
    NX_CPP_NO_COPY(NXResource);

protected:
    nx_u32 _flags;
    volatile ResourceState _state;
    size_t _sizeBytes;
    const NXString _name;
    const NXString _path;

};

typedef NXTLSharedPtr<NXResource> NXResourcePtr_t;

}

#endif // NXRESOURCE_H
