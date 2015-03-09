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
#include <nx/nxcore.h>
#include "nxassimpconverter.h"
#include <nx/media/nx3dmodel.h>
#include <nx/io/nxiobase.h>
#include <nx/gpu/nxgpubuffer.h>
#include <nx/gpu/nxgpu.h>
namespace nx
{

static void logAISceneInfo(const aiScene* pScene)
{

    NXLog(" # Meshes    : %d", pScene->mNumMeshes);
    NXLog(" # Animations: %d", pScene->mNumAnimations);
    NXLog(" # Materials : %d", pScene->mNumMaterials);
    NXLog(" # Textures  : %d", pScene->mNumTextures);
    NXLog(" # Lights    : %d", pScene->mNumLights);
    NXLog(" # Cameras   : %d", pScene->mNumCameras);

    NXLog("");

    for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
    {
        NXLog("  Mesh (%d)", i);
        const aiMesh* mesh= pScene->mMeshes[i];
        NXLog("    # Vertices       : %d", mesh->mNumVertices);
        NXLog("    # Faces          : %d", mesh->mNumFaces);
        NXLog("    # Bones          : %d", mesh->mNumBones);
        NXLog("    # Color Channels : %d", mesh->GetNumColorChannels());
        NXLog("    # UV Components  : %d", mesh->GetNumUVChannels());
        NXLog("    # Anim Meshes    : %d", mesh->mNumAnimMeshes);
    }

    for (unsigned int i = 0; i < pScene->mNumAnimations; ++i)
    {
        NXLog("  Anim (%d)", i);
        const aiAnimation* anim= pScene->mAnimations[i];
        NXLog("    Name            : %d", anim->mName.C_Str());
        NXLog("    Duration        : %f", anim->mDuration);
        NXLog("    # Channes       : %d", anim->mNumChannels);
        NXLog("    # Mesh Channels : %d", anim->mNumMeshChannels);

        for (unsigned int j = 0; j < anim->mNumChannels; ++j)
        {
            NXLog("    Channel (%d)", j );
            const aiNodeAnim *channel = anim->mChannels[j];
            NXLog("      Name            : %d", channel->mNodeName.C_Str());
            NXLog("      # Position Keys : %d", channel->mNumPositionKeys);
            NXLog("      # Rotation Keys : %d", channel->mNumRotationKeys);
            NXLog("      # Scaling Keys  : %d", channel->mNumScalingKeys);
        }

        for (unsigned int j = 0; j < anim->mNumMeshChannels; ++j)
        {
            NXLog("    Mesh Channel (%d)", j );
            const aiMeshAnim* manim = anim->mMeshChannels[j];
            NXLog("      Name   : %d", manim->mName.C_Str());
            NXLog("      # Keys : %d", manim->mNumKeys);
        }
    }
}


bool
NXAssimpConverter::convert(NXInputStateVec_t& output,
                           const char* file,
                           const nx_u32 flags)
{

    (void) flags;
    unsigned int aiflags = aiProcess_GenSmoothNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_GenUVCoords;

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(file, aiflags);
    if (!pScene)
    {
        return false;
    }

    logAISceneInfo(pScene);
    output.resize(pScene->mNumMeshes);
    for (unsigned int m = 0; m < pScene->mNumMeshes; ++m)
    {
        const aiMesh* p_mesh = pScene->mMeshes[m];
        NXInputState& cur_state = output[m];
        if (p_mesh->HasPositions())
        {
            cur_state.vertices.resize(3  * p_mesh->mNumVertices, 0.0f);
            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mVertices[i]);
                cur_state.vertices[i * 3] = p_vec->x;
                cur_state.vertices[i * 3 + 1] = p_vec->y;
                cur_state.vertices[i * 3 + 2] = p_vec->z;
            }
        }
        else
        {
            NXLogError("Model (%d) does not have vertices", m);
            return false;
        }

        if (p_mesh->HasNormals())
        {
            cur_state.normals.resize(3  * p_mesh->mNumVertices, 0.0f);
            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mNormals[i]);
                cur_state.normals[i * 3] = p_vec->x;
                cur_state.normals[i * 3 + 1] = p_vec->y;
                cur_state.normals[i * 3 + 2] = p_vec->z;
            }
        }


        if (p_mesh->HasTextureCoords(0))
        {
            cur_state.texcoord0.resize(2  * p_mesh->mNumVertices, 0.0f);

            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mNormals[i]);
                cur_state.texcoord0[i * 2] = p_vec->x;
                cur_state.texcoord0[i * 2 + 1] = p_vec->y;
            }
        }

        if (p_mesh->HasTextureCoords(1))
        {
            cur_state.texcoord1.resize(2  * p_mesh->mNumVertices, 0.0f);

            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mNormals[i]);
                cur_state.texcoord1[i * 2] = p_vec->x;
                cur_state.texcoord1[i * 2 + 1] = p_vec->y;
            }
        }

        if (p_mesh->HasTangentsAndBitangents())
        {
             cur_state.tangents.resize(3  * p_mesh->mNumVertices, 0.0f);

            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mTangents[i]);
                cur_state.tangents[i * 3] = p_vec->x;
                cur_state.tangents[i * 3 + 1] = p_vec->y;
                cur_state.tangents[i * 3 + 2] = p_vec->z;
            }

            cur_state.binormal.resize(3  * p_mesh->mNumVertices, 0.0f);

            for (unsigned int i = 0; i < p_mesh->mNumVertices; ++i)
            {
                const aiVector3D* p_vec  = &(p_mesh->mBitangents[i]);
                cur_state.binormal[i * 3] = p_vec->x;
                cur_state.binormal[i * 3 + 1] = p_vec->y;
                cur_state.binormal[i * 3 + 2] = p_vec->z;
            }
        }

        if (p_mesh->HasFaces())
        {

            cur_state.indices.reserve(p_mesh->mNumFaces);
            for(unsigned int i = 0; i < p_mesh->mNumFaces; ++i)
            {
                const aiFace* p_face = &p_mesh->mFaces[i];
                if (p_face->mNumIndices <= 3)
                {
                    for (unsigned int j = 0; j < p_face->mNumIndices; ++j)
                    {
                        cur_state.indices.push_back(p_face->mIndices[j]);
                    }
                }
                else
                {
                    NXLogError("Mesh (%u) Face %u has more than 3 indices: nIndices=%u",
                               m, i, p_face->mNumIndices);
                    return false;
                }
            }
        }

    }
return true;
}


}
