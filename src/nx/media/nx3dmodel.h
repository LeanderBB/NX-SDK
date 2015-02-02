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
#ifndef __NX_3DMODEL_H__
#define __NX_3DMODEL_H__

namespace nx
{

/*
 * NX 3D Model Format
 *
 * NX3DModelHeader
 *
 * for each entry
 *      NXModelEntryV1
 *      [Buffer Data]
 * end
 */

struct NX3DModelHeader
{
    nx_u8  id[12];
    nx_u32 version;
    nx_u32 nVertices;
    nx_u32 components;
    nx_u32 nEntries;
    nx_u32 drawmode;
    nx_u32 drawSize; // contains number of indices or primitives
};

enum ModelCompomentBits
{
    kModelComponentVerticesBit = NX_BIT(0),
    kModelComponentNormalsBit = NX_BIT(1),
    kModelComponentUVBit = NX_BIT(2),
    kModelComponentBinormalBit = NX_BIT(3),
    kModelComponentTangentBit = NX_BIT(4),
    kModelComponentColorBit = NX_BIT(5),
    kModelComponentBonesBit = NX_BIT(6),
    kModelComponentBoneIdxsBit = NX_BIT(7),
    kModelComponentBoneNamesBit = NX_BIT(8),
    kModelComponentIdxBit = NX_BIT(9)
};

struct NX3DModelEntry
{
    char name[32];
    nx_u32 idx = NX_U32_MAX;
    nx_u32 format = 0;
    nx_u32 size = 0;
    nx_u32 components = 0;
};

class NXIOBase;
class NX3DModel
{
public:
    struct ModelEntry
    {
        NX3DModelEntry entry;
        const void* ptr = nullptr;
    };

public:
    static const nx_u8 sIdentifier[12];

    static NX3DModel* load(NXIOBase* pIO);

    ~NX3DModel();

    const NX3DModelHeader& header() const;

    const ModelEntry* entry(const unsigned idx) const;

    const ModelEntry* indices() const;

    void logInfo() const;

    void unload();

    size_t memorySize() const;

protected:

    NX3DModel(const NX3DModelHeader& hdr,
              const void* pBuffer,
              const size_t size);

    void init();

protected:
    typedef std::vector<ModelEntry> EntryVec_t;
    const NX3DModelHeader _hdr;
    EntryVec_t _entries;
    ModelEntry _indices;
    const char* _pContent;
    size_t _contentSize;
};


}

#endif
