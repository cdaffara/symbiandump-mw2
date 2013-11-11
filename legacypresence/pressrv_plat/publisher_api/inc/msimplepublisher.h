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
* Description:    SIMPLE Engine presence data publisher
*
*/




#ifndef M_simplepublisher_H
#define M_simplepublisher_H

#include <e32std.h>

// FORWARD DECLARATION
class MSimpleDocument;
class MSimpleConnection;
class MSimpleETagObserver;


/**
 *  MSimplePublisher
 *
 *  SIMPLE Engine presence data publisher
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimplePublisher
    {

public:

    /**
     * Destroy this entity
     *
     * @since S60 v3.2
     */
    virtual void Close() = 0;

    /**
     * Getter for the SIMPLE Engine connection.
     *
     * @since S60 3.2
     * @return the attribute name.
     */
    virtual const MSimpleConnection& Connection() = 0;

    /**
     * Start to publish data.
     * Leaves with KErrInUse if there is already active publication
     * in this MSimplePublisher entity.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE Engine.
     *
     * @since S60 3.2
     * @param aDocument data to be published
     * @param aRefresh whether refresh is made automatically
     * @return operation id
     */
    virtual TInt StartPublishL( MSimpleDocument& aDocument,
     TBool aRefresh ) = 0;         
     
    /**
     * Continue data publication
     * Leaves with KErrInUse if there is already active publication
     * in this MSimplePublisher entity.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE Engine.
     *
     * @since S60 3.2
     * @param aDocument data to be published
     * @param aRefresh whether refresh is made automatically
     * @param aETag SIP ETag header value to be used in SIP-If-Match header.
     * @return operation id
     */
    virtual TInt ContinuePublishL( MSimpleDocument& aDocument,
     TBool aRefresh, const TDesC8& aETag ) = 0;     

    /**
     * Update published data. This will overwrite the previous
     * presence data. StartPublishL() must be completed before
     * calling this method.
     * Remote URI is not modified but taken from an initial
     * StartPublishL document.
     *
     * @since S60 3.2
     * @param aDocument data to be published
     * @return operation id
     */
    virtual TInt ModifyPublishL( MSimpleDocument& aDocument ) = 0;

    /**
     * Stop publishing data.
     *
     * @since S60 3.2
     * @return operation id
     */
    virtual TInt StopPublishL() = 0;
    
    /**
     * Stop publication.
     *
     * @since S60 3.2
     * @param aETag SIP ETag header value to be used in SIP-If-Match header.     
     * @return operation id
     */
    virtual TInt StopPublishL( const TDesC8& aETag ) = 0;    

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
     * SIP ETag header value accessor.
     * Refer to RFC3903.     
     *
     * @since S60 3.2
     * @return Current ETag header value received from a network server.
     *         Zero length if not available. 
     */
    virtual TPtrC8 SIPETag() = 0;  
    
    /**
     * SIP ETag header value change observer setter.
     * Notice that this value may change after each modification or automatic refresh
     * of publication. Observer is called when the ETag value changes next time.
     * Refer to RFC3903.     
     *
     * @since S60 3.2
     * @param aObs ETag value observer. Can be NULL to stop notification.
     */
    virtual void SetSIPETagObserver( MSimpleETagObserver* aObs ) = 0;      

    };

#endif

// End of File
