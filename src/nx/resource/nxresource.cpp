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
#include "nxresource.h"

namespace nx
{

NXResource::NXResource(const char* name,
                       const char* path,
                       const nxresourcetype_t type,
                       const nx_u32 flags):
    _flags(flags & kResourceFlagMask),
    _state(kResourceStateUnloaded),
    _sizeBytes(0),
    _name(name),
    _path((path) ? path : "")
{
    NX_ASSERT(type & NX_HDL_TYPE_MASK);
    _flags |= (type & NX_HDL_TYPE_MASK);
}

NXResource::~NXResource()
{
    NX_ASSERT(isUnloaded());
}

ResourceState
NXResource::state() const
{
    return _state;
}

size_t
NXResource::size() const
{
    return _sizeBytes;
}

const char*
NXResource::name() const
{
    return _name.c_str();
}

const char*
NXResource::path() const
{
    return _path.c_str();
}

bool
NXResource::isLoaded() const
{
    return state() == kResourceStateLoaded;
}

bool
NXResource::isUnloaded() const
{
    return state() == kResourceStateUnloaded;
}

bool
NXResource::isError() const
{
    return state() == kResourceStateError;
}

bool
NXResource::isReloadable() const
{
    return !_path.empty();
}

nxresourcetype_t
NXResource::resourceType() const
{
    return _flags & NX_HDL_TYPE_MASK;
}

nx_u32
NXResource::resourceFlags() const
{
    return _flags & kResourceFlagMask;
}

void
NXResource::setState(const ResourceState state)
{
    _state = state;
}

void
NXResource::setSize(const size_t size)
{
    _sizeBytes = size;
}

}
