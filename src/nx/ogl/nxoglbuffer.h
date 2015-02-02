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
#ifndef __NX_OGLBUFFER_H__
#define __NX_OGLBUFFER_H__


#include "nx/ogl/nxoglobj.h"
#include "nx/gpu/nxgpubuffer.h"

namespace nx
{

nx_u32 NXOGLBufferType(const GPUBufferType type);

class NXOGLBuffer : public NXOGLObj
{

public:

    static NXTLSharedPtr<NXOGLBuffer> create(const NXGPUBufferDesc& desc);

    static NXTLSharedPtr<NXOGLBuffer> create(const NXGPUBufferDesc& desc,
                                            const void* pData);

    NXOGLBuffer(const NXGPUBufferDesc& desc);

    ~NXOGLBuffer();

    GPUBufferType type() const;

    nx_u32 oglType() const;

    nx_u32 size() const;



protected:



    void allocateStorage(const void *ptr = nullptr);

private:
    NX_CPP_NO_COPY(NXOGLBuffer);

protected:
    NXGPUBufferDesc _desc;
    nx_u32 _oglType;
    bool _mapped;
};

typedef NXTLSharedPtr<NXOGLBuffer> NXOGLBufferPtr_t;

}
#endif
