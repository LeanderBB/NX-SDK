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
#ifndef __NX_INPUT_H__
#define __NX_INPUT_H__

namespace nx
{

enum InputEventType
{
    kInputEventTypeKey,
    kInputEventTypeMouseButton,
    kInputEventTypeMouseMove,
    kInputEventTypeScroll,
    kInputEventTypeText
};

enum InputMouseButton
{
    kMouseButtonLeft,
    kMouseButtonMiddle,
    kMouseButtonRight,
    kMouseButtonExtra0,
    kMouseButtonExtra1,

    kMouseButtonTotal,
    kMouseButtonMask = 0xFFFF
};
enum InputKey
{
    kInputKeyA,
    kInputKeyB,
    kInputKeyC,
    kInputKeyD,
    kInputKeyE,
    kInputKeyF,
    kInputKeyG,
    kInputKeyH,
    kInputKeyI,
    kInputKeyJ,
    kInputKeyK,
    kInputKeyL,
    kInputKeyM,
    kInputKeyN,
    kInputKeyO,
    kInputKeyP,
    kInputKeyQ,
    kInputKeyR,
    kInputKeyS,
    kInputKeyT,
    kInputKeyU,
    kInputKeyV,
    kInputKeyW,
    kInputKeyX,
    kInputKeyY,
    kInputKeyZ,
    kInputKey0,
    kInputKey1,
    kInputKey2,
    kInputKey3,
    kInputKey4,
    kInputKey5,
    kInputKey6,
    kInputKey7,
    kInputKey8,
    kInputKey9,
    kInputKeyUp,
    kInputKeyDown,
    kInputKeyRight,
    kInputKeyLeft,
    kInputKeySpace,
    kInputKeyLeftShift,
    kInputKeyRightShift,
    kInputKeyLeftCtrl,
    kInputKeyRightCtrl,
    kInputKeyLeftAlt,
    kInputKeyRightAlt,
    kInputKeyTab,
    kInputKeyTilde,
    kInputKeyPlus,
    kInputKeyMinus,
    kInputKeyBackSlash,
    kInputKeyPeriod,
    kInputKeyComma,
    kInputKeySlash,
    kInputKeyColon,
    kInputKeyApostrophe,
    kInputKeyBackSpace,
    kInputKeyEnter,
    kInputKeyCapsLock,
    kInputKeyEscape,
    kInputKeyEquals,
    kInputKeyLeftBracket,
    kInputKeyRightBracket,
    kInputKeySemiColon,
    kInputKeyF1,
    kInputKeyF2,
    kInputKeyF3,
    kInputKeyF4,
    kInputKeyF5,
    kInputKeyF6,
    kInputKeyF7,
    kInputKeyF8,
    kInputKeyF9,
    kInputKeyF10,
    kInputKeyF11,
    kInputKeyF12,
    kInputKeyPrintScreen,
    kInputKeyScrollLock,
    kInputKeyPause,
    kInputKeyHome,
    kInputKeyDelete,
    kInputKeyInsert,
    kInputKeyEnd,
    kInputKeyPgUp,
    kInputKeyPgDown,
    kInputKeyKPDivide,
    kInputKeyKPMulitply,
    kInputKeyKPPlus,
    kInputKeyKPMinus,
    kInputKeyKPEnter,
    kInputKeyKPPeriod,
    kInputKeyKP0,
    kInputKeyKP1,
    kInputKeyKP2,
    kInputKeyKP3,
    kInputKeyKP4,
    kInputKeyKP5,
    kInputKeyKP6,
    kInputKeyKP7,
    kInputKeyKP8,
    kInputKeyKP9,
    kInputKeyLeftOS,
    kInputKeyRightOS,
    // place holder do not use
    kInputKeyTotal,
    kInputKeyMask = 0xFFFF
};

enum InputKeyModifiers
{
    kInputKeyModLCtrl = NX_BIT(0),
    kInputKeyModLShift = NX_BIT(1),
    kInputKeyModLAlt = NX_BIT(2),
    kInputKeyModLOS = NX_BIT(3),
    kInputKeyModRCtrl = NX_BIT(4),
    kInputKeyModRShift = NX_BIT(5),
    kInputKeyModRAlt = NX_BIT(6),
    kInputKeyModROS = NX_BIT(7),
    kInputKeyModLMouse = NX_BIT(8),
    kInputKeyModMMouse = NX_BIT(9),
    kInputKeyModRMouse = NX_BIT(10)
};

enum InputButtonState
{
    kInputButtonStateDown = NX_BIT(30),
    kInputButtonStateUp = NX_BIT(31),
    kInputButtonMask = (kInputButtonStateDown | kInputButtonStateUp)
};

struct NXInputKeyboardEvent
{
   nx_u32 key;
};

struct NXInputMouseButtonEvent
{
    nx_u32 btn;
    nx_i32 x;
    nx_i32 y;
};

struct NXInputScrollEvent
{
    nx_i32 xvalue;
    nx_i32 yvalue;
};

struct NXInputMouseMoveEvent
{
    nx_i32 deltax;
    nx_i32 deltay;
};


struct NXInputEvent
{
    InputEventType type;
    nx_u32 flags;
    union
    {
        struct NXInputKeyboardEvent keyboard;
        struct NXInputMouseButtonEvent mouseBtn;
        struct NXInputMouseMoveEvent mouseMove;
        struct NXInputScrollEvent scroll;
    }evt;
};


char NXInputEventToASCII(const struct NXInputEvent& rEvt);

nx_u32 NXInputKeyToModifierBit(const nx_u32 key);

nx_u32 NXInputMouseButtonToModifierBit(const nx_u32 button);

const char* NXInputKeyToString(const nx_u32 key);


NX_INLINE bool
NXInputKeyDown(const NXInputKeyboardEvent& input)
{

    return (input.key  & kInputButtonStateDown);
}

NX_INLINE bool
NXInputKeyUp(const NXInputKeyboardEvent& input)
{
   return (input.key & kInputButtonStateUp);
}

NX_INLINE InputKey
NXInputKey(const NXInputKeyboardEvent& input)
{
    return static_cast<InputKey>(input.key & kInputKeyMask);
}

}

#endif
