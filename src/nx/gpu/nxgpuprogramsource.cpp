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
#include "nx/gpu/nxgpuprogramsource.h"
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"
#include "nxgpuprogramsource_generated.h"
namespace nx
{
NXGPUProgramSource::NXGPUProgramSource(void* pData,
                                       const size_t size):
    _pData(pData),
    _dataSize(size),
    _pSource(GetProgramSrc(_pData))
{

}

NXGPUProgramSource::~NXGPUProgramSource()
{
    unload();
    NX_ASSERT(_pData == nullptr);
}

NXGPUProgramSource*
NXGPUProgramSource::load(NXIOBase* pIo)
{
    size_t file_size = pIo->size();

    if (!file_size)
    {
        NXLogError("NXGPUProgramSource::load: Invalid IO, need to know file size");
        return nullptr;
    }

    void* p_data = NXMalloc(file_size);

    if (pIo->read(p_data, file_size) != file_size)
    {
        NXLogError("GPUProgramSource::load: Failed to read from input");
        NXFree(p_data);
        return nullptr;
    }
    flatbuffers::Verifier verifier(static_cast<const uint8_t*>(p_data), file_size);
     if (!VerifyProgramSrcBuffer(verifier))
     {
         NXLogError("GPUProgramSource::load: Invalid GPUProgram source");
         NXFree(p_data);
         return nullptr;
     }

    NXGPUProgramSource* p_source = new NXGPUProgramSource(p_data, file_size);

    return p_source;
}

const char*
NXGPUProgramSource::sourceForType(const GPUShaderType type) const
{
    switch(type)
    {
    case kGPUShaderTypeVertex:
        return _pSource->vertex()->size() ? _pSource->vertex()->Get(0)->c_str() : nullptr;
    case kGPUShaderTypeFragment:
        return _pSource->fragment()->size() ? _pSource->fragment()->Get(0)->c_str() : nullptr;
    case kGPUShaderTypeGeometry:
        return _pSource->geometry()->size() ? _pSource->geometry()->Get(0)->c_str() : nullptr;
    case kGPUShaderTypeTessEval:
        return _pSource->tessEval()->size() ? _pSource->tessEval()->Get(0)->c_str() : nullptr;
    case kGPUShaderTypeTessControl:
        return _pSource->tessCont()->size() ? _pSource->tessCont()->Get(0)->c_str() : nullptr;
    case kGPUShaderTypeCompute:
        return _pSource->compute()->size() ? _pSource->compute()->Get(0)->c_str() : nullptr;
    default:
        return nullptr;
    }
}

void
NXGPUProgramSource::unload()
{
    if (_pData)
    {
        NXFree((void*)_pData);
        _pData = nullptr;
    }
}

size_t
NXGPUProgramSource::memorySize() const
{
    return sizeof(NXGPUProgramSource) + _dataSize;
}

NXGPUProgramSourceManual::NXGPUProgramSourceManual():
    NXGPUProgramSourceBase()
{

}

const char*
NXGPUProgramSourceManual::sourceForType(const GPUShaderType type) const
{
    switch(type)
    {
    case kGPUShaderTypeVertex:
        return _srcVertex.size() ? _srcVertex.c_str() : nullptr;
    case kGPUShaderTypeFragment:
        return _srcFragment.size() ? _srcFragment.c_str() : nullptr;
    case kGPUShaderTypeGeometry:
        return _srcGeometry.size() ? _srcGeometry.c_str() : nullptr;
    case kGPUShaderTypeTessEval:
        return _srcTessEval.size() ? _srcTessEval.c_str() : nullptr;
    case kGPUShaderTypeTessControl:
        return _srcTessControl.size() ? _srcTessControl.c_str() : nullptr;
    case kGPUShaderTypeCompute:
        return _srcCompute.size() ? _srcCompute.c_str() : nullptr;
    default:
        return nullptr;
    }
}


void
NXGPUProgramSourceManual::setSourceVertex(const char* source)
{
    _srcVertex = source;
}

void
NXGPUProgramSourceManual::setSourceFragment(const char* source)
{
    _srcFragment = source;
}

void
NXGPUProgramSourceManual::setSourceGeometry(const char* source)
{
    _srcGeometry = source;
}

void
NXGPUProgramSourceManual::setSourceTessEvaluation(const char* source)
{
    _srcTessEval = source;
}

void
NXGPUProgramSourceManual::setSourceTessControl(const char* source)
{
    _srcTessControl = source;
}

void
NXGPUProgramSourceManual::setSourceCompute(const char* source)
{
    _srcCompute = source;
}

size_t
NXGPUProgramSourceManual::memorySize() const
{
    return sizeof(NXGPUProgramSourceManual) +
            _srcVertex.capacity() +
            _srcFragment.capacity() +
            _srcGeometry.capacity() +
            _srcTessControl.capacity() +
            _srcTessControl.capacity();
}

}
