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
#include "nx/gpu/nxgpuinterface.h"
#include "nx/resource/nxresourcemanager.h"
#include "nx/resource/nxgputextureresource.h"
#include "nx/resource/nxgpuprogramresource.h"

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
layout(location = 1) uniform int window_width;\n\
layout(location = 2) uniform int window_height;\n\
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

    bool onAppInit(const int argc,
                 const char**argv) NX_CPP_OVERRIDE;

    void appRun() NX_CPP_OVERRIDE;

    void onAppWillTerm() NX_CPP_OVERRIDE;

    void onWindowCreated();

    void onWindowWillBeDestroyed();


private:
    NXFileManager _fileManager;
    NXResourceManager _resManager;
    NXString _filePath;
    NXGPUInterface* _pGPUInterface;
    NXHdl _hdlProg;NXGPUTextureResourcePtr_t ptr = nxMakeTLShared<NXGPUTextureResource>("texture", _filePath.c_str());
    NXHdl _hdlTex;
    GLuint _vao;
    GLint _locWinWidth;
    GLint _locWinHeight;

};


NXImageViewer::NXImageViewer():
    NXApp("NXImageViewer"),
    _fileManager(),
    _resManager(_fileManager),
    _filePath(),
    _hdlProg(),
    _hdlTex(),
    _vao(),
    _locWinWidth(-1),
    _locWinHeight(-1)
{

}

NXImageViewer::~NXImageViewer()
{

}

bool
NXImageViewer::onAppInit(const int argc,
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

    if (!_fileManager.mount("/","", true))
    {
        NXLogError("Failed to mount root file system");
        return false;
    }

    // save file
    _filePath = argv[1];

    return true;
}

void
NXImageViewer::appRun()
{
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, green);


    _pGPUInterface->bindProgram(_hdlProg);

    glViewport(0, 0, system()->window()->width(), system()->window()->height());
    glUniform1i(_locWinWidth, system()->window()->width());
    glUniform1i(_locWinHeight, system()->window()->height());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
NXImageViewer::onAppWillTerm()
{
    _resManager.clear();
    _fileManager.clear();
}

void
NXImageViewer::onWindowCreated()
{

    _pGPUInterface = NXGPUInterface::create();

    if (!_pGPUInterface->init())
    {
        NXLogError("Failed to init GPU Interface");
        quit();
        return;
    }

    _resManager.setGPUInterface(_pGPUInterface);


    _hdlTex = _resManager.create("texture", _filePath.c_str());
    _resManager.load(_hdlTex);
    if (!_resManager.isLoaded(_hdlTex))
    {
        quit();
        return;
    }

    NXGPUTextureResourcePtr_t ptr = _resManager.get(_hdlTex);

    NXGPUProgramResourcePtr_t prog_ptr = NXGPUProgramResource::create("program", nullptr);

    auto prg_src = new NXGPUProgramSourceManual();
    prg_src->setSourceVertex(s_vertex_source);
    prg_src->setSourceFragment(s_fragment_source);
    prog_ptr->setSource(prg_src);

    NXHdl prog_hdl = _resManager.registerResource(prog_ptr);

    _resManager.load(prog_hdl);

    if(!_resManager.isLoaded(prog_hdl))
    {
        quit();
        return;
    }

    _hdlProg = prog_ptr->gpuHdl();

    // locate uniforms

    _locWinWidth = 1;
    _locWinHeight = 2;


    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    _pGPUInterface->bindTexture(_hdlTex, 0);

    const NXGPUTexture* p_tex = ptr->texture();
    system()->window()->setDimensions(p_tex->width(), p_tex->height());
}

void
NXImageViewer::onWindowWillBeDestroyed()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);

    if (_pGPUInterface)
    {
        _pGPUInterface->shutdown();
        NX_SAFE_DELETE(_pGPUInterface);
    }
}

}

NX_APP(nx::NXImageViewer)
