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
* Description:    SIMPLE Engine presence watcher
*
*/




#ifndef M_simplewatcher_H
#define M_simplewatcher_H

#include <e32std.h>

// FORWARD DECLARATION
class MSimpleConnection;
class MSimpleFilterDocument;

/**
 *  MSimpleWatcher
 *
 *  SIMPLE Engine presence watcher
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimpleWatcher
    {

public:

    /** SIP subscription-state */    
    enum TSimpleSipSubscriptionState
        {
        // subscription not complete
        ESimpleStateNone=0,
        // subscription is in pending state
        ESimpleStatePending,
        // subscription is in active state        
        ESimpleStateActive,
        // subscription is terminated, either by server or by client.        
        ESimpleStateTerminated
        };

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Getter for the SIMPLE Engine connection.
     *
     * @since S60 3.2
     * @return connection
     */
    virtual const MSimpleConnection& Connection() = 0;

    /**
     * Subscribe presence entity.
     * Expiry time is set based on SIMPLE settings.
     * Leaves with KErrInUse if there is already active subscription
     * in this MSimpleWatcher entity.
     *
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aRefresh whether refresh is made automatically
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    virtual TInt SubscribeL(
        const TDesC8& aURI,
        MSimpleFilterDocument* aFilter,
        TBool aRefresh,
        TBool aAnonymous ) = 0;

    /**
     * Subscribe presence list.
     * Expiry time is set based on SIMPLE settings.
     * Leaves with KErrInUse if there is already active subscription
     * in this MSimpleWatcher entity.
     *
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aRefresh whether refresh is made automatically
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    virtual TInt SubscribeListL(
        const TDesC8& aURI,
        MSimpleFilterDocument* aFilter,
        TBool aRefresh,
        TBool aAnonymous  ) = 0;

    /**
     * Get presence data
     * Leaves with KErrInUse if there is already active subscription
     * in this MSimpleWatcher entity.
     *
     * @since S60 3.2
     * @param aURI URI of entity to be subscribed
     * @param aFilter filter data, may be NULL.
     * @param aAnonymous whether anonymous subscription is wanted
     * @return operation id
     */
    virtual TInt GetPresenceL(
        const TDesC8& aURI,
        MSimpleFilterDocument* aFilter,
        TBool aAnonymous ) = 0;

    /**
     * Unsubscribe.
     * Leaves with KErrNotFound if there is no active subscription.
     *
     * @since S60 3.2
     * @return operation id
     */
    virtual TInt UnsubscribeL( ) = 0;

    /**
     * SIP Status code accessor.
     * Get the status of the last completed request.
     * Refer to RFC3261, RFC3265, RFC3903.
     *
     * @since S60 3.2
     * @return SIP status, 0 if not available
     */
    virtual TUint SIPStatus() = 0;

    /**
     * SIP Retry-after header value accessor.
     * Get the SIP retry-fater header value of the last completed request.
     * Refer to RFC3261, RFC3265, RFC3903.     
     *
     * @since S60 3.2
     * @return retry-after value in seconds, 0 if not available
     */
    virtual TUint SIPRetryAfter() = 0;
    
    /**
     * SIP Subscription-state header value accessor.
     * Get the SIP Subscription-state header value of the last NOTIFY message or
     * ESimpleStateTerminated when the subscription is terminated by a client.
     * Refer to RFC3265     
     *
     * @since S60 3.2
     * @return subscription state
     */
    virtual TSimpleSipSubscriptionState SipSubscriptionState() = 0;    

    };

#endif

// End of File
