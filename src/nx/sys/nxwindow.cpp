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
#include "nx/sys/nxwindow.h"
#include "nx/sys/gl/core44/flextGL.h"
namespace nx
{


NXWindow::NXWindow(const char* title):
    _width(640),
    _height(480),
    _title(title),
    _fullscreen(false)
{

}

NXWindow::~NXWindow()
{
    destroy();
}

void
NXWindow::setTile(const char* title)
{
    _title = title;
    NXWindowImp::setTitleImp(title);
}

void
NXWindow::setDimensions(const nx_u32 width,
                        const nx_u32 height)
{
    _width = width;
    _height = height;
    NXWindowImp::setDimensionsImp(width, height);
}

void
NXWindow::dimensions(nx_i32 &width,
                     nx_i32 &height) const
{
    width = _width;
    height = _height;
}



static void APIENTRY NXWindowGLDbgCallback(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar *message,
                                           const void *userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    const char* chr_severity;
    const char* error_type;
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        chr_severity = "high";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        chr_severity = "notification";
        break;
    default:
        chr_severity = "unknown";
    }

    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR:
        error_type = "error";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        error_type = "deperecated behaviour";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        error_type = "undefined behaviour";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        error_type = "portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        error_type = "perforance";
        NXLogWarning("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    default:
        error_type = "Unknown";
        NXLogError("GLDBG(%s): %s (Severity: %s)", error_type, message, chr_severity);
        break;
    }
}

bool
NXWindow::create(const bool depth,
                 const bool stencil,
                 const bool resizable,
                 const bool dbgctx)
{
    NXWindowDesc desc;
    desc.w = _width;
    desc.h = _height;
    desc.fullscreen = _fullscreen;
    desc.depth = depth;
    desc.stencil = stencil;
    desc.debug = dbgctx;
    desc.title = _title.c_str();
    desc.resizable = resizable;

    if (NXWindowImp::createImp(desc))
    {
        NXWindowImp::dimensionsImp(_width, _height);
        NXLogDebug("NXWindow::created window with size %dx%d", _width, _height);

        if (dbgctx)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(NXWindowGLDbgCallback, nullptr);
        }

        return true;
    }

    return false;
}

void
NXWindow::destroy()
{
    NXWindowImp::destroyImp();
}


bool
NXWindow::bind()
{
    return NXWindowImp::bindImp();
}

bool
NXWindow::unbind()
{
    return NXWindowImp::unbindImp();
}

bool
NXWindow::makeCurrent(void *pWin,
                      void *pCtx)
{
    return NXWindowImp::makeCurrentImp(pWin, pCtx);
}

void
NXWindow::setFullscreen(const bool fullscreen)
{
    _fullscreen = fullscreen;
    NXWindowImp::setFullscreenImp(fullscreen);
}

bool
NXWindow::fullscreen() const
{
    return _fullscreen;
}

void
NXWindow::hide()
{
    NXWindowImp::hideImp();
}

void
NXWindow::show()
{
    NXWindowImp::showImp();
}

void
NXWindow::swapBuffers()
{
    NXWindowImp::swapBuffersImp();
}

void
NXWindow::setCaptureInput(const bool b)
{
    NXWindowImp::setCaptureInputImp(b);
}

}
