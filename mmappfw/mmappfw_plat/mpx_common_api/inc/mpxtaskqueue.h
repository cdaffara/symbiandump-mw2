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


#ifndef CMPXTASKQUEUE_H
#define CMPXTASKQUEUE_H

#include <e32base.h>

class MMPXTaskQueueObserver;
class CMPXTaskQueueScheduler;
class CTask;

/**
* Encapsulates a task queue.
*
* @lib MPXCommon.lib
*/
class CMPXTaskQueue : public CBase
    {
public:
    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @return object of constructed
    */
    IMPORT_C static CMPXTaskQueue* NewL();

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXTaskQueue();

    /**
    *  Add a task into the queue.
    *
    *  @since S60 3.2.3
    *  @param aTask task id
    *  @param aCallback callback
    *  @param aParamData parameter data of the task
    *  @param aBuf extra data in buffer, ownership transferred.
    *  @param aPtrData object pointer, owership not transferred.
    *  @param aCObject1 pointer to CBased object, ownership transferred
    *  @param aCObject2 pointer to CBased object, ownership transferred
    */
    IMPORT_C void AddTaskL(TInt aTask,
                           TAny* aCallback,
                           TInt aParamData=0,
                           CBufBase* aBuf=NULL,
                           TAny* aPtrData=NULL,
                           CBase* aCObject1=NULL,
                           CBase* aCObject2=NULL);

    /**
    *  Remove current task, return callback function.
    *
    *  @since S60 3.2.3
    *  @return callback object
    */
    IMPORT_C TAny* RemoveTask();

    /**
    *  Remove task for one client.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void RemoveTask(TAny* aCallback);

    /**
    *  Return if the queue is empty.
    *
    *  @since S60 3.2.3
    *  @return ETrue empty
    */
    IMPORT_C TBool IsEmpty();

    /**
    *  Reset task queue.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void Reset();

    /**
    *  Get current task id.
    *
    *  @since S60 3.2.3
    *  @return task id
    */
    IMPORT_C TInt Task();

    /**
    *  Get current call back.
    *
    *  @since S60 3.2.3
    *  @return call back
    */
    IMPORT_C TAny* Callback();

    /**
    *  Get current parameter data.
    *
    *  @since S60 3.2.3
    *  @return parameter data
    */
    IMPORT_C TInt Param();

    /**
    *  Get pointer parameter data. Client has ownership of this data.
    *
    *  @since S60 3.2.3
    *  @return pointer paramter
    */
    IMPORT_C TAny* PtrData();

    /**
    *  Get pointer to buffer data. Task Queue has ownership of this buffer.
    *
    *  @since S60 3.2.3
    *  @return buffer paramter
    */
    IMPORT_C const TDesC8& BufData();

protected:
    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    */
    CMPXTaskQueue();

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

protected: //data
    TSglQue<CTask> iTaskList;
    };

/**
* Encapsulates a task queue which will auto schedule the task
*
* @lib MPXCommon.lib
*/
class CMPXActiveTaskQueue : public CMPXTaskQueue
    {
public:
    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aObserver the observer
    * @return object of constructed
    */
    IMPORT_C static CMPXActiveTaskQueue* NewL();

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXActiveTaskQueue();

    /**
    *  Add a task and automatically schedule task.
    *
    *  @since S60 3.2.3
    *  @param aTask task id
    *  @param aCallback callback
    *  @param aHandler task handler
    *  @param aParamData data
    *  @param aBuf pointer to the buffer containing
    *               externalized parameter data.
    *               Client passes ownership of the buffer
    *               to TaskQueue.
    *  @param aPtrData object pointer
    *  @param aCObject1 pointer to CBased object, ownership transferred
    *  @param aCObject2 pointer to CBased object, ownership transferred
    */
    IMPORT_C void AddTaskL(TInt aTask,
                           TAny* aCallback,
                           MMPXTaskQueueObserver* aHandler,
                           TInt aParamData=0,
                           CBufBase* aBuf=NULL,
                           TAny* aPtrData=NULL,
                           CBase* aCObject1=NULL,
                           CBase* aCObject2=NULL);

   /**
    *  Schedule to execute next task.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void ScheduleNextTask();

    /**
    *  Cancel all outstanding, asynchronous requests.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void CancelRequests();

    /**
    *  Complete current task.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void CompleteTask();

    /**
    *  Completes all outstanding tasks with an error code.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void CompleteAllTasks(TInt aError);

private:
    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    */
    CMPXActiveTaskQueue();

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

private: // Data
    CMPXTaskQueueScheduler* iScheduler;

    };

#endif // CMPXTASKQUEUE_H
