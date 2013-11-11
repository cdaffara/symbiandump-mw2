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
* Description:   SIMPLE Engine watcher
*
*/




#ifndef simplewatcher_H
#define simplewatcher_H

#include <e32std.h>
#include "simplecommon.h"
#include "simpleclient.h"
#include "msimpleconnection.h"
#include "msimplewatcher.h"
#include "msimpleowncallback.h"


// FORWARD DECLARATION
class CSimpleEngineRequest;
class MSimpleWatcherObserver;
class MSimpleConnectionObserver;

/**
 *  CSimpleWatcher
 *
 *  SIMPLE Engine core.
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */
class CSimpleWatcher : public CSimpleClient, public MSimpleWatcher
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @param aConn connection
     * @param aObserver callback observer
     * @return CSimpleWatcher
     */
    static CSimpleWatcher* NewL(
        MSimpleConnection& aConn,
        MSimpleWatcherObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CSimpleWatcher();

// from base class MSimpleWatcher

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
     * Subscribe presence entity.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE engine.
     * Leaves with KErrInUse if there is already active subscription.
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aRefresh whether refresh is made automatically
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    TInt SubscribeL(
        const TDesC8& aURI,
        MSimpleFilterDocument* aFilter,
        TBool aRefresh,
        TBool aAnonymous );

    /**
     * Subscribe presence list.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE engine.
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aRefresh whether refresh is made automatically
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    TInt SubscribeListL(
        const TDesC8& aURI,
        MSimpleFilterDocument* aFilter,
        TBool aRefresh,
        TBool aAnonymous );

    /**
     * Get presence data.3
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    TInt GetPresenceL( const TDesC8& aURI, MSimpleFilterDocument* aFilter, TBool aAnonymous );

    /**
     * Unsubscribe.
     * Leaves with KErrNotFound if there is no active subscription.
     * @since S60 3.2
     * @return operation id
     */
    TInt UnsubscribeL();    

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
     * @since S60 3.2     
     * @return retry-after value in seconds, 0 if not available
     */
    TUint SIPRetryAfter();  
        
    /**
     * SIP Subscription-state header value accessor.
     * Get the SIP Subscription-state header value of the last NOTIFY message or
     * ESimpleStateTerminated when the subscription is terminated by a client.
     * Refer to RFC3265     
     *
     * @since S60 3.2
     * @return subscription state
     */
    TSimpleSipSubscriptionState SipSubscriptionState();

// from base class MSimpleOwnCallback

    /**
     * Complete the open client request
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus result status
     * @param aReq request
     */
    void Complete( TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq );


private:


    /**
     * Two-phase constructor
     */
    void ConstructL( );


    /**
     * constructor
     * @param aConn connection
     * @param aObserver callback observer
     */
    CSimpleWatcher(
        MSimpleConnection& aConn,
        MSimpleWatcherObserver& aObserver );

    /**
     * Stream document into request
     * @param aReq request
     * @param aFilter document
     */
    void StreamDocumentL(
        CSimpleEngineRequest& aReq,
        MSimpleFilterDocument& aFilter );

    /**
     * New Request from server
     * @since S60 3.2
     * @param aReq request
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
    MSimpleWatcherObserver&      iObserver;

    /**
     * Subscription id
     */
    TInt iSubsId;

    /**
     * Response counter in error situations, or state. reply + notify
     */
    TInt iResCount;

    /**
     * Whether WatcherReqCompleteL callback method is called
     */
    TBool iComplete;

    /**
     * Data buffer for transmission
     * OWN.
     */
    CBufFlat* iBuffer;

    };

#endif      // simplewatcher_H

// End of File
