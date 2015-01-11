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
#ifndef __NX_GPUBUFFER_H__
#define __NX_GPUBUFFER_H__

namespace nx
{

enum GPUBufferLayout
{
    kGPUBufferLayout3V3N2T,
    kGPUBufferLayout3V3N,
    kGPUBufferLayout3B3T,
    kGPUBufferLayout3V3N2T3C,
    kGPUBufferLayout3FLT,
    kGPUBufferLayout2FLT,
    kGPUBufferLayout1UI,
    kGPUBufferLayout1US
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


struct NXGPUBufferDesc
{
    GPUBufferType type = kGPUBufferTypeData;
    nx_u32 mode = 0;
    nx_u32 size = 0;
};

}
#endif
