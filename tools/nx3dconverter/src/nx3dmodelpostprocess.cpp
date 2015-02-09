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
#include "nx3dmodelpostprocess.h"


namespace nx
{

NX_INLINE
void* nxBufferOffset(void* buffer,
                     const size_t offset)
{
    return static_cast<char*>(buffer) + offset;
}


NX_INLINE
const void* nxBufferOffset(const void* buffer,
                           const size_t offset)
{
    return static_cast<const char*>(buffer) + offset;
}

NX3DModelPostProcess::NX3DModelPostProcess(const NXInputStateVec_t &input):
    _input(std::move(input)),
    _output()
{

}

NX3DModelPostProcess::~NX3DModelPostProcess()
{

}

bool
NX3DModelPostProcess::buildOutputData(const nx_u32 flags)
{
    if (flags & kPostProcessFlagInterleaved)
    {
        NXLogError("PostProcess: Interleaved option not yet implemented!!");
        return false;
    }
    else
    {
        if (!validSeparateBuffers())
        {
            return false;
        }

        return buildSeparate();
    }
}

bool
NX3DModelPostProcess::validInterleavedBuffers() const
{
    NXLogWarning("Validating interleaved buffers not yet implemented");
    return false;
}

bool
NX3DModelPostProcess::validSeparateBuffers() const
{
    nx_u32 count_vert = 0;
    nx_u32 count_color = 0;
    nx_u32 idx_count = 0;

    for (auto& input : _input)
    {
        nx_u32 count_vert_next = count_vert + input.vertices.size() * sizeof(float);
        nx_u32 count_color_next = count_color + input.color.size() * sizeof(float);
        nx_u32 idx_count_next = idx_count + input.indices.size() * sizeof(nx_u32);

        if (count_vert_next < count_vert)
        {
            NXLogError("Total vertex count will overflow buffer capacity");
            return false;
        }

        if (count_color_next < count_color)
        {
            NXLogError("Total color count will overflow buffer capacity");
            return false;
        }

        if (idx_count_next < idx_count)
        {
            NXLogError("Total index count will overflow buffer capacity");
            return false;
        }

        count_vert += count_vert_next;
        count_color += count_color_next;
        idx_count += idx_count_next;
    }

    return true;
}

bool
NX3DModelPostProcess::buildSeparate()
{
    nx_u32 total_vert = 0;
    nx_u32 total_texcoord0 = 0;
    nx_u32 total_texcoord1 = 0;
    nx_u32 total_normals = 0;
    nx_u32 total_tangents = 0;
    nx_u32 total_binormal = 0;
    nx_u32 total_indices = 0;
    nx_u32 total_color = 0;

    nx_u32 offset_vert = 0;
    nx_u32 offset_texcoord0 = 0;
    nx_u32 offset_texcoord1 = 0;
    nx_u32 offset_normals = 0;
    nx_u32 offset_tangents = 0;
    nx_u32 offset_binormal = 0;
    nx_u32 offset_indices = 0;
    nx_u32 offset_color = 0;

    // build total count;
    for (auto& input : _input)
    {
        total_vert += input.vertices.size();
        total_texcoord0 += input.texcoord0.size();
        total_texcoord1 += input.texcoord1.size();
        total_normals += input.normals.size();
        total_tangents += input.tangents.size();
        total_binormal += input.binormal.size();
        total_color += input.color.size();
        total_indices += input.indices.size();
    }

    // create buffers
    _output.globalBuffers.resize(kOutputSeparateBufferBindingMax);

    // create vertex buffer
    {
        NXOutputBuffer& vertices = _output.globalBuffers[kOutputSeparateBufferBindingVertices];
        vertices.idx = kOutputSeparateBufferBindingVertices;
        vertices.size = total_vert * sizeof(float);
        vertices.buffer = NXMalloc(vertices.size);
    }
    // create texture buffer 0
    if (total_texcoord0)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTexcoord0];
        buffer.idx = kOutputSeparateBufferBindingTexcoord0;
        buffer.size = total_texcoord0 * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create texture buffer 1
    if (total_texcoord1)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTexcoord1];
        buffer.idx = kOutputSeparateBufferBindingTexcoord1;
        buffer.size = total_texcoord1 * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create normal buffer
    if (total_normals)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingNormals];
        buffer.idx = kOutputSeparateBufferBindingNormals;
        buffer.size = total_normals * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create tangents buffer
    if (total_tangents)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTangents];
        buffer.idx = kOutputSeparateBufferBindingTangents;
        buffer.size = total_tangents * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create binormal buffer
    if (total_binormal)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingBinormals];
        buffer.idx = kOutputSeparateBufferBindingBinormals;
        buffer.size = total_binormal * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create color buffer
    if (total_color)
    {
        NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingColor];
        buffer.idx = kOutputSeparateBufferBindingColor;
        buffer.size = total_color * sizeof(float);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // create index buffer
    if (total_indices)
    {
        NXOutputBuffer& buffer = _output.globalIndexBuffer;
        buffer.size = total_indices * sizeof(nx_u32);
        buffer.buffer = NXMalloc(buffer.size);
    }

    // copy data and build submeshes
    _output.meshes.resize(_input.size());

    for (unsigned int i = 0; i < _input.size(); ++i)
    {
        const NXInputState& cur_input = _input[i];
        NXOutputMesh& cur_mesh = _output.meshes[i];
        cur_mesh.vertCount = cur_input.vertices.size();
        cur_mesh.indexCount = cur_input.indices.size();
        cur_mesh.bufferRefs.resize(kOutputSeparateBufferBindingMax);

        // setup index buffer
        if (cur_input.indices.size())
        {
            nx_u32 idx_size = cur_input.indices.size() * sizeof(nx_u32);
            memcpy(nxBufferOffset(_output.globalIndexBuffer.buffer, offset_indices),
                   cur_input.indices.data(), idx_size);
            cur_mesh.idxBufferRef.offset = offset_indices;
            offset_indices += idx_size;
        }

        // copy vertex data
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingVertices;
            shader_input.data_count = 3;
            shader_input.data_idx = kGPUShaderInputIdxVertices;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.vertices.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingVertices];
            void* ptr = nxBufferOffset(buffer.buffer, offset_vert);
            memcpy(ptr, cur_input.vertices.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingVertices];
            buffer_ref.idx = kOutputSeparateBufferBindingVertices;
            buffer_ref.offset = offset_vert;

            // update offset
            offset_vert += copy_size;
        }


        // copy texcoord 0
        if (!cur_input.texcoord0.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingTexcoord0;
            shader_input.data_count = 2;
            shader_input.data_idx = kGPUShaderInputIdxTexCoord0;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.texcoord0.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTexcoord0];
            void* ptr = nxBufferOffset(buffer.buffer, offset_texcoord0);
            memcpy(ptr, cur_input.texcoord0.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingTexcoord0];
            buffer_ref.idx = kOutputSeparateBufferBindingTexcoord0;
            buffer_ref.offset = offset_texcoord0;

            // update offset
            offset_texcoord0 += copy_size;
        }

        // copy texcoord 1
        if (!cur_input.texcoord1.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingTexcoord1;
            shader_input.data_count = 2;
            shader_input.data_idx = kGPUShaderInputIdxTexCoord1;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.texcoord1.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTexcoord1];
            void* ptr = nxBufferOffset(buffer.buffer, offset_texcoord1);
            memcpy(ptr, cur_input.texcoord1.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingTexcoord1];
            buffer_ref.idx = kOutputSeparateBufferBindingTexcoord0;
            buffer_ref.offset = offset_texcoord1;

            // update offset
            offset_texcoord1 += copy_size;
        }

        // copy normals
        if (!cur_input.normals.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingNormals;
            shader_input.data_count = 3;
            shader_input.data_idx = kGPUShaderInputIdxNormals;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.normals.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingNormals];
            void* ptr = nxBufferOffset(buffer.buffer, offset_normals);
            memcpy(ptr, cur_input.normals.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingNormals];
            buffer_ref.idx = kOutputSeparateBufferBindingNormals;
            buffer_ref.offset = offset_normals;

            // update offset
            offset_normals += copy_size;
        }

        // copy normals
        if (!cur_input.tangents.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingTangents;
            shader_input.data_count = 3;
            shader_input.data_idx = kGPUShaderInputIdxTangent;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.tangents.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingTangents];
            void* ptr = nxBufferOffset(buffer.buffer, offset_tangents);
            memcpy(ptr, cur_input.tangents.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingTangents];
            buffer_ref.idx = kOutputSeparateBufferBindingTangents;
            buffer_ref.offset = offset_tangents;

            // update offset
            offset_tangents += copy_size;
        }

        // copy binormals
        if (!cur_input.binormal.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingBinormals;
            shader_input.data_count = 3;
            shader_input.data_idx = kGPUShaderInputIdxBinormal;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.binormal.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingBinormals];
            void* ptr = nxBufferOffset(buffer.buffer, offset_binormal);
            memcpy(ptr, cur_input.binormal.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingBinormals];
            buffer_ref.idx = kOutputSeparateBufferBindingBinormals;
            buffer_ref.offset = offset_binormal;

            // update offset
            offset_binormal += copy_size;
        }

        // copy color
        if (!cur_input.color.empty())
        {
            NXGPUShaderInputDesc shader_input;
            shader_input.binding_idx = kOutputSeparateBufferBindingColor;
            shader_input.data_count = 4;
            shader_input.data_idx = kGPUShaderInputIdxColor;
            shader_input.data_offset = 0;
            shader_input.data_type = kGPUDataTypeFloat;
            shader_input.enabled = 1;
            cur_mesh.shaderInput.push_back(shader_input);

            // copy buffer
            nx_u32 copy_size = cur_input.color.size() * sizeof(float);
            NXOutputBuffer& buffer = _output.globalBuffers[kOutputSeparateBufferBindingColor];
            void* ptr = nxBufferOffset(buffer.buffer, offset_color);
            memcpy(ptr, cur_input.color.data(), copy_size);

            // update buffer ref
            NXOutputBufferRef& buffer_ref = cur_mesh.bufferRefs[kOutputSeparateBufferBindingColor];
            buffer_ref.idx = kOutputSeparateBufferBindingColor;
            buffer_ref.offset = offset_color;

            // update offset
            offset_color += copy_size;
        }
    }
    return true;
}

}
