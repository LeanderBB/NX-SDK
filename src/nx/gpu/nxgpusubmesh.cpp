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
#include "nx/nxcore.h"
#include "nx/gpu/nxgpusubmesh.h"
#include "nx/media/nx3dmodel.h"
#include "nx/gpu/nxgpumesh.h"
#include "nx/gpu/nxgpuinterface.h"

namespace nx
{

NXTLSharedPtr<NXGPUSubMesh>
NXGPUSubMesh::create(const NX3DModel& model,
                     const NXGPUMesh* const pParent,
                     const nx_u32 subMeshIdx,
                     NXGPUInterface& gpuInterface)
{
    NX_ASSERT(pParent);
    NXGPUSubMeshPtr_t submesh;

    const NX3DModelMesh* model_mesh = model.subMesh(subMeshIdx);
    if (!model_mesh)
    {
        NXLogError("NXGPUSubMesh::create: Failed to get model submesh (%u)", subMeshIdx);
        return submesh;
    }

    // create submesh
    submesh = nxMakeTLShared<NXGPUSubMesh>(pParent, gpuInterface);

    // add all shader inputs

    const nx_u32 n_shader_input = model_mesh->numShaderInputs();

    for(nx_u32 i = 0; i < n_shader_input; ++i)
    {
        NXGPUShaderInputDesc input_desc;
        if (!model_mesh->fillShaderInputdesc(i, input_desc))
        {
            NXLogError("NXGPUSubMesh::create: Failed to get fill shader input (%u)", i);
            return NXGPUSubMeshPtr_t();
        }

        if(!submesh->_shaderInput.addInput(input_desc))
        {
            NXLogError("NXGPUSubMesh::create: Failed to add shader input (%u)", i);
            return NXGPUSubMeshPtr_t();
        }
    }

    // add all data buffers
    const nx_u32 n_data_buffers = model_mesh->numDataBuffers();
    for (nx_u32 i = 0; i < n_data_buffers; ++i)
    {
        // get buffer info
        nx_u32 local_offset, binding_idx;
        if (!model_mesh->dataBufferInfo(i, binding_idx, local_offset))
        {
            NXLogError("NXGPUSubMesh::create: Failed to get info for data buffer (%u)", i);
            return NXGPUSubMeshPtr_t();
        }

        // get global buffer from mesh
        const NXGPUMesh::GPUDataBuffer* parent_buffer =  submesh->_pParent->gpuDataBufferByBinding(binding_idx);
        if (!parent_buffer)
        {
            NXLogError("NXGPUSubMesh::create: Failed to get parent mesh buffer (%u)",
                       binding_idx);
            return NXGPUSubMeshPtr_t();
        }

        // calculate correct buffer offset
        NXGPUBufferHdl buffer_hdl = parent_buffer->buffer->hdl();
        buffer_hdl.offset += local_offset;
        if (!submesh->_shaderInput.addBuffer(buffer_hdl, parent_buffer->bind_idx))
        {
            NXLogError("NXGPUSubMesh::create: Failed to add buffer (%u) to shader input",i);
            return NXGPUSubMeshPtr_t();
        }
    }

    // add index buffer
    if (model_mesh->hasIndexBuffer())
    {
        const NXGPUBufferPtr_t& idx_buffer_ptr = submesh->_pParent->gpuIndexBuffer();
        if(!idx_buffer_ptr)
        {
            NXLogError("NXGPUSubMesh::create: Failed to get index buffer from parent");
            return NXGPUSubMeshPtr_t();
        }

        const nx_u32 idx_offset = model_mesh->indexBufferOffset();

        NXGPUBufferHdl buffer_hdl = idx_buffer_ptr->hdl();
        buffer_hdl.offset += idx_offset;
        submesh->_shaderInput.setIndexBuffer(buffer_hdl);
    }

    submesh->_idxCount = model_mesh->indexCount();
    submesh->_vertCount = model_mesh->vertexCount();
    submesh->_drawMode = model_mesh->drawMode();

    // create gpu input
    submesh->_gpuHdl = gpuInterface.allocShaderInput(submesh->_shaderInput);
    if (!submesh->_gpuHdl)
    {
        NXLogError("NXGPUSubMesh::create: Failed to to create shader input");
        submesh.reset();
    }
    return submesh;
}

NXGPUSubMesh::NXGPUSubMesh(const NXGPUMesh *pParent,
                           NXGPUInterface &_gpuInterface):
    _pParent(pParent),
    _gpuInterface(_gpuInterface),
    _drawMode(kGPUDrawModeTriangles),
    _vertCount(0),
    _idxCount(0),
    _shaderInput()
{

}

NXGPUSubMesh::~NXGPUSubMesh()
{
   _gpuInterface.releaseShaderInput(_gpuHdl);
}

}
