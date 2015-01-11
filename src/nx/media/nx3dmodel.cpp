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
namespace nx
{

const nx_u8 NX3DModel::sIdentifier[12] =
    {0xAB, 'N', 'X', '3', 'D', 'M', 'D', 'L', 0xBB, 0x0A, 0x1A, 0x0A };


NX3DModel*
NX3DModel::load(NXIOBase* pIO)
{
    NX3DModelHeader hdr;

    if (pIO->read(&hdr, sizeof(hdr)) != sizeof(hdr))
    {
        NXLogError("3DModel: Failed to read header");
        return nullptr;
    }

    if (memcmp(&hdr.id, NX3DModel::sIdentifier, sizeof(NX3DModel::sIdentifier)) != 0)
    {
        NXLogError("3DModel: Input is not a 3DModel file");
        return nullptr;
    }

    size_t buffer_size = pIO->size() - pIO->tell();
    void* p_buffer = NXMalloc(buffer_size);

    if (pIO->read(p_buffer, buffer_size) != buffer_size)
    {
        NXLogError("3DModel: Failed to read contents into memory");
        NXFree(p_buffer);
        return nullptr;
    }

    NX3DModel* p_model = new NX3DModel(hdr, p_buffer);

    p_model->init();

    return p_model;
}

NX3DModel::~NX3DModel()
{
    NX_ASSERT(_pContent == nullptr);
}


const NX3DModelHeader&
NX3DModel::header() const
{
    return _hdr;
}

const NX3DModel::ModelEntry*
NX3DModel::entry(const unsigned idx) const
{
    return (idx < _entries.size()) ? &_entries[idx] : nullptr;
}


const NX3DModel::ModelEntry*
NX3DModel::indices() const
{
    return (_indices.ptr) ? &_indices : nullptr;
}

void
NX3DModel::logInfo() const
{
    NXLog("NX3DModel:");
    NXLog("  Version    : %d", _hdr.version);
    NXLog("  Compoments : %x", _hdr.components);
    NXLog("   > Vertices : %d", (_hdr.components & kModelComponentVerticesBit) ? 1 : 0);
    NXLog("   > Normals  : %d", (_hdr.components & kModelComponentNormalsBit) ? 1 : 0);
    NXLog("   > UV (0)   : %d", (_hdr.components & kModelComponentUVBit) ? 1 : 0);
    NXLog("   > Tangents : %d", (_hdr.components & kModelComponentTangentBit) ? 1 : 0);
    NXLog("   > Binormal : %d", (_hdr.components & kModelComponentBinormalBit) ? 1 : 0);
    NXLog("   > Color    : %d", (_hdr.components & kModelComponentColorBit) ? 1 : 0);
    NXLog("   > Indices  : %d", (_hdr.components & kModelComponentIdxBit) ? 1 : 0);
    NXLog("  nEntries   : %d", _hdr.nEntries);
    NXLog("  nVertices  : %d", _hdr.nVertices);

    for (nx_u32 i = 0; i < _entries.size(); ++i)
    {
        const NX3DModelEntry& entry = _entries[i].entry;
        NXLog("  Entry (%d)", i);
        NXLog("    IDX             : %u", entry.idx);
        NXLog("    Name            : %s", entry.name);
        NXLog("    GPUBufferFormat : %x", entry.format);
        NXLog("    Components      : %x", entry.components);
        NXLog("    Size            : %d", entry.size);
    }

    if (_indices.ptr)
    {
        const NX3DModelEntry& entry = _indices.entry;
        NXLog("  Entry (-)");
        NXLog("    Name            : %s", entry.name);
        NXLog("    GPUBufferFormat : %x", entry.format);
        NXLog("    Components      : %x", entry.components);
        NXLog("    Size            : %d", entry.size);
    }

}

void
NX3DModel::unload()
{
    if (_pContent)
    {
        NXFree((void*)_pContent);
        _pContent = nullptr;
    }
}

NX3DModel::NX3DModel(const NX3DModelHeader& hdr,
                     const void* pBuffer):
    NXMediaItem(),
    _hdr(hdr),
    _entries(),
    _pContent(static_cast<const char*>(pBuffer))
{

}

void
NX3DModel::init()
{
    size_t offset = 0;
    _entries.reserve(_hdr.nEntries);

    for (nx_u32 i = 0; i < _hdr.nEntries; ++i)
    {
        const NX3DModelEntry* p_entry = reinterpret_cast<const NX3DModelEntry*>(_pContent + offset);

        if (p_entry->idx == NX_U32_MAX)
        {
            _indices.entry = *p_entry;
            _indices.ptr = _pContent + offset + sizeof(NX3DModelEntry);
        }
        else
        {
            ModelEntry new_entry;
            new_entry.entry = *p_entry;
            new_entry.ptr = _pContent + offset + sizeof(NX3DModelEntry);
            _entries.push_back(new_entry);
        }

        offset += sizeof(NX3DModelEntry) + p_entry->size;
    }
}

}

