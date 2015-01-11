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
#include "nx/sys/nxapp.h"
#include "nx/sys/nxinputctx.h"



class NXTestInputCtx : public nx::NXInputCtx
{
public:
    NXTestInputCtx():
        nx::NXInputCtx("TestInput", 0)
    {

    }

    virtual bool handleInputEvent(const nx::NXInputEvent& evt)
    {
        switch(evt.type)
        {
        case nx::kInputEventTypeKey:
            nx::NXLog("Key Press Event - Flags : 0x%x Key : %s State : %s",
                      evt.flags, nx::NXInputKeyToString(evt.evt.keyboard.key),
                      (evt.evt.keyboard.key & nx::kInputButtonStateDown) ? "Down" : ((evt.evt.keyboard.key & nx::kInputButtonStateUp) ? "Up" : "Continuous"));
            break;
        case nx::kInputEventTypeMouseButton:
            nx::NXLog("Mouse btn Event - Flags : 0x%x Btn : %d State : %s x : %d y : %d",
                      evt.flags, evt.evt.mouseBtn.btn & nx::kMouseButtonMask,
                      (evt.evt.mouseBtn.btn & nx::kInputButtonStateDown) ? "Down" : "Up",
                      evt.evt.mouseBtn.x, evt.evt.mouseBtn.y);
            break;
        case nx::kInputEventTypeMouseMove:
            nx::NXLog("Mouse move Evt - Flags : 0x%x x : %d y : %d",
                      evt.flags, evt.evt.mouseMove.deltax, evt.evt.mouseMove.deltay);
            break;
        case nx::kInputEventTypeScroll:
            nx::NXLog("Scroll Event - Flags : 0x%x x : %d y : %d",
                      evt.flags, evt.evt.scroll.xvalue, evt.evt.scroll.yvalue);
            break;
        default:
            nx::NXLog("Event not handled 0x%x", evt.type);
            break;
        }
        return false;
    }
};


class NXTestInputApp : public nx::NXApp
{
public:

    NXTestInputApp():
        nx::NXApp("TestInputApp")
    {

    }

    virtual bool appInit(const int , const char**)
    {
        inputManager()->addInputCtx(&_inputCtx);
        return true;
    }

    virtual void appRun()
    {
        inputManager()->tick();
    }

    virtual void appTerm()
    {
        inputManager()->remInputCtx(&_inputCtx);
    }

private:
    NXTestInputCtx _inputCtx;
};






NX_APP(NXTestInputApp)
