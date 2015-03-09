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
#ifndef __NXWINDOWIGLFW_H__
#define __NXWINDOWIGLFW_H__

#if defined(NX_SYSTEM_GLFW)
extern "C"
{
struct GLFWwindow;
}

namespace nx
{

class NXWindowImp
{
public:
    NXWindowImp();

    virtual ~NXWindowImp();

protected:
    friend class NXSystemImp;

    void setTitleImp(const char* title);

    void setDimensionsImp(const nx_u32 width,
                          const nx_u32 height);

    void dimensionsImp(nx_i32& width,
                       nx_i32& height);

    bool createImp(struct NXWindowDesc& desc);

    void destroyImp();

    bool bindImp();

    bool unbindImp();

    void setFullscreenImp(const bool fullscreen);

    void hideImp();

    void showImp();

    void swapBuffersImp();

    void setCaptureInputImp(const bool b);
private:

    bool isFocused() const;

    static void windowFBResize(GLFWwindow* pWin,
                               int width,
                               int height);

private:
    GLFWwindow* _pWindow;
};
}
#endif //defined(NX_SYSTEM_GLFW)
#endif
