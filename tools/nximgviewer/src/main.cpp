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
#include "nx/sys/nxapp.h"
#include "nx/event/nxeventlistener.h"
#include "nx/io/nxfilemanager.h"

#include "nx/gpu/nxgputexture.h"
#include "nx/media/nximage.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiofile.h"
#include "nx/ogl/nxogltexture.h"
#include "nx/ogl/nxoglprogram.h"
#include "nx/gpu/nxgpuprogramsource.h"
#include "nx/sys/nxwindow.h"
#include "nx/ogl/nxoglinternal.h"

namespace nx
{

static const char* s_vertex_source =
        "#version 430 core\n \
        void main(void)\n\
{\
        const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0), \
        vec4( 1.0, -1.0, 0.5, 1.0), \
        vec4(-1.0,  1.0, 0.5, 1.0), \
        vec4( 1.0,  1.0, 0.5, 1.0)); \
\
gl_Position = vertices[gl_VertexID]; \
}";

static const char* s_fragment_source =
"#version 430 core\
//uniform sampler2DArray s;\n\
uniform sampler2D s;\n\
out vec4 color;\n\
uniform int window_width;\n\
uniform int window_height;\n\
\n\
void main(void) \
{ \
    //    color = texture(s, vec3(gl_FragCoord.xy, 0) / textureSize(s, 0)); \n\
    color = texture(s, (gl_FragCoord.xy / textureSize(s, 0)) * \
                    (textureSize(s, 0)/vec2(window_width, window_height)));  \n\
}";


class NXImageViewer : public NXApp
{

public:

    NXImageViewer();

    ~NXImageViewer();

protected:

    bool appInit(const int argc,
                 const char**argv);

    void appRun();

    void appTerm();


private:
    NXFileManager _fileManager;
    NXOGLTexture* _pTex;
    NXOGLProgram* _pProg;
    GLuint _vao;
    GLint _locWinWidth;
    GLint _locWinHeight;
};


NXImageViewer::NXImageViewer():
    NXApp("NXImageViewer"),
    _pTex(nullptr),
    _pProg(nullptr),
    _locWinWidth(-1),
    _locWinHeight(-1)
{

}

NXImageViewer::~NXImageViewer()
{

}

bool
NXImageViewer::appInit(const int argc,
                       const char**argv)
{
    if (argc < 2)
    {
        NXLogError("Usage: %s  nximg0 ... nximgn\n", argv[0]);
        return false;
    }

    if (!_fileManager.init())
    {
        NXLogError("Failed to init file manager");
        return false;
    }

    // open file
    NXIOFile* p_file = NXIOFile::open(argv[1], kIOAccessModeReadBit);

    if (!p_file)
    {
        NXLogError("Failed to open '%s'\n", argv[1]);
        return false;
    }

    // load image
    NXImage* p_imgsrc = NXImage::load(p_file);
    // close io
    NX_SAFE_DELETE(p_file);

    if (!p_imgsrc)
    {
        return false;
    }

    NXImage::ImageInfo(p_imgsrc->header());

    // create ogl tex
    _pTex = NXOGLTexture::create(*p_imgsrc);
    // delete img source
    p_imgsrc->unload();
    NX_SAFE_DELETE(p_imgsrc);

    if (!_pTex)
    {
        return false;
    }

    NXGPUProgramSourceManual prg_src;

    prg_src.setSourceVertex(s_vertex_source);
    prg_src.setSourceFragment(s_fragment_source);

    _pProg = NXOGLProgram::create(&prg_src);

    if (!_pProg)
    {
        return false;
    }

    // locate uniforms

    _locWinWidth = glGetUniformLocation(_pProg->oglHdl(), "window_width");
    _locWinHeight = glGetUniformLocation(_pProg->oglHdl(), "window_height");

    if (_locWinHeight == -1 || _locWinWidth == -1)
    {
        NXLogError("Failed to locate uniforms");
        return false;
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindTexture(_pTex->oglType(), _pTex->oglHdl());

    system()->window()->setDimensions(_pTex->desc().width, _pTex->desc().height);
    return true;
}

void
NXImageViewer::appRun()
{
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, green);


    glUseProgram(_pProg->oglHdl());
    glViewport(0, 0, system()->window()->width(), system()->window()->height());
    glUniform1i(_locWinWidth, system()->window()->width());
    glUniform1i(_locWinHeight, system()->window()->height());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
NXImageViewer::appTerm()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);

    if (_pTex)
    {
        NX_SAFE_DELETE(_pTex);
    }

    if (_pProg)
    {
        NX_SAFE_DELETE(_pProg);
    }

    _fileManager.clear();
}

}

NX_APP(nx::NXImageViewer)
