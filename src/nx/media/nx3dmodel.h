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
#ifndef __NX_3DMODEL_H__
#define __NX_3DMODEL_H__


#include "nx/gpu/nxgpu.h"

namespace nx
{

class NXIOBase;
struct idlModel;
struct idlMesh;
struct NXGPUShaderInputDesc;

class NX3DModelMesh
{
public:
    NX3DModelMesh(const idlMesh* _pMesh);

    nx_u32 vertexCount() const;

    nx_u32 indexCount() const;

    GPUDrawMode drawMode() const;

    nx_u32 numDataBuffers() const;

    bool hasIndexBuffer() const;

    nx_u32 indexBufferOffset() const;

    bool dataBufferInfo(const nx_u32 idx,
                        nx_u32& meshBufferIdx,
                        nx_u32& offset) const;

    nx_u32 numShaderInputs() const;

    bool fillShaderInputdesc(const nx_u32 idx,
                             NXGPUShaderInputDesc& desc) const;

private:
    const idlMesh* _pMesh;
};

class NX3DModel
{
public:

    static NX3DModel* load(NXIOBase* pIO);

    ~NX3DModel();

    void logInfo() const;

    size_t memorySize() const;

    void getIndexBuffer(const void *&ptr,
                        nx_u32& size) const;

    void getDataBuffer(const void*& ptr,
                       nx_u32& size,
                       nx_u32& bindId,
                       const nx_u32 idx) const;

    nx_u32 numDataBuffers() const;

    nx_u32 numSubMeshes() const;

    const NX3DModelMesh* subMesh(const nx_u32 idx) const;

private:

    NX3DModel(const void* pBuffer,
              const size_t size);

    void unload();

private:
    typedef std::vector<NX3DModelMesh> MeshVec_t;

    const void* _pData;
    const size_t _dataSize;
    const idlModel* _pModel;
    MeshVec_t _meshes;
};


}

#endif
