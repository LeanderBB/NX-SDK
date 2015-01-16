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
#ifndef __NX_ANDROIDINPUT_H__
#define __NX_ANDROIDINPUT_H__


#if defined(NX_SYSTEM_ANDROID)

#include <android/native_activity.h>
namespace nx
{

class NXInputManager;

int32_t nxProcessAndroidInput(NXInputManager* pInputManager,
                              AInputEvent* event);

}
#endif // defined(NX_SYSTEM_ANDROID)

#endif
