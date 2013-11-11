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
* Description:    SIMPLE engine core.
*
*/




#ifndef CSimpleSIPConnection_H
#define CSimpleSIPConnection_H


// INCLUDES
#include <e32base.h>
#include <sip.h>
#include <sipobserver.h>
#include "simplecommon.h"
#include "simplesipconncallback.h"
#include "simplecommon.h"
#include "simplerefreshtimer.h"
#include "simplerequest.h"
#include "simplesettings.h"

// FORWARD DECLARATIONS
class CSimpleSipProfileObserver;
class CSIPClientTransaction;
class CSimpleRefreshTimer;
class CSIPMessageElements;
class CSIPSubscribeDialogAssoc;
class CSimpleSipConnectionObserver;

// CLASS DECLARATION



/**
 * CSimpleSipConnection
 *
 * @lib simplesiputils
 * @since S60 v3.2
 */

class CSimpleSipConnection : public CBase,
                             public MSIPObserver,
                             public MSimpleSipConnCallback
    {

public:

    /**
     * Subscription types
     */
    enum TSimpleSubsType
        {
        ESubsRegular,
        ESubsWinfo,
        ESubsList
        };

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSimpleSipConnection* NewL();

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CSimpleSipConnection();

    /**
     * Dispatch a client request
     *
     * @since S60 v3.2
     * @param aReq client request
     */
    IMPORT_C void DispatchReqL( MSimpleEngineRequest& aReq );

    /**
     * SIP Status accessor
     *
     * @since S60 v3.2     
     * @param aOpId operation id
     * @param aRetryAfter SIP header value, OUTPUT
     * @return SIP status, 0 if not found
     */
    IMPORT_C TUint SIPStatus( TInt aOpId, TInt& aRetryAfter );

    /**
     * SIP connection state accessor
     *
     * @since S60 v3.2
     * @return SIP state
     */
    IMPORT_C TSimpleSipState SIPState();
    
    /**
     * current SIP Identity accessor
     * @return SIP identity
     */
	IMPORT_C TPtrC8 CurrentSIPIdentityL(); 
	
	/**
	 * SIP Subscription-state accessor
     * @param aReq corresponding engine's request
     * @return subscription-state. 
	 */
    IMPORT_C MSimpleEngineRequest::TSimpleSipSubscriptionState SipSubscriptionState( 
        MSimpleEngineRequest& aReq );	   
    
    /**
     * Search the corresponding client request
     *
     * @since S60 v3.2      
     * @param aTrans SIP transaction
     * @return request or NULL if request not found
     */
    CSimpleRequest* GetRequest( CSIPClientTransaction& aTrans );

    /**
     * Search the corresponding client request
     *
     * @since S60 v3.2      
     * @param aDialog SIP dialog
     * @return initial SUBSCRIBE request or NULL if request not found
     */
    CSimpleRequest* GetdDialogRequest( const CSIPDialog& aDialog );

    /**
     * Handles publish response.
     * This reads SIP-ETAG and resets refresh timer.
     *
     * @since S60 v3.2      
     * @param aMesElems SIP message elements
     * @param aReq corresponding engine's request
     */
    void HandlePublishRespL(
        const CSIPMessageElements& aMesElems,
        CSimpleRequest* aReq );

    /**     
     * Time to refresh a request
     *
     * @since S60 v3.2 
     * @param aReq request     
     */
    void StartToRefreshL( CSimpleRequest& aReq );

    /**
     * Time to check expiration
     *
     * @since S60 v3.2 
     * @param aReq request     
     */
    void StartToCheckExpiryL( CSimpleRequest& aReq );

    /**
     * Handles server request in a dialog
     * This resets refresh timer when needed.
     *
     * @since S60 v3.2     
     * @param aMesElems SIP message elements
     * @param aReq corresponding engine's request
     * @param aMethod SIP method
     */
    void HandleDialogRequestL(
        const CSIPMessageElements& aMesElems,
        CSimpleRequest& aReq,
        const TDesC8& aMethod );

    /**
     * Retry publication, with a body when pending data exists.
     *
     * @since S60 v3.2 
     * @param aReq request       
     * @return error code
     */
    TInt DoSendPendingPublish( CSimpleRequest& aReq );    

// from base class MSIPObserver

    /**
     * From MSIPObserver.
     * A SIP request has been received from the network.
     * This function is called when the SIP request was received using such
     * an IAP, for which the application has not created a CSIPConnection
     * object.
     *
     * @since S60 v3.2      
     * @pre aTransaction != 0
     * @param aIapId The IapId from which the SIP request was received.
     * @param aTransaction contains local address, remote address of a SIP
     *        message, as well as optional SIP message method, headers and
     *        body. The ownership is transferred.
     */
    void IncomingRequest( TUint32 aIapId,
                          CSIPServerTransaction* aTransaction );

    /**
     * From MSIPObserver.
     * The received SIP request time-outed and it is invalid i.e. cannot be used
     * anymore.
     * This will be called if the user fails to create a SIP connection and
     * does not send an appropriate SIP response.
     *
     * @since S60 v3.2      
     * @param aTransaction The time-outed transaction.
     */
    void TimedOut( CSIPServerTransaction& aTransaction );

// from base class MSimpleSipConnCallback

    /**
     * From MSimpleSipConnCallback.
     * Connection state is changed
     *
     * @since S60 v3.2      
     * @param aState new SIP state
     */
    void ConnectionStateChange( 
        TSimpleSipState aState, TInt aSipError );

    /**
     * Handle received message
     *
     * @since S60 v5.0
     * @param aFrom message received from
     * @param aContent message content
     * @return error code
     */
    TInt HandleReceivedMessage( const TDesC8& aFrom,
        const TDesC8& aContent );

protected:

    /**
     * C++ default constructor.
     */
    CSimpleSipConnection();

private:

    void ConstructL();

    /**
     * Search the corresponding client request
     *
     * @since S60 v3.2       
     * @param aTrans SIP transaction
     * @return request or NULL if request not found
     */
    CSimpleRequest* GetCliRequest( MSimpleEngineRequest& aRequest );

    /**
     * Read static settings and register SIP profile.
     * If already registered then uses the current registeration.
     *
     * @since S60 v3.2       
     * @param aReq request
     * @leave KErrNotFound if SIP profile is not found
     */
    void RegisterL( MSimpleEngineRequest& aReg );

    /**
     * Start to listen to status events
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void ListenStatusL( MSimpleEngineRequest& aReq );

    /**
     * Start publish transaction
     *
     * @since S60 v3.2
     * @param aReq request            
     */
    void StartPublishL( MSimpleEngineRequest& aReq );

    /**
     * Request instant message receiving
     *
     * @since S60 v5.0
     * @param aReq request
     */
    void RequestInstantMessageReceivingL( MSimpleEngineRequest& aReq );

    /**
     * Send instant message
     *
     * @since S60 v5.0
     * @param aReq request
     */
    void SendInstantMessageL( MSimpleEngineRequest& aReq );

    /**
     * Update publish
     *
     * @since S60 v3.2 
     * @param aReq request           
     */
    void ModifyPublishL( MSimpleEngineRequest& aReq );

    /**
     * Stop publish
     *
     * @since S60 v3.2 
     * @param aReq request           
     */
    void StopPublishL( MSimpleEngineRequest& aReq );

    /**
     * Start subscription
     * if aReq expiry time is 0 then it means fetch presence once
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void SubscribeL( MSimpleEngineRequest& aReq );

    /**
     * Start list subscription
     * if aReq expiry time is 0 then it means fetch presence once
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void SubscribeListL( MSimpleEngineRequest& aReq );

    /**
     * Start WINFO subscription
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void SubscribeWinfoL( MSimpleEngineRequest& aReq );

    /**
     * Stop subscription
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void StopSubscribeL( MSimpleEngineRequest& aReq );

    /**
     * Delete all pending client requests
     *
     * @since S60 v3.2       
     */
    void DeleteRequests();

    /**
     * Destroy the request
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void DeleteRequest( MSimpleEngineRequest& aReq );

    /**
     * Destroy the request with a delay
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void DeleteRequestStart( MSimpleEngineRequest& aReq );

    /**
     * Complete all pending register client requests
     *
     * @since S60 v3.2  
     * @param aStatus status
     */
    void CompleteRegisterRequests( TInt aStatus );

    /**
     * Complete all open regular client requests
     *
     * @since S60 v3.2       
     * @param aStatus status
     */
    void CompleteRegularRequests( TInt aStatus );

    /**
     * Complete status event request
     *
     * @since S60 v3.2       
     */
    void CompleteStatusRequest();

    /**
     * Fill the headers and content for SIP PUBLISH request
     * and send the SIP request,
     *
     * @since S60 v3.2  
     * @param aRemoteURI remote request-URI  
     * @param aExpiry expires time  
     * @param aContent payload data
     * @param aSipIfMatch sip-if-match/ETag SIP header value                
     * @return SIP transaction
     */
    CSIPClientTransaction* DoPublishL(
        const TDesC8& aRemoteURI, TInt aExpiry,
        const TDesC8& aContent, const TDesC8& aSipIfMatch,
        const TDesC8& aContentType );

    /**
     * Send SIP SUBSCRIBE request
     *
     * @since S60 v3.2       
     * @param aRemoteURI remote request-URI
     * @param aExpiry expires time
     * @param aContent payload data
     * @param aSubsDialog subscription dialog, OUT
     * @param aRefesh ETrue if subscription is refreshed automatically
     * @param aType subscription use case
     * @param aAnonymous ETrue if anonymous subscription is wanted
     *        (should be EFalse always when aType is WINFO subscription)
     * @return SIP transaction
     */
    CSIPClientTransaction* DoSubscribeL(
        const TDesC8& aRemoteURI,
        TInt aExpiry,
        const TDesC8& aContent,
        CSIPSubscribeDialogAssoc*& aSubsDialog,
        TBool aRefresh,
        TSimpleSubsType aType,
        TBool aAnonymous );

    /**
     * Read static settings and register default SIP profile.
     * If already registered then uses the current registeration.
     *
     * @since S60 v3.2       
     * @param aReq request
     * @leave KErrNotFound if SIP profile is not found
     */
    void RegisterDefaultL( MSimpleEngineRequest& aReg );

    /**
     * Read static settings and register a specified SIP profile.
     * If already registered then uses the current registeration.
     *
     * @since S60 v3.2       
     * @param aReq request
     * @leave KErrNotFound if SIP profile is not found
     */
    void RegisterAnyL( MSimpleEngineRequest& aReg );

    /**
     * Set SIP state
     *
     * @since S60 v3.2       
     * @param aState SIP state
     */
    void SetSipState( TSimpleSipState aState );

    /**
     * Converts ascii reason string into enum and stores into aR entity
     *
     * @since S60 v3.2       
     * @param aReason reason text
     * @param aR request entity
     */
    void DoSetResponseReason(
        const TDesC8& aReason, CSimpleRequest& aR );

    /**
     * Converts ascii reason string into enum and stores into aR entity
     *
     * @since S60 v3.2       
     * @param aReason reason
     * @param aR client request entity
     */
    void DoSetResponseReason2(
        TInt aReason, MSimpleEngineRequest& aR );

    /**
     * Is the reson code permanent reason
     *
     * @since S60 v3.2       
     * @param aReason error code
     * @return TRUE if permanent resason and should not be retried.
     */
    TBool IsPermanentReason( TInt aReason );

    /**
     * Increase iCurrentNbrSubs
     *
     * @since S60 v3.2       
     */
    void IncreaseNbrSubs();

    /**
     * Decrease iCurrentNbrSubs
     *
     * @since S60 v3.2       
     */
    void DecreaseNbrSubs();

    /**
     * Refresh or retry to publish
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void DoRefreshPublishL( CSimpleRequest& aReq );

    /**
     * Retry to subscribe
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void DoRefreshSubscribe( CSimpleRequest& aReq );

    /**
     * Create a dialog
     *
     * @since S60 v3.2       
     * @param aReq request
     */
    void DoCreateDialogL( CSimpleRequest& aReq );

    /**
     * Handle pending operations after network resume
     *
     * @since S60 v3.2       
     */
    void HandlePendings( );
    
    /**
     * Update iSipState. 
     * Recognize if both SIP connection and SIP Profile are active.
     * @param aState SIP state     
     */
    void RecognizeSipState( TSimpleSipState aState ); 
    
    /**
     * Handle received message
     *
     * @since S60 v5.0
     * @param aFrom message received from
     * @param aContent message content
     * @param aRequest SIMPLE request
     * @return error code
     */
    void DoHandleReceivedMessageL( const TDesC8& aFrom,
        const TDesC8& aContent, CSimpleRequest& aRequest );

private:    // Data

    /**
     * SIP Engine connection
     * Own
     */
    CSIP* iSip;

    /**
     * SIP connection
     * Own
     */
    CSIPConnection* iSipConnection;

    /**
     * SIP connection observer
     * Own
     */
    CSimpleSipConnectionObserver* iConnectionObserver;

    /**
     * Engine SIP state. 
     * Active only if both SIP Connection and SIP Profile are active.
     */
    TSimpleSipState iSipState;

    /**
     * SIP profile observer
     * Own
     */
    CSimpleSipProfileObserver* iProfileObserver;

    /**
     * Queue for client requests
     */
    TDblQue<CSimpleRequest> iRequestList;

    /**
     * Engine settings
     */
    TSimpleSettings iSettings;

    /**
     * File system
     */
    RFs iFs;

    /**
     * Current number of subscriptions
     */
    TUint iCurrentNbrSubs;

#ifdef _DEBUG
    friend class T_CSimpleSipConnection;
#endif
    };

#endif

// End of File
