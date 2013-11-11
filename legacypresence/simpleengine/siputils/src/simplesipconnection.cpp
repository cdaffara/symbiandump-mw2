/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    sip connection
*
*/




// INCLUDE FILES

#ifdef _DEBUG
#include <f32file.h>
#endif

#include <badesca.h>

// sip api
#include <sip.h>
#include <sipconnection.h>
#include <sipconnectionobserver.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipclienttransaction.h>
#include <siprefresh.h>
#include <sipmessageelements.h>
#include <sipsubscribedialogassoc.h>

// sip codec api
#include <sipfromheader.h>
#include <siptoheader.h>
#include <sipexpiresheader.h>
#include <sipeventheader.h>
#include <sipcontenttypeheader.h>
#include <sipaddress.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipextensionheader.h>
#include <sipsubscriptionstateheader.h>
#include <sipsupportedheader.h>
#include <sipacceptheader.h>

// own simple
#include "simplesipconnection.h"
#include "simplesipconnectionobserver.h"
#include "simplesipprofileobserver.h"
#include "simplerefreshtimer.h"
#include "msimpleenginerequest.h"
#include "simpleerrors.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#ifdef __LOCAL_MODE
// INTERNAL TEST SUITE
#include "simplelocalmodeutils.h"
#endif
#endif

_LIT8 ( KSimpleWINFO, "winfo");
_LIT8 ( KSimplePRESENCE_LOW, "presence");
_LIT8 ( KSimpleEventlist, "eventlist");
_LIT8 ( KSimpleId, "id");
_LIT8 ( KSimpleAnonymous, "\"Anonymous\" <sip:anonymous@anonymous.invalid>");
_LIT8 ( KSimpleDeactivated, "deactivated" );
_LIT8 ( KSimpleProbation, "probation" );
_LIT8 ( KSimpleRejected, "rejected" );
_LIT8 ( KSimpleTimeout, "timeout" );
_LIT8 ( KSimpleGiveup, "giveup" );
_LIT8 ( KSimpleNoresource, "Noresource" );
_LIT8 ( KSipPrefix, "sip:" );

// UID given for CSIP class. SIP Utils listens to new SIP requests 
// (when receiving IM) outside SIP dialogs.
const TUid KSimpleSIPUtilsUID = { 0x10281EEC };

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleSipConnection::CSimpleSipConnection
// ----------------------------------------------------------
//
CSimpleSipConnection::CSimpleSipConnection()
: iSip(NULL), iSipConnection(NULL), iConnectionObserver(NULL),
  iSipState( ESimpleSipInactive ),
  iRequestList(CSimpleRequest::LinkOffset()),
  iCurrentNbrSubs( 0 )
    {
    }

// ----------------------------------------------------------
// CSimpleSipConnection::~CSimpleSipConnection
// ----------------------------------------------------------
//
EXPORT_C CSimpleSipConnection::~CSimpleSipConnection()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DESTRUCTOR this=%d" ), (TInt)this );
    iFs.Close();
#endif
    delete iSipConnection;
    delete iProfileObserver;
    delete iConnectionObserver;
    delete iSip;
    DeleteRequests();
    SIPStrings::Close();
    }

// ----------------------------------------------------------
// CSimpleSipConnection::NewL
// ----------------------------------------------------------
//
EXPORT_C CSimpleSipConnection* CSimpleSipConnection::NewL()
    {
    CSimpleSipConnection* self = new (ELeave) CSimpleSipConnection;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::ConstructL
// ----------------------------------------------------------
//
void CSimpleSipConnection::ConstructL()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L("SipConnection: ConstructL 20-01-07 this=%d" ), (TInt)this );
#endif
    // initialize members
    iSip = CSIP::NewL( KSimpleSIPUtilsUID, *this );
    iConnectionObserver = CSimpleSipConnectionObserver::NewL( *this);
    iProfileObserver = 
        CSimpleSipProfileObserver::NewL( iSip, *iConnectionObserver);
    SIPStrings::OpenL();
    // read static cenrep settings
    iSettings.ReadCentRepSettings();

#ifdef _DEBUG
    // read ini file for debug version settings
    (void)iFs.Connect();
    TRAP_IGNORE(iSettings.ReadIniFileL( iFs ));
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DispatchReqL
// ----------------------------------------------------------
//
EXPORT_C void CSimpleSipConnection::DispatchReqL( MSimpleEngineRequest& aReq )
    {
    MSimpleEngineRequest::TSimpleRequest type = aReq.RequestType();

#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DispatchReqL type=%d this=%d" ),
        type, (TInt)this );
#endif

    switch ( type )
        {
        case MSimpleEngineRequest::ERegister:
            aReq.SetRefreshTime( 0 );
            RegisterL( aReq );
            break;
        case MSimpleEngineRequest::ESendIM:
            aReq.SetRefreshTime( 0 );
            SendInstantMessageL( aReq );
            break;
        case MSimpleEngineRequest::EReceiveIM:
            aReq.SetRefreshTime( 0 );
            RequestInstantMessageReceivingL( aReq );
            break;
        case MSimpleEngineRequest::EStartPublish:
            aReq.SetRefreshTime( iSettings.ExpiryPuhlish() );
            StartPublishL( aReq );
            break;
        case MSimpleEngineRequest::EPublishModify:
            ModifyPublishL( aReq );
            break;
        case MSimpleEngineRequest::ESubscribe:
            aReq.SetRefreshTime( iSettings.ExpiryWatcher() );
            SubscribeL( aReq);
            break;
        case MSimpleEngineRequest::ESubscribeStop:
            aReq.SetRefreshTime( 0 );
            StopSubscribeL( aReq );
            break;
        case MSimpleEngineRequest::ESubscribeGet:
            aReq.SetRefreshTime( 0 );
            SubscribeL( aReq );
            break;
        case MSimpleEngineRequest::ESubscribeWinfo:
            aReq.SetRefreshTime( iSettings.ExpiryWinfo() );
            SubscribeWinfoL( aReq );
            break;
        case MSimpleEngineRequest::ESubscribeLista:
            aReq.SetRefreshTime( iSettings.ExpiryWatcher() );
            SubscribeListL( aReq );
            break;
        case MSimpleEngineRequest::EStopPublish:
            aReq.SetRefreshTime( 0 );
            StopPublishL( aReq );
            break;
        case MSimpleEngineRequest::EListenEvents:
            aReq.SetRefreshTime( 0 );
            ListenStatusL( aReq );
            break;
        case MSimpleEngineRequest::EDestroy:
            DeleteRequest( aReq );
            break;
        case MSimpleEngineRequest::EDestroyStart:
            DeleteRequestStart( aReq );
            break;
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SIPStatus
// ----------------------------------------------------------
//
EXPORT_C TUint CSimpleSipConnection::SIPStatus( 
    TInt aOpId, TInt& aRetryAfter )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SIPStatus this=%d" ), (TInt)this);
#endif
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();
    
    aRetryAfter = 0;

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        MSimpleEngineRequest& mr = req->Request();
        if ( mr.OpId( ) == aOpId )
            {
            aRetryAfter = req->RetryAfter();
            return req->Status();
            }
        }
    return 0;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SIPState
// ----------------------------------------------------------
//
EXPORT_C TSimpleSipState CSimpleSipConnection::SIPState( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SIPState returns %d this=%d" ),
        iSipState, (TInt)this );
#endif
    return iSipState;
    }
    
// ----------------------------------------------------------
// CSimpleSipConnection::CurrentSIPIdentityL
// ----------------------------------------------------------
//
EXPORT_C TPtrC8 CSimpleSipConnection::CurrentSIPIdentityL()
    {
    return iProfileObserver->GiveUserAorL();
    }  
    
// ----------------------------------------------------------
// CSimpleSipConnection::SipSubscriptionState
// ----------------------------------------------------------
//
EXPORT_C MSimpleEngineRequest::TSimpleSipSubscriptionState CSimpleSipConnection::SipSubscriptionState( 
    MSimpleEngineRequest& aReq )
    {
    MSimpleEngineRequest::TSimpleSipSubscriptionState retVal = MSimpleEngineRequest::ESimpleStateNone;
    
    CSimpleRequest* r = GetCliRequest( aReq );             
    if ( r )
        {
        retVal = r->SipSubscriptionState();
        }
    else
        {        
        }

    return retVal;        
    }      

// ----------------------------------------------------------
// CSimpleSipConnection::RegisterL
// ----------------------------------------------------------
//
void CSimpleSipConnection::RegisterL( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: RegisterL" ));
#endif
    // Check if already registered
    switch ( iSipState )
        {
        case ESimpleSipInactive:
            {
            if ( !aReq.Aux() )
                {
                // Default profile used
                RegisterDefaultL( aReq );
                }
            else
                {
                RegisterAnyL( aReq );
                }
            }
            break;
        case ESimpleSipActive:
        case ESimpleSipSuspend:
            // The sip connection is already registered,
            // return the callback.
            aReq.Complete( KErrNone );
            break;
        case ESimpleSipIdle:
            {
            // already initiated, just wait
            // Save the request to wait a response
            CSimpleRequest* simpleR = CSimpleRequest::NewL(
                *this, aReq, CSimpleRequest::EReqRegister, aReq.RefreshTime());
            iRequestList.AddLast( *simpleR );
            }
            break;
        case ESimpleSipUnavailable:
        default:
            User::Leave( KErrNotFound );
            break;
        }; //lint !e960
    }

// ----------------------------------------------------------
// CSimpleSipConnection::ListenStatusL
// ----------------------------------------------------------
//
void CSimpleSipConnection::ListenStatusL( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: ListenStatusL" ));
#endif
    // Save the request to wait a response
    CSimpleRequest* simpleR = CSimpleRequest::NewL(
        *this, aReq, CSimpleRequest::EReqListenEvents, aReq.RefreshTime() );
    iRequestList.AddLast( *simpleR );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::RequestInstantMessageReceivingL
// ----------------------------------------------------------
//
void CSimpleSipConnection::RequestInstantMessageReceivingL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log( _L(
        "CSimpleSipConnection::RequestInstantMessageReceivingL - Start" ) );
#endif
    
    // Create the request
    CSimpleRequest* request = CSimpleRequest::NewL( *this,
        aReq, CSimpleRequest::EReqReceiveIM, aReq.RefreshTime() );
    
    // Save the request into queue
    iRequestList.AddLast( *request );
    
#ifdef _DEBUG
    TSimpleLogger::Log( _L(
        "CSimpleSipConnection::RequestInstantMessageReceivingL - End" ) );
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SendInstantMessageL
// ----------------------------------------------------------
//
void CSimpleSipConnection::SendInstantMessageL( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleSipConnection::SendInstantMessageL - Start" ) );
#endif

    // Create the request
    CSimpleRequest* request = CSimpleRequest::NewL( *this, aReq,
        CSimpleRequest::EReqSendIM, aReq.RefreshTime() );
    CleanupStack::PushL( request );  // CS: 1
    
    request->SetDataL( aReq.RequestData() );
    request->SetRequestContentTypeL( aReq.RequestContentType() );
    request->SetRefreshTime( aReq.RefreshTime() );
    
    if ( ESimpleSipActive == iSipState )
        {
        // Set request state
        request->SetReqState( CSimpleRequest::ESimpleRunningInit );
        const TDesC8& aContent = request->Data();
        
        if ( !iSipConnection || ESimpleSipActive != iSipState ||
            !iProfileObserver )
            {
            User::Leave( KErrNotReady );
            }
        
        MSIPRegistrationContext* regContext = iProfileObserver->
            ProfileContext();
        
        if ( !regContext )
            {
            User::Leave( KErrNotReady );
            }
        
        // Check CLIENT-OBJ-DATA-LIMIT
        TUint myLimit = iSettings.ObjLimit();

        if ( myLimit && ( TUint )aContent.Size() > myLimit )
           {
           // This is too big mesasge, an error is returned
           User::Leave( KSimpleErrSettingsLimit );
           }
        
        // Get request URI from a request
        TUriParser8 parser;
        HBufC8* temp = HBufC8::NewLC( KSipPrefix().Length() +
            aReq.Recipient().Length() ); // CS: 2
        temp->Des().Copy( KSipPrefix() );
        temp->Des().Append( aReq.Recipient() );
        User::LeaveIfError( parser.Parse( *temp ));
        CUri8* uri = CUri8::NewL( parser );
        
        // Start to fill header, Remote URI
        CSIPRequestElements* elems = CSIPRequestElements::NewL( uri );
        CleanupStack::PushL( elems );  // CS: 3

        TUriParser8 parser3;
        User::LeaveIfError( parser3.Parse( iProfileObserver->
            GiveUserAorL() ));
        uri = CUri8::NewLC( parser3 );  // CS: 4
        
        CSIPAddress* sipAddress = CSIPAddress::NewL( uri );
        CleanupStack::Pop( uri ); // ownership given to sipAddress  // CS: 3
        CleanupStack::PushL( sipAddress );  // CS: 4
        
        // From Header
        CSIPFromHeader* fromH = CSIPFromHeader::NewL( sipAddress );
        CleanupStack::Pop( sipAddress ); // ownership given to FromH  // CS: 3
        CleanupStack::PushL( fromH );  // CS: 4
        elems->SetFromHeaderL( fromH );
        // fromH, ownership given to elems
        CleanupStack::Pop( fromH );  // CS: 3
        
        uri = CUri8::NewLC( parser );  // CS: 4
        
        CSIPAddress* addr = CSIPAddress::NewL( uri );
        CleanupStack::Pop( uri );  // CS: 3
        CleanupStack::PushL( addr );  // CS: 4
        
        // To Header
        CSIPToHeader* toHeader = CSIPToHeader::NewL( addr );
        CleanupStack::Pop( addr );  // CS: 3
        CleanupStack::PushL( toHeader );  // CS: 4   
        elems->SetToHeaderL( toHeader );
        CleanupStack::Pop( toHeader );  // CS: 3
        
        CSIPMessageElements& mesElems = elems->MessageElements();
        
        // Add Expires Header
        if ( aReq.RefreshTime() )
            {
            RPointerArray<CSIPHeaderBase> headers;
            CleanupClosePushL( headers );  // CS: 4
            
            CSIPExpiresHeader* exprH = new (ELeave) CSIPExpiresHeader(
                aReq.RefreshTime() );
            CleanupStack::PushL( exprH );  // CS: 5
            User::LeaveIfError( headers.Append( exprH ) );
            CleanupStack::Pop( exprH ); // CS: 4
            
            mesElems.SetUserHeadersL( headers );
            // headers ownership given to mesElems
            CleanupStack::Pop( &headers ); // CS: 3            
            }
        
        // Set content type and content
        CSIPContentTypeHeader* contTypeH = CSIPContentTypeHeader::NewLC(
            KSimpleMediaType, KSimpleMediaSubType );  // CS: 4
        HBufC8* buffer = aContent.AllocLC();  // CS: 5
        mesElems.SetContentL( buffer, contTypeH );
        // buffer ownership given to mesElems
        CleanupStack::Pop( buffer );  // CS: 4
        // contTypeH ownership given to mesElems
        CleanupStack::Pop( contTypeH );  // CS: 3
        
        // Set method
        elems->SetMethodL( SIPStrings::StringF( SipStrConsts::EMessage ) );
        
        // Send the request transaction
        CSIPClientTransaction* sipTrans = iSipConnection->SendRequestL( elems,
            *regContext );
        
        // Save SIP client transaction
        request->SetTransaction( sipTrans );
        CleanupStack::Pop( elems ); // elems, ownership given  // CS: 2
        CleanupStack::PopAndDestroy( temp ); // CS: 1
        
        // Start refresh timer, it's used for garbage collection too.
        request->StartRefreshTimer();
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        request->SetReqState( CSimpleRequest::ESimplePendingInit );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    
    // Save the request into queue
    iRequestList.AddLast( *request );
    CleanupStack::Pop( request );  // CS: 0
    
    // Update SOURCE-THROTTLE-PUBLISH time
    if ( iSettings.Throttle() )
        {
        request->SetThrottleTime( iSettings.Throttle() );
        }
    
    // Start timer to detect infinite wait situation
    request->StartExpiryTimer( iSettings.ExpiryApi() );
    
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleSipConnection::SendInstantMessageL - End" ) );
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::StartPublishL
// ----------------------------------------------------------
//
void CSimpleSipConnection::StartPublishL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: StartPublishL" ));
    TSimpleLogger::Dump( aReq.RequestData(), iFs, 0 );
#endif

    // create the request
    CSimpleRequest* r = CSimpleRequest::NewL( *this,
        aReq, CSimpleRequest::EReqPublish, aReq.RefreshTime() );
    CleanupStack::PushL( r );
    r->SetDataL( aReq.RequestData() );
    r->SetRequestContentTypeL( aReq.RequestContentType() );
    
    // Set ETag if given
    if ( aReq.ETag().Length() )
        {
        HBufC8* etagBuf = aReq.ETag().AllocL();
        // Ownership is transferred
        r->SetETag( etagBuf );        
        }  

    // save refresh time
    r->SetRefreshTime( aReq.RefreshTime() );

    if ( iSipState == ESimpleSipActive )
        {
        // set request state
        r->SetReqState( CSimpleRequest::ESimpleRunningInit );
        // Do the SIP Publish transaction
        CSIPClientTransaction* sipTrans = DoPublishL(
            iProfileObserver->GiveUserAorL(),            
            aReq.RefreshTime(),
            r->Data(),
            r->ETag(), r->RequestContentType() );
        // Save SIP client transaction
        r->SetTransaction( sipTrans );

        // Start re-fresh timer, it's used for garbage collection too.
        // expires [ no refresh ]
        r->StartRefreshTimer();
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        r->SetReqState( CSimpleRequest::ESimplePendingInit );
        }
    else
        {
        User::Leave( KErrNotReady );
        }

    // Save the request into queue
    iRequestList.AddLast( *r );
    CleanupStack::Pop( r );

    // update SOURCE-THROTTLE-PUBLISH time
    if ( iSettings.Throttle() )
        {
        r->SetThrottleTime( iSettings.Throttle() );
        }

    // Start timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::ModifyPublishL
// ----------------------------------------------------------
//
void CSimpleSipConnection::ModifyPublishL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: ModifyPublishL" ));
    TSimpleLogger::Dump( aReq.RequestData(), iFs, 0 );
#endif

    // Get initial SIP transaction in order to get ETag value.
    CSimpleRequest* r = GetCliRequest( aReq );
    if ( !r || r->ETag().Length() == 0  )
        {
        User::Leave( KErrNotFound );
        }

    // Check the request state is valid
    CSimpleRequest::TSimpleReqState myState = r->ReqState();
    if ( myState != CSimpleRequest::ESimpleActive &&
         myState != CSimpleRequest::ESimplePending &&
         myState != CSimpleRequest::ESimpleRunning )
        {
        User::Leave( KErrNotReady );
        }

    // initialize SIP status
    r->SetStatus( 0 );

    // check SOURCE-THROTTLE-PUBLISH
    TTime myTime;
    myTime.HomeTime();
    if ( iSettings.Throttle() && myTime < r->ThrottleTime() )
        {
        // Leave if not enough time elapsed since last publish 
        // for this publication.
        User::Leave( KSimpleErrSettingsLimit );
        }

    // send PUBLISH modify transaction if network is available
    if ( iSipState == ESimpleSipActive && 
         myState != CSimpleRequest::ESimpleRunning )
        {
        // set state
        r->SetReqState( CSimpleRequest::ESimpleRunning );
        r->SetDataL( aReq.RequestData() );
        r->SetRequestContentTypeL( aReq.RequestContentType() );        

        CSIPClientTransaction* sipTrans = DoPublishL(
            iProfileObserver->GiveUserAorL(),            
            aReq.RefreshTime(),
            r->Data(),
            r->ETag(), r->RequestContentType() );

        // save latest client transaction
        r->SetTransaction( sipTrans );

        // Re-fresh timer is started in ok response, not yet here.

        // update SOURCE-THROTTLE-PUBLISH time
        if ( iSettings.Throttle() )
            {
            r->SetThrottleTime( iSettings.Throttle() );
            }
        }
    else if ( iSipState == ESimpleSipActive && 
              myState == CSimpleRequest::ESimpleRunning )
        {
        // not to send the ETag only.
        r->AddPendingState( CSimpleRequest::EPendingModify );
        r->SetDataL( aReq.RequestData() );
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        // set state
        r->SetReqState( CSimpleRequest::ESimplePending );
        // not to send the ETag only.
        r->AddPendingState( CSimpleRequest::EPendingModify );
        r->SetDataL( aReq.RequestData() );
        }
    else
        {
        // very rare situation
        User::Leave( KErrNotReady );
        }

    // start expiry timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );

#ifdef _DEBUG
    TBuf<100> myETag;
    myETag.Copy( r->ETag() );
    TSimpleLogger::Log(_L("SipConnection: ModifyPublishL ETag=%S" ), &myETag );
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::StartToRefreshL
// ----------------------------------------------------------
//
void CSimpleSipConnection::StartToRefreshL( CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: StartToRefresh"));
#endif

    // This refreshes PUBLISH request or makes garbage collection for
    // subscription without refresh.

    // There are two cases: normal re-publish
    // or expiration of publication

    if ( aReq.Match( CSimpleRequest::EReqPublish ) )
        {
        // refresh[ network ]
        // time to retry
        // expires[ no refresh ]
        // expires
        DoRefreshPublishL( aReq );
        }
    else if ( aReq.Match( CSimpleRequest::EReqSubscribe ) ||
              aReq.Match( CSimpleRequest::EReqSubscribeList ) ||
              aReq.Match( CSimpleRequest::EReqSubscribeWinfo ))
        {
        DoRefreshSubscribe( aReq );
        }
    else
        {
        // No other cases
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::StopPublishL
// ----------------------------------------------------------
//
void CSimpleSipConnection::StopPublishL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: StopPublishL" ));
#endif

    CSIPClientTransaction* sipTrans = NULL;
    
    TBool stopETagScratch (EFalse);

    // Get SIP transaction in order to get ETag value.
    CSimpleRequest* r = GetCliRequest( aReq );
    if ( !r && aReq.ETag().Length() == 0 )
        {       
        User::Leave( KErrNotFound );
        }
    else if ( !r )
        {
        // create the request
        r = CSimpleRequest::NewL( *this,
            aReq, CSimpleRequest::EReqPublish, aReq.RefreshTime() );
        CleanupStack::PushL( r ); 
        stopETagScratch = ETrue;       
        }
        
    // Set ETag if given
    if ( aReq.ETag().Length() )
        {
        HBufC8* etagBuf = aReq.ETag().AllocL();
        // Ownership is transferred
        r->SetETag( etagBuf ); 
        r->SetGivenETag( ETrue );       
        }        

    // initialize SIP status
    r->SetStatus( 0 );

    // refresh timer not needed
    r->StopRefreshTimer();

    // set state
    r->SetReqState( CSimpleRequest::ESimpleStopping );
    
    // Check if the transaction is pending or running
    if ( r->ETag().Length() == 0  )
        {
        r->SetReqState( CSimpleRequest::ESimpleComplete );
        r->Complete( KErrNone );
        return;
        }

    // send PUBLISH transaction
    TRAPD( errx, sipTrans = DoPublishL(
        iProfileObserver->GiveUserAorL(),        
        0,  // 0 epiry headers value
        KNullDesC8,  //  request data
        r->ETag(), r->RequestContentType() ));
    if ( errx )
        {
        // In error case complete the request immediately
        r->Complete( errx );
        return;
        }

    // save latest client transaction
    r->SetTransaction( sipTrans );

    // start expiry timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );
    
    if ( stopETagScratch )
        {
        // Save the request into queue
        iRequestList.AddLast( *r );
        CleanupStack::Pop( r );        
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SubscribeL
// ----------------------------------------------------------
//
void CSimpleSipConnection::SubscribeL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SubscribeL" ));
#endif

    CSIPSubscribeDialogAssoc* subsDialog = NULL;

    // create the request
    CSimpleRequest* r = CSimpleRequest::NewL( *this,
        aReq, CSimpleRequest::EReqSubscribe, aReq.RefreshTime() );
    CleanupStack::PushL( r );

    if ( iSipState == ESimpleSipActive )
        {
        r->SetDataL( aReq.RequestData() );
        r->SetReqState( CSimpleRequest::ESimpleRunningInit );
        CSIPClientTransaction* sipTrans = DoSubscribeL(
            aReq.RemoteURI(),
            aReq.RefreshTime(),
            r->Data(),
            subsDialog,
            aReq.IsRefresh(),
            ESubsRegular,
            aReq.Aux() ? ETrue : EFalse );
        // save SIP Dialog and SIP client transaction
        r->SetDialog( subsDialog );
        r->SetTransaction( sipTrans );
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        r->SetReqState( CSimpleRequest::ESimplePendingInit );
        }
    else
        {
        User::Leave( KErrNotReady );
        }

    CleanupStack::Pop( r );
    iRequestList.AddLast( *r );

    // start timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );

    if ( !aReq.IsRefresh() )
        {
        // Start refresh timer for garbage colletion for
        // subscription not needing refresh.
        TUint myTime = ( aReq.RefreshTime() > iSettings.ExpiryApi() ?
                         aReq.RefreshTime() : iSettings.ExpiryApi() );
        r->SetRefreshTime( myTime );
        r->StartRefreshTimer();
        }
    // increase subscription counter
    IncreaseNbrSubs();
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SubscribeListL
// ----------------------------------------------------------
//
void CSimpleSipConnection::SubscribeListL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SubscribeListL" ));
#endif

    CSIPSubscribeDialogAssoc* subsDialog = NULL;

    // create the request
    CSimpleRequest* r = CSimpleRequest::NewL( *this,
        aReq, CSimpleRequest::EReqSubscribeList, aReq.RefreshTime() );
    CleanupStack::PushL( r );

    if ( iSipState == ESimpleSipActive )
        {
        r->SetDataL( aReq.RequestData() );
        r->SetReqState( CSimpleRequest::ESimpleRunningInit );
        CSIPClientTransaction* sipTrans = DoSubscribeL(
            aReq.RemoteURI(),
            aReq.RefreshTime(),
            r->Data(),
            subsDialog,
            aReq.IsRefresh(),
            ESubsList,
            aReq.Aux() ? ETrue : EFalse );
        // save SIP Dialog and SIP client transaction
        r->SetDialog( subsDialog );
        r->SetTransaction( sipTrans );
        }
    else if ( iSipState == ESimpleSipUnavailable || iSipState == ESimpleSipIdle )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        r->SetReqState( CSimpleRequest::ESimplePendingInit );
        }

    CleanupStack::Pop( r );
    iRequestList.AddLast( *r );

    // Start timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );

    if ( !aReq.IsRefresh() )
        {
        // Start refresh timer for garbage colletion for
        // subscription not needing refresh.
        TUint myTime = ( aReq.RefreshTime() > iSettings.ExpiryApi() ?
                        aReq.RefreshTime() : iSettings.ExpiryApi() );
        r->SetRefreshTime( myTime );
        r->StartRefreshTimer();
        }
    // increase subscription counter
    IncreaseNbrSubs();
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SubscribeWinfoL
// ----------------------------------------------------------
//
void CSimpleSipConnection::SubscribeWinfoL(
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SubscribeWinfoL" ));
#endif
    // get request-URI from SIP profiles
    TPtrC8 aor = iProfileObserver->GiveUserAorL();
    aReq.SetRemoteURIL( aor );

    CSIPSubscribeDialogAssoc* subsDialog = NULL;

    // create the request
    CSimpleRequest* r = CSimpleRequest::NewL( *this,
        aReq, CSimpleRequest::EReqSubscribeWinfo, aReq.RefreshTime() );
    CleanupStack::PushL( r );

    if ( iSipState == ESimpleSipActive )
        {
        r->SetDataL( aReq.RequestData() );
        r->SetReqState( CSimpleRequest::ESimpleRunningInit );
        CSIPClientTransaction* sipTrans = DoSubscribeL(
            aReq.RemoteURI(),
            aReq.RefreshTime(),
            r->Data(),
            subsDialog,
            ETrue,
            ESubsWinfo,
            EFalse );
        // save SIP Dialog and SIP client transaction
        r->SetDialog( subsDialog );
        r->SetTransaction( sipTrans );
        }
    else if ( iSipState == ESimpleSipUnavailable || iSipState == ESimpleSipIdle )
        {
        User::Leave( KErrNotReady );
        }
    else
        {
        r->SetReqState( CSimpleRequest::ESimplePendingInit );
        }

    CleanupStack::Pop( r );
    iRequestList.AddLast( *r );

    // Start timer to detect infinite wait situation
    r->StartExpiryTimer( iSettings.ExpiryApi() );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::StopSubscribeL
// ----------------------------------------------------------
//
void CSimpleSipConnection::StopSubscribeL(
    MSimpleEngineRequest& aReq )
    {

#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: StopSubscribeL" ));
#endif

    // Get SIP transaction
    CSimpleRequest* r = GetCliRequest( aReq );
    if ( !r )
        {
        User::Leave( KErrNotFound );
        }

    CSIPSubscribeDialogAssoc* dialog = r->Dialog();
    if ( !dialog )
        {
        User::Leave(KErrNotFound);
        }

    if ( r->ReqState() == CSimpleRequest::ESimpleReTry ||
         r->ReqState() == CSimpleRequest::ESimplePendingInit ||
         r->ReqState() == CSimpleRequest::ESimplePending )
        {
        // There is not active SIP dialog,
        // therefore real stopping is not needed.
        r->SetReqState( CSimpleRequest::ESimpleStopping );
        r->Request().SetResponseMethod( MSimpleEngineRequest::EUnknownResponse );
        r->Complete( KErrCompletion );
        return;
        }
    else if ( r->ReqState() == CSimpleRequest::ESimpleDeleting ) 
        {
        // there is no active subscription
        User::Leave( KErrNotFound );
        }

    // modify the old request entity
    r->SetTransaction( NULL );

    // create new unsubscribe transaction in the dialog
    // Add accepted header
    CSIPMessageElements* mesElems = CSIPMessageElements::NewLC();
    RPointerArray<CSIPHeaderBase> headers;
    CleanupClosePushL( headers );
    // Accept header with application/pidf+xml is
    // added for reqular subscription and list subscription
    CSIPAcceptHeader* acceH = NULL;
    TInt popCount(0);

    if ( r->Match( CSimpleRequest::EReqSubscribe ) ||
         r->Match( CSimpleRequest::EReqSubscribeList ))
        {
        acceH = CSIPAcceptHeader::NewLC(
                KSimpleApplicationType, KSimplePidfSubType );
        User::LeaveIfError( headers.Append( acceH ));
        ++popCount;
        }
    if ( r->Match( CSimpleRequest::EReqSubscribeList ))
        {
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleApplicationType, KSimpleListSubType );
        User::LeaveIfError( headers.Append( acceH ));
        ++popCount;

        acceH = CSIPAcceptHeader::NewLC(
            KSimpleMultiType, KSimpleMultipartSubType );
        User::LeaveIfError( headers.Append( acceH ));
        ++popCount;
        }
    if ( r->Match( CSimpleRequest::EReqSubscribeWinfo ))
        {
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleApplicationType, KSimpleWinfoSubType );
        User::LeaveIfError( headers.Append( acceH ));
        ++popCount;
        }
        
    // ---------------------------------------------------------        
#ifdef _DEBUG        
#ifdef __LOCAL_MODE
    /**
     * INTERNAL TEST SUITE
     */
    CSimpleLocalModeUtils::AddTestHeaderL( headers );    	
#endif
#endif
    // ---------------------------------------------------------         

    // add content
    mesElems->SetUserHeadersL( headers );

    // Pop AccessHeaders
    CleanupStack::Pop( popCount );
    CleanupStack::PopAndDestroy( &headers );

    // Send SUBSCRIBE message
    CSIPClientTransaction* sipTrans = dialog->SendUnsubscribeL( mesElems );
    CleanupStack::Pop( mesElems );

    // save client transaction
    r->SetTransaction( sipTrans );

    // Start timer to detect infinite wait situation
    r->SetReqState( CSimpleRequest::ESimpleStopping );
    r->StartExpiryTimer( iSettings.ExpiryApi() );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::IncomingRequest
// ----------------------------------------------------------
//
void CSimpleSipConnection::IncomingRequest( TUint32 /*aIapId*/,
                     CSIPServerTransaction* /*aTransaction*/)
    {
    // Nothing to do.
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: IncomingRequest" ));
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::TimedOut
// ----------------------------------------------------------
//
void CSimpleSipConnection::TimedOut( CSIPServerTransaction& /*aTransaction*/)
    {
    // Nothing to do.
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: TimeOut" ));
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::ConnectionStateChange
// ----------------------------------------------------------
//
void CSimpleSipConnection::ConnectionStateChange( 
    TSimpleSipState aState, TInt aSipError )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: ConnectionStateChange %d->%d"),
        iSipState, aState);
#endif

    // re-try to get connection
    TSimpleSipState oldState = iSipState;
        
    RecognizeSipState( aState );
    
    // Complete status event
    if ( oldState != iSipState )
        {
        CompleteStatusRequest();
        }

    // Profile observer gives active event and then the connection can be started to create.
    if ( oldState == ESimpleSipIdle && aState == ESimpleSipActive )
        {
        TRAPD( err, iSipConnection = iProfileObserver->GiveConnectionL() );
        if ( !err )
            {
            SetSipState( ESimpleSipActive );
            // complete client requests
            CompleteRegisterRequests( KErrNone );
            }
        else if ( err != KErrNotReady )
            {
            // error callback to SimpleEngine
            SetSipState( ESimpleSipInactive );
            CompleteRegisterRequests( err );
            }
        else
            {
            // KErrNotReady is a normal error indicating
            // pending initialization.
            }
        }
    else if ( iSipState == ESimpleSipUnavailable )
        {
        iCurrentNbrSubs = 0;
        // Complete all pending register requests with an error code
        TInt retErr = aSipError ? aSipError : KErrDisconnected;
        CompleteRegisterRequests( retErr );
        // Complete all active subscriptions and publications and
        // call API callbacks. KErrDisconnected has a special handling
        // in CSimpleRequest::Complete().
        CompleteRegularRequests( KErrDisconnected );
        }
    else if ( iSipState == ESimpleSipInactive )
        {
        iCurrentNbrSubs = 0;
        // Complete all pending register requests with an error code
        TInt retErr = aSipError ? aSipError : KErrDisconnected;
        CompleteRegisterRequests( retErr );        
        // All the dialogs and transactions are lost.
        // Registeration is done automatically when state is active again.
        // Complete all active subscriptions and publications and
        // call API callbacks now. KErrDisconnected has a special handling
        // in CSimpleRequest::Complete().
        CompleteRegularRequests( KErrDisconnected );
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        // Wait till the active state
        }
    else if ( oldState == ESimpleSipSuspend && iSipState == ESimpleSipActive )
        {
        // Scan all the pending requests and start to proceed them
        HandlePendings();
        }
    else
        {
        // nothing to do
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::HandleReceivedMessage
// ----------------------------------------------------------
//
TInt CSimpleSipConnection::HandleReceivedMessage( const TDesC8& aFrom,
    const TDesC8& aContent )
    {
#ifdef _DEBUG
    TSimpleLogger::Log( _L("CSimpleSipConnection::HandleReceivedMessage") );
#endif
    TInt error( KErrNone );
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();
    
    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        
        if ( req->Match( CSimpleRequest::EReqReceiveIM ) )
            {
            TRAP( error, DoHandleReceivedMessageL( aFrom, aContent, *req ) )
            }
        }
    
    // KErrNoMemory is returned here if error occurred when allocating
    // memory for heap descriptors in DoHandleReceivedMessageL
    return error;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::StartToCheckExpiryL
// ----------------------------------------------------------
//
void CSimpleSipConnection::StartToCheckExpiryL( CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: StartToCheckExpiryL"));
#endif

    // This is expired
    aReq.Complete( KErrTimedOut );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::GetRequest
// ----------------------------------------------------------
//
CSimpleRequest* CSimpleSipConnection::GetRequest(
    CSIPClientTransaction& aTrans )
    {
    // Search the correspoding request element
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( aTrans ))
            {
            return req;
            }
        }

    return (CSimpleRequest*)NULL;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::GetCliRequest
// ----------------------------------------------------------
//
CSimpleRequest* CSimpleSipConnection::GetCliRequest(
    MSimpleEngineRequest& aReq )
    {
    // Search the correspoding request element
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( aReq ))
            {
            return req;
            }
        }

    return (CSimpleRequest*)NULL;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::GetdDialogRequest
// ----------------------------------------------------------
//
CSimpleRequest* CSimpleSipConnection::GetdDialogRequest( const CSIPDialog& aDialog )
    {
    // Search the correspoding request element
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( aDialog ))
            {
            return req;
            }
        }

    return (CSimpleRequest*)NULL;
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::DeleteRequests
// -----------------------------------------------------------------------------
void CSimpleSipConnection::DeleteRequests()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DeleteRequests" ));
#endif
    // Delete all buffered transaction requests
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        req->Destroy();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::DeleteRequest
// -----------------------------------------------------------------------------
void CSimpleSipConnection::DeleteRequest( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DeleteRequest" ));
#endif

    CSimpleRequest* r = CSimpleSipConnection::GetCliRequest( aReq );
    if ( r )
        {
        if ( r->Match( CSimpleRequest::EReqSubscribe ) ||
             r->Match( CSimpleRequest::EReqSubscribeList ) )
            {
            DecreaseNbrSubs();
            }
        r->Destroy();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::DeleteRequestStart
// -----------------------------------------------------------------------------
void CSimpleSipConnection::DeleteRequestStart( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DeleteRequestStart" ));
#endif

    CSimpleRequest* r = CSimpleSipConnection::GetCliRequest( aReq );
    if ( r )
        {
        if ( r->Match( CSimpleRequest::EReqSubscribe ) ||
             r->Match( CSimpleRequest::EReqSubscribeList ))
            {
            DecreaseNbrSubs();
            }
        r->DestroyStart();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::CompleteRegisterRequests
// -----------------------------------------------------------------------------
void CSimpleSipConnection::CompleteRegisterRequests( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: CompleteRegisterRequests %d" ), aStatus);
#endif
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( CSimpleRequest::EReqRegister ))
            {
            req->Complete( aStatus );
            // Open request can be deletetd after completion.
            req->Destroy();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::CompleteRegularRequests
// -----------------------------------------------------------------------------
void CSimpleSipConnection::CompleteRegularRequests( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: CompleteRegularRequests %d" ), aStatus);
#endif
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( CSimpleRequest::EReqPublish ) ||
             req->Match( CSimpleRequest::EReqSubscribe ) ||
             req->Match( CSimpleRequest::EReqSubscribeList ) ||
             req->Match( CSimpleRequest::EReqSubscribeWinfo ))
            {
            req->SetReason( KErrNone );
            req->Request().SetResponseMethod( MSimpleEngineRequest::EUnknownResponse );
            req->Complete( aStatus );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnection::CompleteStatusRequest
// -----------------------------------------------------------------------------
void CSimpleSipConnection::CompleteStatusRequest( )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: CompleteStatusRequest" ) );
#endif
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->Match( CSimpleRequest::EReqListenEvents ))
            {
            req->CompleteEvent( );
            return;
            }
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoPublishL
// ----------------------------------------------------------
//
CSIPClientTransaction* CSimpleSipConnection::DoPublishL(
    const TDesC8& aRemoteURI, TInt aExpiry,
    const TDesC8& aContent, const TDesC8& aSipIfMatch,
    const TDesC8& aContentType )
    {
#ifdef _DEBUG
    TBuf<100> testBuf;
    testBuf.Copy( aSipIfMatch );
    TSimpleLogger::Log(_L("SipConnection: DoPublishL expiry=%d sip-if-match=%S" ),
                       aExpiry, &testBuf);
#endif

    TBool multi( EFalse );
              
    if ( aContent.Length() && 
         aContentType.CompareF( KSimpleDocumentType ))
        {
        // content exists and not pidf+xml (not refresh) 
        multi = ETrue;            
        }

    if ( !iSipConnection || iSipState != ESimpleSipActive || !iProfileObserver )
        {
        User::Leave( KErrNotReady );
        }

    MSIPRegistrationContext* regContext = iProfileObserver->ProfileContext();
    if ( !regContext )
        {
        User::Leave( KErrNotReady );
        }

    // Check CLIENT-OBJ-DATA-LIMIT
    TUint myLimit = iSettings.ObjLimit();
    // Breakpoint:
    if ( myLimit && (TUint)aContent.Size() > myLimit )
        {
        // This is too big mesasge, an error is returned
        User::Leave( KSimpleErrSettingsLimit );
        }

    // get request uri from a request
    TUriParser8 parser;
    User::LeaveIfError( parser.Parse( aRemoteURI ));
    CUri8* uri = CUri8::NewL( parser );

    // Start to fill headres,  Remote URI
    CSIPRequestElements* elems = CSIPRequestElements::NewL( uri );
    CleanupStack::PushL( elems );
    // To header not needed when remote uri set

    uri = CUri8::NewLC( parser );
    CSIPAddress* sipAddress = CSIPAddress::NewL( uri );
    CleanupStack::Pop( uri ); // ownership given to sipAddress
    CleanupStack::PushL( sipAddress );

    // From header
    CSIPFromHeader* fromH = CSIPFromHeader::NewL( sipAddress );
    CleanupStack::Pop( sipAddress ); // ownership given to FromHeader
    CleanupStack::PushL( fromH );
    elems->SetFromHeaderL( fromH );
    CleanupStack::Pop( fromH ); // fromH, ownership given to elems

    // start to add other headers
    RPointerArray<CSIPHeaderBase> headers;
    CleanupClosePushL(headers);

    // Expires header
    CSIPExpiresHeader* exprH = new (ELeave) CSIPExpiresHeader(aExpiry);
    CleanupStack::PushL( exprH );
    User::LeaveIfError( headers.Append( exprH ));
    CleanupStack::Pop( exprH ); // exprH

    // Event header
    CSIPEventHeader* eventH = CSIPEventHeader::NewLC( KSimplePRESENCE_LOW );
    User::LeaveIfError( headers.Append( eventH ));
    CleanupStack::Pop( eventH ); // eventH

    // create SIP-ETAG header with CSIPExtensionHeader
    if ( aSipIfMatch.Length() )
        {
        CSIPExtensionHeader* etagH = CSIPExtensionHeader::NewLC(
            KSimpleSipIfMatch, aSipIfMatch );
        User::LeaveIfError( headers.Append( etagH ));
        CleanupStack::Pop( etagH ); // etagH
        }
            
    // ---------------------------------------------------------        
#ifdef _DEBUG        
#ifdef __LOCAL_MODE
    /**
     * INTERNAL TEST SUITE
     */
    CSimpleLocalModeUtils::AddTestHeaderL( headers );    	
#endif
#endif
      // --------------------------------------------------------- 

    CSIPMessageElements& mesElems = elems->MessageElements();
    mesElems.SetUserHeadersL( headers );
    CleanupStack::PopAndDestroy( &headers );

    // set Content type and content    
      
    CSIPContentTypeHeader* contTypeH = NULL;
    if ( aContent.Length() ) //lint !e830
        {
        if ( multi )
            {             
            // Set CID and Boundary parameters as well
            // ***************************************     
            /*
            HBufC8* contentTypeB = HBufC8::NewLC( 
                NSimpleDocument::NSimpleMulti::KContentTypeSize + 
                NSimpleDocument::KSimpleBoundarySize );    // << contentTypeB
            TPtr8 typeAppend( contentTypeB->Des() );
            TBuf8<10> temppi;
            temppi.Copy( NSimpleDocument::KSimpleBoundary );
            typeAppend.Format( NSimpleDocument::NSimpleMulti::KContentType, &temppi ); 
            */
            
            TBuf8<200> temppi;
            temppi.Copy( NSimpleDocument::NSimpleMulti::KContentTypeFullValue );
                                                             
            contTypeH = CSIPContentTypeHeader::DecodeL( temppi );              
            // CleanupStack::PopAndDestroy( contentTypeB );   // >> contentTypeB         
            CleanupStack::PushL( contTypeH );              // << contTypeH
            }
        else
            {
            contTypeH = CSIPContentTypeHeader::NewLC(
              KSimpleApplicationType, KSimplePidfSubType );  //  // << contTypeH           
            }    
                                
        // copy content from a request parameter
        HBufC8* buffer = aContent.AllocLC();
        mesElems.SetContentL( buffer, contTypeH );   //  >> contTypeH
        // buffer ownership given to mesElems
        CleanupStack::Pop( buffer );
        // contTypeH ownership given to mesElems
        CleanupStack::Pop( contTypeH );
        }
    else
        {
        // No need to insert content, there wasn't any.
        }

    // set method
    RStringF method = SIPStrings::Pool().OpenFStringL( KSimplePUBLISH );
    CleanupClosePushL( method );
    elems->SetMethodL( method );

    // Send the request transaction
    CSIPClientTransaction* sipTrans = iSipConnection->SendRequestL( elems,  *regContext );
    // PopAndDestroy calls method.Close()
    CleanupStack::PopAndDestroy( &method );
    CleanupStack::Pop( elems ); // elems, ownership given
    return sipTrans;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::HandlePublishRespL
// ----------------------------------------------------------
//
void CSimpleSipConnection::HandlePublishRespL(
    const CSIPMessageElements& aMesElems, CSimpleRequest* aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: HandlePublishRespL" ));
#endif
    TBool ETagReceived(EFalse);
    RStringF extensionName = SIPStrings::Pool().OpenFStringL( KSimpleETag );
    CleanupClosePushL( extensionName );
    const RPointerArray<CSIPHeaderBase>& userHs = aMesElems.UserHeaders();
    for ( TInt i = 0; i < userHs.Count(); i++ )
        {
        const CSIPHeaderBase* header = userHs[ i ];
        if ( header->Name() == extensionName )
            {
            HBufC8* hValue = header->ToTextValueL();
            // hValue ownership is transferred
            aReq->SetETag( hValue );
            ETagReceived = ETrue;
            }
        else if ( header->Name() == SIPStrings::StringF( SipStrConsts::EExpiresHeader))
            {
            // Save the new refresh time.
            // Start the timer later in CSimplerequest::Complete()
            CSIPExpiresHeader* eH = (CSIPExpiresHeader*)header;
            TUint expires = eH->Value();
            aReq->SetRefreshTime( expires );
            }
        else
            {
            // We igonre other than Expires and ETag headers
            }
        }
    if ( !ETagReceived || !aReq->RefreshTime() )
        {
        // Remove old ETag if nore received or if the expires header was 0 in our request.
        aReq->SetETag( NULL );
        }
    // PopAndDestroy calls extensionName.Close()
    CleanupStack::PopAndDestroy( &extensionName );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::HandleDialogRequestL
// ----------------------------------------------------------
//
void CSimpleSipConnection::HandleDialogRequestL( 
    const CSIPMessageElements& aMesElems, CSimpleRequest& aReq,
    const TDesC8& aMethod )
    {
#ifdef _DEBUG
    TBuf<20> myBuf;
    myBuf.Copy( aMethod );
    TSimpleLogger::Log(_L("SipConnection: HandleDialogRequestL %S" ), &myBuf);
    CSIPRefresh* sipR = aReq.Refresh();
    if ( sipR )
        {
        TSimpleLogger::Log(_L("SIPREFRESH STATE = %d" ), sipR->State() );
        }
#endif

    _LIT8( KMyActive, "active");
    _LIT8( KMyPending, "pending");

    if ( aMethod.CompareF( KSimpleNOTIFY ))
        {
        // Only NOTIFY is handled.
        return;
        }

    aReq.Request().SetResponseMethod( MSimpleEngineRequest::ENotify );
    aReq.SetReason( KErrNone );

    // Start to search subscription-state header
    const RPointerArray<CSIPHeaderBase>& userHs = aMesElems.UserHeaders();
    for ( TInt i = 0; i < userHs.Count(); i++ )
        {
        const CSIPHeaderBase* header = userHs[ i ];
        if ( header->Name() == SIPStrings::StringF( SipStrConsts::ESubscriptionStateHeader))
            {
            CSIPSubscriptionStateHeader* stateH =
                (CSIPSubscriptionStateHeader*)header;
            TPtrC8 p = stateH->SubStateValue();
#ifdef _DEBUG
            myBuf.Copy( p);
            TSimpleLogger::Log(_L("SipConnection: Subscription-State = %S" ), &myBuf);
#endif
            if ( !p.CompareF( KMyActive ))
                {
                // "active" Notification received
                aReq.ResetErrCount();
                aReq.Complete( KErrNone );
                }
            else if ( !p.CompareF( KMyPending ))
                {
                // "pending" Notification received
                aReq.ResetErrCount();
                aReq.Complete( KSimpleErrPending );
                }
            else // KMyTerminated
                {
                // "terminated"

                aReq.PlusErrCount();

                // Get the value of reason parameter if any exists.
                RStringF value;
                value = stateH->ParamValue( SIPStrings::StringF(
                    SipStrConsts::EReason ));
                DoSetResponseReason( value.DesC(), aReq );

                // Get retry-after parameter
                TInt retryAfter = stateH->RetryAfterParameter();
                if ( retryAfter < 0 )
                    {
                    retryAfter = 0;
                    }
                aReq.SetRetryAfter( retryAfter );

                // Retry in certain situations and do not send
                // the final error to the client yet.

                // Detect an error loop if server always returns "terminated"
                // Certain errors are serious, and no reason to retry.
                if ( aReq.ErrCount() > 1 ||
                     IsPermanentReason( aReq.Reason() ) ||
                     !(aReq.Request().IsRefresh()) ||
                     aReq.ReqState() != CSimpleRequest::ESimpleActiveSubs )
                    {
                    // Complete the client request.
                    // KErrCompletion has a special handling in a client observer.
                    // Set the reason code too.
                    DoSetResponseReason2( aReq.Reason(), aReq.Request() );
                    aReq.Complete( KErrCompletion );
                    }
                else // ESimpleActiveSubs, no fatal error
                    {
                    // terminated[ refresh ]
                    // Retry later
                    // Notice that permanent reason is handled above
                    // also for ESimpleActiveSubs
                    //
                    DoSetResponseReason2( aReq.Reason(), aReq.Request() );
                    // CSimpleRequest::Complete handles this case even when re-tried
                    // without API callback..
                    aReq.Complete( KSimpleErrTemporary );
                    }
                }
            // No need to scan rest of the SIP headers
            return;
            }
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoSubscribeL
// ----------------------------------------------------------
//
CSIPClientTransaction* CSimpleSipConnection::DoSubscribeL(
    const TDesC8& aRemoteURI, TInt aExpiry,
    const TDesC8& aContent,
    CSIPSubscribeDialogAssoc*& aSubsDialog,
    TBool aRefresh,
    TSimpleSubsType aType,
    TBool aAnonymous )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DoSubscribeL expiry=%d" ), aExpiry);
#endif

    // array grow granularity
    const TInt KMyGran = 10; 

    if ( !iSipConnection || iSipState != ESimpleSipActive )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("SipConnection: DoSubscribeL LEAVES **" ));
#endif
        User::Leave( KErrNotReady );
        }

    TUint myLimit = iSettings.MaxSubscriptions();
    // Breakpoint:
    if ( myLimit && iCurrentNbrSubs >= myLimit && aType != ESubsWinfo )
        {
        User::Leave( KSimpleErrSettingsLimit );
        }

    // get request (remote) uri from a request
    TUriParser8 parser;
    User::LeaveIfError( parser.Parse( aRemoteURI ));
    CUri8* uri = CUri8::NewLC( parser );

    // Event header
    CSIPEventHeader* eventH = NULL;
    if ( aType == ESubsRegular || aType == ESubsList )
        {
        eventH = CSIPEventHeader::NewLC( KSimplePRESENCE_LOW );
        }
    else // if ( aType == ESubsWinfo )
        {
        eventH = CSIPEventHeader::NewLC( KSimplePRESENCE_LOW );
        CDesC8ArrayFlat* flat =  new (ELeave) CDesC8ArrayFlat( KMyGran );
        CleanupStack::PushL( flat );
        flat->AppendL( KSimpleWINFO );
        eventH->SetEventTemplatesL( *flat );
        CleanupStack::PopAndDestroy( flat );
        }

    // From header
    // Create is if anonymous request, otherwise use SIP Profile AOR
    CSIPFromHeader* fromH  = NULL;

    if ( aAnonymous )
        {
        // Generate From header
        fromH = CSIPFromHeader::DecodeL( KSimpleAnonymous );
        }
    CleanupStack::PushL( fromH );

    // Subscribe Dialog
    CSIPSubscribeDialogAssoc* sda = CSIPSubscribeDialogAssoc::NewL(
        *iSipConnection, uri, *iProfileObserver->ProfileContext(),
        eventH, fromH );

    CleanupStack::Pop( fromH );
    CleanupStack::Pop( eventH );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( sda );

    // start to add other headers
    CSIPMessageElements* mesElems = CSIPMessageElements::NewLC();
    RPointerArray<CSIPHeaderBase> headers;
    CleanupClosePushL( headers );

    // Expires header
    CSIPExpiresHeader* exprH = new (ELeave) CSIPExpiresHeader(aExpiry);
    CleanupStack::PushL( exprH );
    User::LeaveIfError( headers.Append( exprH ));

    // Accept header with application/pidf+xml is
    // added for reqular subscription and list subscription
    CSIPAcceptHeader* acceH = NULL;
    if ( aType != ESubsWinfo )
        {
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleApplicationType, KSimplePidfSubType );
        User::LeaveIfError( headers.Append( acceH ));
        
        // notice: allow multipart/related always!
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleMultiType, KSimpleMultipartSubType );
        User::LeaveIfError( headers.Append( acceH ));        
        }

    if ( aType == ESubsList )
        {
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleApplicationType, KSimpleListSubType );
        User::LeaveIfError( headers.Append( acceH ));

        // add supported header with value eventlist
        RPointerArray<CSIPSupportedHeader> suppHs =
            CSIPSupportedHeader::DecodeL( KSimpleEventlist);
        for( TInt count=0; count < suppHs.Count(); count++ )
            {
            User::LeaveIfError( headers.Append( suppHs[count] ));
            }
        suppHs.Close();
        }

    if ( aType == ESubsWinfo )
        {
        acceH = CSIPAcceptHeader::NewLC(
            KSimpleApplicationType, KSimpleWinfoSubType );
        User::LeaveIfError( headers.Append( acceH ));
        }

    // For anonymous request add Privacy and P-Preferred-Identity headers
    CSIPExtensionHeader* privacyH = NULL;
    CSIPExtensionHeader* pPIH = NULL;
    HBufC8* myBuffer = NULL;
    if ( aAnonymous )
        {
        privacyH = CSIPExtensionHeader::NewLC(
            KSimplePrivacy, KSimpleId );
        User::LeaveIfError( headers.Append( privacyH ));
        TInt aorLength = iProfileObserver->GiveUserAorL().Length();
        myBuffer = HBufC8::NewLC( aorLength + 2 ); // room for brackets
        *myBuffer = _L8("<");
        myBuffer->Des().Append( iProfileObserver->GiveUserAorL() );
        myBuffer->Des().Append( _L8(">"));
        pPIH = CSIPExtensionHeader::NewLC(
            KSimplePIdentity, myBuffer->Des() );
        User::LeaveIfError( headers.Append( pPIH ));
        }
        
    // ---------------------------------------------------------        

#ifdef _DEBUG        
#ifdef __LOCAL_MODE 
    /**
     * INTERNAL TEST SUITE
     */
    CSimpleLocalModeUtils::AddTestHeaderL(headers);   
#endif
#endif  
      // ---------------------------------------------------------         

    // add content and create refresh entity
    mesElems->SetUserHeadersL( headers );
    // ownership of elements is given to mesElems
    if ( aAnonymous )
        {
        CleanupStack::Pop( pPIH );
        CleanupStack::PopAndDestroy( myBuffer );
        CleanupStack::Pop( privacyH );
        }
    if ( aType == ESubsList )
        {
        CleanupStack::Pop( 3 ); //  3 * acceH
        }
    else if ( aType == ESubsRegular )
        {
        CleanupStack::Pop( 2 ); //  2 * acceH
        }
    else // ESubsWinfo
        {
        CleanupStack::Pop( 1 ); //  1 * acceH
        }
    CleanupStack::Pop( exprH );
    CleanupStack::PopAndDestroy( &headers );

    // set Content type and content
    if ( aContent.Length() )
        {
        CSIPContentTypeHeader* contTypeH = CSIPContentTypeHeader::NewLC(
          KSimpleApplicationType, KSimpleSubscribeSubType );
        // copy content from a request parameter
        HBufC8* buffer = aContent.AllocLC();
        mesElems->SetContentL( buffer, contTypeH );
        // buffer ownership given to mesElems
        CleanupStack::Pop( buffer );
        // contTypeH ownership given to mesElems
        CleanupStack::Pop( contTypeH );
        }
                
    CSIPRefresh* refresh = NULL;
    if ( aExpiry && aRefresh )
        {
        refresh = CSIPRefresh::NewLC();
        }
    // Refresh gets its value from expires header.
    CSIPClientTransaction* sipTrans = sda->SendSubscribeL( mesElems, refresh );
    if ( refresh )
        {
        CleanupStack::Pop( refresh );
        }
    CleanupStack::Pop( mesElems );
    // CSIPSubscribeDialogAssoc must not be deleted until it is ubsubscribed
    CleanupStack::Pop( sda );
    aSubsDialog = sda;

    return sipTrans;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::RegisterDefaultL
// ----------------------------------------------------------
//
void CSimpleSipConnection::RegisterDefaultL( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: RegisterDefaultL" ));
#endif
    iProfileObserver->RegisterDefaultProfileL();
    SetSipState( ESimpleSipIdle );
    TRAPD( err, iSipConnection = iProfileObserver->GiveConnectionL() );
    if ( !err )
        {
        SetSipState( ESimpleSipActive );
        aReq.Complete( KErrNone );
        }
    else if ( err != KErrNotReady )
        {
        // KErrNotReady is a normal error indicating
        // pending initialization
        User::Leave( err );
        }
    else
        {
        // KErrNotReady situation
        }
    // Save the request to wait a response
    CSimpleRequest* simpleR = CSimpleRequest::NewL(
        *this, aReq, CSimpleRequest::EReqRegister, aReq.RefreshTime() );
    iRequestList.AddLast( *simpleR );
    // Start timer to detect infinite wait situation
    simpleR->StartExpiryTimer( iSettings.ExpiryApi() );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::RegisterAnyL
// ----------------------------------------------------------
//
void CSimpleSipConnection::RegisterAnyL( MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: RegisterAnyL" ));
#endif

    iSettings.ReadOTASettingsL( aReq.Aux() );

    iProfileObserver->RegisterGivenProfileL( iSettings.SipProfileId() );
    SetSipState( ESimpleSipIdle );
    TRAPD( err, iSipConnection = iProfileObserver->GiveConnectionL() );
    if ( !err )
        {
        SetSipState( ESimpleSipActive );
        aReq.Complete( KErrNone );
        }
    else if ( err != KErrNotReady )
        {
        // KErrNotReady is a normal error indicating
        // pending initialization
        User::Leave( err );
        }
    else
        {
        // KErrNotReady situation
        }
    // Save the request to wait a response
    CSimpleRequest* simpleR = CSimpleRequest::NewL(
        *this, aReq, CSimpleRequest::EReqRegister, aReq.RefreshTime() );
    iRequestList.AddLast( *simpleR );
    // Start timer to detect infinite wait situation
    simpleR->StartExpiryTimer( iSettings.ExpiryApi() );
    }

// ----------------------------------------------------------
// CSimpleSipConnection::SetSipState
// ----------------------------------------------------------
//
void CSimpleSipConnection::SetSipState( TSimpleSipState aState )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: SetSipState %d this=%d" ), aState, (TInt)this );
#endif
    iSipState = aState;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoSetResponseReason
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoSetResponseReason(
    const TDesC8& aReason, CSimpleRequest& aR )
    {
    if ( aReason.Length() == 0 )
        {
        aR.SetReason( KErrNone );
        }
    else if ( !aReason.CompareF( KSimpleRejected ))
        {
        aR.SetReason( KSimpleErrRejected );
        }
    else if ( !aReason.CompareF( KSimpleTimeout ))
        {
        aR.SetReason( KSimpleErrTimeout );
        }
    else if ( !aReason.CompareF( KSimpleDeactivated ))
        {
        aR.SetReason( KSimpleErrDeactivated );
        }
    else if ( !aReason.CompareF( KSimpleProbation ))
        {
        aR.SetReason( KSimpleErrProbation );
        }
    else if ( !aReason.CompareF( KSimpleNoresource ))
        {
        aR.SetReason( KSimpleErrNoresource );
        }
    else if ( !aReason.CompareF( KSimpleGiveup ))
        {
        aR.SetReason( KSimpleErrGiveup);
        }
    else
        {
        aR.SetReason( KErrNone );
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoSetResponseReason2
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoSetResponseReason2(
    TInt aReason, MSimpleEngineRequest& aR )
    {
    switch ( aReason )
        {
        case KSimpleErrRejected:
            aR.SetResponseReason( MSimpleEngineRequest::ERejected );
            break;
        case KSimpleErrTimeout:
            aR.SetResponseReason( MSimpleEngineRequest::ETimeout );
            break;
        case KSimpleErrDeactivated:
            aR.SetResponseReason( MSimpleEngineRequest::EDeactivated );
            break;
        case KSimpleErrProbation:
            aR.SetResponseReason( MSimpleEngineRequest::EProbation );
            break;
        case KSimpleErrNoresource:
            aR.SetResponseReason( MSimpleEngineRequest::ENoresource );
            break;
        case KSimpleErrGiveup:
            aR.SetResponseReason( MSimpleEngineRequest::EGiveup );
            break;
        default:
            aR.SetResponseReason( MSimpleEngineRequest::ENoReason );
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::IsPermanentReason
// ----------------------------------------------------------
//
TBool CSimpleSipConnection::IsPermanentReason( TInt aReason )
    {
    switch ( aReason )
        {
        case KSimpleErrPending:
        case KSimpleErrDeactivated:
        case KSimpleErrProbation:
        case KSimpleErrTimeout:
        case KSimpleErrGiveup:
        case KSimpleErrTemporary:
            return EFalse;
        default:
            return ETrue;
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::IncreaseNbrSubs
// ----------------------------------------------------------
//
void CSimpleSipConnection::IncreaseNbrSubs()
    {
    ++iCurrentNbrSubs;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: ++iCurrentNbrSubs %d" ),
        iCurrentNbrSubs );
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DecreaseNbrSubs
// ----------------------------------------------------------
//
void CSimpleSipConnection::DecreaseNbrSubs()
    {
    if ( iCurrentNbrSubs )
        {
        --iCurrentNbrSubs;
        }
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: --iCurrentNbrSubs %d" ),
        iCurrentNbrSubs );
#endif
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoRefreshPublishL
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoRefreshPublishL( CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DoRefreshPublishL"));
#endif

    CSimpleRequest::TSimpleReqState orig = aReq.ReqState();

    // First, handle
    // expires [ no refresh ]
    if ( !aReq.Request().IsRefresh() && orig==CSimpleRequest::ESimpleActive )
        {
        aReq.SetStatus( 0 );
        aReq.Complete( KErrTimedOut );
        return;
        }
    if ( orig == CSimpleRequest::ESimpleRunningInit )
        {
        // This might happen with short expiry time when there is no response
        // from a network.
        aReq.SetStatus( 0 );        
        aReq.Complete( KErrTimedOut );
        return;
        }
    else if ( orig == CSimpleRequest::ESimplePending )
        {
        // If refresh is already once set to pending,then this publication is terminated (expired)
        if ( aReq.PendingState() == CSimpleRequest::EPendingRefresh ||
             aReq.PendingState() == CSimpleRequest::EPendingModifyAndRefresh )
            {
            // special error code that terminates the whole publication.        
            aReq.Complete( KErrCompletion );
            return;
            }

        }        
    else if ( orig != CSimpleRequest::ESimpleActive )
        {
        // special error code that terminates the whole publication.        
        aReq.Complete( KErrCompletion );
        return;
        }

    if ( iSipState == ESimpleSipActive )
        {       
        // set request state
        aReq.SetReqState( CSimpleRequest::ESimpleRunning );
        TInt errx = DoSendPendingPublish( aReq );
        if ( errx )
            {
            // Error changes the state and calls API callback when needed
            aReq.Complete( errx );
            }
        else
            {
            // start refresh timer to detect expiry of publication
            aReq.StartRefreshTimer( aReq.RetryExpiryTime() );
            // Do not start expiry timer
            }                
        }
    else if ( iSipState == ESimpleSipSuspend )
        {
        // no network available
        // refresh [ no network ]
        // start refresh timer to detect expiry of publication
        aReq.StartRefreshTimer( aReq.RetryExpiryTime() );
        // Do not start expiry timer
        aReq.SetReqState( CSimpleRequest::ESimplePending );
        aReq.AddPendingState( CSimpleRequest::EPendingRefresh );
        }
    else
        {
        User::Leave( KErrNotReady );
        }

    // DO NOT update SOURCE-THROTTLE-PUBLISH time
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoRefreshSubscribe
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoRefreshSubscribe( CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DoRefreshSubscribe"));
#endif

    if ( !aReq.Request().IsRefresh() )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("SipConnection: Refresh EXPIRED **"));
#endif
        // This is expired and no need to refresh
        aReq.Complete( KErrTimedOut );
        }

    aReq.StartExpiryTimer( aReq.RetryExpiryTime() );

    if ( iSipState != ESimpleSipActive )
        {
        if ( aReq.ReqState() == CSimpleRequest::ESimpleReTry )
            {
            // time to retry [ no network ]
            aReq.SetReqState( CSimpleRequest::ESimplePending );
            // expiry timer i already runnig
            }
        }
    else
        {
        if ( aReq.ReqState() == CSimpleRequest::ESimplePending || 
             aReq.ReqState() == CSimpleRequest::ESimpleReTry )
            {
            // time to retry / SIP req
            // recreate a dialog
            aReq.SetReqState( CSimpleRequest::ESimpleDialogReCre );
            TRAPD( errx, DoCreateDialogL( aReq ) );
            if ( errx )
                {
                // error
                // This will call API callback
                // Original error has been SIP error
                aReq.SetReqState( CSimpleRequest::ESimpleFailed );
                aReq.Complete( KSimpleErrTemporary );
                }
            else
                {
                // waiting OK resp
                }
            }
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoCreateDialogL
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoCreateDialogL( CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DoCreateDialogL"));
#endif

    CSIPSubscribeDialogAssoc* subsDialog = NULL;
    CSIPClientTransaction* sipTrans = NULL;

    // Check whether regular subscription or WINFO subscription is needed.
    MSimpleEngineRequest::TSimpleRequest reqType= aReq.Request().RequestType();
    TSimpleSubsType subsType = ESubsRegular;
    if ( reqType == MSimpleEngineRequest::ESubscribeLista )
        {
        subsType = ESubsList;
        }
    else if ( reqType == MSimpleEngineRequest::ESubscribeWinfo )
        {
        subsType = ESubsWinfo;
        }
    
    // send SIP request
    sipTrans = DoSubscribeL(
        aReq.Request().RemoteURI(),
        aReq.Request().RefreshTime(),
        aReq.Data(),
        subsDialog,
        aReq.Request().IsRefresh(),
        subsType,
        aReq.Request().Aux() ? ETrue : EFalse );

    // save SIP Dialog and SIP client transaction
    aReq.SetDialog( subsDialog );
    aReq.SetTransaction( sipTrans );

    }

// ----------------------------------------------------------
// CSimpleSipConnection::DoSendPendingPublish
// ----------------------------------------------------------
//
TInt CSimpleSipConnection::DoSendPendingPublish(
    CSimpleRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnection: DoSendPendingPublish" ));
#endif

    // Do the SIP Publish transaction
    TBool sendData = aReq.PendingState() == CSimpleRequest::EPendingModify || 
                     aReq.PendingState() == CSimpleRequest::EPendingModifyAndRefresh  ? 
                     ETrue : EFalse;
    TPtrC8 myData = sendData ? aReq.Data() : TPtrC8();
    CSIPClientTransaction* sipTrans = NULL;
    TRAPD( errx, sipTrans = DoPublishL(
        iProfileObserver->GiveUserAorL(),        
        aReq.Request().RefreshTime(),
        myData,
        aReq.ETag(), aReq.RequestContentType() ));
    if ( errx )
        {
        return errx;
        }
    // Save SIP client transaction
    aReq.SetTransaction( sipTrans );
    
    aReq.SetPendingState( CSimpleRequest::ENoPending );

    // Do not update SOURCE-THROTTLE-PUBLISH time

    // Expiry timer is already running

    return KErrNone;
    }

// ----------------------------------------------------------
// CSimpleSipConnection::HandlePendings
// ----------------------------------------------------------
//
void CSimpleSipConnection::HandlePendings( )
    {
    TInt errx( KErrNone );
    TDblQueIter<CSimpleRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleRequest* req = rIter;
        rIter++; //lint !e1757

        if ( req->Match( CSimpleRequest::EReqSubscribe ) ||
             req->Match( CSimpleRequest::EReqSubscribeList ) ||
             req->Match( CSimpleRequest::EReqSubscribeWinfo ))
            {
            // Subscritions
            // Set new request state
            if ( req->ReqState() == CSimpleRequest::ESimplePendingInit )
                {
                req->SetReqState( CSimpleRequest::ESimpleRunningInit );
                }
            else if ( req->ReqState() == CSimpleRequest::ESimplePending )
                {
                req->SetReqState( CSimpleRequest::ESimpleDialogReCre );
                }
            else
                {
                // other states do not need network recovery
                break;
                }

            // create new dialog
            TRAP( errx, DoCreateDialogL( *req ) );
            if ( errx )
                {
                // error
                // This will call API callback
                req->SetReqState( CSimpleRequest::ESimpleFailed );
                req->Complete( errx );
                }
            else
                {
                // Start to wait response from SIP stack.
                // Expiry timer is already runnig to detect infinite wait situation
                }
            }
        else if ( req->Match( CSimpleRequest::EReqPublish ) )
            {
            // Publications
            if ( req->ReqState() == CSimpleRequest::ESimplePendingInit )
                {
                req->SetReqState( CSimpleRequest::ESimpleRunningInit );
                }
            else if ( req->ReqState() == CSimpleRequest::ESimplePending )
                {
                req->SetReqState( CSimpleRequest::ESimpleRunning );
                }
            else
                {
                // other states do not need network recovery
                break;
                }
            // retry publication
            if ( DoSendPendingPublish( *req ) )
                {
                // error
                // This will call API callback
                req->SetReqState( CSimpleRequest::ESimpleFailed );
                req->Complete( errx );
                }
            else
                {
                // Start to wait response from SIP stack.
                // Expiry timer is already runnig to detect infinite wait situation
                }
            }
        } // while
    }
    
// ----------------------------------------------------------
// CSimpleSipConnection::RecognizeSipState
// ----------------------------------------------------------
//
void CSimpleSipConnection::RecognizeSipState( 
    TSimpleSipState aState )
    {
    if ( aState == ESimpleSipActive )
        {
        // Check that both SIP Profile and SIP connection are ready.
        // iProfileObserver is created in ConstructL.
        if ( iProfileObserver->IsProfileActive() && 
             iSipConnection &&
             iSipConnection->State() == CSIPConnection::EActive )
            {
            SetSipState( aState );            
            }
        }
    else
        {
        SetSipState( aState );
        }    
    }    

// ----------------------------------------------------------
// CSimpleSipConnection::DoHandleReceivedMessageL
// ----------------------------------------------------------
//
void CSimpleSipConnection::DoHandleReceivedMessageL( const TDesC8& aFrom,
    const TDesC8& aContent, CSimpleRequest& aRequest )
    {
#ifdef _DEBUG
    TSimpleLogger::Log( _L(
        "CSimpleSipConnection::DoHandleReceivedMessageL - Start" ) );
#endif
    aRequest.SetReason( KErrNone );
    aRequest.SetRecipientL( aFrom );
    aRequest.SetDataL( aContent );
    aRequest.Complete( KErrNone );
    // Open request can be deleted after completion.
    aRequest.Destroy();
#ifdef _DEBUG
    TSimpleLogger::Log( _L(
        "CSimpleSipConnection::DoHandleReceivedMessageL - End" ) );
#endif
    }

