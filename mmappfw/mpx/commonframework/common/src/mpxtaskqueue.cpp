/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Serializing the task
*
*/

#include <mpxlog.h>
#include "mpxtaskqueueobserver.h"
#include "mpxtaskqueue.h"

// ============================ PRIVATE CLASS =================================
NONSHARABLE_CLASS(CTask) : public CBase
    {
public:
    /**
    *  C++ constructor
    *
    *  @param aTask task id
    *  @param aCallback callback
    *  @param aHandler task handler
    *  @param aParamData data
    *  @param aBuf, extra data in buffer, ownership transferred.
    *  @param aPtrData, extra pointer data
    *  @param aAlive1, pointer to CBased object, ownership transferred
    *  @param aCObject2, pointer to CBased object, ownership transferred
    */
    CTask(TInt aTask,
          TAny* aCallback,
          MMPXTaskQueueObserver* aHandler,
          TInt aParamData=0,
          CBufBase* aBuf=NULL,
          TAny* aPtrData=NULL,
          CBase* aCObject1=NULL,
          CBase* aCObject2=NULL
          );

    /**
    * destructor
    */
    ~CTask();

    /**
    *  Offset to the task queue
    *
    *  @return the offset
    */
    static TInt Offset();

public:
    TInt iTask;
    TAny* iCallback; // call back when task completed
    MMPXTaskQueueObserver* iHandler; // task handler which will execute the task
    TInt iParamData; // Integer data
    CBufBase* iBuf;  // owned. deleted when task is removed
    TAny* iPtrData;  // ojbect data, Not owned.
    CBase* iCObject1;  // owned
    CBase* iCObject2;  // owned

    TPtrC8 iPtr;     // Descriptor to iBuf
    TSglQueLink iLink;
    };

// ----------------------------------------------------------------------------
// Task contructor
// ----------------------------------------------------------------------------
//
CTask::CTask(
    TInt aTask,
    TAny* aCallback,
    MMPXTaskQueueObserver* aHandler,
    TInt aParamData/*=0*/,
    CBufBase* aBuf/*=NULL*/,
    TAny* aPtrData/*=NULL*/,
    CBase* aCObject1/*=NULL*/,
    CBase* aCObject2/*=NULL*/)
:   iTask(aTask), iCallback(aCallback), iHandler(aHandler),
    iParamData(aParamData),iBuf(aBuf), iPtrData(aPtrData),
    iCObject1(aCObject1),iCObject2(aCObject2)
    {
    if (iBuf)
        {
        iPtr.Set(iBuf->Ptr(0));
        }
    else
        {
        iPtr.Set(KNullDesC8);
        }
    }

// ----------------------------------------------------------------------------
// Task destructor
// ----------------------------------------------------------------------------
//
CTask::~CTask()
    {
    delete iBuf;
    delete iCObject1;
    delete iCObject2;
    }

// ----------------------------------------------------------------------------
// Offset of the task queue
// ----------------------------------------------------------------------------
//
TInt CTask::Offset()
    {
    return _FOFF(CTask,iLink);
    }

// ============================ PRIVATE CLASS =================================
NONSHARABLE_CLASS(CMPXTaskQueueScheduler) : public CActive
    {
    public:
        /**
        * Two-phased constructor
        * @param aObserver the observer
        * @return object of constructed
        */
        static CMPXTaskQueueScheduler* NewL(TSglQue<CTask>& aTaskQueue);

        /**
        *  Destructor
        */
        ~CMPXTaskQueueScheduler();

        /**
        *  Schedule to execute next task
        */
        void ScheduleNextTask();

        /**
        *  Complete current task
        */
        void CompleteTask();

    public: // From base classes

        /**
        * From CActive
        * Handles an active object's request completion event
        */
        void RunL();

        /**
        * From CActive
        * Implements cancellation of an outstanding request.
        */
        void DoCancel();

    private:
        /**
        *  C++ constructor
        *
        *  @param aObserver observer
        */
        CMPXTaskQueueScheduler(TSglQue<CTask>& aTaskQueue);

        /**
        *  2nd phase contructor
        *
        */
        void ConstructL();

    private:
        TSglQue<CTask>& iTaskQueue;
        TBool iTaskProcessing;
    };

// ----------------------------------------------------------------------------
// Two-phased constructor
// ----------------------------------------------------------------------------
//
CMPXTaskQueueScheduler* CMPXTaskQueueScheduler::NewL(TSglQue<CTask>& aTaskQueue)
    {
    CMPXTaskQueueScheduler* s = new(ELeave)CMPXTaskQueueScheduler(aTaskQueue);
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXTaskQueueScheduler::CMPXTaskQueueScheduler(TSglQue<CTask>& aTaskQueue)
    :CActive(EPriorityStandard),
    iTaskQueue(aTaskQueue),
    iTaskProcessing(EFalse)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
void CMPXTaskQueueScheduler::ConstructL()
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXTaskQueueScheduler::~CMPXTaskQueueScheduler()
    {
    Cancel();
    }

// ----------------------------------------------------------------------------
// Schedule to execute next task
// ----------------------------------------------------------------------------
//
void CMPXTaskQueueScheduler::ScheduleNextTask()
    {
    if (!IsActive() && !iTaskProcessing)
        {
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    }

// ----------------------------------------------------------------------------
// Complete current task
// ----------------------------------------------------------------------------
//
void CMPXTaskQueueScheduler::CompleteTask()
    {
    iTaskProcessing = EFalse;
    }

// ----------------------------------------------------------------------------
// Plugin has completed the request
// ----------------------------------------------------------------------------
//
void CMPXTaskQueueScheduler::RunL()
    {
    MPX_FUNC_EX("CMPXTaskQueueScheduler::RunL");
    if (!iTaskQueue.IsEmpty())
        {
        CTask* task=iTaskQueue.First();
        if (task->iHandler)
            {
            iTaskProcessing = ETrue;
            MPX_DEBUG3("CMPXTaskQueueScheduler::RunL %d cb 0x%08x",
                       task->iTask, task->iCallback);
            task->iHandler->ExecuteTask(task->iTask,
                                        task->iParamData,
                                        task->iPtrData,
                                        *(task->iBuf),
                                        task->iCallback,
                                        task->iCObject1,
                                        task->iCObject2);
            }
        }
    }

// ----------------------------------------------------------------------------
// Canceling has to be done by plugin
// ----------------------------------------------------------------------------
//
void CMPXTaskQueueScheduler::DoCancel()
    {
    iTaskProcessing = EFalse;
    }


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXTaskQueue* CMPXTaskQueue::NewL()
    {
    CMPXTaskQueue* s = new(ELeave)CMPXTaskQueue();
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXTaskQueue::CMPXTaskQueue()
   :iTaskList(CTask::Offset())
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXTaskQueue::~CMPXTaskQueue()
    {
    Reset();
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
void CMPXTaskQueue::ConstructL()
    {
    }


// ----------------------------------------------------------------------------
// Add a task
// ----------------------------------------------------------------------------
//
EXPORT_C  void CMPXTaskQueue::AddTaskL(
    TInt aTask,
    TAny* aCallback,
    TInt aParamData/*=0*/,
    CBufBase* aBuf/*=NULL*/,
    TAny* aPtrData/*=NULL*/,
    CBase* aCObject1/*=NULL*/,
    CBase* aCObject2/*=NULL*/)
    {
    MPX_FUNC_EX("CMPXTaskQueue::AddTaskL");
    CTask* t=new(ELeave)CTask(aTask,aCallback,NULL, aParamData,
                              aBuf, aPtrData,aCObject1,aCObject2);
    iTaskList.AddLast(*t);
    }

// ----------------------------------------------------------------------------
// Remove a task
// ----------------------------------------------------------------------------
//
EXPORT_C TAny* CMPXTaskQueue::RemoveTask()
    {
    MPX_FUNC_EX("CMPXTaskQueue::RemoveTask()");
    // Remove current task from queque
    if (iTaskList.IsEmpty())
        {
        return NULL;
        }
    CTask* task=iTaskList.First();
    TAny* cb=task->iCallback;
    iTaskList.Remove(*task);
    MPX_DEBUG3("CMPXTaskQueue::RemoveTask %d cb 0x%08x", task->iTask, cb);
    delete task;
    return cb;
    }

// ----------------------------------------------------------------------------
// Remove task from a client
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXTaskQueue::RemoveTask(TAny* aCallback)
    {
    MPX_FUNC_EX("CMPXTaskQueue::RemoveTask(TAny* aCallback)");
    // Remove current task from queque
    TSglQueIter<CTask> iter(iTaskList);
    CTask* t=NULL;
    while ((t=iter++) != NULL) // Compiler warning on while (t=iter++)
        {
        if (t->iCallback == aCallback)
            {
            iTaskList.Remove(*t);
            MPX_DEBUG3("CMPXTaskQueue::RemoveTask %d cb 0x%08x",
                       t->iTask, aCallback);
            delete t;
            }
        }
    }

// ----------------------------------------------------------------------------
// Reutrn if the task queue empty
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXTaskQueue::IsEmpty()
    {
    return iTaskList.IsEmpty();
    }

// ----------------------------------------------------------------------------
// Reset task queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXTaskQueue::Reset()
    {
    TSglQueIter<CTask> iter(iTaskList);
    CTask* t=NULL;
    while ((t=iter++) != NULL) // Compiler warning on while (t=iter++)
        {
        delete t;
        }
    iTaskList.Reset();
    }

// ----------------------------------------------------------------------------
// Return current task
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXTaskQueue::Param()
    {
    return iTaskList.IsEmpty() ? KErrNotFound :  iTaskList.First()->iParamData;
    }

// ----------------------------------------------------------------------------
// Return current parameter data
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXTaskQueue::Task()
    {
    return iTaskList.IsEmpty() ? KErrNotFound :  iTaskList.First()->iTask;
    }

// ----------------------------------------------------------------------------
// Return current call back
// ----------------------------------------------------------------------------
//
EXPORT_C TAny* CMPXTaskQueue::Callback()
    {
    return iTaskList.IsEmpty() ? NULL :  iTaskList.First()->iCallback;
    }

// ----------------------------------------------------------------------------
// Return pointer data
// ----------------------------------------------------------------------------
//
EXPORT_C TAny* CMPXTaskQueue::PtrData()
    {
    return iTaskList.IsEmpty() ? NULL :  iTaskList.First()->iPtrData;
    }

// ----------------------------------------------------------------------------
// Return buffer data
// ----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMPXTaskQueue::BufData()
    {
    if (iTaskList.IsEmpty())
        {
        return KNullDesC8;
        }
    else
        {
        return iTaskList.First()->iPtr;
        }
    }

// ----------------------------------------------------------------------------
// Two-phased constructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXActiveTaskQueue* CMPXActiveTaskQueue::NewL()
    {
    CMPXActiveTaskQueue* s = new(ELeave)CMPXActiveTaskQueue();
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXActiveTaskQueue::CMPXActiveTaskQueue()
    {
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
void CMPXActiveTaskQueue::ConstructL()
    {
    iScheduler = CMPXTaskQueueScheduler::NewL(iTaskList);
    }


// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXActiveTaskQueue::~CMPXActiveTaskQueue()
    {
    MPX_FUNC_EX("CMPXActiveTaskQueue::~CMPXActiveTaskQueue");
    if (iScheduler)
        {
        delete iScheduler;
        }
    }

// ----------------------------------------------------------------------------
// Add a task
// ----------------------------------------------------------------------------
//
EXPORT_C  void CMPXActiveTaskQueue::AddTaskL(
    TInt aTask,
    TAny* aCallback,
    MMPXTaskQueueObserver* aHandler,
    TInt aParamData/*=0*/,
    CBufBase* aBuf/*=NULL*/,
    TAny* aPtrData/*=NULL*/,
    CBase* aCObject1/*=NULL*/,
    CBase* aCObject2/*=NULL*/)
    {
    MPX_FUNC_EX("CMPXActiveTaskQueue::AddTaskL");
    CTask* t=new(ELeave)CTask(aTask,aCallback,aHandler,aParamData,
                              aBuf,aPtrData,aCObject1,aCObject2);
    iTaskList.AddLast(*t);
    ScheduleNextTask();
    }

// ----------------------------------------------------------------------------
// Schedule to execute next task
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXActiveTaskQueue::ScheduleNextTask()
    {
    iScheduler->ScheduleNextTask();
    }

// ----------------------------------------------------------------------------
// Complete current task
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXActiveTaskQueue::CompleteTask()
    {
    MPX_FUNC_EX("CMPXActiveTaskQueue::CompleteTask()");
    (void)RemoveTask();
    iScheduler->CompleteTask();
    ScheduleNextTask();
    }

// ----------------------------------------------------------------------------
// Cancels all outsatnding tasks
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXActiveTaskQueue::CancelRequests()
    {
    MPX_FUNC_EX("CMPXActiveTaskQueue::CancelRequests");
    iScheduler->Cancel();
    CMPXTaskQueue::Reset();
    }

// ----------------------------------------------------------------------------
// CMPXActiveTaskQueue::CompleteAllTasks
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXActiveTaskQueue::CompleteAllTasks(
    TInt aError)
    {
    MPX_FUNC_EX("CMPXActiveTaskQueue::CompleteAllTasks");
    iScheduler->Cancel();

    TSglQueIter<CTask> iter(iTaskList);
    CTask* task(NULL);
    while ((task = iter++) != NULL) // Compiler warning on while (t=iter++)
        {
        task->iHandler->HandleTaskError(task->iTask, task->iPtrData,
                                        task->iCallback, aError);
        delete task;
        }
    iTaskList.Reset();
    }
