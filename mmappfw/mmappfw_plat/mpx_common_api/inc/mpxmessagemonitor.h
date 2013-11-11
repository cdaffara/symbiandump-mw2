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
* Description:  Message monitor
*
*/



#ifndef CMPXMESSAGEMONITOR_H
#define CMPXMESSAGEMONITOR_H

#include <e32base.h>
#include <e32msgqueue.h>
#include "mpxmessageobserver.h"

class RMPXSession;

/**
*  Monitor messages in the message queue.
*  The class is always derived from and is never instantiated explicitly.
*
*  @lib mpxcommon.lib
*/
class CMPXMessageMonitor : public CActive
    {
public:
    /**
     * Two-phased constructor.
     * 
     * @since S60 3.2.3
     * @param aSession refrence to session object
     * @param aObserver observer
     */
    IMPORT_C static CMPXMessageMonitor* NewL(const RMPXSession& aSession,
                                             MMPXMessageObserver& aObserver);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C virtual ~CMPXMessageMonitor();
    
    /**
    * Get next message.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void GetNextMessage();
    
protected: // From base class
    
    /**
    * From CActive.
    * Handles request completion event.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void RunL();
    
    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void DoCancel();
    
    /**
    * From CActive.
    * Handles a leave occurring in the request completion event handler RunL().
    *
    * @since S60 3.2.3
    * @param aError error code
    */
    IMPORT_C TInt RunError(TInt aError);

    
protected: // New function
    
    /**
    * Wait for next message.
    *
    * @since S60 3.2.3
    */
    void WaitForNextMessage();


private:

     /**
     * C++ constructor.
     *
     * @since S60 3.2.3
     * @param aSession refrence to session object
     * @param aObserver observer
     */
    CMPXMessageMonitor(const RMPXSession& aSession,
                       MMPXMessageObserver& aObserver);

    /**
     * Second phase constructor.
     *
     * @since S60 3.2.3
     */
    void ConstructL();
    
private:
    const RMPXSession& iSession;
    MMPXMessageObserver& iObserver;
    TPckgBuf<TInt> iMsgDataPkg; // Message data package
    TPckgBuf<TInt> iErrorPkg; // Error code package
    };

#endif // CMPXMESSAGEMONITOR_H

// End of file
