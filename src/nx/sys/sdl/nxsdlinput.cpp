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
#if defined(NX_SYSTEM_SDL2)
#include "nx/sys/sdl/nxsdlinput.h"
#include "nx/sys/nxinputmanager.h"
#include "nx/sys/nxinput.h"
#include <SDL2/SDL.h>


#define NX_SDL_EVENT_BUFFER_LEN 64

namespace nx
{
static InputKey gSDLKeyToInputKey[SDL_NUM_SCANCODES];

#define NX_SDL_KEY(sdl, engine) gSDLKeyToInputKey[sdl] = InputKey::engine
void
NXSetupSDLKeys()
{
    memset(gSDLKeyToInputKey, NX_U32_MAX, sizeof(gSDLKeyToInputKey));
    NX_SDL_KEY(SDL_SCANCODE_0, kInputKey0);
    NX_SDL_KEY(SDL_SCANCODE_1, kInputKey1);
    NX_SDL_KEY(SDL_SCANCODE_2, kInputKey2);
    NX_SDL_KEY(SDL_SCANCODE_3, kInputKey3);
    NX_SDL_KEY(SDL_SCANCODE_4, kInputKey4);
    NX_SDL_KEY(SDL_SCANCODE_5, kInputKey5);
    NX_SDL_KEY(SDL_SCANCODE_6, kInputKey6);
    NX_SDL_KEY(SDL_SCANCODE_7, kInputKey7);
    NX_SDL_KEY(SDL_SCANCODE_8, kInputKey8);
    NX_SDL_KEY(SDL_SCANCODE_9, kInputKey9);
    NX_SDL_KEY(SDL_SCANCODE_A, kInputKeyA);
    NX_SDL_KEY(SDL_SCANCODE_B, kInputKeyB);
    NX_SDL_KEY(SDL_SCANCODE_C, kInputKeyC);
    NX_SDL_KEY(SDL_SCANCODE_D, kInputKeyD);
    NX_SDL_KEY(SDL_SCANCODE_E, kInputKeyE);
    NX_SDL_KEY(SDL_SCANCODE_F, kInputKeyF);
    NX_SDL_KEY(SDL_SCANCODE_G, kInputKeyG);
    NX_SDL_KEY(SDL_SCANCODE_H, kInputKeyH);
    NX_SDL_KEY(SDL_SCANCODE_I, kInputKeyI);
    NX_SDL_KEY(SDL_SCANCODE_J, kInputKeyJ);
    NX_SDL_KEY(SDL_SCANCODE_K, kInputKeyK);
    NX_SDL_KEY(SDL_SCANCODE_L, kInputKeyL);
    NX_SDL_KEY(SDL_SCANCODE_M, kInputKeyM);
    NX_SDL_KEY(SDL_SCANCODE_N, kInputKeyN);
    NX_SDL_KEY(SDL_SCANCODE_O, kInputKeyO);
    NX_SDL_KEY(SDL_SCANCODE_P, kInputKeyP);
    NX_SDL_KEY(SDL_SCANCODE_Q, kInputKeyQ);
    NX_SDL_KEY(SDL_SCANCODE_R, kInputKeyR);
    NX_SDL_KEY(SDL_SCANCODE_S, kInputKeyS);
    NX_SDL_KEY(SDL_SCANCODE_T, kInputKeyT);
    NX_SDL_KEY(SDL_SCANCODE_U, kInputKeyU);
    NX_SDL_KEY(SDL_SCANCODE_V, kInputKeyV);
    NX_SDL_KEY(SDL_SCANCODE_W, kInputKeyW);
    NX_SDL_KEY(SDL_SCANCODE_X, kInputKeyX);
    NX_SDL_KEY(SDL_SCANCODE_Y, kInputKeyY);
    NX_SDL_KEY(SDL_SCANCODE_Z, kInputKeyZ);
    NX_SDL_KEY(SDL_SCANCODE_RETURN, kInputKeyEnter);
    NX_SDL_KEY(SDL_SCANCODE_ESCAPE, kInputKeyEscape);
    NX_SDL_KEY(SDL_SCANCODE_BACKSPACE, kInputKeyBackSpace);
    NX_SDL_KEY(SDL_SCANCODE_TAB, kInputKeyTab);
    NX_SDL_KEY(SDL_SCANCODE_SPACE, kInputKeySpace);
    NX_SDL_KEY(SDL_SCANCODE_MINUS, kInputKeyMinus);
    NX_SDL_KEY(SDL_SCANCODE_EQUALS, kInputKeyEquals);
    NX_SDL_KEY(SDL_SCANCODE_LEFTBRACKET, kInputKeyLeftBracket);
    NX_SDL_KEY(SDL_SCANCODE_RIGHTBRACKET, kInputKeyRightBracket);
    NX_SDL_KEY(SDL_SCANCODE_BACKSLASH, kInputKeyBackSlash);
    NX_SDL_KEY(SDL_SCANCODE_SEMICOLON, kInputKeySemiColon);
    NX_SDL_KEY(SDL_SCANCODE_APOSTROPHE, kInputKeyApostrophe);
    NX_SDL_KEY(SDL_SCANCODE_GRAVE, kInputKeyTilde);
    NX_SDL_KEY(SDL_SCANCODE_COMMA, kInputKeyComma);
    NX_SDL_KEY(SDL_SCANCODE_PERIOD, kInputKeyPeriod);
    NX_SDL_KEY(SDL_SCANCODE_SLASH, kInputKeySlash);
    NX_SDL_KEY(SDL_SCANCODE_CAPSLOCK, kInputKeyCapsLock);
    NX_SDL_KEY(SDL_SCANCODE_F1, kInputKeyF1);
    NX_SDL_KEY(SDL_SCANCODE_F2, kInputKeyF2);
    NX_SDL_KEY(SDL_SCANCODE_F3, kInputKeyF3);
    NX_SDL_KEY(SDL_SCANCODE_F4, kInputKeyF4);
    NX_SDL_KEY(SDL_SCANCODE_F5, kInputKeyF5);
    NX_SDL_KEY(SDL_SCANCODE_F6, kInputKeyF6);
    NX_SDL_KEY(SDL_SCANCODE_F7, kInputKeyF7);
    NX_SDL_KEY(SDL_SCANCODE_F8, kInputKeyF8);
    NX_SDL_KEY(SDL_SCANCODE_F9, kInputKeyF9);
    NX_SDL_KEY(SDL_SCANCODE_F10, kInputKeyF10);
    NX_SDL_KEY(SDL_SCANCODE_F11, kInputKeyF11);
    NX_SDL_KEY(SDL_SCANCODE_F12, kInputKeyF12);
    NX_SDL_KEY(SDL_SCANCODE_PRINTSCREEN, kInputKeyPrintScreen);
    NX_SDL_KEY(SDL_SCANCODE_SCROLLLOCK, kInputKeyScrollLock);
    NX_SDL_KEY(SDL_SCANCODE_PAUSE, kInputKeyPause);
    NX_SDL_KEY(SDL_SCANCODE_INSERT, kInputKeyInsert);
    NX_SDL_KEY(SDL_SCANCODE_HOME, kInputKeyHome);
    NX_SDL_KEY(SDL_SCANCODE_PAGEUP, kInputKeyPgUp);
    NX_SDL_KEY(SDL_SCANCODE_DELETE, kInputKeyDelete);
    NX_SDL_KEY(SDL_SCANCODE_END, kInputKeyEnd);
    NX_SDL_KEY(SDL_SCANCODE_PAGEDOWN, kInputKeyPgDown);
    NX_SDL_KEY(SDL_SCANCODE_RIGHT, kInputKeyRight);
    NX_SDL_KEY(SDL_SCANCODE_LEFT, kInputKeyLeft);
    NX_SDL_KEY(SDL_SCANCODE_DOWN, kInputKeyDown);
    NX_SDL_KEY(SDL_SCANCODE_UP, kInputKeyUp);
    NX_SDL_KEY(SDL_SCANCODE_KP_DIVIDE, kInputKeyKPDivide);
    NX_SDL_KEY(SDL_SCANCODE_KP_MULTIPLY, kInputKeyKPMulitply);
    NX_SDL_KEY(SDL_SCANCODE_KP_MINUS, kInputKeyKPMinus);
    NX_SDL_KEY(SDL_SCANCODE_KP_PLUS, kInputKeyKPPlus);
    NX_SDL_KEY(SDL_SCANCODE_KP_ENTER, kInputKeyKPEnter);
    NX_SDL_KEY(SDL_SCANCODE_KP_1, kInputKeyKP1);
    NX_SDL_KEY(SDL_SCANCODE_KP_2, kInputKeyKP2);
    NX_SDL_KEY(SDL_SCANCODE_KP_3, kInputKeyKP3);
    NX_SDL_KEY(SDL_SCANCODE_KP_4, kInputKeyKP4);
    NX_SDL_KEY(SDL_SCANCODE_KP_5, kInputKeyKP5);
    NX_SDL_KEY(SDL_SCANCODE_KP_6, kInputKeyKP6);
    NX_SDL_KEY(SDL_SCANCODE_KP_7, kInputKeyKP7);
    NX_SDL_KEY(SDL_SCANCODE_KP_8, kInputKeyKP8);
    NX_SDL_KEY(SDL_SCANCODE_KP_9, kInputKeyKP9);
    NX_SDL_KEY(SDL_SCANCODE_KP_0, kInputKeyKP0);
    NX_SDL_KEY(SDL_SCANCODE_KP_PERIOD, kInputKeyKPPeriod);
    NX_SDL_KEY(SDL_SCANCODE_LSHIFT, kInputKeyLeftShift);
    NX_SDL_KEY(SDL_SCANCODE_RSHIFT, kInputKeyRightShift);
    NX_SDL_KEY(SDL_SCANCODE_LCTRL, kInputKeyLeftCtrl);
    NX_SDL_KEY(SDL_SCANCODE_RCTRL, kInputKeyRightCtrl);
    NX_SDL_KEY(SDL_SCANCODE_LALT, kInputKeyLeftAlt);
    NX_SDL_KEY(SDL_SCANCODE_RALT, kInputKeyRightAlt);
    NX_SDL_KEY(SDL_SCANCODE_LGUI, kInputKeyLeftOS);
    NX_SDL_KEY(SDL_SCANCODE_RGUI, kInputKeyRightOS);
    NX_SDL_KEY(SDL_SCANCODE_NONUSBACKSLASH, kInputKeyBackSlash);
}


static nx_u32
NXSDLKeyToInputKey(const nx_u32 sdlkey)
{
    return gSDLKeyToInputKey[sdlkey];
}

void
NXProcessSDLInput(NXInputManager* pManager)
{
    // peek events
    SDL_Event event_buffer[NX_SDL_EVENT_BUFFER_LEN];
    int n_events;
    nx_u32 evt_type;
    SDL_Event* p_sdl_evt;
    // -- Key board events ------------------------------------------------------
    n_events = SDL_PeepEvents(event_buffer, NX_SDL_EVENT_BUFFER_LEN, SDL_GETEVENT,
                              SDL_KEYDOWN, SDL_KEYUP);
    for (int i = 0; i < n_events; ++i)
    {
        evt_type = event_buffer[i].type;
        p_sdl_evt = &event_buffer[i];
        // check if valid
        if (evt_type == SDL_KEYDOWN || evt_type == SDL_KEYUP)
        {
            // translate key;
            nx_u32 input_key = NXSDLKeyToInputKey(p_sdl_evt->key.keysym.scancode);
            if (input_key >= kInputKeyTotal)
            {
                NXLogError("ProcessSDLInput: Unknown keyboard key 0x%x",
                                  p_sdl_evt->key.keysym.scancode);
                continue;
            }
            // Create keyboard event
            NXInputKeyboardEvent evt;
            evt.key = input_key;
            evt.key |= (evt_type == SDL_KEYDOWN) ? kInputButtonStateDown: kInputButtonStateUp;
            // submit event
            pManager->postKeyBoardEvent(evt);
        }
        else
        {
            NXLogError("ProcessSDLInput: Invalid keyboard event 0x%x", evt_type);
            continue;
        }
    }
    // -- Mouse Events ----------------------------------------------------------
    n_events = SDL_PeepEvents(event_buffer, NX_SDL_EVENT_BUFFER_LEN, SDL_GETEVENT,
                              SDL_MOUSEMOTION, SDL_MOUSEWHEEL);
    for (int i = 0; i < n_events; ++i)
    {
        evt_type = event_buffer[i].type;
        p_sdl_evt = &event_buffer[i];
        // check if valid
        if (evt_type == SDL_MOUSEMOTION)
        {
            NXInputMouseMoveEvent evt;
            evt.deltax = p_sdl_evt->motion.xrel;
            evt.deltay = -1 * p_sdl_evt->motion.yrel;
            pManager->postMouseMoveEvent(evt);
        }
        else if (evt_type == SDL_MOUSEBUTTONDOWN || evt_type == SDL_MOUSEBUTTONUP)
        {
            NXInputMouseButtonEvent evt;
            evt.btn = (p_sdl_evt->button.button - 1);
            evt.btn |= (evt_type == SDL_MOUSEBUTTONDOWN) ? kInputButtonStateDown: kInputButtonStateUp;
            evt.x = p_sdl_evt->button.x;
            evt.y = p_sdl_evt->button.y;
            pManager->postMouseButtonEvent(evt);
        }
        else if (evt_type == SDL_MOUSEWHEEL)
        {
            NXInputScrollEvent evt;
            evt.xvalue = p_sdl_evt->wheel.x;
            evt.yvalue = p_sdl_evt->wheel.y;
            pManager->postScrollEvent(evt);
        }
        else
        {
            NXLogError("ProcessSDLInput: Invalid mouse event 0x%x", evt_type);
            continue;
        }
    }

}

}
#endif // defined (NX_SYSTEM_SDL2)
