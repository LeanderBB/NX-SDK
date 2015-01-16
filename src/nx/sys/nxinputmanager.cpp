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
#include "nx/sys/nxinputmanager.h"
#include "nx/sys/nxinputctx.h"

namespace nx
{

NXInputManager::NXInputManager():
    _inputs(),
    _activeKeyMods(0)
{

}

NXInputManager::~NXInputManager()
{
    if(_inputs.size())
    {
        NXLogWarning("InputManager: %d input contexts are still in the manager",
                     (unsigned int)_inputs.size());
    }
}

bool
NXInputManager::addInputCtx(NXInputCtx* pInputCtx)
{
    bool result = true;
    bool added = false;
    for(auto it = _inputs.begin(); result && it != _inputs.end(); ++it)
    {
        // context has already been added
        if (pInputCtx == *it)
        {
            result = false;
        }
        else if (pInputCtx->priority() < (*it)->priority())
        {
            _inputs.insert(it, pInputCtx);
            added = true;
            break;
        }
    }
    if (!added)
    {
        _inputs.push_back(pInputCtx);
    }
    return result;
}

void
NXInputManager::remInputCtx(NXInputCtx* pInputCtx)
{
    auto it = std::find(_inputs.begin(), _inputs.end(), pInputCtx);
    if (it != _inputs.end())
    {
        _inputs.erase(it);
    }
}

void
NXInputManager::postKeyBoardEvent(const NXInputKeyboardEvent& evt)
{
    NXInputEvent input_evt;
    input_evt.type = kInputEventTypeKey;

    nx_u32 modifier_bit = NXInputKeyToModifierBit(evt.key);
    const nx_u32 key = (evt.key & kInputKeyMask);
    input_evt.evt.keyboard.key = key;
    if (evt.key & kInputButtonStateDown)
    {
        _activeKeyMods |= modifier_bit;
        input_evt.evt.keyboard.key |= kInputButtonStateDown;
    }
    else
    {
        _activeKeyMods &= ~modifier_bit;
        input_evt.evt.keyboard.key |= kInputButtonStateUp;
    }
    input_evt.flags = _activeKeyMods;
    this->postEvent(input_evt);
}

void
NXInputManager::postMouseButtonEvent(const NXInputMouseButtonEvent& evt)
{
    NXInputEvent input_evt;
    input_evt.type = kInputEventTypeMouseButton;
    nx_u32 modifier_bit = NXInputMouseButtonToModifierBit(evt.btn);

    if (evt.btn & kInputButtonStateDown)
    {
        _activeKeyMods |= modifier_bit;
    }
    else
    {
        _activeKeyMods &= ~modifier_bit;
    }
    input_evt.flags = _activeKeyMods;
    input_evt.evt.mouseBtn = evt;
    this->postEvent(input_evt);
}

void
NXInputManager::postMouseMoveEvent(const NXInputMouseMoveEvent& evt)
{
    NXInputEvent input_evt;
    input_evt.type = kInputEventTypeMouseMove;
    input_evt.flags = _activeKeyMods;
    input_evt.evt.mouseMove = evt;
    this->postEvent(input_evt);
}

void
NXInputManager::postScrollEvent(const NXInputScrollEvent& evt)
{
    NXInputEvent input_evt;
    input_evt.type = kInputEventTypeScroll;
    input_evt.flags = _activeKeyMods;
    input_evt.evt.scroll.xvalue = evt.xvalue;
    input_evt.evt.scroll.yvalue = evt.yvalue;
    this->postEvent(input_evt);
}

void
NXInputManager::postEvent(const NXInputEvent& evt)
{
    bool abort = false;
    for(auto it = _inputs.begin(); !abort && it != _inputs.end(); ++it)
    {
        abort = (*it)->handleInputEvent(evt);
    }
}

void
NXInputManager::clear()
{
    if(_inputs.size())
    {
        NXLogWarning("InputManager::clear %d input contexts are still in the manager",
                     (unsigned int)_inputs.size());
    }
    _inputs.clear();
}
}
