/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for active object wrapper
*
*/


#ifndef MSATASYNCTOSYNC_H
#define MSATASYNCTOSYNC_H


#include <e32base.h>

/**
 *  Active object wrapper for SAT Server
 *  Interface provides asynchronous function calls to be synchronous functions.
 *
 *  @lib satengine.lib
 *  @since S60 v5.0.1
 */
class MSatAsyncToSync
    {

public:

    /**
     * Gives reference to this active object wrapper's request status. Use this 
     * when calling asynchronous function and you want to use this wrapper.
     *
     * @return  Request status to be sent to asynchronous function call
     */
    virtual TRequestStatus& RequestStatus() = 0;
    
    /**
     * Sets this wrapper active and starts waiting for request to complete.
     * Function returns after request status given to asynchronous funtion with 
     * RequestStatus is completed or wrapper is cancelled.
     *
     * @return System-wide error code indicating the completion of request
     */
    virtual TInt SetActiveAndWait() = 0;

    /**
     * Cancels request status or timer depending which is active
     */
    virtual void CancelWrapper() = 0;

    /**
     * Release object
     */
    virtual void Release() = 0;
   };

#endif // MSATASYNCTOSYNC_H
