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
#ifndef __NX_INPUTMANAGER_H__
#define __NX_INPUTMANAGER_H__

#include "nxinput.h"

namespace nx
{
class NXInputCtx;
class NXInputManager
{
public:
    NXInputManager();

    ~NXInputManager();

    bool addInputCtx(NXInputCtx* pInputCtx);

    void remInputCtx(NXInputCtx* pInputCtx);

    void postKeyBoardEvent(const NXInputKeyboardEvent& evt);

    void postMouseButtonEvent(const NXInputMouseButtonEvent& evt);

    void postMouseMoveEvent(const NXInputMouseMoveEvent& evt);

    void postScrollEvent(const NXInputScrollEvent& evt);

    void clear();

protected:

    void postEvent(const NXInputEvent& evt);

protected:
    typedef std::vector<NXInputCtx*> InputCtxs;

    InputCtxs _inputs;
    nx_u32 _activeKeyMods;
};


}

#endif
