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
#include "nx/gpu/nxgpumesh.h"
#include "nx/gpu/nxgpuinterface.h"
#include "nx/media/nx3dmodel.h"
#include "nx/gpu/nxgpuinterface.h"
namespace nx
{

NXTLSharedPtr<NXGPUMesh>
NXGPUMesh::create(const NX3DModel& model,
                  NXGPUInterface &gpuInterface)
{
    NXGPUMeshPtr_t gpu_mesh;

    const void* buffer_ptr = nullptr;
    nx_u32 buffer_size = 0;
    nx_u32 buffer_binding = 0;

    NXGPUBufferPtr_t idxbuf_ptr;
    GPUDataBufferVec_t databuf_ptrs;

    auto& buffer_manager = gpuInterface.gpuBufferManager();

    // create and init index buffer
    model.getIndexBuffer(buffer_ptr, buffer_size);

    if (buffer_ptr)
    {
        NXGPUBufferDesc idxbuf_desc;
        idxbuf_desc.size = buffer_size;
        idxbuf_desc.flags = kGPUBufferAccessStaticBit;
        idxbuf_desc.type = kGPUBufferTypeIndex;
        idxbuf_desc.mode = 0;
        idxbuf_desc.data = buffer_ptr;
        idxbuf_ptr = buffer_manager.create(idxbuf_desc);

        if (!idxbuf_ptr)
        {
            NXLogError("NXMesh::create: Could not create index buffer");
            goto exit_point;
        }
    }

    // load data buffers
    {
        const nx_u32 num_databufs = model.numDataBuffers();
        databuf_ptrs.resize(num_databufs);
        for (nx_u32 i = 0; i < num_databufs; ++i)
        {
            model.getDataBuffer(buffer_ptr, buffer_size, buffer_binding, i);
            if (!buffer_ptr)
            {
                NXLogError("NXMesh::create: Could not get data buffer (%u)", i);
                goto exit_point;
            }

            NXGPUBufferDesc databuf_desc;
            databuf_desc.size = buffer_size;
            databuf_desc.flags = kGPUBufferAccessStaticBit;
            databuf_desc.type = kGPUBufferTypeData;
            databuf_desc.mode = 0;
            databuf_desc.data = buffer_ptr;

            auto& data_buf = databuf_ptrs[i];
            data_buf.bind_idx = buffer_binding;
            data_buf.buffer = buffer_manager.create(databuf_desc);

            if (!data_buf.buffer)
            {
                NXLogError("NXMesh::create: Could not create data buffer (%u)", i);
                goto exit_point;
            }
        }
    }

    // create GPUMesh and move data
    gpu_mesh = nxMakeTLShared<NXGPUMesh>();
    gpu_mesh->_idxBuffer = std::move(idxbuf_ptr);
    gpu_mesh->_dataBuffers = std::move(databuf_ptrs);

    // create submeshes
    for (nx_u32 i = 0; i < model.numSubMeshes(); ++i)
    {
        NXGPUSubMeshPtr_t submesh_ptr = NXGPUSubMesh::create(model, gpu_mesh.get(),
                                                             i,buffer_manager.gpuInterface());
        if (!submesh_ptr)
        {
            NXLogError("NXMesh::create: Could not create sub mesh (%u)", i);
            gpu_mesh.reset();
            break;
        }
        gpu_mesh->_subMeshes.push_back(submesh_ptr);
    }

exit_point:
    return gpu_mesh;

}

NXGPUMesh::NXGPUMesh()
{

}

NXGPUMesh::~NXGPUMesh()
{

}

NXGPUSubMeshPtr_t
NXGPUMesh::submesh(const nx_u32 idx) const
{
    return (idx < _subMeshes.size()) ? _subMeshes[idx] : NXGPUSubMeshPtr_t();
}

nx_u32
NXGPUMesh::numSubMeshes() const
{
    return _subMeshes.size();
}

const NXGPUMesh::GPUDataBuffer*
NXGPUMesh::gpuDataBuffer(const nx_u32 idx) const
{
    return (idx < _dataBuffers.size()) ? &_dataBuffers[idx] : nullptr;
}

const NXGPUMesh::GPUDataBuffer*
NXGPUMesh::gpuDataBufferByBinding(const nx_u32 binding) const
{
    for(auto& val : _dataBuffers)
    {
        if (val.bind_idx == binding)
        {
            return &val;
        }
    }
    return nullptr;
}

size_t
NXGPUMesh::memorySize() const
{
    return sizeof(NXGPUMesh)
            + (_dataBuffers.size() * sizeof(GPUDataBufferVec_t::value_type))
            + (_subMeshes.size() * sizeof(NXGPUSubMeshPtrVec_t::value_type));
}

}

