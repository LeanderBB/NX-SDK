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
#include "nx/sys/nxinput.h"

namespace nx
{
char NXInputEventToASCII(const NXInputEvent &evt)
{
    char result = '\0';
    if (evt.type == kInputEventTypeKey)
    {
        switch(evt.evt.keyboard.key & kInputKeyMask)
        {
        case kInputKeyA:
            result = 'a';
            break;
        case kInputKeyB:
            result = 'b';
            break;
        case kInputKeyC:
            result = 'c';
            break;
        case kInputKeyD:
            result = 'd';
            break;
        case kInputKeyE:
            result = 'e';
            break;
        case kInputKeyF:
            result = 'f';
            break;
        case kInputKeyG:
            result = 'g';
            break;
        case kInputKeyH:
            result = 'h';
            break;
        case kInputKeyI:
            result = 'i';
            break;
        case kInputKeyJ:
            result = 'j';
            break;
        case kInputKeyK:
            result = 'k';
            break;
        case kInputKeyL:
            result = 'l';
            break;
        case kInputKeyM:
            result = 'm';
            break;
        case kInputKeyN:
            result = 'n';
            break;
        case kInputKeyO:
            result = 'o';
            break;
        case kInputKeyP:
            result = 'p';
            break;
        case kInputKeyQ:
            result = 'q';
            break;
        case kInputKeyR:
            result = 'r';
            break;
        case kInputKeyS:
            result = 's';
            break;
        case kInputKeyT:
            result = 't';
            break;
        case kInputKeyU:
            result = 'u';
            break;
        case kInputKeyV:
            result = 'v';
            break;
        case kInputKeyW:
            result = 'w';
            break;
        case kInputKeyX:
            result = 'x';
            break;
        case kInputKeyY:
            result = 'y';
            break;
        case kInputKeyZ:
            result = 'z';
            break;
        case kInputKeyKP0:
        case kInputKey0:
            result = '0';
            break;
        case kInputKeyKP1:
        case kInputKey1:
            result = '1';
            break;
        case kInputKeyKP2:
        case kInputKey2:
            result = '2';
            break;
        case kInputKeyKP3:
        case kInputKey3:
            result = '3';
            break;
        case kInputKeyKP4:
        case kInputKey4:
            result = '4';
            break;
        case kInputKeyKP5:
        case kInputKey5:
            result = '5';
            break;
        case kInputKeyKP6:
        case kInputKey6:
            result = '6';
            break;
        case kInputKeyKP7:
        case kInputKey7:
            result = '7';
            break;
        case kInputKeyKP8:
        case kInputKey8:
            result = '8';
        case kInputKeyKP9:
        case kInputKey9:
            result = '9';
            break;
        case kInputKeyTilde:
            result = '~';
            break;
        case kInputKeyKPPlus:
        case kInputKeyPlus:
            result = '+';
            break;
        case kInputKeyKPMinus:
        case kInputKeyMinus:
            result = '-';
            break;
        case kInputKeyBackSlash:
            result = '\\';
            break;
        case kInputKeyKPPeriod:
        case kInputKeyPeriod:
            result = '.';
            break;
        case kInputKeyComma:
            result = ',';
            break;
        case kInputKeySlash:
            result = '/';
            break;
        case kInputKeyColon:
            result = ':';
            break;
        case kInputKeyApostrophe:
            result = '\'';
            break;
        case kInputKeyEquals:
            result = '=';
            break;
        case kInputKeyLeftBracket:
            result = '[';
            break;
        case kInputKeyRightBracket:
            result = ']';
            break;
        case kInputKeySemiColon:
            result = ';';
            break;
        case kInputKeyKPDivide:
            result = '/';
            break;
        case kInputKeyKPMulitply:
            result = '*';
            break;
        default:
            NX_ASSERT(false && "Should not be reached");
            break;
        }
    }
    if (evt.flags & (kInputKeyModLShift | kInputKeyModRShift))
    {
        result = toupper(result);
    }
    return result;
}
// ----------------------------------------------------------------------------
nx_u32
NXInputKeyToModifierBit(const nx_u32 key)
{
    switch((key & kInputKeyMask))
    {
    case kInputKeyLeftAlt:
        return kInputKeyModLAlt;
    case kInputKeyLeftCtrl:
        return kInputKeyModRCtrl;
    case kInputKeyLeftShift:
        return kInputKeyModLShift;
    case kInputKeyLeftOS:
        return kInputKeyModLOS;
    case kInputKeyRightAlt:
        return kInputKeyModRAlt;
    case kInputKeyRightCtrl:
        return kInputKeyModRCtrl;
    case kInputKeyRightShift:
        return kInputKeyModRShift;
    case kInputKeyRightOS:
        return kInputKeyModROS;
    default:
        return 0;
    }
}
// ----------------------------------------------------------------------------
nx_u32
NXInputMouseButtonToModifierBit(const nx_u32 button)
{
    switch(button & kMouseButtonMask)
    {
    case kMouseButtonLeft:
        return kInputKeyModLMouse;
    case kMouseButtonMiddle:
        return kInputKeyModMMouse;
    case kMouseButtonRight:
        return kInputKeyModRMouse;
    default:
        return 0;
    }
}


#define NX_INPUT_KEY_TO_STR(K) #K
static const char* gInputKeyToString[kInputKeyTotal] =
{
    NX_INPUT_KEY_TO_STR(kInputKeyA),
    NX_INPUT_KEY_TO_STR(kInputKeyB),
    NX_INPUT_KEY_TO_STR(kInputKeyC),
    NX_INPUT_KEY_TO_STR(kInputKeyD),
    NX_INPUT_KEY_TO_STR(kInputKeyE),
    NX_INPUT_KEY_TO_STR(kInputKeyF),
    NX_INPUT_KEY_TO_STR(kInputKeyG),
    NX_INPUT_KEY_TO_STR(kInputKeyH),
    NX_INPUT_KEY_TO_STR(kInputKeyI),
    NX_INPUT_KEY_TO_STR(kInputKeyJ),
    NX_INPUT_KEY_TO_STR(kInputKeyK),
    NX_INPUT_KEY_TO_STR(kInputKeyL),
    NX_INPUT_KEY_TO_STR(kInputKeyM),
    NX_INPUT_KEY_TO_STR(kInputKeyN),
    NX_INPUT_KEY_TO_STR(kInputKeyO),
    NX_INPUT_KEY_TO_STR(kInputKeyP),
    NX_INPUT_KEY_TO_STR(kInputKeyQ),
    NX_INPUT_KEY_TO_STR(kInputKeyR),
    NX_INPUT_KEY_TO_STR(kInputKeyS),
    NX_INPUT_KEY_TO_STR(kInputKeyT),
    NX_INPUT_KEY_TO_STR(kInputKeyU),
    NX_INPUT_KEY_TO_STR(kInputKeyV),
    NX_INPUT_KEY_TO_STR(kInputKeyW),
    NX_INPUT_KEY_TO_STR(kInputKeyX),
    NX_INPUT_KEY_TO_STR(kInputKeyY),
    NX_INPUT_KEY_TO_STR(kInputKeyZ),
    NX_INPUT_KEY_TO_STR(kInputKey0),
    NX_INPUT_KEY_TO_STR(kInputKey1),
    NX_INPUT_KEY_TO_STR(kInputKey2),
    NX_INPUT_KEY_TO_STR(kInputKey3),
    NX_INPUT_KEY_TO_STR(kInputKey4),
    NX_INPUT_KEY_TO_STR(kInputKey5),
    NX_INPUT_KEY_TO_STR(kInputKey6),
    NX_INPUT_KEY_TO_STR(kInputKey7),
    NX_INPUT_KEY_TO_STR(kInputKey8),
    NX_INPUT_KEY_TO_STR(kInputKey9),
    NX_INPUT_KEY_TO_STR(kInputKeyUp),
    NX_INPUT_KEY_TO_STR(kInputKeyDown),
    NX_INPUT_KEY_TO_STR(kInputKeyRight),
    NX_INPUT_KEY_TO_STR(kInputKeyLeft),
    NX_INPUT_KEY_TO_STR(kInputKeySpace),
    NX_INPUT_KEY_TO_STR(kInputKeyLeftShift),
    NX_INPUT_KEY_TO_STR(kInputKeyRightShift),
    NX_INPUT_KEY_TO_STR(kInputKeyLeftCtrl),
    NX_INPUT_KEY_TO_STR(kInputKeyRightCtrl),
    NX_INPUT_KEY_TO_STR(kInputKeyLeftAlt),
    NX_INPUT_KEY_TO_STR(kInputKeyRightAlt),
    NX_INPUT_KEY_TO_STR(kInputKeyTab),
    NX_INPUT_KEY_TO_STR(kInputKeyTilde),
    NX_INPUT_KEY_TO_STR(kInputKeyPlus),
    NX_INPUT_KEY_TO_STR(kInputKeyMinus),
    NX_INPUT_KEY_TO_STR(kInputKeyBackSlash),
    NX_INPUT_KEY_TO_STR(kInputKeyPeriod),
    NX_INPUT_KEY_TO_STR(kInputKeyComma),
    NX_INPUT_KEY_TO_STR(kInputKeySlash),
    NX_INPUT_KEY_TO_STR(kInputKeyColon),
    NX_INPUT_KEY_TO_STR(kInputKeyApostrophe),
    NX_INPUT_KEY_TO_STR(kInputKeyBackSpace),
    NX_INPUT_KEY_TO_STR(kInputKeyEnter),
    NX_INPUT_KEY_TO_STR(kInputKeyCapsLock),
    NX_INPUT_KEY_TO_STR(kInputKeyEscape),
    NX_INPUT_KEY_TO_STR(kInputKeyEquals),
    NX_INPUT_KEY_TO_STR(kInputKeyLeftBracket),
    NX_INPUT_KEY_TO_STR(kInputKeyRightBracket),
    NX_INPUT_KEY_TO_STR(kInputKeySemiColon),
    NX_INPUT_KEY_TO_STR(kInputKeyF1),
    NX_INPUT_KEY_TO_STR(kInputKeyF2),
    NX_INPUT_KEY_TO_STR(kInputKeyF3),
    NX_INPUT_KEY_TO_STR(kInputKeyF4),
    NX_INPUT_KEY_TO_STR(kInputKeyF5),
    NX_INPUT_KEY_TO_STR(kInputKeyF6),
    NX_INPUT_KEY_TO_STR(kInputKeyF7),
    NX_INPUT_KEY_TO_STR(kInputKeyF8),
    NX_INPUT_KEY_TO_STR(kInputKeyF9),
    NX_INPUT_KEY_TO_STR(kInputKeyF10),
    NX_INPUT_KEY_TO_STR(kInputKeyF11),
    NX_INPUT_KEY_TO_STR(kInputKeyF12),
    NX_INPUT_KEY_TO_STR(kInputKeyPrintScreen),
    NX_INPUT_KEY_TO_STR(kInputKeyScrollLock),
    NX_INPUT_KEY_TO_STR(kInputKeyPause),
    NX_INPUT_KEY_TO_STR(kInputKeyHome),
    NX_INPUT_KEY_TO_STR(kInputKeyDelete),
    NX_INPUT_KEY_TO_STR(kInputKeyInsert),
    NX_INPUT_KEY_TO_STR(kInputKeyEnd),
    NX_INPUT_KEY_TO_STR(kInputKeyPgUp),
    NX_INPUT_KEY_TO_STR(kInputKeyPgDown),
    NX_INPUT_KEY_TO_STR(kInputKeyKPDivide),
    NX_INPUT_KEY_TO_STR(kInputKeyKPMulitply),
    NX_INPUT_KEY_TO_STR(kInputKeyKPPlus),
    NX_INPUT_KEY_TO_STR(kInputKeyKPMinus),
    NX_INPUT_KEY_TO_STR(kInputKeyKPEnter),
    NX_INPUT_KEY_TO_STR(kInputKeyKPPeriod),
    NX_INPUT_KEY_TO_STR(kInputKeyKP0),
    NX_INPUT_KEY_TO_STR(kInputKeyKP1),
    NX_INPUT_KEY_TO_STR(kInputKeyKP2),
    NX_INPUT_KEY_TO_STR(kInputKeyKP3),
    NX_INPUT_KEY_TO_STR(kInputKeyKP4),
    NX_INPUT_KEY_TO_STR(kInputKeyKP5),
    NX_INPUT_KEY_TO_STR(kInputKeyKP6),
    NX_INPUT_KEY_TO_STR(kInputKeyKP7),
    NX_INPUT_KEY_TO_STR(kInputKeyKP8),
    NX_INPUT_KEY_TO_STR(kInputKeyKP9),
    NX_INPUT_KEY_TO_STR(kInputKeyLeftOS),
    NX_INPUT_KEY_TO_STR(kInputKeyRightOS)
};


const char*
NXInputKeyToString(const nx_u32 key)
{
    const nx_u32 idx = key & kInputKeyMask;
    return (idx < kInputKeyTotal) ? gInputKeyToString[idx] : "Invalid";
}

}
