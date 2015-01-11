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
#ifndef __NX_OGL_H__
#define __NX_OGL_H__

namespace nx
{
class NXHdl;
struct NXOGLLimits
{
    struct
    {
        int maxActiveTextureUnits;
        int max3DTextureSize;
        int maxCubeMapTextureSize;
        int maxArrayLayers;
        int maxTextureSize;
        float maxLODBias;
    } texture;
    struct
    {
        int maxVertexAttribs;
        int maxVertexUniforms;
        int maxVertexUniformBlocks;
        int maxVertexUniformVectors;
        int maxVertexOutputComponents;
        int maxFragmentInputComponents;
        int maxFragmentUniforms;
        int maxFragmentUniformBlocks;
        int maxFragmentUniformVectors;
        int maxUniformBlockSize;
        int maxUniformBlockBindings;
    } program;
    struct
    {
        int maxColorAttachments;
        int maxViewportDims[2]; // [width, height]
        int maxSamples; // Max multisamples
        int maxDrawBuffers;
    } raster;
};

class NXOGL
{
public:

    static void logGLInfo(const bool logExtensions = false);

    static void loadLimits(NXOGLLimits& limits);

    static void logLimits(const NXOGLLimits& limits);

    static NXHdl createProgram();

    static void deleteProgram(NXHdl hdl);
};

}

#endif
