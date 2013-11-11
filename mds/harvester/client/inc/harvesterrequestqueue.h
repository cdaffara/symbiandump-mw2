/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Processor object for running harvester requests
*
*/


#ifndef HARVESTERREQUESTQUEUE_H
#define HARVESTERREQUESTQUEUE_H

#include <e32base.h>

class CHarvesterRequestActive;

/**
 *  Processor object for running queued tasks.
 *
 *  @since S60 v5.0
 */
class CHarvesterRequestQueue: public CActive
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v5.0
     * @return Instance of CHarvesterRequestQueue.
     */
    static CHarvesterRequestQueue* NewL();

    /**
     * Destructor
     *
     * @since S60 v5.0
     */
    virtual ~CHarvesterRequestQueue();

    /**
     * Activates next request if possible.
     *
     * @since S60 v5.0
     */
    void Process();
    
    /**
     * Adds new request to the queue.
     *
     * @since S60 v5.0
     * @param aRequest Request to be added to the queue.
     */
    void AddRequestL( CHarvesterRequestActive* aRequest );
    
    /**
     * Marks request completed.
     *
     * @since S60 v5.0
     */
    void RequestComplete();
    
    /**
     * Force requests
     *
     * @since S60 v5.0
     */
    void ForceRequests();

    /**
     * ETrue if there are any unfinished harvesting requests
     *
     * @since S60 v5.0
     */
    TBool RequestsPending();

protected:
    // from base class CActive

    /**
     * Handles an active object's request completion event.
     *
     * @since S60 v5.0
     */
    void RunL();

    /**
     * Implements cancellation of an outstanding request.
     *
     * @since S60 v5.0
     */
    void DoCancel();

    /**
     * Handles a leave occurring in the request completion event handler
     * RunL().
     *
     * @since S60 v5.0
     * @return Error code.
     */
    TInt RunError( TInt aError );

private:

    /**
     * C++ default constructor
     *
     * @since S60 v5.0
     * @return Instance of CHarvesterRequestQueue.
     */
    CHarvesterRequestQueue();

    /**
     * Symbian 2nd phase constructor can leave.
     *
     * @since S60 v5.0
     */
    void ConstructL();

private:

    /**
     * Array of active objects for each pending harvester request.
     */
    RPointerArray <CHarvesterRequestActive> iRequests;
    
    // number of currently active requests
    TInt iActiveRequests;
    
    TBool iShutdown;

};

#endif // HARVESTERREQUESTQUEUE_H
