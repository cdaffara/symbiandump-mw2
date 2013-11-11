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
* Description:    SIMPLE engine request
*
*/




#ifndef CSimpleRequest_H
#define CSimpleRequest_H


// INCLUDES
#include <e32base.h>
#include "simplecommon.h"
#include "simplesipconncallback.h"
#include "simplecommon.h"
#include "msimpleenginerequest.h"
#include "simplerefreshtimer.h"

// FORWARD DECLARATIONS
class CSimpleSipProfileObserver;
class CSIPClientTransaction;
class CSIPSubscribeDialogAssoc;
class CSimpleRefreshTimer;
class CSIPRefresh;

// CLASS DECLARATION

/**
 * Client request buffer element
 *
 * @lib siputils
 * @since S60 3.2
 */
class CSimpleRequest : public CBase
{
public:

/** request type */
enum TSimpleSipReqType
    {
    EReqRegister=1,
    EReqPublish,
    EReqSubscribe,
    EReqSubscribeList,
    EReqSubscribeWinfo,
    EReqListenEvents,
    EReqSendIM,
    EReqReceiveIM
    };

/** request state */
enum TSimpleReqState
    {
    ESimpleInit=1,
    ESimplePendingInit,
    ESimpleRunningInit,
    ESimpleActive,
    ESimpleComplete,
    ESimpleFailed,
    ESimpleStopping,
    ESimplePending,
    ESimpleReTry,
    ESimpleRunning,
    ESimpleActiveSubs,
    ESimpleDialogReCre,
    ESimpleDeleting
    };
   
/** pending state substate */    
enum TSimplePendingSubState
    {
    ENoPending=0,
    EPendingModify,
    EPendingRefresh,
    EPendingModifyAndRefresh
    }; 
    

    /**
     * Contructor
     * @param aEngine engine api callback implementation
     * @param aReq client request
     * @param aType request type
     * @param aExpires SIP expiry time of the request 
     */
    static CSimpleRequest* NewL(
        MSimpleSipConnCallback& aEngine,
        MSimpleEngineRequest& aReq,
        TSimpleSipReqType aType,
        TUint aExpires );

    /**
     * Destructor
     */
    void Destroy();

    /**
     * Destructor
     */
    void DestroyStart();
    
    inline static TInt LinkOffset();       

    /**
     * Client request accessor
     *
     * @since S60 3.2
     * @return client request
     */
    MSimpleEngineRequest& Request();

    /**
     * Client SIP transaction accessor
     *
     * @since S60 3.2     
     * @return SIP transaction, may be NULL.
     */
    CSIPClientTransaction* Transaction();

    /**
     * Client SIP transaction setter
     *
     * @since S60 3.2     
     * @param aTrans SIP transaction
     */
    void SetTransaction( CSIPClientTransaction* aTrans );

    /**
     * CSIPRefresh entity accessor
     *
     * @since S60 3.2     
     * @return CSIPRefresh entity
     */
    CSIPRefresh* Refresh();

    /**
     * If the request matches with the transaction
     *
     * @since S60 3.2     
     * @param aTrans transaction
     * @return ETrue if matches
     */
    TBool Match( CSIPClientTransaction& aTrans );

    /**
     * If the request matches with the transaction
     *
     * @since S60 3.2     
     * @param aType transaction type
     * @return ETrue if matches
     */
    TBool Match( TSimpleSipReqType aType ) const;

    /**
     * If the request matches with the transaction
     *
     * @since S60 3.2     
     * @param aDialog dialog
     * @return ETrue if matches
     */
    TBool Match( const CSIPDialog& aDialog ) const;

    /**
     * If the request matches with client request
     *
     * @since S60 3.2     
     * @param aReq client request
     * @return ETrue if matches
     */
    TBool Match( MSimpleEngineRequest& aReq ) const;

    /**
     * Complete the pending request
     *
     * @since S60 3.2     
     * @param aStatus response status
     */
    void Complete( TInt aStatus );

    /**
     * Complete event request
     *
     * @since S60 3.2
     */
    void CompleteEvent( );

    /**
     * Set refresh time for the SIP request
     *
     * @since S60 3.2   
     * @param aTime time in seconds       
     */
    void SetRefreshTime( TUint aTime );

    /**
     * Refresh time accessor
     *
     * @since S60 3.2     
     * @return refresh time in seconds
     */
    TUint RefreshTime () const;

    /**
     * Start refresh timer for this request based
     * on iRefreshTime.
     *
     * @since S60 3.2     
     * @param aTime special time, if iRefreshTime not used
     */
    void StartRefreshTimer( TUint aTime = 0 );

    /**
     * Stop refresh timer
     *
     * @since S60 3.2     
     */
    void StopRefreshTimer();

    /**
     * Start expiry timer for this request
     *
     * @since S60 3.2     
     * @param aTime time
     */
    void StartExpiryTimer( TUint aTime );

    /**
     * Stop expiry timer
     *
     * @since S60 3.2     
     */
    void StopExpiryTimer();

    /**
     * Counts retry time period
     *
     * @since S60 3.2     
     * @return time
     */
    TUint RetryTime();

    /**
     * Counts retry expiry time period
     *
     * @since S60 3.2     
     * @return time
     */
    TInt RetryExpiryTime();

    /**
     * Set SIP-ETag value
     *
     * @since S60 3.2     
     * @param aTag tag, OWNERSHIP IS TRANSFERRED.
     */
    void SetETag( HBufC8* aTag );

    /**
     * get SIP-ETag value
     *
     * @since S60 3.2     
     * @return SIP-ETag value.
     */
    TPtrC8 ETag() const;

    /**
     * Status getter
     *
     * @since S60 3.2     
     * @return status
     */
    TUint Status() const;

    /**
     * Status setter
     *
     * @since S60 3.2     
     * @param aVal status value
     */
    void SetStatus( TUint aVal );

    /**
     * Dialog setter
     *
     * @since S60 3.2     
     * @param aDialog dialog
     */
    void SetDialog( CSIPSubscribeDialogAssoc* aDialog );

    /**
     * Dialog getter
     *
     * @since S60 3.2     
     * @return dialog
     */
    CSIPSubscribeDialogAssoc* Dialog( );

    /**
     * State accessor
     *
     * @since S60 3.2     
     * @return state
     */
    TSimpleReqState ReqState() const;

    /**
     * State setter
     *
     * @since S60 3.2     
     * @param aState state
     */
    void SetReqState( TSimpleReqState aState );

    /**
     * SOURCE-THROTTLE-PUBLISH time setter
     * @param expiry time in seconds
     */
    void SetThrottleTime( TUint aSeconds );

    /**
     * SOURCE-THROTTLE-PUBLISH time getter
     *
     * @since S60 3.2     
     * @return expiry time
     */
    TTime ThrottleTime( );

    /**
     * Retry-After SIP header parameter value accessor
     *
     * @since S60 3.2     
     * @return Retry-After SIP header parameter, 0 if not present
     */
    TUint RetryAfter();

    /**
     * Retry-After SIP header parameter value setter
     *
     * @since S60 3.2     
     * @param aVal Retry-After SIP header parameter, 0 if not present
     */
    void SetRetryAfter( TUint aVal );

    /**
     * Original error reason setter
     *
     * @since S60 3.2     
     * @param aVal error code
     */
    void SetReason( TInt aVal );

    /**
     * Original error reason getter
     *
     * @since S60 3.2     
     * @param aVal error code
     */
    TInt Reason( );

    /**
     * Error notify counter increse
     *
     * @since S60 3.2     
     */
    void PlusErrCount( );

    /**
     * Error notify reset
     *
     * @since S60 3.2     
     */
    void ResetErrCount( );

    /**
     * Error notify getter
     *
     * @since S60 3.2     
     */
    TInt ErrCount( );

    /**
     * Pending accessor
     *
     * @since S60 3.2     
     * return pending substate
     */
    TSimplePendingSubState PendingState();

    /**
     * Pending state setter
     *
     * @since S60 3.2     
     * @param aVal state
     */
    void SetPendingState( TSimplePendingSubState aVal );
    
    /**
     * Add new Pending state to the current state
     *
     * @since S60 3.2     
     * @param aVal state
     */
    void AddPendingState( TSimplePendingSubState aVal );    

    /**
     * Access iData
     *
     * @since S60 3.2     
     * @return data
     */
    TPtrC8 Data() const;

    /**
     * Set iData
     *
     * @since S60 3.2     
     * @param aData data
     */
    void SetDataL( const TDesC8& aData );
    
    /**
     * Access iRecipientId
     *
     * @since S60 5.0
     * @return recipient ID
     */
    TPtrC8 RecipientL() const;

    /**
     * Set iRecipientId
     *
     * @since S60 5.0
     * @param aRecipientId recipient ID
     */
    void SetRecipientL( const TDesC8& aRecipientId );

    /**
     * Accesor for iGivenETag
     * 
     * @since S60 3.2       
     * @return true if given ETag
     */
    TBool GivenETag() const; 
     
    /**
     * Setter for iGivenETag
     * 
     * @since S60 3.2       
     * @param aVal true if given ETag
     */  
    void SetGivenETag( TBool aVal );         
    
    /**
     * Accesor for request MIME content type
     * 
     * @since S60 3.2       
     * @return content type
     */
    TPtrC8 RequestContentType();

    /**
     * Setter for request MIME content type
     *
     * @since S60 3.2       
     * @param aData content type
     */     
     
    void SetRequestContentTypeL( const TDesC8& aData );
    
    /**
     * Get SIP Subscription-State.
     * @return Subscription-State
     */
    MSimpleEngineRequest::TSimpleSipSubscriptionState SipSubscriptionState();
    

private:

    /**
     * contructor
     */
    CSimpleRequest( MSimpleSipConnCallback& aEngine,
                    MSimpleEngineRequest& aReq,
                    TSimpleSipReqType aType,
                    TUint aExpires );

    /**
     * contructor
     */
    void ConstructL();

    /**
     * Keep destructor private since it's easier to
     * find Destroy() methods in the code when
     * debugging.
     *
     * @since S60 3.2     
     */
    virtual ~CSimpleRequest();

    /**
     * Handle state machine of subscription
     *
     * @since S60 3.2     
     * @param aStatus status
     */
    void DoCompleteSubscription( TInt aStatus );

    /**
     * Handle notification of subscription and update state machine if terminated.
     *
     * @since S60 3.2     
     * @param aStatus status
     */
    void DoCompleteNotification( TInt aStatus );

    /**
     * Handle state machine of publication
     *
     * @since S60 3.2     
     * @param aStatus status
     */
    void DoCompletePublication( TInt aStatus );


private: // data

    TDblQueLink iLink;
    
    /**
     * Client callback observer
     */
    MSimpleSipConnCallback& iEngine;

    /**
     * Client request
     */
    MSimpleEngineRequest& iReq;

    /**
     * SIP client transaction
     * Own,
     */
    CSIPClientTransaction* iTrans;

    /**
     * SIP refresher
     * Own.
     */
    CSIPRefresh* iSipRefresh;

    /**
     * Request type
     */
    TSimpleSipReqType iType;

    /**
     * Current Refresh period
     */
    TUint iRefreshTime;

    /**
     * Request expiry time in IF
     */
    TUint iExpirationTime;

    /**
     * Refresh timer.
     * Own.
     */
    CSimpleRefreshTimer* iRefreshTimer;

    /**
     * Refresh timer.
     * Own.
     */
    CSimpleExpiryTimer* iExpiryTimer;

    /**
     * ETag.
     * Own.
     */
    HBufC8* iETag;

    /**
     * Subscribe dialog
     * Own.
     */
    CSIPSubscribeDialogAssoc* iDialog;

    /**
     * SIP status (response)
     */
    TUint iStatus;

    /**
     * Current request state
     */
    TSimpleReqState iState;

    /**
     * The expiry time of SOURCE-THROTTLE-PUBLISH
     */
    TTime iThrottleTime;

    /**
     * SIP header retry-after parameter
     */
    TUint iRetryAfter;

    /**
     * Reason code
     */
    TInt iReason;

    /**
     * Error notify counter
     */
    TInt iErrNotify;

    /**
     * Expires
     */
    TUint iExpires;

    /**
     * Pending request state
     */
    TSimplePendingSubState iPendingState;

    /**
     * Request data
     * Own.
     */
    HBufC8* iData;
    
    /**
     * IM message recipient ID
     * Own.
     */
    HBufC8* iRecipientId;

    /**
     * Request Content type
     * Own.
     */
    HBufC8* iRequestContentType; 
    
    /**
     * If publish stoppig uses given ETag
     */
    TBool iGivenETag;
    
    /**
     * SIP Subscription-State
     */
    MSimpleEngineRequest::TSimpleSipSubscriptionState iSubscriptionState;
    
    /**
     * response counter for subscription stop (unsubscribe)
     */
    TInt iRespCount;    

#ifdef _DEBUG
    friend class T_CSimpleRequest;
    friend class T_CSimpleSipConnectionObserver;
    friend class CSimpleSipConnCallbackStub;
#endif
};

    TInt CSimpleRequest::LinkOffset()
        {
        return _FOFF(CSimpleRequest, iLink);
        }  

#endif

// End of File
