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
#include <nx/media/nx3dmodel.h>
#include <nx/io/nxiobase.h>
#include <nx/io/nxiofile.h>
#include "nxassimpconverter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

using namespace nx;

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



int main(const int argc, const char** argv)
{

    int exit = EXIT_FAILURE;
    if (argc < 2)
    {
        NXLogError("Usage: %s model", argv[0]);
        return exit;
    }
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile( argv[1],
            aiProcess_Triangulate);
    if (scene)
    {
        logAISceneInfo(scene);
        if (argc > 2)
        {
            NXIOFile* p_file = NXIOFile::open(argv[2], kIOAccessModeWriteBit | kIOAccessModeOverwriteBit);
            if (p_file)
            {
                if(!NXAssimpConverter::convert(scene, p_file, 0))
                {
                    NXLogError("Failed to convert model '%s'", argv[1]);
                }
                else
                {
                    exit = EXIT_SUCCESS;
                }
                delete p_file;
            }
            else
            {
                NXLogError("Failed to open output '%s'", argv[2]);
            }
        }
    }
    else
    {
        NXLogError("Failed to open/load '%s'", argv[1]);
    }
    return exit;
}
