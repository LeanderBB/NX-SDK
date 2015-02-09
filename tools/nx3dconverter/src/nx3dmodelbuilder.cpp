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
#include "nx3dmodelbuilder.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"

namespace nx
{

NX3DModelBuilderHelper::NX3DModelBuilderHelper()
{

}

NX3DModelBuilderHelper::~NX3DModelBuilderHelper()
{

}

NX_INLINE static idlShaderInput
nxGPUShaderInputDescToIdlShaderInput(const NXGPUShaderInputDesc& input)
{
    return idlShaderInput(input.binding_idx,
                          input.data_type,
                          input.data_idx,
                          input.data_count,
                          input.data_offset);
}

bool
NX3DModelBuilderHelper::build(const NXOutputState& output)
{
    std::vector< flatbuffers::Offset<idlMesh>> meshes_offset;

    // build meshes
    for (auto& mesh : output.meshes)
    {


        idlBufferRef index_buffer_ref(mesh.idxBufferRef.idx,
                                      mesh.idxBufferRef.offset);

        // copy vectors
        std::vector<idlBufferRef> buffer_refs;
        for (auto& bufferRef : mesh.bufferRefs)
        {
            if (bufferRef.idx != NX_U8_MAX)
            {
                idlBufferRef buffer_ref(bufferRef.idx, bufferRef.offset);
                buffer_refs.push_back(buffer_ref);
            }
        }
        auto idlvector = _builder.CreateVectorOfStructs(buffer_refs);


        // copy shader descriptions
        std::vector<idlShaderInput> mesh_input;
        std::vector<const idlShaderInput*> meshptr_input;
        for (auto& input : mesh.shaderInput)
        {
            idlShaderInput idl_input = nxGPUShaderInputDescToIdlShaderInput(input);
            mesh_input.push_back(idl_input);
        }
        auto idl_input_vec = _builder.CreateVectorOfStructs(mesh_input);

        // build the mesh
        idlMeshBuilder mesh_builder(_builder);
        mesh_builder.add_drawMode(mesh.drawMode);
        mesh_builder.add_vertCount(mesh.vertCount);
        mesh_builder.add_indexCount(mesh.indexCount);
        mesh_builder.add_localBuffers(idlvector);
        mesh_builder.add_input(idl_input_vec);
        mesh_builder.add_localIdxBuffer(&index_buffer_ref);
        auto mesh_offset = mesh_builder.Finish();
        meshes_offset.push_back(mesh_offset);
    }

    auto mesh_offset_vec = _builder.CreateVector(meshes_offset);



    // create global index buffer

    auto gobalidx_vec = _builder.CreateVector(static_cast<const uint8_t*>(output.globalIndexBuffer.buffer),
                                              output.globalIndexBuffer.size);


    idlBufferBuilder globalidx_builder(_builder);
    globalidx_builder.add_bind_idx(output.globalIndexBuffer.idx);
    globalidx_builder.add_type(idlBufferType_Index);
    globalidx_builder.add_data(gobalidx_vec);
    auto globalidx_offset = globalidx_builder.Finish();


    // create buffers
    std::vector< flatbuffers::Offset<idlBuffer>> globalbuf_vec;

    for (auto& buffer : output.globalBuffers)
    {
        if (!buffer.buffer)
        {
            continue;
        }

        auto buffer_data = _builder.CreateVector(static_cast<const uint8_t*>(buffer.buffer),
                                                 buffer.size);

        idlBufferBuilder buffer_builder(_builder);
        buffer_builder.add_bind_idx(buffer.idx);
        buffer_builder.add_type(idlBufferType_Vertex);
        buffer_builder.add_data(buffer_data);
        auto buffer_offset = buffer_builder.Finish();
        globalbuf_vec.push_back(buffer_offset);
    }

    auto globalbufoffset_vec = _builder.CreateVector(globalbuf_vec);

    idlModelBuilder model_builder(_builder);
    model_builder.add_globalBuffers(globalbufoffset_vec);
    model_builder.add_globalIdxBuffer(globalidx_offset);
    model_builder.add_meshes(mesh_offset_vec);

    // finish
    auto model_offset = model_builder.Finish();
    FinishidlModelBuffer(_builder, model_offset);

    return true;
}


bool
NX3DModelBuilderHelper::write(const char* output)
{
    (void) output;

    std::unique_ptr<NXIOFile> io(NXIOFile::open(output, kIOAccessModeOverwriteBit | kIOAccessModeWriteBit));
    if (!io)
    {
        return false;
    }
    auto ptr = _builder.GetBufferPointer();
    auto size = _builder.GetSize();

    size_t bytes_written = io->write(ptr, size);

    if (bytes_written != size)
    {
        return false;
    }
    return true;
}

}

