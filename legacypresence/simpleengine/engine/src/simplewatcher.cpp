/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Simple Engine
*
*/




// INCLUDE FILES

#include <e32std.h>
#include <s32mem.h>

// own simple
#include "msimpleconnection.h"
#include "simpleconnection.h"
#include "simplecommon.h"
#include "simpleenginerequest.h"
#include "msimpledocument.h"
#include "msimplefilterdocument.h"
#include "msimplewatcherobserver.h"
#include "msimplepresencelist.h"
#include "simplewatcher.h"
#include "simpleerrors.h"
#include "simplexmlfactory.h"

#include "simplesipconnection.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

const TInt KExpandSize = 512;


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleWatcher::CSimpleWatcher
// ----------------------------------------------------------
//
CSimpleWatcher::CSimpleWatcher(
    MSimpleConnection& aConn,
    MSimpleWatcherObserver& aObserver )
: CSimpleClient( aConn ),
  iObserver( aObserver ),
  iResCount(0), iComplete( EFalse ),
  iBuffer(NULL)
    {
    }

// ----------------------------------------------------------
// CSimpleWatcher::~CSimpleWatcher
// ----------------------------------------------------------
//
CSimpleWatcher::~CSimpleWatcher()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: Destructor this=%d" ), (TInt)this );
#endif
    delete iBuffer;
    }

// ----------------------------------------------------------
// CSimpleWatcher::ConstructL
// ----------------------------------------------------------
//
void CSimpleWatcher::ConstructL()
    {
    BaseConstructL();
    iBuffer = CBufFlat::NewL(KExpandSize);   
    }

// ----------------------------------------------------------
// CSimpleWatcher::NewL
// ----------------------------------------------------------
//
CSimpleWatcher* CSimpleWatcher::NewL(
    MSimpleConnection& aConn,
    MSimpleWatcherObserver& aObserver )
    {
    CSimpleWatcher* self = new (ELeave) CSimpleWatcher(
        aConn, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: NewL this=%d" ), (TInt)self );
#endif     
    return self;
    }

// ----------------------------------------------------------
// CSimpleWatcher::Connection
// ----------------------------------------------------------
//
const MSimpleConnection& CSimpleWatcher::Connection()
    {
    return iConn;
    }

// ----------------------------------------------------------
// CSimpleWatcher::SIPStatus
// ----------------------------------------------------------
//
TUint CSimpleWatcher::SIPStatus()
    {
    return DoSIPStatus();
    }
    
// ----------------------------------------------------------
// CSimpleWatcher::SIPRetryAfter
// ----------------------------------------------------------
//
TUint CSimpleWatcher::SIPRetryAfter()
    {
    return DoRetryAfter();
    }
    
// ----------------------------------------------------------
// CSimpleWatcher::SipSubscriptionState
// ----------------------------------------------------------
//
MSimpleWatcher::TSimpleSipSubscriptionState CSimpleWatcher::SipSubscriptionState()
    {
    MSimpleWatcher::TSimpleSipSubscriptionState retVal = ESimpleStateNone;
    
    // get the old request
    CSimpleEngineRequest* req = SearchRequests( iSubsId );
    if ( req )
        {
        CSimpleConnection* conn = STATIC_CAST( CSimpleConnection*, &iConn); //lint !e826
        CSimpleSipConnection* engine = conn->Connection();                       
        MSimpleEngineRequest::TSimpleSipSubscriptionState intVal = engine->SipSubscriptionState( *req );
        switch ( intVal )
            {
            case MSimpleEngineRequest::ESimpleStateNone:
                retVal = MSimpleWatcher::ESimpleStateNone;
                break;
            case MSimpleEngineRequest::ESimpleStatePending:
                retVal = MSimpleWatcher::ESimpleStatePending;
                break;
            case MSimpleEngineRequest::ESimpleStateActive:
                retVal = MSimpleWatcher::ESimpleStateActive;
                break;            
            case MSimpleEngineRequest::ESimpleStateTerminated:
            default:
                retVal = MSimpleWatcher::ESimpleStateTerminated;
                break;            
            }
        }  
    return retVal;
    }    
      

// ----------------------------------------------------------
// CSimpleWatcher::Close
// ----------------------------------------------------------
//
void CSimpleWatcher::Close( )
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleWatcher::NewRequestL
// ----------------------------------------------------------
//
void CSimpleWatcher::NewRequestL( MSimpleEngineRequest& aReq )
    {
    TPtrC8 content = aReq.ResponseData();
    TPtrC8 contType = aReq.ResponseContentType();
    TPtrC8 p8;
    p8.Set( KSimpleDocumentType );
    TInt mySize = p8.Length();
    p8.Set( KSimpleMultipartType );
    TInt mySize2 = p8.Length();
    if (!contType.Left(mySize).CompareF(KSimpleDocumentType))
        {
        MSimpleDocument* d = TSimpleXmlFactory::NewDocumentL( content );
        CleanupClosePushL( *d );
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Watcher: call WatcherNotificationL"));
#endif
        iObserver.WatcherNotificationL( *d );
        CleanupStack::PopAndDestroy( d );
        }
    else if ( !(contType.Left(mySize2).CompareF( KSimpleMultipartType )))
        {
    // Detect the difference between multipart/related cases
    // application/pidf+xml and application/rlmi+xml
        if ( contType.FindF( KSimpleDocumentType ) > 0 )  
            {
            // Multipart for a single presentity having a direct content.            
            MSimpleDocument* d = TSimpleXmlFactory::NewDocumentInMultiPartL(
                content, aReq.ResponseBoundary(), aReq.ResponseStart() );            
            CleanupClosePushL( *d );
#ifdef _DEBUG
            TSimpleLogger::Log(_L("Watcher: call WatcherNotificationL (content)"));
#endif
            iObserver.WatcherNotificationL( *d );
            CleanupStack::PopAndDestroy( d );            
            }
        else
            {
            // Multipart for a presence list.
            MSimplePresenceList* l = TSimpleXmlFactory::NewPresenceListL( 
                content, aReq.ResponseBoundary(), aReq.ResponseStart() );
            CleanupClosePushL( *l );
#ifdef _DEBUG
            TSimpleLogger::Log(_L("Watcher: call WatcherListNotificationL"));
#endif            
            iObserver.WatcherListNotificationL( *l );
            CleanupStack::PopAndDestroy( l );            
            }    
        }
    else
        {
        // Unsupported content type, ignore it.
        }
    }

// ----------------------------------------------------------
// CSimpleWatcher::Complete
// ----------------------------------------------------------
//
void CSimpleWatcher::Complete(
    TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq )
    {   
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: Complete opid=%d status=%d" ),
    aOpId, aStatus );
#endif

    // FUNCTIONALITY
    // Get
    // - SIP ok + terminated -> WatcherReqCompleteL(ok) + WatcherNotificationL(terminated)
    // - terminated + SIP ok -> WatcherNotificationL(terminated) + WatcherReqCompleteL(ok)
    //
    // Stop
    // - terminated + SIP ok -> WatcherReqCompleteL(ok)
    // - SIP ok + terminated -> WatcherReqCompleteL(ok)    
    //
    // Subscribe
    // - SIP error -> WatcherReqCompleteL(error)
    // - SIP ok + terminated ->
    //    WatcherReqCompleteL(ok) + WatcherNotificationL(terminated) + WatcherTerminated()
    // - SIP ok + active + terminated ->
    //        WatcherReqCompleteL(ok) + WatcherNotificationL(active) +
    //        WatcherNotificationL(terminated) + WatcherTErminated
    // - active + SIP ok + terminated ->
    //        WatcherNotificationL(active) + WatcherReqCompleteL(ok) +
    //        WatcherNotificationL(terminated) + WatcherTerminated
    //

    TBool completeNow( EFalse );
    MSimpleEngineRequest::TSimpleRequest orig = aReq.RequestType();

    if ( orig == MSimpleEngineRequest::EDestroyStart )
        {
        // It's time to delete the request of delayed deletion from both DLLs.
        // Delete immediately from another DLL.
        aReq.ModifyType( MSimpleEngineRequest::EDestroy );
        TRAP_IGNORE( SendReqL( aReq ));
        // Delete from this DLL,
        aReq.Destroy();
        return;
        }

    GetSIPStatus( aOpId );

    // Reset data buffer
    iBuffer->Reset();

    // Set the member to point to stack variable
    TBool destroyed( EFalse );
    iDestroyedPtr = &destroyed;

    MSimpleEngineRequest::TSimpleSIPResponse respMet = aReq.ResponseMethod();

    // Convert KSimpleErrPending to OK when needed
    if ( aStatus == KSimpleErrPending )
        {
        aStatus = KErrNone;
        }

    // Handle SIP notification first
    if ( respMet == MSimpleEngineRequest::ENotify )
        {
        // This is true in notifications. Ignore some responses.
        if ( aStatus == KErrCompletion )
            {
            iResCount++;
            }
        if ( orig != MSimpleEngineRequest::ESubscribeStop )
            {
            TRAP_IGNORE( NewRequestL( aReq ) );
            // Check whether an application has called destructor in callback method.
            // Destructor will handle deletion of all the open requests.
            if ( destroyed )
                {
                return;
                }
            } 
        else if ( iResCount > 1  )
            {
            // Stop request is not completed until ok + Notify(terminated) received.  
            // Error completes the stop reqest without Notification.        
            completeNow = ETrue;
            if ( DoCallReqComplete( aOpId, KErrNone ))
                {
                return;
                }            
            }    
        }
    else
        {
        // SIP Status response or client originated cancellation
        iResCount++;
        if ( aStatus != KErrNone )        
            {
            iResCount++;
            if ( orig == MSimpleEngineRequest::ESubscribeStop )     
                {
                // Any response to stop subscribe is ok.
                aStatus = KErrNone;
                }
            }
               
        if ( !iComplete && 
              ( orig != MSimpleEngineRequest::ESubscribeStop || 
                orig == MSimpleEngineRequest::ESubscribeStop && iResCount > 1 ) )
            {
            // Stop request is not completed until ok + Notify(terminated) received.  
            // Error completes the stop reqest without Notification.        
            completeNow = ETrue;
            if ( DoCallReqComplete( aOpId, aStatus ))
                {
                return;
                }
            }
        }

    // Delete request when not needed
    if ( iResCount > 1 )
        {
        iRequest = MSimpleEngineRequest::ENone;
        iComplete = EFalse;
        TInt reason = ResponseReason( aReq );
        // Delete corresponding request from another DLL with delay. This decreases
        // the counter of active subscriptions there.
        aReq.ModifyType( MSimpleEngineRequest::EDestroyStart );
        TRAP_IGNORE( SendReqL( aReq ));
        // call WatcherTerminatedL when needed, i.e. no Stop or Get
        if ( orig != MSimpleEngineRequest::ESubscribeStop &&
             orig != MSimpleEngineRequest::ESubscribeGet &&
             !completeNow )
            {
#ifdef _DEBUG
            TSimpleLogger::Log(_L("Watcher: call WatcherTerminatedL opid=%d"),
                aOpId );
#endif
            TRAP_IGNORE( iObserver.WatcherTerminatedL( aOpId, reason ));
            // Check whether an application has called destructor in callback method.
            // Destructor of CSimpleClient base class will handle deletion of
            // all the open requests.
            if ( destroyed )
                {
                return;
                }
            }
        // delete request from this DLL later
        }

    iDestroyedPtr = NULL;
    }

// ----------------------------------------------------------
// CSimpleWatcher::SubscribeL
// ----------------------------------------------------------
//
TInt CSimpleWatcher::SubscribeL(
    const TDesC8& aURI,
    MSimpleFilterDocument* aFilter,
    TBool aRefresh,
    TBool aAnonymous  )
    {

    if ( iRequest != MSimpleEngineRequest::ENone )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Watcher: SubscribeL IN-USE **" ) );
#endif
        User::Leave( KErrInUse );
        }

    IncreaseOpId();

#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: SubscribeL opid=%d" ), iOpId );
#endif

    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::ESubscribe, iOpId );
    CleanupStack::PushL( req );
    req->SetRemoteURIL( aURI );
    req->SetRefresh( aRefresh );
    if ( aAnonymous )
        {
        req->SetAux( 1 );
        }

    // handle optional filter document
    if ( aFilter )
        {
        StreamDocumentL( *req, *aFilter );
        }

    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );

    iRequest = MSimpleEngineRequest::ESubscribe;
    iResCount = 0;
    iSubsId = iOpId;

    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleWatcher::SubscribeListL
// ----------------------------------------------------------
//
TInt CSimpleWatcher::SubscribeListL(
    const TDesC8& aURI,
    MSimpleFilterDocument* aFilter,
    TBool aRefresh,
    TBool aAnonymous )
    {
    if ( iRequest != MSimpleEngineRequest::ENone )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Watcher: SubscribeListL IN-USE **" ) );
#endif
        User::Leave( KErrInUse );
        }

    IncreaseOpId();
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: SubscribeListL opid=%d" ), iOpId);
#endif

    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::ESubscribeLista, iOpId );
    CleanupStack::PushL( req );
    req->SetRemoteURIL( aURI );
    req->SetRefresh( aRefresh );
    if ( aAnonymous )
        {
        req->SetAux( 1 );
        }

    // handle optional filter document
    if ( aFilter )
        {
        StreamDocumentL( *req, *aFilter );
        }

    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );

    iRequest = MSimpleEngineRequest::ESubscribeLista;
    iResCount = 0;
    iSubsId = iOpId;

    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleWatcher::GetPresenceL
// ----------------------------------------------------------
//
TInt CSimpleWatcher::GetPresenceL(
    const TDesC8& aURI,
    MSimpleFilterDocument* aFilter,
    TBool aAnonymous  )
    {
    if ( iRequest != MSimpleEngineRequest::ENone )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Watcher: GetPresenceL IN-USE **" ) );
#endif
        User::Leave( KErrInUse );
        }

    IncreaseOpId();
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: GetPresenceL opid=%d" ), iOpId );
#endif

    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::ESubscribeGet, iOpId );
    CleanupStack::PushL( req );
    req->SetRemoteURIL( aURI );
    if ( aAnonymous )
        {
        req->SetAux( 1 );
        }

    // handle optional filter document
    if ( aFilter )
        {
        StreamDocumentL( *req, *aFilter );
        }

    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );

    iRequest = MSimpleEngineRequest::ESubscribeGet;
    iResCount = 0;

    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleWatcher::UnsubscribeL
// ----------------------------------------------------------
//
TInt CSimpleWatcher::UnsubscribeL( )
    {

#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: UnsubscribeL opid=%d" ), iSubsId);
#endif
    // use the old opid and request
    CSimpleEngineRequest* req = SearchRequests( iSubsId );
    if ( !req )
        {
        User::Leave( KErrNotFound );
        }
    req->ModifyType( MSimpleEngineRequest::ESubscribeStop );

    SendReqL( *req );

    iRequest = MSimpleEngineRequest::ESubscribeStop;
    iComplete = EFalse;
    iResCount = 0;

    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleWatcher::StreamDocumentL
// ----------------------------------------------------------
//
void CSimpleWatcher::StreamDocumentL(
    CSimpleEngineRequest& aReq,
    MSimpleFilterDocument& aFilter )
    {
    // add request data
    // externalize the document a stream
    iBuffer->Reset();
    RBufWriteStream stream( *iBuffer );
    stream.Open( *iBuffer );
    aFilter.ExternalizeL( stream );
    stream.Close();
    aReq.SetRequestData( iBuffer->Ptr(0) );
    }
    
// ----------------------------------------------------------
// CSimpleWatcher::DoCallReqComplete
// ----------------------------------------------------------
//
TInt CSimpleWatcher::DoCallReqComplete(
    TInt aOpId, TInt aStatus )
    {
    // Set the member to point to stack variable
    TBool destroyed( EFalse );
    iDestroyedPtr = &destroyed;
        
    iComplete = ETrue;    
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Watcher: call WatcherReqCompleteL opid=%d status=%d"),
        aOpId, aStatus);
#endif
    TRAP_IGNORE( iObserver.WatcherReqCompleteL( aOpId, aStatus ));
    // Check whether an application has called destructor in callback method.
    // Destructor will handle deletion of all the open requests.
    if ( destroyed )
        {
        return KErrGeneral;
        }
    return KErrNone; 
    }

