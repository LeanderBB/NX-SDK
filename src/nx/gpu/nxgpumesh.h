//
// This file is part of the NX Project
//
// Copyright (c) 2015 Leander Beernaert
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
#ifndef __NX_GPUMESH_H__
#define __NX_GPUMESH_H__

#include "nx/gpu/nxgpubuffer.h"
#include "nx/gpu/nxgpusubmesh.h"
namespace nx
{

class NXGPUMesh
{
public:
    struct GPUDataBuffer
    {
        NXGPUBufferPtr_t buffer;
        nx_u32 bind_idx;
    };

    static NXTLSharedPtr<NXGPUMesh> create(const NX3DModel& model,
                                           NXGPUInterface& gpuInterface);

    NXGPUMesh();

    ~NXGPUMesh();

    const NXGPUSubMeshPtrVec_t& submeshes() const
    {
        return _subMeshes;
    }

    NXGPUSubMeshPtr_t submesh(const nx_u32 idx) const;

    nx_u32 numSubMeshes() const;

    const GPUDataBuffer* gpuDataBuffer(const nx_u32 idx) const;

    const GPUDataBuffer* gpuDataBufferByBinding(const nx_u32 binding) const;

    const NXGPUBufferPtr_t& gpuIndexBuffer() const
    {
        return _idxBuffer;
    }

    size_t memorySize() const;

private:
    typedef std::vector<GPUDataBuffer> GPUDataBufferVec_t;
    GPUDataBufferVec_t _dataBuffers;
    NXGPUBufferPtr_t _idxBuffer;
    NXGPUSubMeshPtrVec_t _subMeshes;
};

typedef NXTLSharedPtr<NXGPUMesh> NXGPUMeshPtr_t;

}
#endif
