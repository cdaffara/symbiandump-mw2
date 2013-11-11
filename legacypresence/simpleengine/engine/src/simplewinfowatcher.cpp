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
* Description:    Simple Engine
*
*/




// INCLUDE FILES

#include <e32std.h>
#include <s32mem.h>

// own simple
#include "msimpleconnection.h"
#include "simplecommon.h"
#include "simpleenginerequest.h"
#include "msimplewinfo.h"
#include "msimplefilterdocument.h"
#include "msimplewinfoobserver.h"
#include "simplewinfowatcher.h"
#include "simplexmlfactory.h"
#include "simplesipconnection.h"
#include "simpleerrors.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

const TInt KExpandSize = 512;

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleWinfoWatcher::CSimpleWinfoWatcher
// ----------------------------------------------------------
//
CSimpleWinfoWatcher::CSimpleWinfoWatcher(
    MSimpleConnection& aConn,
    MSimpleWinfoObserver& aObserver )
: CSimpleClient( aConn ),
  iObserver( aObserver ),
  iResCount(0), iComplete( EFalse ),
  iBuffer(NULL)
    {
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::~CSimpleWinfoWatcher
// ----------------------------------------------------------
//
CSimpleWinfoWatcher::~CSimpleWinfoWatcher()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: Destructor this=%d" ), (TInt)this );
#endif
    delete iBuffer;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::ConstructL
// ----------------------------------------------------------
//
void CSimpleWinfoWatcher::ConstructL()
    {
    BaseConstructL();
    iBuffer = CBufFlat::NewL(KExpandSize);
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::NewL
// ----------------------------------------------------------
//
CSimpleWinfoWatcher* CSimpleWinfoWatcher::NewL(
    MSimpleConnection& aConn,
    MSimpleWinfoObserver& aObserver )
    {
    CSimpleWinfoWatcher* self = new (ELeave) CSimpleWinfoWatcher(
        aConn, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: NewL this=%d"), (TInt)self );
#endif    
    return self;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::Connection
// ----------------------------------------------------------
//
const MSimpleConnection& CSimpleWinfoWatcher::Connection()
    {
    return iConn;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::SIPStatus
// ----------------------------------------------------------
//
TUint CSimpleWinfoWatcher::SIPStatus()
    {
    return DoSIPStatus();
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::SIPRetryAfter
// ----------------------------------------------------------
//
TUint CSimpleWinfoWatcher::SIPRetryAfter()
    {
    return DoRetryAfter();
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::Close
// ----------------------------------------------------------
//
void CSimpleWinfoWatcher::Close( )
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::NewRequestL
// ----------------------------------------------------------
//
void CSimpleWinfoWatcher::NewRequestL( MSimpleEngineRequest& aReq )
    {
    TPtrC8 content = aReq.ResponseData();
    TPtrC8 contType = aReq.ResponseContentType();
    TPtrC8 p8;
    p8.Set( KSimpleWinfoType );
    TInt mySize = p8.Length();

    if ( !contType.Left(mySize).CompareF(KSimpleWinfoType))
        {
        MSimpleWinfo* d = TSimpleXmlFactory::NewWinfoL( content );
        CleanupClosePushL( *d );
#ifdef _DEBUG
        TSimpleLogger::Log(_L("WinfoWatcher: call WinfoNotificationL"));
#endif
        iObserver.WinfoNotificationL( *d );
        CleanupStack::PopAndDestroy( d );
        }
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::Complete
// ----------------------------------------------------------
//
void CSimpleWinfoWatcher::Complete(
    TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: Complete opid=%d, status=%d" ),
                       aOpId, aStatus );
#endif

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
        // call WinfoTerminatedL when needed, i.e. no Stop 
        if ( orig != MSimpleEngineRequest::ESubscribeStop &&
             !completeNow )
            {
#ifdef _DEBUG
            TSimpleLogger::Log(_L("WinfoWatcher: call WinfoTerminatedL opid=%d"),
                aOpId );
#endif
            TRAP_IGNORE( iObserver.WinfoTerminatedL( aOpId, reason ) );
            // Check whether an application has called destructor in callback method.
            // Destructor of CSimpleClient base class will handle deletion of
            // all the open requests.
            if ( destroyed )
                {
                return;
                }
            }
        // delete request from this DLL later.
        }

    iDestroyedPtr = NULL;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::SubscribeWatcherListL
// ----------------------------------------------------------
//
TInt CSimpleWinfoWatcher::SubscribeWatcherListL(
    MSimpleFilterDocument* aFilter )
    {

    if ( iRequest != MSimpleEngineRequest::ENone )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("winfoWatcher: SubscribeWatcherListL IN-USE **" ) );
#endif
        User::Leave( KErrInUse );
        }

    IncreaseOpId();

#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: SubscribeWatcherListL opid=%d"),iOpId);
#endif
    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::ESubscribeWinfo, iOpId );
    CleanupStack::PushL( req );

    // handle optional filter document
    if ( aFilter )
        {
        StreamDocumentL( *req, *aFilter );
        }

    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );

    iRequest = MSimpleEngineRequest::ESubscribeWinfo;
    iResCount = 0;
    iSubsId = iOpId;

    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleWinfoWatcher::UnsubscribeL
// ----------------------------------------------------------
//
TInt CSimpleWinfoWatcher::UnsubscribeL( )
    {

#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: UnsubscribeL opid=%d" ), iSubsId);
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
// CSimpleWinfoWatcher::StreamDocumentL
// ----------------------------------------------------------
//
void CSimpleWinfoWatcher::StreamDocumentL(
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
// CSimpleWinfoWatcher::DoCallReqComplete
// ----------------------------------------------------------
//
TInt CSimpleWinfoWatcher::DoCallReqComplete(
    TInt aOpId, TInt aStatus )
    {
    // Set the member to point to stack variable
    TBool destroyed( EFalse );
    iDestroyedPtr = &destroyed;
        
    iComplete = ETrue;    
#ifdef _DEBUG
    TSimpleLogger::Log(_L("WinfoWatcher: call WinfoReqCompleteL opid=%d status=%d"),
        aOpId, aStatus);
#endif
    TRAP_IGNORE( iObserver.WinfoReqCompleteL( aOpId, aStatus ));
    // Check whether an application has called destructor in callback method.
    // Destructor will handle deletion of all the open requests.
    if ( destroyed )
        {
        return KErrGeneral;
        }
    return KErrNone; 
    }

