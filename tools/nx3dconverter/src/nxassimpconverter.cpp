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
#include <nx/nxcore.h>
#include "nxassimpconverter.h"
#include <nx/media/nx3dmodel.h>
#include <nx/io/nxiobase.h>
#include <nx/gpu/nxgpubuffer.h>
#include <nx/gpu/nxgpu.h>
namespace nx
{


bool
NXAssimpConverter::convert(const aiScene* pScene,
                           NXIOBase* pIO,
                           const nx_u32 flags)
{

    (void) flags;
    (void) *pIO;

    float* p_vertices = nullptr;
    float* p_normals = nullptr;
    float* p_uv0=nullptr;
    float* p_tangents = nullptr;
    float* p_binormal = nullptr;
    float* p_color = nullptr;
    std::vector<unsigned int> idx;

    size_t bytes_written = 0;

    NX3DModelHeader hdr;
    NX3DModelEntry model_entry;
    memset(&hdr, 0, sizeof(hdr));

    //TODO: Update
    hdr.drawmode = kGPUDrawModeTriangles;

    bool result = false;
    const aiMesh* p_mesh = pScene->mMeshes[0];

    if (p_mesh->HasPositions())
    {
        p_vertices = (float*) NXMalloc(sizeof(float) * 3  * p_mesh->mNumVertices);

        for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
        {
            const aiVector3D* p_vec  = &(p_mesh->mVertices[i]);
            p_vertices[i * 3] = p_vec->x;
            p_vertices[i * 3 + 1] = p_vec->y;
            p_vertices[i * 3 + 2] = p_vec->z;
        }

        hdr.nEntries++;
        hdr.components |= kModelComponentVerticesBit;
        hdr.nVertices = p_mesh->mNumVertices;
        hdr.drawSize = p_mesh->mNumVertices;
    }
    else
    {
        NXLogError("Model does not have vertices");
        return false;
    }

    if (p_mesh->HasNormals())
    {
        p_normals = (float*) NXMalloc(sizeof(float) * 3  * p_mesh->mNumVertices);

        for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
        {
            const aiVector3D* p_vec  = &(p_mesh->mNormals[i]);
            p_normals[i * 3] = p_vec->x;
            p_normals[i * 3 + 1] = p_vec->y;
            p_normals[i * 3 + 2] = p_vec->z;
        }
        hdr.nEntries++;
        hdr.components |= kModelComponentNormalsBit;
    }


    if (p_mesh->HasTextureCoords(0))
    {
        p_uv0 = (float*) NXMalloc(sizeof(float) * 2  * p_mesh->mNumVertices);

        for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
        {
            const aiVector3D* p_vec  = &(p_mesh->mNormals[i]);
            p_uv0[i * 2] = p_vec->x;
            p_uv0[i * 2 + 1] = p_vec->y;
        }

        hdr.nEntries++;
        hdr.components |= kModelComponentUVBit;
    }

    if (p_mesh->HasTangentsAndBitangents())
    {
        p_tangents = (float*) NXMalloc(sizeof(float) * 3  * p_mesh->mNumVertices);

        for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
        {
            const aiVector3D* p_vec  = &(p_mesh->mTangents[i]);
            p_tangents[i * 3] = p_vec->x;
            p_tangents[i * 3 + 1] = p_vec->y;
            p_tangents[i * 3 + 2] = p_vec->z;
        }

        p_binormal = (float*) NXMalloc(sizeof(float) * 3  * p_mesh->mNumVertices);

        for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
        {
            const aiVector3D* p_vec  = &(p_mesh->mBitangents[i]);
            p_binormal[i * 3] = p_vec->x;
            p_binormal[i * 3 + 1] = p_vec->y;
            p_binormal[i * 3 + 2] = p_vec->z;
        }
        hdr.nEntries += 2;
        hdr.components |= kModelComponentBinormalBit | kModelComponentTangentBit;
    }

    if (p_mesh->HasFaces())
    {
        for(unsigned int i = 0; i < p_mesh->mNumFaces; ++i)
        {
            const aiFace* p_face = &p_mesh->mFaces[i];
            if (p_face->mNumIndices == 3)
            {
                idx.push_back(p_face->mIndices[0]);
                idx.push_back(p_face->mIndices[1]);
                idx.push_back(p_face->mIndices[2]);
            }
            else
            {
                NXLogError("Face has more than 3 indices!!");
                idx.clear();
                goto error_exit;
            }
        }

        hdr.nEntries++;
        hdr.components |= kModelComponentIdxBit;
        hdr.drawSize = p_mesh->mNumFaces;
    }


    memcpy(hdr.id, NX3DModel::sIdentifier, sizeof(NX3DModel::sIdentifier));
    hdr.version = 1;

    bytes_written = pIO->write(&hdr, sizeof(hdr));
    if(bytes_written != sizeof(hdr))
    {
        NXLogError("Failed to write NX3DModel Header");
        goto error_exit;
    }


    // write vertices
    if (p_vertices)
    {
        model_entry.idx = 0;
        model_entry.components = kModelComponentVerticesBit;
        model_entry.format = kGPUBufferLayout3FLT;
        strncpy(model_entry.name,"vertices", 31);
        model_entry.size = sizeof(float) * 3 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_vertices, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);
            goto error_exit;
        }

    }

    // write normals
    if (p_normals)
    {
        model_entry.idx = 1;
        model_entry.components = kModelComponentNormalsBit;
        model_entry.format = kGPUBufferLayout3FLT;
        strncpy(model_entry.name,"normals", 31);
        model_entry.size = sizeof(float) * 3 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_normals, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }

    }

    // write UV
    if (p_uv0)
    {
        model_entry.idx = 2;
        model_entry.components = kModelComponentNormalsBit;
        model_entry.format = kGPUBufferLayout2FLT;
        strncpy(model_entry.name,"uv 0", 31);
        model_entry.size = sizeof(float) * 2 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_uv0, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }

    }

    // write tangents
    if (p_tangents)
    {
        model_entry.idx = 3;
        model_entry.components = kModelComponentTangentBit;
        model_entry.format = kGPUBufferLayout3FLT;
        strncpy(model_entry.name,"tangents", 31);
        model_entry.size = sizeof(float) * 3 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_tangents, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }
    }


    // write binormals
    if (p_binormal)
    {
        model_entry.idx = 4;
        model_entry.components = kModelComponentTangentBit;
        model_entry.format = kGPUBufferLayout3FLT;
        strncpy(model_entry.name,"binormals", 31);
        model_entry.size = sizeof(float) * 3 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_binormal, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }
    }

    // write color
    if (p_color)
    {
        model_entry.idx = 5;
        model_entry.components = kModelComponentColorBit;
        model_entry.format = kGPUBufferLayout3FLT;
        strncpy(model_entry.name,"color", 31);
        model_entry.size = sizeof(float) * 4 * hdr.nVertices;

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(p_color, model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }
    }

    // write idx
    if (idx.size())
    {
        model_entry.idx = NX_U32_MAX;
        model_entry.components = kModelComponentIdxBit;
        model_entry.format = kGPUBufferLayout1UI;
        strncpy(model_entry.name,"indicies", 31);
        model_entry.size = sizeof(unsigned int) * idx.size();

        bytes_written = pIO->write(&model_entry, sizeof(model_entry));
        if (bytes_written != sizeof(model_entry))
        {
            NXLogError("Failed to write NX3DModel Entry for %s", model_entry.name);
            goto error_exit;
        }

        bytes_written = pIO->write(&idx[0], model_entry.size);
        if (bytes_written != model_entry.size)
        {
            NXLogError("Failed to write NX3DModel content for %s", model_entry.name);;
            goto error_exit;
        }
    }


    result = true;

error_exit:
    // free data

    if (p_vertices)
    {
        NXFree(p_vertices);
    }

    if (p_normals)
    {
        NXFree(p_normals);
    }

    if (p_uv0)
    {
        NXFree(p_uv0);
    }

    if (p_tangents)
    {
        NXFree(p_tangents);
    }

    if (p_binormal)
    {
        NXFree(p_binormal);
    }

    if (p_color)
    {
        NXFree(p_color);
    }
    return result;
}


}
