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
#ifndef __NX_3DCONVERTER_H__
#define __NX_3DCONVERTER_H__

#include "nx/gpu/nxgpushaderinput.h"

namespace nx
{

enum Option
{
    kOptionInterleavedBuffers = 2,
    kOptionOutput = 3,
};


class NXInputState
{
public:
    std::vector<float> vertices;
    std::vector<float> texcoord0;
    std::vector<float> texcoord1;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> binormal;
    std::vector<float> color;
    std::vector<nx_u32> indices;
};

typedef std::vector<NXInputState> NXInputStateVec_t;


struct NXOutputBuffer
{
    void* buffer = nullptr;
    nx_u32 size = 0;
    nx_u8 idx = NX_U8_MAX;
};
typedef std::vector<NXOutputBuffer> NXOutputBufferVec_t;

struct NXOutputBufferRef
{
    nx_u32 offset = 0;
    nx_u8 idx = NX_U8_MAX;
};
typedef std::vector<NXOutputBufferRef> NXOutputBufferRefVec_t;

struct NXOutputMesh
{
    nx_u32 drawMode = kGPUDrawModeTriangles;
    nx_u32 vertCount = 0;
    nx_u32 indexCount = 0;
    NXOutputBufferRefVec_t bufferRefs;
    NXOutputBufferRef idxBufferRef;
    NXGPUShaderInputDescVec_t shaderInput;
};
typedef std::vector<NXOutputMesh> NXOutputMeshVec_t;

enum OutputSeparateBufferBinding
{
    kOutputSeparateBufferBindingVertices = 0,
    kOutputSeparateBufferBindingTexcoord0,
    kOutputSeparateBufferBindingTexcoord1,
    kOutputSeparateBufferBindingNormals,
    kOutputSeparateBufferBindingTangents,
    kOutputSeparateBufferBindingBinormals,
    kOutputSeparateBufferBindingColor,
    kOutputSeparateBufferBindingMax
};

enum InterleavedBufferBinding
{
    kInterLeavedBufferBindingDefault = 0
};

class NXOutputState
{
public:

    NXOutputBufferVec_t globalBuffers;
    NXOutputBuffer globalIndexBuffer;
    NXOutputMeshVec_t meshes;
};



}

#endif // NX3DCONVERTER_H

