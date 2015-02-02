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
#ifndef __NX_GPUPROGRAMSOURCE_H__
#define __NX_GPUPROGRAMSOURCE_H__

#include "nx/gpu/nxgpu.h"

namespace nx
{
class NXFileManager;
struct ProgramSrc;
class NXIOBase;


class NXGPUProgramSourceBase
{
public:

    virtual ~NXGPUProgramSourceBase(){}

    virtual const char* sourceForType(const GPUShaderType type) const = 0;

    virtual size_t memorySize() const = 0;

protected:
    NXGPUProgramSourceBase(){}
};


class NXGPUProgramSource : public NXGPUProgramSourceBase
{
public:


    static NXGPUProgramSource* load(NXIOBase* pIO);

    virtual ~NXGPUProgramSource();


    const char* sourceForType(const GPUShaderType type) const NX_CPP_OVERRIDE;

    void unload();

    size_t memorySize() const NX_CPP_OVERRIDE;

protected:
        NXGPUProgramSource(void* pData,
                           const size_t size);

protected:
    const void* _pData;
    const size_t _dataSize;
    const ProgramSrc* _pSource;
};


class NXGPUProgramSourceManual : public NXGPUProgramSourceBase
{
public:

    NXGPUProgramSourceManual();

    const char* sourceForType(const GPUShaderType type) const NX_CPP_OVERRIDE;

    void setSourceVertex(const char* source);

    void setSourceFragment(const char* source);

    void setSourceGeometry(const char* source);

    void setSourceTessEvaluation(const char* source);

    void setSourceTessControl(const char* source);

    void setSourceCompute(const char* source);

    size_t memorySize() const NX_CPP_OVERRIDE;

protected:
    NXString _srcVertex;
    NXString _srcFragment;
    NXString _srcGeometry;
    NXString _srcTessEval;
    NXString _srcTessControl;
    NXString _srcCompute;
};

}

#endif
