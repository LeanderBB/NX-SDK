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

#if defined(NX_SYSTEM_ANDROID)
#include "nx/sys/android/nxsystemandroid.h"
#include "nx/sys/nxsysevents.h"
#include "nx/sys/nxsystem.h"
#include "nx/sys/nxinputmanager.h"
#include "nx/event/nxeventmanager.h"
#include "nx/sys/nxwindow.h"
#include "nx/sys/android/nxandroidinput.h"
#include "nx/sys/android/android_native_app_glue.h"
#include "nx/sys/nxapp.h"
#include "nx/util/nxtime.h"


namespace nx
{

struct android_app* g_pAndroidApp = nullptr;

void
NXSystemImp::handleAppCmd(struct android_app* pApp,
                          int32_t cmd)
{

    NXSystem* p_system = static_cast<NXSystem*>(pApp->userData);
    NXSystemImp* p_system_imp = static_cast<NXSystemImp*>(pApp->userData);
    (void) p_system;
    switch(cmd)
    {
    case APP_CMD_LOW_MEMORY:
        p_system_imp->onLowMemory(p_system->eventManager());
        break;
    case APP_CMD_PAUSE:
        //p_system->setPaused(true);
        p_system_imp->_waitForEvents = true;
        NXLogDebug("NXSystemImp: APP_CMD_PAUSE");
        break;
    case APP_CMD_RESUME:
        //p_system->setPaused(false);
        p_system_imp->_waitForEvents = false;
        NXLogDebug("NXSystemImp: APP_CMD_RESUME");
        break;
    case APP_CMD_GAINED_FOCUS:
        NXLogDebug("NXSystemImp: APP_CMD_GAINED_FOCUS");
        p_system->setPaused(false);
        p_system_imp->_waitForEvents = false;
        break;
    case APP_CMD_LOST_FOCUS:
        NXLogDebug("NXSystemImp: APP_CMD_LOST_FOCUS");
        p_system->setPaused(true);
        p_system_imp->_waitForEvents = true;
        break;
    case APP_CMD_INIT_WINDOW:
    {
        NXLogDebug("NXSystemImp: APP_CMD_INIT_WINDOW");
        if (!p_system_imp->createWindowInCallback(p_system->eventManager()))
        {
            p_system->signalQuit();
        }
        break;
    }
    case APP_CMD_TERM_WINDOW:
    {
        NXLogDebug("NXSystemImp: APP_CMD_TERM_WINDOW");
        p_system_imp->destroyWindowInCallback(p_system->eventManager());
        break;
    }
    case APP_CMD_CONFIG_CHANGED:
    {
        NXLogDebug("NXSystemImp: APP_CMD_CONFIG_CHANGED");
        break;
    }
    case APP_CMD_DESTROY:
    {
        NXLogDebug("NXSystemImp: APP_CMD_DESTROY");
        p_system->signalQuit();
        break;
    }
    case APP_CMD_INPUT_CHANGED:
        NXLogDebug("NXSystemImp: APP_CMD_INPUT_CHANGED");
        break;
    case APP_CMD_WINDOW_RESIZED:
        NXLogDebug("NXSystemImp: APP_CMD_WINDOW_RESIZED");
        break;
    case APP_CMD_WINDOW_REDRAW_NEEDED:
        NXLogDebug("NXSystemImp: APP_CMD_WINDOW_REDRAW_NEEDED");
        break;
    case APP_CMD_CONTENT_RECT_CHANGED:
        NXLogDebug("NXSystemImp: APP_CMD_CONTENT_RECT_CHANGED");
        break;
    case APP_CMD_START:
        NXLogDebug("NXSystemImp: APP_CMD_START");
        break;
    case APP_CMD_STOP:
        NXLogDebug("NXSystemImp: APP_CMD_STOP");
        break;
    default:
        break;
    }
}

static int32_t
nxHandleAppInput(struct android_app* pApp,
                 AInputEvent* event)
{
    NXSystem* p_system = static_cast<NXSystem*>(pApp->userData);
    NXInputManager* p_input = p_system->inputManager();
    if (p_input)
    {
        return nxProcessAndroidInput(p_input, event);
    }
    return 0;
}

NXSystemImp::NXSystemImp():
    _pWindow(nullptr),
    _pOptions(nullptr),
    _waitForEvents(false)
{

}

NXSystemImp::~NXSystemImp()
{

}

bool
NXSystemImp::initImp(const int argc,
                     const char** argv,
                     const NXAppOptions *pOptions)
{
    (void) argc;
    (void) argv;

    _pOptions = pOptions;

    if (!g_pAndroidApp)
    {
        NXLogFatal("NXSystem (Android): android app is not initialized");
        return false;
    }

    // Make sure glue isn't stripped.
    app_dummy();

    // set NXSystem as local data
    g_pAndroidApp->userData = this;

    //set callbacks
    g_pAndroidApp->onAppCmd = handleAppCmd;
    g_pAndroidApp->onInputEvent = nxHandleAppInput;

    NXLogDebug("NXSystemImp::initImp Completed");

    return true;
}

void
NXSystemImp::termImp(NXEventManager* pEvtManager)
{
    (void) pEvtManager;
    NXLogDebug("NXSystemImp::termImp Completed");
    //ANativeActivity_finish(g_pAndroidApp->activity);
}


bool
NXSystemImp::tickImp(NXEventManager* pEvtManager,
                     NXInputManager*)
{
    int ident;
    int events;
    struct android_poll_source* source;

    // read events
    while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {

        // Process this event.
        if (source != NULL) {
            source->process(g_pAndroidApp, source);
        }

        if(g_pAndroidApp->destroyRequested != 0)
        {
            NXLogDebug("NXSystemImp: Destroy requested");
            return false;
        }
    }



    // check if window resolution changed
    if (_pWindow)
    {
        nx_i32 w,h;
        _pWindow->dimensionsImp(w,h);
        if (w != _pWindow->width() || h != _pWindow->height())
        {
            _pWindow->_height = h;
            _pWindow->_width = w;
            NXSysEvtWinResize evt_resize(w,h);
            pEvtManager->trigger(&evt_resize);
        }
    }

    return true;
}

bool
NXSystemImp::createWindowInCallback( NXEventManager* pEvtManager)
{
    if (!_pWindow)
    {
        // create window
        _pWindow = new NXWindow("NX");

        bool depth = false, stencil = false, dbgctx = false, resizable = false;
        if (_pOptions)
        {
            _pWindow->setDimensions(_pOptions->width, _pOptions->height);
            _pWindow->setFullscreen(_pOptions->fullscreen);
            depth = _pOptions->depth;
            stencil = _pOptions->stencil;
            dbgctx = _pOptions->dbgctx;
            resizable = _pOptions->resizable;
        }

        if (!_pWindow->create(depth, stencil, resizable, dbgctx))
        {
            return false;
        }

        NXLogDebug("NXSystemImp: Window created ");
        NXSysEvtWinCreated evt_created;
        pEvtManager->trigger(&evt_created);
    }
    return true;
}

void
NXSystemImp::destroyWindowInCallback(NXEventManager *pEvtManager)
{
    if (_pWindow)
    {
        NXSysEvtWinDestroy evt_destroy;
        pEvtManager->trigger(&evt_destroy);
        NX_SAFE_DELETE(_pWindow);
        NXLogDebug("NXSystemImp: Window destroyed ");
    }
}

bool
NXSystemImp::createWindowImp(const struct NXAppOptions* pOptions,
                             NXEventManager* pEvtManager)
{
    (void) pOptions;
    (void) pEvtManager;
    // window is created later on with the callbacks
    return true;
}

NXWindow*
NXSystemImp::windowImp()
{
    return _pWindow;
}

void
NXSystemImp::onAppConfigChanged(NXEventManager*)
{
    NXLogDebug("NXSystemImp::onAppConfigChanged");
}

void
NXSystemImp::onLowMemory(NXEventManager* pEvtManager)
{
    NXLogDebug("NXSystemImp::onLowMemory");
    NXSysEvtLowMem evt_lowmem;
    pEvtManager->trigger(&evt_lowmem);
}

}

extern void nx_android_main(const int argc,
                            const char** argv);


// Default entry for the android native_activity
extern "C"
{

NX_EXPORT void android_main(struct android_app* state)
{
    nx::g_pAndroidApp = state;
    pthread_setname_np(pthread_self(), "NX-MAIN");

#if defined(NX_DEBUG)
    //nx::nxSleep(10);
#endif
    nx::NXLogDebug("Android application entry");
    nx_android_main(0, NULL);
    nx::NXLogDebug("Android application exit");

}

}

#endif // defined(NX_SYSTEM_ANDROID)
