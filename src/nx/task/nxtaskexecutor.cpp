#include "nx/nxcore.h"
#include "nx/task/nxtaskexecutor.h"
#include "nx/task/nxtask.h"
#include "nx/task/nxtaskmanager.h"
#include "nx/util/nxtime.h"
namespace nx
{

NXTaskExecutor::NXTaskExecutor(const char* name,
                               NXTaskManager* pManager):
    NXThread(name, kThreadPriotityNormal),
    _pManager(pManager),
    _nExecuted(0)
{
}


NXTaskExecutor::~NXTaskExecutor()
{

}

int
NXTaskExecutor::run()
{
    NXLog("TaskExecutor (%s) Started", _name.c_str());
    while(!this->shouldExit())
    {
        // Grab a job from the job queue
        NXTask* p_task = _pManager->requestTask();
        if (p_task)
        {
            p_task->execute();
            ++ _nExecuted;
        }
        else
        {
            // no task to execute, sleep
            nxSleepMicro(3);
        }
    }
    NXLog("TaskExecutor (%s) Finished", _name.c_str());
    NXLogDebug("TaskExecutor (%s) executed %" NX_PRIsize " jobs", _nExecuted);
    return 0;
}

}
