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
* Description:   SIMPLE Engine publisher
*
*/




#ifndef simplepublisher_H
#define simplepublisher_H

#include <e32std.h>
#include "simplecommon.h"
#include "simpleclient.h"
#include "msimpleconnection.h"
#include "msimplepublisher.h"
#include "msimpleowncallback.h"
#include "msimpleenginerequest.h"

// FORWARD DECLARATION
class CSimpleEngineRequest;
class MSimplePublishObserver;
class MSimpleConnectionObserver;
class MSimpleETagObserver;

/**
 *  CSimplePublisher
 *
 *  SIMPLE Engine core.
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class CSimplePublisher : public CSimpleClient, public MSimplePublisher
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 3.2
     * @param aConn connection
     * @param aObserver callback observer
     * @return CSimplePublisher
     */
    static CSimplePublisher* NewL(
        MSimpleConnection& aConn,
        MSimplePublishObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CSimplePublisher();

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    void Close();

// from base class MSimplePublisher

    /**
     * Getter for the SIMPLE engine connection.
     * @since S60 3.2
     * @return the attribute name.
     */
    const MSimpleConnection& Connection();

    /**
     * Start to publish data.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE engine.
     * @since S60 3.2
     * @param aDocument data to be published
     * @param aRefresh where refresh is made automatically
     * @return operation id
     */
    TInt StartPublishL( MSimpleDocument& aDocument,
        TBool aRefresh);
        
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
    TInt ContinuePublishL( MSimpleDocument& aDocument,
        TBool aRefresh, const TDesC8& aETag );          

    /**
     * Update published data.
     * @since S60 3.2
     * @param aDocument data to be published
     * @return operation id
     */
    TInt ModifyPublishL( MSimpleDocument& aDocument );

    /**
     * Stop publishing data.
     * @since S60 3.2
     * @return operation id
     */
    TInt StopPublishL();
    
    /**
     * Stop publication.
     *
     * @since S60 3.2
     * @param aETag SIP ETag header value to be used in SIP-If-Match header.     
     * @return operation id
     */
    TInt StopPublishL( const TDesC8& aETag );    

    /**
     * SIP Status accessor
     * Get the status of the last completed request.
     * @since S60 3.2     
     * @return SIP status, 0 if not available
     */
    TUint SIPStatus( );

    /**
     * SIP Retry-after accessor
     * Get the header of the last completed request.
     * @since S60 3.2     
     * @return retry-after value in seconds, 0 if not available
     */
    TUint SIPRetryAfter( );
    
    /**
     * SIP ETag header value accessor.
     * Notice that this value changes after each modification or automatic refresh
     * of publication. 
     * Refer to RFC3903.     
     *
     * @since S60 3.2
     * @return ETag header value received from a network server.. 
     *         Zero length if not available. 
     */
    TPtrC8 SIPETag();
    
    /**
     * SIP ETag header value change observer setter.    
     * Notice that this value changes after each modification or automatic refresh
     * of publication. Observer is called when the ETag value changes next time.
     * Refer to RFC3903.     
     *
     * @since S60 3.2
     * @param aObs ETag value observer. Can be NULL to stop notification.
     */
    void SetSIPETagObserver( MSimpleETagObserver* aObs );


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
    CSimplePublisher(
        MSimpleConnection& aConn,
        MSimplePublishObserver& aObserver );

    /**
     * Stream document into request
     *
     * @since S60 3.2     
     * @param aReq request
     * @param aDocument document
     */
    void StreamDocumentL(
        CSimpleEngineRequest& aReq,
        MSimpleDocument& aDocument );
    
      
    /**
     * Publish data.
     * Leaves with KErrInUse if there is already active publication
     * in this MSimplePublisher entity.
     * Expiry time is set based on SIMPLE settings.
     * Refreshing is made by SIMPLE Engine.
     *
     * @since S60 3.2
     * @param aDocument data to be published
     * @param aRefresh whether refresh is made automatically
     * @param aETag SIP ETag header value to be used in SIP-If-Match header. 
     * Zero length is ignored.
     * @return operation id
     */        
    TInt DoStartPublishL( MSimpleDocument& aDocument,
        TBool aRefresh, const TDesC8& aETag );


private:    // data

    /**
     * Publish state
     */
    TBool                       iPublished;

    /**
     * Client API observer for events
     */
    MSimplePublishObserver&      iObserver;

    /**
     * Data buffer for transmission
     * OWN.
     */
    CBufFlat* iBuffer;
    
    /**
     * current SIP ETag.
     * Own.
     */
    HBufC8* iETag; 
    
    /**
     * ETag change callback observer
     * Not own.
     **/
    MSimpleETagObserver* iETagObserver;    

    };

#endif      // simplepublisher_H

// End of File
