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
* Description:  Task queue observer
*
*/


#ifndef MMPXTASKQUEUEOBSERVER_H
#define MMPXTASKQUEUEOBSERVER_H

#include <e32base.h>

/**
*  Mix-in class for callback.
*
*  @lib MPXCommon.lib
*/
NONSHARABLE_CLASS( MMPXTaskQueueObserver )
    {
public:
    /**
    * Execute a task.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aParamData parameter
    * @param aPtrData pointer to any object
    * @param aBuf buffer containing externalized parameters for the task.
    * @param aCallback call back function pointer
    * @param aCObject1 object 1 owned by task queue
    * @param aCObject2 object 2 owned by task queue
    */
    virtual void ExecuteTask(TInt aTask,
                             TInt aParamData,
                             TAny* aPtrData,
                             const CBufBase& aBuf,
                             TAny* aCallback,
                             CBase* aCObject1,
                             CBase* aCObject2) = 0;

    /**
    * Indicates that the specified task was completed with an error. This may have happened
    * before the task was actually executed.
    *
    * @since S60 3.2.3
    * @param aTask task number
    * @param aPtrData pointer to any object
    * @param aCallback call back function pointer
    * @param aError error code
    */
    virtual void HandleTaskError(TInt /* aTask */,
                                 TAny* /*aPtrData*/,
                                 TAny* /*aCallback*/,
                                 TInt /* aError */){};
    };

#endif // MMPXTASKQUEUEOBSERVER_H
