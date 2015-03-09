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
#include "nx/nxcore.h"

#if defined(NX_SYSTEM_GLFW)
#include "nx/sys/glfw/nxglfwinput.h"
#include "nx/sys/nxinput.h"
#include "nx/sys/nxinputmanager.h"
#include "nx/sys/nxsystem.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace nx
{
static InputKey gGLFWKeyToInputKey[GLFW_KEY_LAST + 1];


#define NX_GLFW_KEY(glfwkey, engine) gGLFWKeyToInputKey[glfwkey] = InputKey::engine

void
nxSetupGLFWKeys()
{
    memset(gGLFWKeyToInputKey, NX_U32_MAX, sizeof(gGLFWKeyToInputKey));
    NX_GLFW_KEY(GLFW_KEY_0, kInputKey0);
    NX_GLFW_KEY(GLFW_KEY_1, kInputKey1);
    NX_GLFW_KEY(GLFW_KEY_2, kInputKey2);
    NX_GLFW_KEY(GLFW_KEY_3, kInputKey3);
    NX_GLFW_KEY(GLFW_KEY_4, kInputKey4);
    NX_GLFW_KEY(GLFW_KEY_5, kInputKey5);
    NX_GLFW_KEY(GLFW_KEY_6, kInputKey6);
    NX_GLFW_KEY(GLFW_KEY_7, kInputKey7);
    NX_GLFW_KEY(GLFW_KEY_8, kInputKey8);
    NX_GLFW_KEY(GLFW_KEY_9, kInputKey9);
    NX_GLFW_KEY(GLFW_KEY_A, kInputKeyA);
    NX_GLFW_KEY(GLFW_KEY_B, kInputKeyB);
    NX_GLFW_KEY(GLFW_KEY_C, kInputKeyC);
    NX_GLFW_KEY(GLFW_KEY_D, kInputKeyD);
    NX_GLFW_KEY(GLFW_KEY_E, kInputKeyE);
    NX_GLFW_KEY(GLFW_KEY_F, kInputKeyF);
    NX_GLFW_KEY(GLFW_KEY_G, kInputKeyG);
    NX_GLFW_KEY(GLFW_KEY_H, kInputKeyH);
    NX_GLFW_KEY(GLFW_KEY_I, kInputKeyI);
    NX_GLFW_KEY(GLFW_KEY_J, kInputKeyJ);
    NX_GLFW_KEY(GLFW_KEY_K, kInputKeyK);
    NX_GLFW_KEY(GLFW_KEY_L, kInputKeyL);
    NX_GLFW_KEY(GLFW_KEY_M, kInputKeyM);
    NX_GLFW_KEY(GLFW_KEY_N, kInputKeyN);
    NX_GLFW_KEY(GLFW_KEY_O, kInputKeyO);
    NX_GLFW_KEY(GLFW_KEY_P, kInputKeyP);
    NX_GLFW_KEY(GLFW_KEY_Q, kInputKeyQ);
    NX_GLFW_KEY(GLFW_KEY_R, kInputKeyR);
    NX_GLFW_KEY(GLFW_KEY_S, kInputKeyS);
    NX_GLFW_KEY(GLFW_KEY_T, kInputKeyT);
    NX_GLFW_KEY(GLFW_KEY_U, kInputKeyU);
    NX_GLFW_KEY(GLFW_KEY_V, kInputKeyV);
    NX_GLFW_KEY(GLFW_KEY_W, kInputKeyW);
    NX_GLFW_KEY(GLFW_KEY_X, kInputKeyX);
    NX_GLFW_KEY(GLFW_KEY_Y, kInputKeyY);
    NX_GLFW_KEY(GLFW_KEY_Z, kInputKeyZ);
    NX_GLFW_KEY(GLFW_KEY_ENTER, kInputKeyEnter);
    NX_GLFW_KEY(GLFW_KEY_ESCAPE, kInputKeyEscape);
    NX_GLFW_KEY(GLFW_KEY_BACKSPACE, kInputKeyBackSpace);
    NX_GLFW_KEY(GLFW_KEY_TAB, kInputKeyTab);
    NX_GLFW_KEY(GLFW_KEY_SPACE, kInputKeySpace);
    NX_GLFW_KEY(GLFW_KEY_MINUS, kInputKeyMinus);
    NX_GLFW_KEY(GLFW_KEY_EQUAL, kInputKeyEquals);
    NX_GLFW_KEY(GLFW_KEY_LEFT_BRACKET, kInputKeyLeftBracket);
    NX_GLFW_KEY(GLFW_KEY_RIGHT_BRACKET, kInputKeyRightBracket);
    NX_GLFW_KEY(GLFW_KEY_BACKSLASH, kInputKeyBackSlash);
    NX_GLFW_KEY(GLFW_KEY_SEMICOLON, kInputKeySemiColon);
    NX_GLFW_KEY(GLFW_KEY_APOSTROPHE, kInputKeyApostrophe);
    NX_GLFW_KEY(GLFW_KEY_GRAVE_ACCENT, kInputKeyTilde);
    NX_GLFW_KEY(GLFW_KEY_COMMA, kInputKeyComma);
    NX_GLFW_KEY(GLFW_KEY_PERIOD, kInputKeyPeriod);
    NX_GLFW_KEY(GLFW_KEY_SLASH, kInputKeySlash);
    NX_GLFW_KEY(GLFW_KEY_CAPS_LOCK, kInputKeyCapsLock);
    NX_GLFW_KEY(GLFW_KEY_F1, kInputKeyF1);
    NX_GLFW_KEY(GLFW_KEY_F2, kInputKeyF2);
    NX_GLFW_KEY(GLFW_KEY_F3, kInputKeyF3);
    NX_GLFW_KEY(GLFW_KEY_F4, kInputKeyF4);
    NX_GLFW_KEY(GLFW_KEY_F5, kInputKeyF5);
    NX_GLFW_KEY(GLFW_KEY_F6, kInputKeyF6);
    NX_GLFW_KEY(GLFW_KEY_F7, kInputKeyF7);
    NX_GLFW_KEY(GLFW_KEY_F8, kInputKeyF8);
    NX_GLFW_KEY(GLFW_KEY_F9, kInputKeyF9);
    NX_GLFW_KEY(GLFW_KEY_F10, kInputKeyF10);
    NX_GLFW_KEY(GLFW_KEY_F11, kInputKeyF11);
    NX_GLFW_KEY(GLFW_KEY_F12, kInputKeyF12);
    NX_GLFW_KEY(GLFW_KEY_PRINT_SCREEN, kInputKeyPrintScreen);
    NX_GLFW_KEY(GLFW_KEY_SCROLL_LOCK, kInputKeyScrollLock);
    NX_GLFW_KEY(GLFW_KEY_PAUSE, kInputKeyPause);
    NX_GLFW_KEY(GLFW_KEY_INSERT, kInputKeyInsert);
    NX_GLFW_KEY(GLFW_KEY_HOME, kInputKeyHome);
    NX_GLFW_KEY(GLFW_KEY_PAGE_UP, kInputKeyPgUp);
    NX_GLFW_KEY(GLFW_KEY_DELETE, kInputKeyDelete);
    NX_GLFW_KEY(GLFW_KEY_END, kInputKeyEnd);
    NX_GLFW_KEY(GLFW_KEY_PAGE_DOWN, kInputKeyPgDown);
    NX_GLFW_KEY(GLFW_KEY_RIGHT, kInputKeyRight);
    NX_GLFW_KEY(GLFW_KEY_LEFT, kInputKeyLeft);
    NX_GLFW_KEY(GLFW_KEY_DOWN, kInputKeyDown);
    NX_GLFW_KEY(GLFW_KEY_UP, kInputKeyUp);
    NX_GLFW_KEY(GLFW_KEY_KP_DIVIDE, kInputKeyKPDivide);
    NX_GLFW_KEY(GLFW_KEY_KP_MULTIPLY, kInputKeyKPMulitply);
    NX_GLFW_KEY(GLFW_KEY_KP_SUBTRACT, kInputKeyKPMinus);
    NX_GLFW_KEY(GLFW_KEY_KP_ADD, kInputKeyKPPlus);
    NX_GLFW_KEY(GLFW_KEY_KP_ENTER, kInputKeyKPEnter);
    NX_GLFW_KEY(GLFW_KEY_KP_1, kInputKeyKP1);
    NX_GLFW_KEY(GLFW_KEY_KP_2, kInputKeyKP2);
    NX_GLFW_KEY(GLFW_KEY_KP_3, kInputKeyKP3);
    NX_GLFW_KEY(GLFW_KEY_KP_4, kInputKeyKP4);
    NX_GLFW_KEY(GLFW_KEY_KP_5, kInputKeyKP5);
    NX_GLFW_KEY(GLFW_KEY_KP_6, kInputKeyKP6);
    NX_GLFW_KEY(GLFW_KEY_KP_7, kInputKeyKP7);
    NX_GLFW_KEY(GLFW_KEY_KP_8, kInputKeyKP8);
    NX_GLFW_KEY(GLFW_KEY_KP_9, kInputKeyKP9);
    NX_GLFW_KEY(GLFW_KEY_KP_0, kInputKeyKP0);
    NX_GLFW_KEY(GLFW_KEY_KP_DECIMAL, kInputKeyKPPeriod);
    NX_GLFW_KEY(GLFW_KEY_LEFT_SHIFT, kInputKeyLeftShift);
    NX_GLFW_KEY(GLFW_KEY_RIGHT_SHIFT, kInputKeyRightShift);
    NX_GLFW_KEY(GLFW_KEY_LEFT_CONTROL, kInputKeyLeftCtrl);
    NX_GLFW_KEY(GLFW_KEY_RIGHT_CONTROL, kInputKeyRightCtrl);
    NX_GLFW_KEY(GLFW_KEY_LEFT_ALT, kInputKeyLeftAlt);
    NX_GLFW_KEY(GLFW_KEY_RIGHT_ALT, kInputKeyRightAlt);
    NX_GLFW_KEY(GLFW_KEY_LEFT_SUPER, kInputKeyLeftOS);
    NX_GLFW_KEY(GLFW_KEY_RIGHT_SUPER, kInputKeyRightOS);
}

static
nx_u32 nxGLFWKeyToInputKey(const nx_u32 glfwkey)
{
    return gGLFWKeyToInputKey[glfwkey];
}

void
nxGLFWMouseCallback(GLFWwindow* pWindow,
                    int button,
                    int action,
                    int mods)
{
    NXInputMouseButtonEvent evt;
    (void) mods;
    evt.btn = button;
    evt.btn |= (action == GLFW_PRESS) ? kInputButtonStateDown : kInputButtonStateUp;

    double xpos, ypos;
    glfwGetCursorPos(pWindow, &xpos, &ypos);
    evt.x = floor(xpos);
    evt.y = floor(ypos);

    NXInputManager* p_input = NXSystem::instance()->inputManager();
    NX_ASSERT(p_input);
    p_input->postMouseButtonEvent(evt);
}

void
nxGLFWMousePositionCallback(GLFWwindow*,
                            double x,
                            double y)
{
    static nx_i32 prev_x = 0, prev_y = 0;

    nx_i32 cur_x = floor(x);
    nx_i32 cur_y = floor(y);

    nx_i32 delta_x = cur_x - prev_x;
    nx_i32 delta_y = cur_y - prev_y;

    prev_x = cur_x;
    prev_y = cur_y;

    NXInputMouseMoveEvent evt;
    evt.deltax = delta_x;
    evt.deltay = delta_y;

    NXInputManager* p_input = NXSystem::instance()->inputManager();
    NX_ASSERT(p_input);
    p_input->postMouseMoveEvent(evt);
}

void
nxGLFWScrollCallback(GLFWwindow*,
                     double x ,
                     double y)
{
    NXInputScrollEvent evt;
    evt.xvalue = floor(x);
    evt.yvalue = floor(y);

    NXInputManager* p_input = NXSystem::instance()->inputManager();
    NX_ASSERT(p_input);
    p_input->postScrollEvent(evt);
}

void
nxGLFWKeyboardCallback(GLFWwindow*,
                       int key,
                       int scancode,
                       int action,
                       int mods)
{
    (void) mods;
    nx_u32 input_key = nxGLFWKeyToInputKey(key);
    if (input_key >= kInputKeyTotal)
    {
        NXLogError("nxGLFWKeyboardCallback: Unknown key %d, scancode %d",
                   key, scancode);
        return;
    }

    NXInputKeyboardEvent evt;
    evt.key = input_key;
    evt.key |= (action == GLFW_PRESS || action == GLFW_REPEAT) ? kInputButtonStateDown : kInputButtonStateUp;
    NXInputManager* p_input = NXSystem::instance()->inputManager();
    NX_ASSERT(p_input);
    p_input->postKeyBoardEvent(evt);
}

}

#endif //defined(NX_SYSTEM_GLFW)
