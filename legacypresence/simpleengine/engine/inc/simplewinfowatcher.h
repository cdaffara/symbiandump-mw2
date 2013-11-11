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
* Description:   SIMPLE Engine winfo watcher
*
*/




#ifndef simplewinfowatcher_H
#define simplewinfowatcher_H

#include <e32std.h>
#include "simplecommon.h"
#include "simpleclient.h"
#include "msimpleconnection.h"
#include "msimplewinfowatcher.h"
#include "msimpleowncallback.h"

// FORWARD DECLARATION
class CSimpleEngineRequest;
class MSimpleWinfoObserver;
class MSimpleConnectionObserver;

/**
 *  CSimpleWinfoWatcher
 *
 *  SIMPLE Engine core.
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class CSimpleWinfoWatcher : public CSimpleClient, public MSimpleWinfoWatcher
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @param aConn connection observer
     * @param aObserver callback observer
     * @return CSimpleWinfoWatcher
     */
    static CSimpleWinfoWatcher* NewL(
        MSimpleConnection& aConn,
        MSimpleWinfoObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CSimpleWinfoWatcher();

// from base class MSimpleWinfoWatcher

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * Getter for the SIMPLE engine connection.
     * @since S60 3.2
     * @return the connection
     */
    const MSimpleConnection& Connection();

    /**
     * Callback for WINFO termination
     * @since S60 3.2
     * @param aFilter filter data, may be null and then you'll get all
     * the changes in WINFO.
     */
    TInt SubscribeWatcherListL( MSimpleFilterDocument* aFilter );

    /**
     * Unsubscribe.
     * Leaves with KErrNotFound if there is no active subscription.
     * @since S60 3.2
     * @return operation id
     */
    TInt UnsubscribeL( );

    /**
     * SIP Status accessor
     * Get the status of the last completed request.
     * @since S60 3.2     
     * @return SIP status, 0 if not available
     */
    TUint SIPStatus();

    /**
     * SIP Retry-after accessor
     * Get the header of the last completed request.
     * @return retry-after value in seconds, 0 if not available
     */
    TUint SIPRetryAfter();

// from base class MSimpleOwnCallback

    /**
     * Complete the open client request
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus result status
     */
    void Complete( TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq );

private:

    /**
     * Two-phase constructor
     */
    void ConstructL( );

    /**
     * constructor
     */
    CSimpleWinfoWatcher(
        MSimpleConnection& aConn,
        MSimpleWinfoObserver& aObserver );

    /**
     * Stream document into request
     * @since S60 3.2     
     * @param aReq request
     * @param aFilter document
     */
    void StreamDocumentL(
        CSimpleEngineRequest& aReq,
        MSimpleFilterDocument& aFilter );

    /**
     * New Request from server
     * @since S60 3.2
     * @return request type
     */
    void NewRequestL( MSimpleEngineRequest& aReq );
    
    /**
     * Complete client request
     * @param aOpId operation id
     * @param aStatus result status     
     * @return error when observer has deleted ourself in callback
     */
    TInt DoCallReqComplete(
        TInt aOpId, TInt aStatus );     


private:    // data

    /**
     * Client API observer for events
     */
    MSimpleWinfoObserver&      iObserver;

    /**
     * Subscription id
     */
    TInt iSubsId;

    /**
     * Response counter in error situations, or state. reply + notify
     */
    TInt iResCount;

    /**
     * Whether completion callback method is called
     */
    TBool iComplete;

    /**
     * Data buffer for transmission
     * OWN.
     */
    CBufFlat* iBuffer;

    };

#endif      // simplewinfowatcher_H

// End of File
