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
#include "nx/media/nx3dmodel.h"
#include "nx/io/nxiobase.h"
#include "nx/gpu/nxgpushaderinput.h"
#include "nx3dmodel_generated.h"

namespace nx
{

NX3DModelMesh::NX3DModelMesh(const idlMesh* pMesh):
    _pMesh(pMesh)
{
    NX_ASSERT(pMesh);
}

nx_u32
NX3DModelMesh::vertexCount() const
{
    return _pMesh->vertCount();
}

nx_u32
NX3DModelMesh::indexCount() const
{
    return _pMesh->indexCount();
}

GPUDrawMode
NX3DModelMesh::drawMode() const
{
    return static_cast<GPUDrawMode>(_pMesh->drawMode());
}

nx_u32
NX3DModelMesh::numDataBuffers() const
{
    return _pMesh->localBuffers()->size();
}

bool
NX3DModelMesh::hasIndexBuffer() const
{
    return _pMesh->indexCount() != 0;
}

nx_u32
NX3DModelMesh::indexBufferOffset() const
{
    return _pMesh->localIdxBuffer()->offset();
}

bool
NX3DModelMesh::dataBufferInfo(const nx_u32 idx,
                              nx_u32& meshBufferIdx,
                              nx_u32& offset) const
{
    if (idx < numDataBuffers())
    {
        auto buf = _pMesh->localBuffers()->Get(idx);
        meshBufferIdx =  buf->bind_idx();
        offset = buf->offset();
        return true;
    }
    return false;
}

nx_u32
NX3DModelMesh::numShaderInputs() const
{
    return _pMesh->input()->size();
}

bool
NX3DModelMesh::fillShaderInputdesc(const nx_u32 idx,
                                   NXGPUShaderInputDesc& desc) const
{
    if (idx < numShaderInputs())
    {
        const idlShaderInput* input = _pMesh->input()->Get(idx);

        desc.binding_idx = input->bind_idx();
        desc.data_count = input->data_count();
        desc.data_idx = input->data_idx();
        desc.data_offset = input->data_offset();
        desc.data_type = input->data_type();
        desc.enabled = 1;
        return true;
    }
    return false;
}


NX3DModel*
NX3DModel::load(NXIOBase* pIO)
{
    size_t file_size = pIO->size();

    if (!file_size)
    {
        NXLogError("NX3DModel::load: Invalid IO, need to know file size");
        return nullptr;
    }

    void* ptr = NXMalloc(file_size);

    if (pIO->read(ptr, file_size) != file_size)
    {
        NXLogError("NX3DModel::load: Failed to read into memory");
        NXFree(ptr);
        return nullptr;
    }
    flatbuffers::Verifier verifier(static_cast<const uint8_t*>(ptr), file_size);
    if (!VerifyidlModelBuffer(verifier))
    {
        NXLogError("NX3DModel::load: Invalid 3dmodel format");
        NXFree(ptr);
        return nullptr;
    }

    return  new NX3DModel(ptr, file_size);
}

NX3DModel::~NX3DModel()
{
    unload();
    NX_ASSERT(_pData == nullptr);
}

void
NX3DModel::logInfo() const
{
    size_t index_buffer_size = _pModel->globalIdxBuffer()->data()->size();
    NXLog("NX3DModel:");
    NXLog("  Num Meshes             : %u", (nx_u32)_pModel->meshes()->size());
    NXLog("  Num Buffers            : %u", (nx_u32)_pModel->globalBuffers()->size());
    NXLog("  Idx Buffer             : %s", index_buffer_size ? "yes" : "no");
    if (index_buffer_size)
    {
        NXLog("  Idx Buffer Size        : %u", index_buffer_size);
    }
    auto global_buffers = _pModel->globalBuffers();


    for(size_t i = 0; i < global_buffers->size(); ++i)
    {
        auto global_buffer = global_buffers->Get(i);
        NXLog("  Buffer (%u):", i);
        NXLog("    bind_idx             : %x", global_buffer->bind_idx());
        NXLog("    size                 : %u", global_buffer->data()->size());
    }


    auto meshes = _pModel->meshes();

    for (size_t i = 0; i < meshes->size(); ++i)
    {
        auto mesh = meshes->Get(i);

        NXLog("  Mesh (%u):", i);
        NXLog("    Draw Mode            : %x", mesh->drawMode());
        NXLog("    Num Vertices         : %u", mesh->vertCount());
        NXLog("    Num Indices          : %u", mesh->indexCount());
        auto idxBuffer = mesh->localIdxBuffer();
        NXLog("    Index Buffer (offset): %u", idxBuffer->offset());

        auto buffers = mesh->localBuffers();
        for (size_t j = 0; j < buffers->size(); ++j)
        {
            auto buffer = buffers->Get(j);
            NXLog("    Vert. Buffer (id:off): %u:%u", buffer->bind_idx(), buffer->offset());
        }

        auto shaderInput = mesh->input();

        for (size_t j = 0; j < shaderInput->size(); ++j)
        {
            auto cur_input = shaderInput->Get(j);
            NXLog("    Shader Input (%u):", j);
            NXLog("      bind_idx           : %u", cur_input->bind_idx());
            NXLog("      data_type          : %u", cur_input->data_type());
            NXLog("      data_idx           : %u", cur_input->data_idx());
            NXLog("      data_count         : %u", cur_input->data_count());
            NXLog("      data_offset        : %u", cur_input->data_offset());
        }

    }
}

void
NX3DModel::unload()
{
    if (_pData)
    {
        NXFree((void*)_pData);
        _pData = nullptr;
        _pModel = nullptr;
    }
}

NX3DModel::NX3DModel(const void* pBuffer,
                     const size_t size):
    _pData(pBuffer),
    _dataSize(size),
    _pModel(GetidlModel(pBuffer))
{
    size_t num_meshes = _pModel->meshes()->size();
    _meshes.reserve(num_meshes);

    for (size_t i = 0; i < num_meshes; ++i)
    {
        _meshes.push_back(NX3DModelMesh(_pModel->meshes()->Get(i)));
    }
}

void
NX3DModel::getIndexBuffer(const void*& ptr,
                          nx_u32& size) const
{
    auto global_idx = _pModel->globalIdxBuffer()->data();
    if (global_idx->size())
    {
        ptr = global_idx->Data();
        size = global_idx->size();
    }
    else
    {
        ptr = nullptr;
        size = 0;
    }
}

void
NX3DModel::getDataBuffer(const void*& ptr,
                         nx_u32& size,
                         nx_u32& bindIdx,
                         const nx_u32 idx) const
{
    if (idx < _pModel->globalBuffers()->size())
    {
        const idlBuffer* buffer = _pModel->globalBuffers()->Get(idx);
        ptr = buffer->data()->Data();
        size = buffer->data()->size();
        bindIdx = buffer->bind_idx();
    }
    else
    {
        ptr = nullptr;
        bindIdx = NX_U32_MAX;
        size = 0;
    }
}

size_t
NX3DModel::memorySize() const
{
    return sizeof(NX3DModel) + _dataSize + (sizeof(NX3DModelMesh) * _meshes.capacity());
}

nx_u32
NX3DModel::numDataBuffers() const
{
    return _pModel->globalBuffers()->size();
}

nx_u32
NX3DModel::numSubMeshes() const
{
    return _pModel->meshes()->size();
}

const NX3DModelMesh*
NX3DModel::subMesh(const nx_u32 idx) const
{
    return (idx < _meshes.size()) ? &_meshes[idx] : nullptr;
}

}

