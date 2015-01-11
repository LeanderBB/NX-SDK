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
#include "nx/ogl/nxoglinternal.h"
#include "nx/ogl/nxogl.h"
#include "nx/hdl/nxhdlmanager.h"
namespace nx
{

void
NXOGL::logGLInfo(const bool logExtensions)
{
    NXLog("OpenGL Vendor                        : %s", glGetString(GL_VENDOR));
    NXLog("OpenGL Renderer                      : %s", glGetString(GL_RENDERER));
    NXLog("OpenGL Version                       : %s", glGetString(GL_VERSION));
    NXLog("OpenGL GLSL Version                  : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    NXLog("OpenGL Direct State Access (ARB)     : %s", (FLEXT_ARB_direct_state_access == GL_TRUE) ? "yes" : "no");
    NXLog("OpenGL Direct State Access (EXT)     : %s", (FLEXT_EXT_direct_state_access == GL_TRUE) ? "yes" : "no");
    NXLog("OpenGL Bindless Textures             : %s", (FLEXT_ARB_bindless_texture == GL_TRUE) ? "yes" : "no");
    NXLog("OpenGL ASTC Textures                 : %s", ((FLEXT_KHR_texture_compression_astc_hdr == GL_TRUE) || (FLEXT_KHR_texture_compression_astc_ldr == GL_TRUE)) ? "yes" : "no");

    // get extensions
    GLint n_ext;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n_ext);
    NXLog("OpenGL Extensions                    : %d", n_ext);
    if(logExtensions)
    {
        for(int i = 0; i < n_ext; ++i)
        {
            NXLog("    > %s", glGetStringi(GL_EXTENSIONS, i));
        }
    }
}

void
NXOGL::loadLimits(NXOGLLimits& limits)
{
    memset(&limits, 0, sizeof(limits));
    // texture
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &limits.texture.maxActiveTextureUnits);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &limits.texture.max3DTextureSize);
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &limits.texture.maxCubeMapTextureSize);
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &limits.texture.maxArrayLayers);
    glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &limits.texture.maxLODBias);
    // program
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &limits.program.maxVertexAttribs);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &limits.program.maxVertexUniforms);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &limits.program.maxVertexUniformBlocks);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &limits.program.maxVertexUniformVectors);
    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &limits.program.maxVertexOutputComponents);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &limits.program.maxFragmentUniforms);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &limits.program.maxFragmentUniformBlocks);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &limits.program.maxFragmentUniformVectors);
    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &limits.program.maxFragmentInputComponents);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &limits.program.maxUniformBlockSize);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &limits.program.maxUniformBlockBindings);
    // raster
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &limits.raster.maxColorAttachments);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS,limits.raster.maxViewportDims);
    glGetIntegerv(GL_MAX_SAMPLES, &limits.raster.maxSamples);
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &limits.raster.maxDrawBuffers);
}

void
NXOGL::logLimits(const NXOGLLimits& limits)
{
    // Textures

    NXLog("OpenGL Max Active Texture Units      : %d",
          limits.texture.maxActiveTextureUnits);
    NXLog("OpenGL Max 3D Texture Size           : %d",
          limits.texture.max3DTextureSize);
    NXLog("OpenGL Max Cube Map Texture Size     : %d",
          limits.texture.maxCubeMapTextureSize);
    NXLog("OpenGL Max Array Texture Layer       : %d",
          limits.texture.maxArrayLayers);
    NXLog("OpenGL Max Texture LOD Bias          : %f",
          limits.texture.maxLODBias);
    // Programs
    NXLog("OpenGL Max Vertex Attributes         : %d",
          limits.program.maxVertexAttribs);
    NXLog("OpenGL Max Vertex Uniforms           : %d",
          limits.program.maxVertexUniforms);
    NXLog("OpenGL Max Vertex Uniform Blocks     : %d",
          limits.program.maxVertexUniformBlocks);
    NXLog("OpenGL Max Vertex Uniform Vectors    : %d",
          limits.program.maxVertexUniformVectors);
    NXLog("OpenGL Max Vertex Output Components  : %d",
          limits.program.maxVertexOutputComponents);
    NXLog("OpenGL Max Fragment Uniforms         : %d",
          limits.program.maxFragmentUniforms);
    NXLog("OpenGL Max Fragment Uniform Blocks   : %d",
          limits.program.maxFragmentUniformBlocks);
    NXLog("OpenGL Max Fragment Uniform Vectors  : %d",
          limits.program.maxFragmentUniformVectors);
    NXLog("OpenGL Max Fragment Input Components : %d",
          limits.program.maxFragmentInputComponents);
    NXLog("OpenGL Max Uniform Block Size        : %d",
          limits.program.maxUniformBlockSize);
    NXLog("OpenGL Max Uniform Block Bindings    : %d",
          limits.program.maxUniformBlockBindings);
    // Raster
    NXLog("OpenGL Max Color Attachments         : %d",
          limits.raster.maxColorAttachments);
    NXLog("OpenGL Max ViewPort Dim              : %dx%d",
          limits.raster.maxViewportDims[0], limits.raster.maxViewportDims[1]);
    NXLog("OpenGL Max Draw Buffers              : %d",
          limits.raster.maxDrawBuffers);
    NXLog("OpenGL Max Multisample Samples       : %d",
          limits.raster.maxSamples);
}

NXHdl
NXOGL::createProgram()
{
    return NXHdl();
}

void
NXOGL::deleteProgram(NXHdl hdl)
{
    hdl.reset();
}

}
