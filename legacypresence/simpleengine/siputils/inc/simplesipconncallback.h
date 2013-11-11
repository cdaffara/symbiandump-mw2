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
* Description:    Callback methods used by SIP stack callback implementations
*
*/




#ifndef MSimpleSipConnCallback_H
#define MSimpleSipConnCallback_H

//  INCLUDES
#include <e32base.h>
#include "simplecommon.h"

// FORWARD DECLARATIONS

class CSIPConnection;
class CSIPClientTransaction;
class CSIPDialogAssocBase;
class CSIPMessageElements;
class CSimpleRequest;
class CSIPDialog;

/**
 * Callback methods used by SIP stack callback implementations.
 *
 * @lib simplesiputils
 * @since S60 3.2
 */
class MSimpleSipConnCallback
    {

public:

    /**
     * Connection state is changed
     *
     * @since S60 3.2     
     * @param aState new SIP state
     */
    virtual void ConnectionStateChange( 
        TSimpleSipState aState, TInt aSipError) = 0;

    /**
     * Time to refresh a request
     *
     * @since S60 3.2       
     * @param aReq corresponding engine's request
     */
    virtual void StartToRefreshL( CSimpleRequest& aReq ) = 0;

    /**
     * Time to check expiration
     * @param aReq corresponding engine's request     
     */
    virtual void StartToCheckExpiryL( CSimpleRequest& aReq ) = 0;

    /**
     * Search the corresponding client request
     *
     * @since S60 3.2       
     * @param aTrans SIP transaction
     * @return request or NULL if request not found
     */
    virtual CSimpleRequest* GetRequest( CSIPClientTransaction& aTrans ) = 0;

    /**
     * Search the corresponding client request
     *
     * @since S60 3.2       
     * @param aDialog SIP dialog
     * @return initial SUBSCRIBE request or NULL if request not found
     */
    virtual CSimpleRequest* GetdDialogRequest( const CSIPDialog& aDialog ) = 0;

    /**
     * Handles publish response.
     * This reads SIP-ETAG and resets refresh timer.
     *
     * @since S60 3.2       
     * @param aMesElems SIP message elements
     * @param aReq corresponding engine's request
     */
    virtual void HandlePublishRespL( const CSIPMessageElements& aMesElems, CSimpleRequest* aReq ) = 0;

    /**
     * Handles server request in a dialog
     * This resets refresh timer when needed.
     *
     * @since S60 3.2       
     * @param aMesElems SIP message elements
     * @param aReq corresponding engine's request
     * @param aMethod SIP method
     */
    virtual void HandleDialogRequestL(
        const CSIPMessageElements& aMesElems,
        CSimpleRequest& aReq,
        const TDesC8& aMethod  ) = 0;

    /**
     * Retry publication, with a body when pending data exists.
     *
     * @since S60 3.2       
     * @param aReq corresponding engine's request
     * @return error code
     */
    virtual TInt DoSendPendingPublish( CSimpleRequest& aReq ) = 0;

    /**
     * Handle received message
     *
     * @since S60 5.0
     * @param aFrom message received from
     * @param aContent message content
     * @return error code
     */
    virtual TInt HandleReceivedMessage( const TDesC8& aFrom,
        const TDesC8& aContent ) = 0;

    };

#endif
