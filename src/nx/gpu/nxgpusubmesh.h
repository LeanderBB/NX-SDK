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
#ifndef __NX_GPUSUBMESH_H__
#define __NX_GPUSUBMESH_H__

#include "nx/gpu/nxgpushaderinput.h"
namespace nx
{
class NXGPUMesh;
class NX3DModel;
class NXGPUInterface;
class NXGPUSubMesh
{
public:

    static NXTLSharedPtr<NXGPUSubMesh> create(const NX3DModel& model,
                                              const NXGPUMesh* const pParent,
                                              const nx_u32 subMeshIdx,
                                              NXGPUInterface& gpuInterface);

    NXGPUSubMesh(const NXGPUMesh* pParent,
                 NXGPUInterface& _gpuInterface);

    ~NXGPUSubMesh();

    GPUDrawMode drawMode() const
    {
        return _drawMode;
    }

    nx_u32 vertexCount() const
    {
        return _vertCount;
    }

    nx_u32 indexCount() const
    {
        return _idxCount;
    }

    NXHdl gpuHdl() const
    {
        return _gpuHdl;
    }
private:

    NX_CPP_NO_COPY(NXGPUSubMesh);

private:
    const NXGPUMesh* const _pParent;
    NXGPUInterface& _gpuInterface;
    GPUDrawMode _drawMode;
    nx_u32 _vertCount;
    nx_u32 _idxCount;
    NXHdl _gpuHdl;
    NXGPUShaderInput _shaderInput;
};

typedef NXTLSharedPtr<NXGPUSubMesh> NXGPUSubMeshPtr_t;
typedef std::vector<NXGPUSubMeshPtr_t> NXGPUSubMeshPtrVec_t;
}
#endif
