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
* Description:    sip request
*
*/




// INCLUDE FILES

// sip api
#include <sip.h>
#include <sipconnection.h>
#include <sipconnectionobserver.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipclienttransaction.h>
#include <siprefresh.h>
#include <sipdialog.h>
#include <sipsubscribedialogassoc.h>

// simple
#include "simplesipconnection.h"
#include "simplerefreshtimer.h"
#include "simpleerrors.h"
#include "simplecommon.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

// time limits in seconds
const TInt KRefreshLimit = 1200; // limit when KRefreshBefore is used
const TInt KRefreshBefore = 600; // how much before expiration a refresh is done
const TInt KRetryBefore = 300; // how much before expiration a retry is done
const TInt KMinorDelay = 5; // garbage collection and retry delay
const TInt KDeleteDelay = 1; // delay before deleting a subscription request
const TInt KMinorExpiry = 65; // default expiration time for re-try 


// ================= MEMBER FUNCTIONS =======================
//

// -----------------------------------------------------------------------------
// CSimpleRequest::CSimpleRequest
// -----------------------------------------------------------------------------

CSimpleRequest::CSimpleRequest(
    MSimpleSipConnCallback& aEngine,
    MSimpleEngineRequest& aReq,
    TSimpleSipReqType aType,
    TUint aExpires )
    : iEngine(aEngine), iReq(aReq),
      iTrans(NULL), iSipRefresh(NULL), iType(aType),
      iDialog(NULL), iStatus(KErrNone),
      iState(ESimpleInit), iRetryAfter(0),
      iReason(KErrNone), iExpires(aExpires),
      iPendingState(ENoPending), iData(NULL),
      iGivenETag( EFalse ), iSubscriptionState(MSimpleEngineRequest::ESimpleStateNone),
      iRespCount(0)
        {}

CSimpleRequest::~CSimpleRequest()
    {
    StopExpiryTimer();
    StopRefreshTimer();
    delete iSipRefresh;
    delete iRefreshTimer;
    delete iExpiryTimer;
    delete iETag;
    delete iDialog;
    delete iTrans;
    delete iData;
    delete iRecipientId;
    delete iRequestContentType;
    }

// ----------------------------------------------------------
// CSimpleRequest::NewL
// ----------------------------------------------------------
//
CSimpleRequest* CSimpleRequest::NewL(
    MSimpleSipConnCallback& aEngine,
    MSimpleEngineRequest& aReq,
    TSimpleSipReqType aType,
    TUint aExpires )
    {
    CSimpleRequest* self = new (ELeave) CSimpleRequest(
        aEngine,
        aReq,
        aType,
        aExpires );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: NewL this=%d" ), (TInt)self );
#endif
    return self;
    }

// ----------------------------------------------------------
// CSimpleRequest::ConstructL
// ----------------------------------------------------------
//
void CSimpleRequest::ConstructL(  )
    {
    iExpiryTimer = new (ELeave) CSimpleExpiryTimer( iEngine, *this );
    iRefreshTimer = new (ELeave) CSimpleRefreshTimer( iEngine, *this );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Destroy
// -----------------------------------------------------------------------------
void CSimpleRequest::Destroy()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: DESTROY this=%d"), (TInt)this );
#endif
    iLink.Deque();
    delete this;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::DestroyStart
// -----------------------------------------------------------------------------
void CSimpleRequest::DestroyStart()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: DestroyStart this=%d"), (TInt)this );
#endif
    SetReqState( ESimpleDeleting );
    // Yield control to active scheduler before deleting a request. The request may have
    // called multiple callback methods, so it's safer to break the call.
    StartExpiryTimer( KDeleteDelay );
    StopRefreshTimer();
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Request
// -----------------------------------------------------------------------------
MSimpleEngineRequest& CSimpleRequest::Request()
    {
    return iReq;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Transaction
// -----------------------------------------------------------------------------
CSIPClientTransaction* CSimpleRequest::Transaction()
    {
    return iTrans;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetTransaction
// -----------------------------------------------------------------------------
void CSimpleRequest::SetTransaction( CSIPClientTransaction* aTrans )
    {
    delete iTrans;
    iTrans = aTrans;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Refresh
// -----------------------------------------------------------------------------
CSIPRefresh* CSimpleRequest::Refresh()
    {
    return iSipRefresh;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Match
// -----------------------------------------------------------------------------
TBool CSimpleRequest::Match( CSIPClientTransaction& aTrans )
    {
    CSIPClientTransaction* tr = Transaction();
    return ( tr && aTrans == *tr ? ETrue : EFalse );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Match
// -----------------------------------------------------------------------------
TBool CSimpleRequest::Match( TSimpleSipReqType aType ) const
    {
    return ( aType == iType ? ETrue : EFalse );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Match
// -----------------------------------------------------------------------------
TBool CSimpleRequest::Match( const CSIPDialog& aDialog ) const
    {
    return aDialog.IsAssociated( *iDialog );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Match
// -----------------------------------------------------------------------------
TBool CSimpleRequest::Match( MSimpleEngineRequest& aReq ) const
    {
    if ( aReq.OpId() == iReq.OpId() )
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Complete
// -----------------------------------------------------------------------------
void CSimpleRequest::Complete( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: Complete %d this=%d"),aStatus, (TInt)this );
#endif

    if ( iState == ESimpleDeleting )
        {
        // Tell to other DLL that this can be deleted now.
        iReq.Complete( aStatus );
        return;
        }
    if (( aStatus == KErrDisconnected ) ||
        ( aStatus == KErrTimedOut && iType != EReqPublish ))
        {
        // This is a serious network problem
        SetRetryAfter( 0 );
        StopExpiryTimer();
        StopRefreshTimer();
        SetReqState( ESimpleFailed );
        iReq.Complete( aStatus );
        return;
        }

    if ( iType == EReqRegister )
        {
        // stop expiry timer
        StopExpiryTimer();
        // Complete the client request
        iReq.Complete( aStatus );
        }
    else if ( iType == EReqPublish )
        {
        DoCompletePublication( aStatus );
        }
    else if ( EReqSendIM == iType )
        {
        iReq.Complete( aStatus );
        return;
        }
    else if ( EReqReceiveIM == iType )
        {
        TRAP_IGNORE( iReq.SetResponseDataL( Data() ) )
        TRAP_IGNORE( iReq.SetRecipientL( RecipientL() ) )
        iReq.Complete( aStatus );
        return;
        }
    else if ( iType == EReqSubscribe || iType == EReqSubscribeList || iType == EReqSubscribeWinfo )
        {
        if ( Request().ResponseMethod( ) == MSimpleEngineRequest::ENotify )
            {
            DoCompleteNotification( aStatus );
            }
        else
            {
            DoCompleteSubscription( aStatus );
            }
        }
    // EReqListEvents will go to CompleteEvent method.
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::CompleteEvent
// -----------------------------------------------------------------------------
void CSimpleRequest::CompleteEvent( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: CompleteEvent this=%d"),(TInt)this );
#endif
    iReq.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetRefreshTime
// -----------------------------------------------------------------------------
void CSimpleRequest::SetRefreshTime( TUint aTime )
    {
    iRefreshTime = aTime;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::RefreshTime
// -----------------------------------------------------------------------------
TUint CSimpleRequest::RefreshTime( ) const
    {
    return iRefreshTime;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::StartRefreshTimer
// -----------------------------------------------------------------------------
void CSimpleRequest::StartRefreshTimer( TUint aTime )
    {

    // Refresh a little bit earlier, KRefreshBefore sec
    // If expiration time is greater than 1200 seconds,
    // the interval is set to 600 seconds lower than expiration time.
    // Otherwise, interval is set to half of the expiration time.

    if ( aTime )
        {
        // special time used for error recovery
        iRefreshTimer->Start( aTime );
        return;
        }

    // This time is for garbage collection
    TUint myTime = iRefreshTime + KMinorDelay;

    if ( iReq.IsRefresh() )
        {
        // This time is for regular PUBLISH refresh
        if ( iRefreshTime > KRefreshLimit )
            {
            myTime = iRefreshTime - KRefreshBefore;
            }
        else
            {
            myTime = iRefreshTime / 2 ;
            }
        }

    iRefreshTimer->Start( myTime );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::StopRefreshTimer
// -----------------------------------------------------------------------------
void CSimpleRequest::StopRefreshTimer()
    {
    if ( iRefreshTimer )
        {
        iRefreshTimer->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::StartExpiryTimer
// -----------------------------------------------------------------------------
void CSimpleRequest::StartExpiryTimer( TUint aTime )
    {
    iExpiryTimer->Start( aTime );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::StopExpiryTimer
// -----------------------------------------------------------------------------
void CSimpleRequest::StopExpiryTimer()
    {
    if ( iExpiryTimer )
        {
        iExpiryTimer->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::RetryTime
// -----------------------------------------------------------------------------
TUint CSimpleRequest::RetryTime()
    {
    // Retry (refresh) time in error recovery retry.

    // If retry-after SIP header value is present then use it.

    // If retry-after is not available:
    // A quarter of expires value (=half of the current time until expires),
    // maximum 300 seconds.

    // This time policy is in line with SIP stack and presence refreshment policy.
    // Too long time interval is checked outside this method,

    TUint myTime(0);

    if ( iRetryAfter )
        {
        // Retry-after header value
        myTime = iRetryAfter;
        }
    else if ( iRefreshTime > KRefreshLimit )
        {
        myTime = KRetryBefore;
        }
    else
        {
        myTime = iRefreshTime / 4 ;
        }
    // Minun value is few seconds.
    return ( myTime > KMinorDelay ? myTime : KMinorDelay );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::RetryExpiryTime
// -----------------------------------------------------------------------------
TInt CSimpleRequest::RetryExpiryTime()
    {
    // Expiry for retry in error cases, should be calculated at the same time as
    // retry time above.
    TInt myTime(0);

    if ( iRefreshTime > KRefreshLimit )
        {
        myTime = KRefreshBefore;
        }
    else
        {
        myTime = iRefreshTime / 2 ;
        }

    return ( myTime > KMinorExpiry ? myTime : KMinorExpiry );
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetETag
// -----------------------------------------------------------------------------
void CSimpleRequest::SetETag( HBufC8* aTag )
    {
    // ownership is transferred
    delete iETag;
    iETag = aTag;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::ETag
// -----------------------------------------------------------------------------
TPtrC8 CSimpleRequest::ETag() const
    {
    return iETag ? iETag->Des(): TPtrC8();
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Status
// -----------------------------------------------------------------------------
TUint CSimpleRequest::Status() const
    {
    return iStatus;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetStatus
// -----------------------------------------------------------------------------
void CSimpleRequest::SetStatus( TUint aVal )
    {
    iStatus = aVal;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Dialog
// -----------------------------------------------------------------------------
CSIPSubscribeDialogAssoc* CSimpleRequest::Dialog( )
    {
    return iDialog;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetDialog
// -----------------------------------------------------------------------------
 void CSimpleRequest::SetDialog( CSIPSubscribeDialogAssoc* aDialog )
    {
    delete iDialog;
    iDialog = NULL;
    iDialog = aDialog;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::ReqState
// -----------------------------------------------------------------------------
CSimpleRequest::TSimpleReqState CSimpleRequest::ReqState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetReqState
// -----------------------------------------------------------------------------
void CSimpleRequest::SetReqState( CSimpleRequest::TSimpleReqState aState )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: SetReqState %d->%d this=%d"),
        iState, aState,(TInt)this );
#endif
    iState = aState;
    if ( aState == ESimpleFailed || aState == ESimpleComplete || aState == ESimpleDeleting )
        {
        iSubscriptionState = MSimpleEngineRequest::ESimpleStateTerminated;
        }        
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetThrottleTime
// -----------------------------------------------------------------------------
void CSimpleRequest::SetThrottleTime( TUint aSeconds )
    {
    TTime myTime;
    myTime.HomeTime();
    myTime += TTimeIntervalSeconds( aSeconds );
    iThrottleTime = myTime;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::ThrottleTime
// -----------------------------------------------------------------------------
TTime CSimpleRequest::ThrottleTime( )
    {
    return iThrottleTime;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::RetryAfter
// -----------------------------------------------------------------------------
TUint CSimpleRequest::RetryAfter( )
    {
    return iRetryAfter;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetRetryAfter
// -----------------------------------------------------------------------------
void CSimpleRequest::SetRetryAfter( TUint aVal )
    {
    iRetryAfter = aVal;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetReason
// -----------------------------------------------------------------------------
void CSimpleRequest::SetReason( TInt aVal )
    {
    iReason = aVal;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Reason
// -----------------------------------------------------------------------------
TInt CSimpleRequest::Reason( )
    {
    return iReason;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::PlusErrNotify
// -----------------------------------------------------------------------------
void CSimpleRequest::PlusErrCount( )
    {
    ++iErrNotify;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::ResetErrNotify
// -----------------------------------------------------------------------------
void CSimpleRequest::ResetErrCount( )
    {
    iErrNotify = 0;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::ErrNotify
// -----------------------------------------------------------------------------
TInt CSimpleRequest::ErrCount( )
    {
    return iErrNotify;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::PendingState
// -----------------------------------------------------------------------------
CSimpleRequest::TSimplePendingSubState CSimpleRequest::PendingState( )
    {
    return iPendingState;
    }
    
// -----------------------------------------------------------------------------
// CSimpleRequest::AddPendingState
// -----------------------------------------------------------------------------
void CSimpleRequest::AddPendingState( CSimpleRequest::TSimplePendingSubState aVal )
    {   
    if (( aVal == EPendingModify && iPendingState == EPendingRefresh ) ||
        ( aVal == EPendingRefresh && iPendingState == EPendingModify ))
        {
        SetPendingState( EPendingModifyAndRefresh );
        }
    else if ( aVal == EPendingModify && iPendingState == ENoPending )
        {
        SetPendingState( EPendingModify );
        }   
    else if ( aVal == EPendingRefresh && iPendingState == ENoPending )
        {
        SetPendingState( EPendingRefresh );
        }                          
    }    

// -----------------------------------------------------------------------------
// CSimpleRequest::SetPendingState
// -----------------------------------------------------------------------------
void CSimpleRequest::SetPendingState( CSimpleRequest::TSimplePendingSubState aVal )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: iPendingState=%d"), aVal );
#endif    
    iPendingState = aVal;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::Data
// -----------------------------------------------------------------------------
TPtrC8 CSimpleRequest::Data() const
    {
    return iData ? iData->Des() : TPtrC8();
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetRecipientL
// -----------------------------------------------------------------------------
void CSimpleRequest::SetRecipientL( const TDesC8& aRecipientId )
    {
    delete iRecipientId;
    iRecipientId = NULL;
    iRecipientId = aRecipientId.AllocL();
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::RecipientL
// -----------------------------------------------------------------------------
TPtrC8 CSimpleRequest::RecipientL() const
    {
    return iRecipientId ? iRecipientId->Des() : TPtrC8();
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetDataL
// -----------------------------------------------------------------------------
void CSimpleRequest::SetDataL( const TDesC8& aData )
    {
    delete iData;
    iData = NULL;
    iData = aData.AllocL();
    }
    
// -----------------------------------------------------------------------------
// CSimpleRequest::GivenETag
// -----------------------------------------------------------------------------
TBool CSimpleRequest::GivenETag() const
    {
    return iGivenETag;
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::SetGivenETag
// -----------------------------------------------------------------------------
void CSimpleRequest::SetGivenETag( TBool aVal )
    {
    iGivenETag = aVal;
    }    
    
// ----------------------------------------------------------
// CSimpleRequest::RequestContentType
// ----------------------------------------------------------
//
TPtrC8 CSimpleRequest::RequestContentType()
    {
    return iRequestContentType ? iRequestContentType->Des() : TPtrC8();
    }
    
// ----------------------------------------------------------
// CSimpleRequest::SetRequestContentTypeL
// ----------------------------------------------------------
//
void CSimpleRequest::SetRequestContentTypeL( const TDesC8& aData )
    {
    delete iRequestContentType;
    iRequestContentType = NULL;
    iRequestContentType = aData.AllocL(); 
    }
    
// ----------------------------------------------------------
// CSimpleRequest::SipSubscriptionState
// ----------------------------------------------------------
//
MSimpleEngineRequest::TSimpleSipSubscriptionState CSimpleRequest::SipSubscriptionState()
    {
    return iSubscriptionState; 
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::DoCompleteSubscription
// -----------------------------------------------------------------------------
void CSimpleRequest::DoCompleteSubscription( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: DoCompleteSubscription %d"), aStatus);
#endif

    // This is a part of the the state machine for subscriptions.
    // Mainly those cases that call API callback

    TSimpleReqState orig = iState;

    // Stop expiry timer always except a stop request is a special case, because of then
    // both 200 and Notify are required, or an error code alone.
    if ( orig != ESimpleStopping )
        {
        StopExpiryTimer();        
        }
    // Stop refresh timer used for garbage collection
    if ( orig != ESimpleRunningInit )
        {
        StopRefreshTimer();
        }

    if ( orig == ESimpleRunningInit )
        {
        if ( aStatus == KErrNone || aStatus == KSimpleErrPending )
            {
            if ( iExpires )
                {
                // OK response [ expires != 0 ]
                if ( aStatus == KSimpleErrPending )
                    {
                    iSubscriptionState = MSimpleEngineRequest::ESimpleStatePending;
                    }
                SetReqState( ESimpleActiveSubs );
                }
            else
                {
                // OK response [ expires == 0 ]
                SetReqState( ESimpleComplete );
                }
            }
        else
            {
            // error
            SetReqState( ESimpleFailed );
            }
         // Complete API callback always in ESimpleRunningInit
        iReq.Complete( aStatus );
        }
    else if ( orig == ESimpleActiveSubs )
        {
        // This happens if refresh has failed without response notify
        TUint retryTime = RetryTime();
        // If too long interval in the retry-after header then return an error
        if ( retryTime > KRetryBefore )
            {
            SetReqState( ESimpleFailed );             
            iReq.Complete( KErrCompletion );
            return;
            }
        SetReqState( ESimpleReTry );          
        StartRefreshTimer( retryTime );
        }
    else if ( orig == ESimpleStopping )
        {
        // Stop expiry timer always except a stop request is a special case, because of then
        // both 200 and Notify are required, or an error code alone.        
        ++iRespCount;
        if (( aStatus != KErrNone && aStatus != KSimpleErrPending ) ||
            ( iRespCount == 2 ) )
            {
            SetReqState( ESimpleComplete );  
            StopExpiryTimer();                       
            }
        iReq.Complete( aStatus );
        }
    else if ( orig == ESimpleDialogReCre && aStatus == KErrNone )
        {
        // OK resp
        SetReqState( ESimpleActiveSubs );
        }
    else
        {
        // expires or stop / callback
        // error / callback
        // Expires or stop /callbc, respectively
        SetReqState( ESimpleFailed );
        iReq.Complete( aStatus );
        }
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::DoCompleteNotification
// -----------------------------------------------------------------------------
void CSimpleRequest::DoCompleteNotification( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: DoCompleteNotification %d"), aStatus);
#endif

    // This is a part of the the state machine for subscriptions.
    // Mainly those cases that call API callback

    TSimpleReqState orig = iState;

    if ( aStatus == KErrNone || aStatus == KSimpleErrPending )
        {
        if ( KSimpleErrPending == aStatus )
            {
            iSubscriptionState = MSimpleEngineRequest::ESimpleStatePending;
            }
        else
            {
            iSubscriptionState = MSimpleEngineRequest::ESimpleStateActive;             
            }    
           
        if ( orig == ESimpleStopping && aStatus != KErrNone )
            {
            // A stop request is a special case, because of then
            // both 200 and Notify are required, or an error code alone.             
            ++iRespCount;
            if ( iRespCount == 2 )
                {
                SetReqState( ESimpleComplete );  
                StopExpiryTimer();                       
                }
            }
                    
        // Notification without error does not change the state,
        // just pass the notification thru to the API
        iReq.Complete( aStatus );
        return;
        }

    if ( orig  == ESimpleActiveSubs )
        {
        // Ensure that Dialog refreshing will be stopped now.        
        SetDialog( NULL );
        if ( iReq.IsRefresh() && aStatus == KSimpleErrTemporary )
            {
            // terminated [ refresh ]
            StopRefreshTimer();
            TUint retryTime = RetryTime();
            // If too long interval in the retry-after header then return an error
            if ( retryTime > KRetryBefore )
                {                                 
                SetReqState( ESimpleFailed );
                iReq.Complete( KErrCompletion );
                return;
                }
            // Do not send the notification to the client API now, but
            // go to retry-state and retry later dialog re-creation.
            SetReqState( ESimpleReTry );
            StartRefreshTimer( retryTime );
            }
        else
            {
            // Expires[ no refresh ]
            // error [ no refresh ]
            // Timer may stop the request if not refresh.
            SetReqState( (aStatus==KErrTimedOut || aStatus==KErrCompletion) ?
                ESimpleComplete : ESimpleFailed );                 
            // Complete API callback
            iReq.Complete( aStatus );
            }
        }
    else
        {         
        iReq.Complete( aStatus );
        }
    }

// -----------------------------------------------------------------------------
// CSimpleRequest::DoCompletePublication
// -----------------------------------------------------------------------------
void CSimpleRequest::DoCompletePublication( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleRequest: DoCompletePublication %d"), aStatus);
#endif

    // This is a part of the the state machine for publications.
    // Mainly those cases that call API callback
    
    // SIP error code 412, RFC3903
    const TUint KMySip412 = 412;        
    
    TSimpleReqState orig = iState;
    TSimplePendingSubState nextSub = ENoPending;
    
    // Set ETag that's received from network or reset 
    // when an error has taken a place. If it fails, it is not very serious, 
    // and it is very rare OOM situation.
    TRAP_IGNORE( iReq.SetETagL( ETag() ));    

    // Stop refresh timer and start only when needed.
    if ( orig != ESimplePending )
        {                
        StopRefreshTimer();
        }

    if ( orig == ESimpleRunningInit )
        {
        // stop expiry timer
        StopExpiryTimer();
        if ( aStatus == KErrNone )
            {
            // OK response
            SetReqState( ESimpleActive );
            // Ensure API callback when terminated
            StartRefreshTimer();
            }
        else
            {
            // error
            SetReqState( ESimpleFailed );
            }
         // Complete API callback always in ESimpleRunningInit
        iReq.Complete( aStatus );
        }
    else if ( orig == ESimpleRunning )
        {
        TBool isModify = Request().RequestType() == MSimpleEngineRequest::EPublishModify ? ETrue : EFalse;
        // Handle pending/running modifify request situation first
        if ( isModify )
            {
            SetReqState( ESimpleActive );
            // Check SIP status and decide whether publication is terminated or
            // the modify request failed only.
            if ( PendingState() != ENoPending )
                {
                // The previous refresh request is completed and the next one is the
                // pending modify request.
                if ( aStatus )
                    {
                    // error[ callback ]
                    // terminate the publication
                    // stop expiry timer
                    StopExpiryTimer();                    
                    iReq.Complete( KErrCompletion );
                    SetReqState( ESimpleFailed );
                    }
                else
                    {
                    StartRefreshTimer();
                    // Send modification request
                    TInt errx = iEngine.DoSendPendingPublish( *this );
                    if ( errx )
                        {
                        // Modify request failed only
                        // stop expiry timer
                        StopExpiryTimer();
                        iReq.Complete( errx );
                        SetReqState( ESimpleActive );
                        }
                    else
                        {
                        // Just wait modify response, do not complete API request.
                        // Do not stop or restart expiry timer
                        }
                    }
                } // pending
            else
                {
                // This is the actual modify request response.
                TUint sipStatus = Status();
                // check if not 412, see RFC3903
                if ( ( !aStatus || aStatus == KSimpleErrTemporary ) && sipStatus != KMySip412 )
                    {
                    // Modify failed only
                    StartRefreshTimer();
                    // Complete modification in API callback
                    iReq.Complete( aStatus );
                    SetReqState( ESimpleActive );
                    }
                else
                    {
                    // The whole publication is terminated
                    iReq.Complete( KErrCompletion );
                    SetReqState( ESimpleFailed );
                    }
                // stop expiry timer
                StopExpiryTimer();
                } // not pending
            } // modify
        else
            {
            // stop expiry timer
            StopExpiryTimer();
            // re-freshing publication
            if ( aStatus == KErrNone )
                {
                StartRefreshTimer();
                SetReqState( ESimpleActive );
                // Complete the request for ETag observers only
                iReq.SetResponseMethod( MSimpleEngineRequest::EStatusETag );                
                }
            else
                {
                // error, no recovery
                SetReqState( ESimpleFailed );                
                iReq.Complete( aStatus );
                }
            }
        }
    else if ( orig  == ESimpleStopping )
        {
        // stop expiry timer
        StopExpiryTimer();
        // Any kind of response is OK for stopping,
        // either timeout or OK response except with given ETag the real SIP response
        // is sent to the client.
        SetReqState( ESimpleComplete );
        TInt retVal = GivenETag() ? aStatus : KErrNone;
        iReq.Complete( retVal );
        }
    else if ( orig == ESimplePending && 
             ( iPendingState==EPendingModify || iPendingState==EPendingModifyAndRefresh ) &&
             aStatus != KErrCompletion )
        {
        // Check if just modify terminates or the entire publication is terminated
        // stop expiry timer
        StopExpiryTimer();
        // Modify failed, automatic refresh continues
        // Complete modification in API callback
        iReq.Complete( aStatus );
        SetReqState( ESimpleActive );
        if ( iPendingState == EPendingModifyAndRefresh && aStatus )
            {
            // The publication may still be alive but automatic refresh is pending
            SetReqState( ESimplePending );            
            nextSub = EPendingRefresh;
            }
        }        
    else // odds and ends like time-out
        {
        // stop expiry timer
        StopExpiryTimer();
        SetReqState( ESimpleFailed );        
        iReq.Complete( aStatus );
        }
    // Reset the request body data
    delete iData;
    iData = NULL;
    // Reset pendig modify request
    SetPendingState( nextSub );
    }
