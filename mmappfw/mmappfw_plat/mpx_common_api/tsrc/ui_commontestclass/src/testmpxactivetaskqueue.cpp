/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPXActiveTaskQueue testing implementation
*
*/


#include <s32mem.h>
#include "commontestclass.h"


// Begin CMPXActiveTaskQueue testing implementation (mpxtaskqueue.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXActiveTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXActiveTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::NewL() begin"));
    TInt err = KErrNone;
    CMPXActiveTaskQueue* taskQueue = NULL;
    taskQueue = CMPXActiveTaskQueue::NewL();
    if ( taskQueue == NULL)
        {
        err = KErrNotFound;
        }
    delete taskQueue;
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::NewL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::NewL() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CancelRequests begin")));
    iLog->Log(_L("CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CancelRequests begin"));
    TInt err = KErrNone;
    CMPXActiveTaskQueue* taskQueue = NULL;
    taskQueue = CMPXActiveTaskQueue::NewL();
    taskQueue->CancelRequests();
    delete taskQueue;
	FTRACE(FPrint(_L("CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CancelRequests end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CancelRequests end=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::AddTaskCMPXActiveTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::AddTaskCMPXActiveTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::AddTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::AddTaskL begin")));
    iLog->Log(_L("CCommonTestClass::AddTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::AddTaskL begin"));
    TInt err = KErrNone;

    CMPXActiveTaskQueue* taskQueue = NULL;
    taskQueue = CMPXActiveTaskQueue::NewL();

    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          NULL,   // callback when task completed
                          NULL,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL );   
    taskQueue->CancelRequests();
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::AddTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::AddTaskL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::AddTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueue::AddTaskL end err=%d"), err);
	return err;
    }
    

// -----------------------------------------------------------------------------
// CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask begin")));
    iLog->Log(_L("CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask begin"));
    TInt err = KErrNone;
    CMPXActiveTaskQueue* taskQueue = NULL;
    taskQueue = CMPXActiveTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          NULL,   // callback when task completed
                          NULL,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL );   
    taskQueue->CompleteTask();
    taskQueue->CancelRequests();
    delete taskQueue;
	FTRACE(FPrint(_L("CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask end=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask begin")));
    iLog->Log(_L("CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask begin"));
    TInt err = KErrNone;
    CMPXActiveTaskQueue* taskQueue = NULL;
    taskQueue = CMPXActiveTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          NULL,   // callback when task completed
                          NULL,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL );   
    taskQueue->CompleteTask();
    taskQueue->CompleteAllTasks( 0 );
    taskQueue->CancelRequests();
    delete taskQueue;
	FTRACE(FPrint(_L("CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL testing CMPXActiveTaskQueueL::CompleteTask end=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::IsEmptyCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::IsEmptyCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::IsEmptyCMPXTaskQueueL testing CMPXTaskQueueL::IsEmpty begin")));
    iLog->Log(_L("CCommonTestClass::IsEmptyCMPXTaskQueueL testing CMPXTaskQueueL::IsEmpty begin"));
    TInt err = KErrNone;
    CMPXTaskQueue* taskQueue = NULL;
    taskQueue = CMPXTaskQueue::NewL();
    taskQueue->IsEmpty();
    delete taskQueue;
	FTRACE(FPrint(_L("CCommonTestClass::IsEmptyCMPXTaskQueueL testing CMPXTaskQueueL::IsEmpty end=%d"), err));
    iLog->Log(_L("CCommonTestClass::IsEmptyCMPXTaskQueueL testing CMPXTaskQueueL::IsEmpty end=%d"), err);
	return err;
    }
    
    
// -----------------------------------------------------------------------------
// CCommonTestClass::AddTaskCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::AddTaskCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::AddTaskCMPXTaskQueueL testing CMPXTaskQueue::AddTaskL begin")));
    iLog->Log(_L("CCommonTestClass::AddTaskCMPXTaskQueueL testing CMPXTaskQueue::AddTaskL begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          NULL,   // callback when task completed
                          NULL,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL );   
    taskQueue->Reset();
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::AddTaskCMPXTaskQueueL testing CMPXTaskQueue::AddTaskL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::AddTaskCMPXTaskQueueL testing CMPXTaskQueue::AddTaskL end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::RemoveTaskCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::RemoveTaskCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::RemoveTaskCMPXTaskQueueL testing CMPXTaskQueue::RemoveTask begin")));
    iLog->Log(_L("CCommonTestClass::RemoveTaskCMPXTaskQueueL testing CMPXTaskQueue::RemoveTask begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->RemoveTask(this);
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::RemoveTaskCMPXTaskQueueL testing CMPXTaskQueue::RemoveTask end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::RemoveTaskCMPXTaskQueueL testing CMPXTaskQueue::RemoveTask end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::CallbackCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CallbackCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CallbackCMPXTaskQueueL testing CMPXTaskQueue::Callback begin")));
    iLog->Log(_L("CCommonTestClass::CallbackCMPXTaskQueueL testing CMPXTaskQueue::Callback begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->Callback();
    taskQueue->RemoveTask(this);
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::CallbackCMPXTaskQueueL testing CMPXTaskQueue::Callback end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CallbackCMPXTaskQueueL testing CMPXTaskQueue::Callback end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::ParamCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ParamCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::Param begin")));
    iLog->Log(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::Param begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->Param();
    taskQueue->RemoveTask(this);
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::Param end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::Param end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::PtrDataCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::PtrDataCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::PtrData begin")));
    iLog->Log(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::PtrData begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->PtrData();
    taskQueue->RemoveTask(this);
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::PtrData end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ParamCMPXTaskQueueL testing CMPXTaskQueue::PtrData end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::BufDataCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::BufDataCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::BufDataCMPXTaskQueueL testing CMPXTaskQueue::BufData begin")));
    iLog->Log(_L("CCommonTestClass::BufDataCMPXTaskQueueL testing CMPXTaskQueue::BufData begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->BufData();
    taskQueue->RemoveTask(this);
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::BufDataCMPXTaskQueueL testing CMPXTaskQueue::BufData end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::BufDataCMPXTaskQueueL testing CMPXTaskQueue::BufData end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::RemoveTask2CMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::RemoveTask2CMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::RemoveTask2CMPXTaskQueueL testing CMPXTaskQueue::RemoveTask begin")));
    iLog->Log(_L("CCommonTestClass::RemoveTask2CMPXTaskQueueL testing CMPXTaskQueue::RemoveTask begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->RemoveTask();
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::RemoveTask2CMPXTaskQueueL testing CMPXTaskQueue::RemoveTask end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::RemoveTask2CMPXTaskQueueL testing CMPXTaskQueue::RemoveTask end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::TaskCMPXTaskQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::TaskCMPXTaskQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::TaskCMPXTaskQueueL testing CMPXTaskQueue::Task begin")));
    iLog->Log(_L("CCommonTestClass::TaskCMPXTaskQueueL testing CMPXTaskQueue::Task begin"));
    TInt err = KErrNone;

    CMPXTaskQueue* taskQueue = CMPXTaskQueue::NewL();
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    taskQueue->AddTaskL( task,
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL,
                          NULL );   

    taskQueue->Task();
    delete taskQueue;

	FTRACE(FPrint(_L("CCommonTestClass::TaskCMPXTaskQueueL testing CMPXTaskQueue::Task end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::TaskCMPXTaskQueueL testing CMPXTaskQueue::Task end err=%d"), err);
	return err;
    }
void CCommonTestClass::ExecuteTask(TInt /*aTask*/, TInt /*aParamData*/, 
                             TAny* /*aPtrData*/,
                             const CBufBase& /*aBuf*/,
                             TAny* /*aCallback*/,
                             CBase* /*aCObject1*/,
                             CBase* /*aCObject2*/)
    {
	FTRACE(FPrint(_L("CCommonTestClass::ExecuteTask is called")));
    iLog->Log(_L("CCommonTestClass::ExecuteTask is called"));
    }
//-----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXTaskQueue()
// Returns: Symbian OS errors.
// add CreateCMPXTaskQueue to testmpxactivetaskqueue.cpp
//-----------------------------------------------------------------------------

TInt CCommonTestClass::CreateCMPXTaskQueue()
    {
      FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXTaskQueue testing CMPXTaskQueue::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXTaskQueue testing CMPXTaskQueue::NewL() begin"));
    TInt err = KErrNone;
    CMPXTaskQueue* taskQueue = NULL;
    TRAP(err, taskQueue = CMPXTaskQueue::NewL());
    if ( taskQueue == NULL)
        {
        err = KErrNotFound;
        }
    delete taskQueue;
      FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXTaskQueue testing CMPXTaskQueue::NewL() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXTaskQueue testing CMPXTaskQueue::NewL() end err=%d"), err);
      return err;
    }
//-----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXTaskQueue()
// Returns: Symbian OS errors.
// add CreateCMPXTaskQueue to testmpxactivetaskqueue.cpp
//-----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXTaskQueue()
    {
      FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXTaskQueue testing CMPXTaskQueue::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXTaskQueue testing CMPXTaskQueue::~ begin"));
    TInt err = KErrNone;
    CMPXTaskQueue* taskQueue = NULL;
    TRAP(err , taskQueue = CMPXTaskQueue::NewL());
    delete taskQueue;
    taskQueue=NULL;
      FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXTaskQueue testing CMPXTaskQueue::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXTaskQueue testing CMPXTaskQueue::~ end err=%d"), err);
      return err;
    }
//-----------------------------------------------------------------------------
// CCommonTestClass::ScheduleNextTaskCMPXTaskQueue()
// Returns: Symbian OS errors.
// add ScheduleNextTaskCMPXTaskQueue to testmpxactivetaskqueue.cpp
//-----------------------------------------------------------------------------
TInt CCommonTestClass::ScheduleNextTaskCMPXTaskQueue()
    {
      FTRACE(FPrint(_L("CCommonTestClass::ScheduleNextTaskCMPXTaskQueue testing CMPXTaskQueue::ScheduleNextTask begin")));
    iLog->Log(_L("CCommonTestClass::ScheduleNextTaskCMPXTaskQueue testing CMPXTaskQueue::ScheduleNextTask begin"));
    TInt err = KErrNone; 
    CMPXActiveTaskQueue* taskQueue = NULL;
    TRAP(err , taskQueue = CMPXActiveTaskQueue::NewL());
    taskQueue->ScheduleNextTask();
    delete taskQueue;
      FTRACE(FPrint(_L("CCommonTestClass::ScheduleNextTaskCMPXTaskQueue testing CMPXTaskQueue::ScheduleNextTask end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ScheduleNextTaskCMPXTaskQueue testing CMPXTaskQueue::ScheduleNextTask end err=%d"), err);
      return err;
    }
//-----------------------------------------------------------------------------
// CCommonTestClass::ResetCMPXTaskQueue()
// Returns: Symbian OS errors.
// add ResetCMPXTaskQueue to testmpxactivetaskqueue.cpp
//-----------------------------------------------------------------------------
TInt CCommonTestClass::ResetCMPXTaskQueue()
    {
      FTRACE(FPrint(_L("CCommonTestClass::ResetCMPXTaskQueue testing CMPXTaskQueue::Reset begin")));
    iLog->Log(_L("CCommonTestClass::ResetCMPXTaskQueue testing CMPXTaskQueue::Reset begin"));
    TInt err = KErrNone;
    CMPXTaskQueue* taskQueue = NULL;
    TRAP(err , taskQueue = CMPXTaskQueue::NewL());
    TInt task(0); //ETaskNone
    CBufBase* taskParam(NULL);
    TRAP(err, taskQueue->AddTaskL( task,
                          NULL,   // callback when task completed
                          NULL,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data 
                          NULL ));   
    taskQueue->Reset();
    delete taskQueue;
      FTRACE(FPrint(_L("CCommonTestClass::ResetCMPXTaskQueue testing CMPXTaskQueue::Reset end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ResetCMPXTaskQueue testing CMPXTaskQueue::Reset end err=%d"), err);
      return err;
    }
//-----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXActiveTaskQueue()
// Returns: Symbian OS errors.
// add DeleteCMPXActiveTaskQueue to testmpxactivetaskqueue.cpp
//-----------------------------------------------------------------------------

TInt CCommonTestClass::DeleteCMPXActiveTaskQueue()
    {
      FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXActiveTaskQueue testing  CMPXTaskQueue::~CMPXActiveTaskQueue begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXActiveTaskQueue testing CMPXTaskQueue::~CMPXActiveTaskQueue begin"));
    TInt err = KErrNone;
    CMPXActiveTaskQueue* taskQueue = NULL;
    TRAP(err , taskQueue = CMPXActiveTaskQueue::NewL());
    delete taskQueue;
    taskQueue=NULL;
      FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXActiveTaskQueue testing CMPXTaskQueue::~CMPXActiveTaskQueue end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXActiveTaskQueue testing CMPXTaskQueue::~CMPXActiveTaskQueue end err=%d"), err);
      return err;
    }   
