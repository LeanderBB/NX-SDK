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
#ifndef __NX_IMAGERESOURCE_H__
#define __NX_IMAGERESOURCE_H__

#include "nx/resource/nxresource.h"

namespace nx
{
class NXImage;
class NXImageResource : public NXResource
{
public:

    static const nxresourcetype_t sType;

    static NXResourcePtr_t create(const char* name,
                                  const char* path);

    NXImageResource(const char* name,
                    const char* path,
                    const bool skipPathReg = false);

    virtual ~NXImageResource();

    void load(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    void unload(NXResourceManager& resourceManager) NX_CPP_OVERRIDE;

    const NXImage* image() const
    {
        return _pImage;
    }

protected:
    NXImage* _pImage;
};

typedef NXTLSharedPtr<NXImageResource> NXImageResourcePtr_t;

}
#endif
