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
#include "nx/sys/nxsysevents.h"

namespace nx
{

const nx_u32 NXSysEvtWinResize::sEvtType = kSystemEventWinResize;

const nx_u32 NXSysEvtLowMem::sEvtType = kSystemEventLowMem;

const nx_u32 NXSysEvtWinHide::sEvtType = kSystemEventWinHide;

const nx_u32 NXSysEvtWinShow::sEvtType =kSystemEventWinShow;

const nx_u32 NXSysEvtWinCreated::sEvtType = kSystemEventWinCreated;

const nx_u32 NXSysEvtWinDestroy::sEvtType = kSystemEventWinDestroy;

}
