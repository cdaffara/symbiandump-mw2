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
* Description:    SIMPLE Engine watcher info subscriber
*
*/




#ifndef M_simplewinfowatcher_H
#define M_simplewinfowatcher_H

#include <e32std.h>

// FORWARD DECLARATION
class MSimpleFilterDocument;
class MSimpleWinfo;

/**
 *  MSimpleWinfoWatcher
 *
 *  SIMPLE Engine watcher info subscriber
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimpleWinfoWatcher
    {

public:

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Getter for the SIMPLE engine connection.
     *
     * @since S60 3.2
     * @return the connection.
     */
    virtual const MSimpleConnection& Connection() = 0;

    /**
     * Subscribe watcher info.
     *
     * @since S60 3.2
     * @param aFilter filter data, may be null and then you'll get all
     * the changes in WINFO.
     * @return operation id     
     */
    virtual TInt SubscribeWatcherListL( MSimpleFilterDocument* aFilter ) = 0;

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
    };

#endif

// End of File
