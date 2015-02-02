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
#ifndef __NX_GPU_H__
#define __NX_GPU_H__


#define NX_GPU_DEFAULT_MAX_SHADER_INPUT 16

enum GPUShaderType
{
    kGPUShaderTypeVertex,
    kGPUShaderTypeFragment,
    kGPUShaderTypeGeometry,
    kGPUShaderTypeTessEval,
    kGPUShaderTypeTessControl,
    kGPUShaderTypeCompute,
    kGPUShaderTypeTotal
};

enum GPUDrawMode
{
    kGPUDrawModeTriangles,
    kGPUDrawModeTriangeList,
    kGPUDrawModeTriangeFan,
    kGPUDrawModeLines,
    kGPUDrawModePoints
};

enum GPUResourceType
{
    kGPUResourceTypeUnknown = 0,
    kGPUResourceTypeProgram,
    kGPUResourceTypeBuffer,
    kGPUResourceTypeTexture,
    kGPUResourceTypeShaderInput,
    kGPUResourceTypeRT
};

enum GPUBufferLayout
{
    kGPUBufferLayout3V3N2T,
    kGPUBufferLayout3V3N,
    kGPUBufferLayout3B3T,
    kGPUBufferLayout3V3N2T3C,
    kGPUBufferLayout3FLT,
    kGPUBufferLayout2FLT,
    kGPUBufferLayout1FLT,
    kGPUBufferLayout1UI,
    kGPUBufferLayout1US
};

enum GPUDataType
{
    kGPUDataTypeUnknown = 0,
    kGPUDataTypeFloat,
    kGPUDataTypeUInt,
    kGPUDataTypeInt,
    kGPUDataTypeUShort,
    kGPUDataTypeShort,
    kGPUDataTypeUByte,
    kGPUDataTypeByte
};

enum GPUShaderInputIdx
{
    kGPUShaderInputIdxVertices = 0,
    kGPUShaderInputIdxNormals = 1,
    kGPUShaderInputIdxTexCoord0 = 2,
    kGPUShaderInputIdxTexCoord1 = 3,
    kGPUShaderInputIdxBinormal = 4,
    kGPUShaderInputIdxTangent = 5,
    kGPUShaderInputIdxColor = 6,
    kGPUShaderInputIdxOther = 7,
    kGPUShaderInputIdxMax = 16
};


enum GPUBufferType
{
    kGPUBufferTypeData,
    kGPUBufferTypeIndex,
    kGPUBufferTypeUniform,
    kGPUBufferTypeFeedback,
    kGPUBufferTypeCompute
};

enum GPUBufferMode
{
    kGPUBufferModePresistentBit,
    kGPUBufferModeCoherentBit,
    kGPUBufferModeDynamicStorageBit,
    kGPUBufferModeWriteBit,
    kGPUBufferModeReadBit
};


#endif
