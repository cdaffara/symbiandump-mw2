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
* Description:    sip connection
*
*/





#include <sipconnectionobserver.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <sipregistrationbinding.h>
#include <sipconnection.h>
#include <siprefresh.h>
#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <sipsubscribedialogassoc.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipclienttransaction.h>
#include <sipservertransaction.h>
#include <siprequestelements.h>

#include <sipretryafterheader.h>
#include <sipextensionheader.h>
#include <sipcontenttypeheader.h>

#include <siperr.h>
#include <sipaddress.h>
#include <sipfromheader.h>

#include "simplecommon.h"
#include "simplesipconnectionobserver.h"
#include "simplesipconncallback.h"
#include "simplerequest.h"
#include "simpleerrors.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

// SIP response Call/Transaction does not exists
const TUint  KSimple481 = 481;

_LIT8 ( KSimpleBoundary, "boundary" );
_LIT8 ( KSimpleStart, "start");
_LIT8 ( KAt, "@" );

#ifdef _DEBUG
#include "simpledebugutils.h"
#ifdef __LOCAL_MODE
//  INTERNAL TEST SUITE
#include "simplelocalmodeutils.h"
#endif
#endif

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::CSimpleSipConnectionObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSimpleSipConnectionObserver::CSimpleSipConnectionObserver(
    MSimpleSipConnCallback& aCallback )
    : iCallback( aCallback )
    {
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::~CSimpleSipConnectionObserver
// -----------------------------------------------------------------------------
//
CSimpleSipConnectionObserver::~CSimpleSipConnectionObserver()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: DESTRUCTOR this=%d"), (TInt)this);
#ifdef __LOCAL_MODE
    /**
     * INTERNAL TEST SUITE
     */
    delete iLocalModeUtils;
#endif        
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::NewL
// -----------------------------------------------------------------------------
//
CSimpleSipConnectionObserver* CSimpleSipConnectionObserver::NewL(
    MSimpleSipConnCallback& aCallback )
    {
    CSimpleSipConnectionObserver* self = new (ELeave) CSimpleSipConnectionObserver( aCallback );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: NewL this=%d"), (TInt)self);
#endif
    return self;
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ConstructL
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ConstructL()
    {
#ifdef _DEBUG
#ifdef __LOCAL_MODE
    /**
     * INTERNAL TEST SUITE
     */
    iLocalModeUtils = CSimpleLocalModeUtils::NewL( );
    iLocalModeUtils->SubscribeNetworkStateL( this );
#endif
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ConnectionStateChanged(
    CSIPConnection::TState aState )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ConnectionStateChanged state=%d sip=%d"), aState );
#endif  
    ConnectionStateChanged2( aState, KErrNone );
    }
    
// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ProfileStateChanged
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ProfileStateChanged(
    CSIPConnection::TState aState, TInt aSipError )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ProfileStateChanged state=%d sip=%d"), aState );
#endif
      ConnectionStateChanged2( aState, aSipError );      
    }     
    
// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ConnectionStateChanged2
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ConnectionStateChanged2(
    CSIPConnection::TState aState, TInt aSipError )
    {
    TInt retErr = aSipError ? ConvertSipCode( aSipError, EFalse, EFalse ) : KErrNone;
    switch ( aState )
      {
      // Here is a logic, how to tell active state upward after waiting both
      // connection and profile to be ok.
      case CSIPConnection::EInit:
          iCallback.ConnectionStateChange( ESimpleSipIdle, retErr );
          break;
      case CSIPConnection::EActive:                      
         iCallback.ConnectionStateChange( ESimpleSipActive, retErr );
          break;
      case CSIPConnection::ESuspended:
          iCallback.ConnectionStateChange( ESimpleSipSuspend, retErr);
          break;
      case CSIPConnection::EInactive:
          iCallback.ConnectionStateChange( ESimpleSipInactive, retErr );
          break;
      case CSIPConnection::EUnavailable:
          iCallback.ConnectionStateChange( ESimpleSipUnavailable, retErr );
          break;
      default:
          break;
      }; 
    }    

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::IncomingResponse
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingResponse (
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: IncomingResponse" ));
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt aError,
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/)
    {
    iError = aError;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 1 err=%d"), aError );
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt aError,
    CSIPRegistrationBinding& /*aRegistration*/)
    {
    iError = aError;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 2 err=%d" ), aError);
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// This is mainly for Publish errors, no dialog.
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt aError,
    CSIPTransactionBase& aTransaction )
    {
    iError = aError;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 3 err=%d" ), aError);
#endif
    if ( aTransaction.IsSIPClientTransaction() )
        {
        CSIPTransactionBase::TState sipState = CSIPTransactionBase::ETrying;
        TRAP_IGNORE( sipState = aTransaction.StateL() );
        CSimpleRequest* r = iCallback.GetRequest(
            STATIC_CAST( CSIPClientTransaction&, aTransaction) );
        if ( r && sipState == CSIPTransactionBase::ETerminated )
            {
            TInt retCode = aError == KErrSIPForbidden ? KErrAccessDenied : KErrCommsBreak;
            r->SetStatus( 0 );
            // update response method
            r->Request().SetResponseMethod( MSimpleEngineRequest::EUnknownResponse );
            r->Complete( retCode );
            }
        else
            {
            // nothing to do
            }
        }
    else
        {
        // nothing to do
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// Dialog refresh has failed, a transaction has been deleted.
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt aError,
    CSIPDialogAssocBase& aDialogAssoc )
    {
    // This is called when SUBSCRIBE refresh fails with
    // an error response from a remote server.
    iError = aError;
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 4 err=%d" ), aError );
#endif

    TInt retCode = aError == KErrSIPForbidden ? KErrAccessDenied : KErrCommsBreak;
    TUint status(0);
    TUint myRefresh(0);

    CSIPSubscribeDialogAssoc& myDialogA = STATIC_CAST( CSIPSubscribeDialogAssoc&, aDialogAssoc );
    const CSIPDialog& myDialog = myDialogA.Dialog();
    CSimpleRequest* r = iCallback.GetdDialogRequest( myDialog );
    if ( !r )
        {
        // request not found
        return;
        }

    if ( aError == KErrSIPTerminatedWithResponse )
        {
        // It's subscription's response with data
        const CSIPRefresh* refresh = myDialogA.SIPRefresh();
        if ( refresh  )
            {
            // This instance is not the initial transaction but
            // the refresh transaction instance.
            // Ownership is not transferred. May be NULL.
            const CSIPClientTransaction* trans = refresh->SIPTransaction();
            if ( trans )
                {
                // ownership isn't transferred.
                // Elems must exist with KErrSIPTerminatedWithResponse
                const CSIPResponseElements* elems = trans->ResponseElements();
               // Get retry-after header value
                myRefresh = GetRetryAfter( elems );
                // Get SIP Status
                status = elems->StatusCode();
                if ( status >= KSimpleOK )
                    {
                    retCode = ConvertSipCode( status, myRefresh ? ETrue: EFalse, EFalse );
                    }
                }
            }
        }

    // Update the request entity and complete it.
    r->SetRetryAfter( myRefresh );
    r->SetStatus( status );
    r->Request().SetResponseMethod( MSimpleEngineRequest::EUnknownResponse );
    r->Complete( retCode );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// Sending of either request or response failed, dialog specific.
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt aError,
    CSIPTransactionBase& aTransaction,
    CSIPDialogAssocBase& /*aDialogAssoc*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 5 err=%d" ), aError);
#endif

    TInt retCode = aError == KErrSIPForbidden ? KErrAccessDenied : KErrCommsBreak;
    TUint status = 0;

    if ( aTransaction.IsSIPClientTransaction() )
        {
        // This is an initial SUBSCRIBE transaction that has failed.
        CSimpleRequest* r2 = iCallback.GetRequest(
            STATIC_CAST( CSIPClientTransaction&, aTransaction )); 
        if ( !r2 )
            {
            // request not found
            return;
            }                               
        // This instance is the initial transaction.
        CSIPClientTransaction& trans = STATIC_CAST( CSIPClientTransaction&, aTransaction );
        // ownership isn't transferred.
        const CSIPResponseElements* elems = trans.ResponseElements();
        if ( elems )
            {
            status = elems->StatusCode();
            }
        // convert SIP status code
        if ( status >= KSimpleOK )
            {
            retCode = ConvertSipCode( status, EFalse, EFalse );
            }
        r2->SetStatus( status );
        // update response method
        r2->Request().SetResponseMethod( MSimpleEngineRequest::EUnknownResponse );
        r2->Complete( retCode );
        }
    else
        {
        // Server transactions are not handled.
        // An error in responding to NOTIFY request comes here.
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::ErrorOccured
// This is not really needed because of engine does not use standalone CSIPRefesh
// requests.
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::ErrorOccured (
    TInt /*aError*/,
    CSIPRefresh& /*aRefresh*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: ErrorOccured 6" ));
#endif
    }

// -----------------------------------------------------------------------------
// A SIP request outside a dialog has been received from the network.
// CSimpleSipConnectionObserver::IncomingRequest
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingRequest(
    CSIPServerTransaction* aTransaction )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L("CSimpleSipConnectionObserver::IncomingRequest" ) );
#endif
    
    TRAP_IGNORE( DoIncomingRequestL( aTransaction ) );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::IncomingRequest
// Dialog specific request from server..
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingRequest (
    CSIPServerTransaction* aTransaction,
    CSIPDialog& aDialog )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: IncomingRequest (dialog)" ));
#endif

    TRAP_IGNORE( DoIncomingRequestL( aTransaction, aDialog ) );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::IncomingResponse
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingResponse(
    CSIPClientTransaction& aTransaction)
    {
#ifdef _DEBUG
    TInt status = aTransaction.ResponseElements()->StatusCode();
    TSimpleLogger::Log(_L("SipConnectionObserver: IncomingResponse status=%d" ), status);
#endif

    DoIncomingResponse( aTransaction );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::IncomingResponse
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingResponse(
    CSIPClientTransaction& aTransaction,
    CSIPDialogAssocBase& aDialogAssoc)
    {
#ifdef _DEBUG
    TInt status( KErrNone );
    status = aTransaction.ResponseElements()->StatusCode();
    TSimpleLogger::Log(_L("SipConnectionObserver: IncomingResponse (dialog) status=%d"), status);
#endif

    DoIncomingResponse( aTransaction, aDialogAssoc );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::IncomingResponse
// Invite not supported by SIMPLE
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::IncomingResponse (
    CSIPClientTransaction& /*aTransaction*/,
    CSIPInviteDialogAssoc* /*aDialogAssoc*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: IncomingResponse (invite)" ));
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::InviteCompleted
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::InviteCompleted (
    CSIPClientTransaction& /*aTransaction*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: InviteCompleted" ));
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::InviteCanceled
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::InviteCanceled (
    CSIPServerTransaction& /*aTransaction*/)
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SipConnectionObserver: InviteCanceled" ));
#endif
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::DoHandleSrvrReqContentL
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::DoHandleSrvrReqContentL(
    const CSIPMessageElements& aElems,
    MSimpleEngineRequest& aR )
    {
    RStringF boundary = SIPStrings::Pool().OpenFStringL( KSimpleBoundary );
    CleanupClosePushL( boundary );
    RStringF start = SIPStrings::Pool().OpenFStringL( KSimpleStart );
    CleanupClosePushL( start );
    RStringF value;

    // ownership of typeHeader is not transferred and it may be NULL.
    const CSIPContentTypeHeader* typeHeader = aElems.ContentType();
    if ( typeHeader )
        {
        // buffer ownership is transferred
        HBufC8* buffer = typeHeader->ToTextValueL();
        aR.SetResponseContentType( buffer );
        aR.SetResponseDataL( aElems.Content() );
        // set boundary
        if ( typeHeader->HasParam( boundary ) )
            {
            value = typeHeader->ParamValue( boundary );
            aR.SetResponseBoundaryL( value.DesC() );
            }
        // set start-content-id
        if ( typeHeader->HasParam( start ) )
            {
            value = typeHeader->ParamValue( start );
            aR.SetResponseStartL( value.DesC() );
            }
#ifdef _DEBUG
        RFs myFs;
        (void)myFs.Connect();
        TSimpleLogger::Dump( aR.ResponseData(), myFs, 1 );
        myFs.Close();
#endif
        }
    CleanupStack::PopAndDestroy( &start );
    CleanupStack::PopAndDestroy( &boundary );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::DoIncomingRequestL
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::DoIncomingRequestL(
    CSIPServerTransaction* aTransaction )
    {
    const CSIPRequestElements* elems = aTransaction->RequestElements();
    const CSIPContentTypeHeader* contentType = elems->MessageElements().
        ContentType();
    
    if( KSimpleMediaType() == contentType->MediaType() &&
        KSimpleMediaSubType() == contentType->MediaSubtype() )
        {
        const CSIPFromHeader* fromHeader = elems->FromHeader();
        const TDesC8& content = elems->MessageElements().Content();
        const TDesC8& user = fromHeader->SIPAddress().Uri8().Uri().
            Extract( EUriUserinfo );
        const TDesC8& host = fromHeader->SIPAddress().Uri8().Uri().
            Extract( EUriHost );
        HBufC8* from = HBufC8::NewLC( user.Length() + KAt().Length() +
            host.Length() ); // CS: 1
        from->Des().Copy( user );
        from->Des().Append( KAt() );
        from->Des().Append( host );
        
        CleanupStack::PushL( aTransaction ); // CS: 2
        CSIPResponseElements* respElem = CSIPResponseElements::NewLC( // CS: 3
            KSimpleOK, SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
        
        // Use the transaction to send 200 OK
        aTransaction->SendResponseL( respElem );
        
        CleanupStack::Pop( respElem );     // CS: 2
        CleanupStack::Pop( aTransaction ); // CS: 1
        
        iCallback.HandleReceivedMessage( *from, content );
        CleanupStack::PopAndDestroy( from ); // CS: 0
        }
    
    // We no longer need aTransaction. Just delete it.
    delete aTransaction;
    aTransaction = NULL;
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::DoIncomingRequestL
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::DoIncomingRequestL (
    CSIPServerTransaction* aTransaction,
    CSIPDialog& aDialog )
    {
    // Ownership of aTransaction is transferred.
    CleanupStack::PushL( aTransaction );

    const CSIPRequestElements* reqE = aTransaction->RequestElements();
    TPtrC8 method = reqE->Method().DesC();

    // search the right request
    CSimpleRequest* req = iCallback.GetdDialogRequest( aDialog );
    if ( ! req )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("SipConnectionObserver: IncomingRequest ERROR **" ));
#endif

        // This is unknown response, send response 481 in order to unsubscribe the dialog.
        // See RFC3265
        CSIPResponseElements* resE = CSIPResponseElements::NewLC( KSimple481,
        SIPStrings::StringF( SipStrConsts::EPhraseCallTransactionDoesNotExist ) );
        aTransaction->SendResponseL( resE );
        // resE ownership is transferred
        CleanupStack::Pop( resE );
        CleanupStack::PopAndDestroy( aTransaction );
        return;
        }

    // send a response to PA in NOTIFY
    if ( aTransaction->ResponseAllowed()  )
        {
        if ( !method.CompareF( KSimpleNOTIFY ))
            {
            CSIPResponseElements* resE = CSIPResponseElements::NewLC( KSimpleOK,
            SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
            aTransaction->SendResponseL( resE );
            // resE ownership is transferred
            CleanupStack::Pop( resE );
#ifdef _DEBUG
            TSimpleLogger::Log(_L("SipConnectionObserver: response sent *" ));
#endif
            }
        }

    // Fill the data of the request
    const CSIPMessageElements& elems = reqE->MessageElements();
    TRAPD( err, DoHandleSrvrReqContentL( elems, req->Request() ));
    // Update expires when necessary
    if ( !err )
        {
        // This completes the client request when needed
        TRAP( err, iCallback.HandleDialogRequestL( elems, *req, method ));
        }

    CleanupStack::PopAndDestroy( aTransaction );
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::DoIncomingResponse
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::DoIncomingResponse(
    CSIPClientTransaction& aTransaction)
    {
    const CSIPResponseElements* elems = aTransaction.ResponseElements();
    // elems ownership is not transferred
    TUint status = elems->StatusCode();

    if ( status >= KSimpleOK )
        {
        // Final response received

        // Search the right request
        CSimpleRequest* req = iCallback.GetRequest( aTransaction );
        
        if ( req )
            {
            if( req->Match( CSimpleRequest::EReqSendIM ) )
                {
                TBool isModify = MSimpleEngineRequest::ESendIM ==
                    req->Request().RequestType() ? ETrue : EFalse;
                
                // Get retry-after header value
                TUint myRefresh = GetRetryAfter( elems );
                req->SetRetryAfter( myRefresh );
                
                TInt retCode = ConvertSipCode( status, myRefresh ?
                    ETrue: EFalse, isModify );
                req->SetStatus( status );
                req->Request().SetResponseMethod(
                    MSimpleEngineRequest::EStatus );
                TInt err( KErrNone );
                
                //  Complete the client request and change the request state
                req->Complete( err ? err : retCode );
                }
            else if ( req->Match( CSimpleRequest::EReqPublish ) )
                {
                TBool isModify = req->Request().RequestType() ==
                    MSimpleEngineRequest::EPublishModify ? ETrue : EFalse;
                
                // Get retry-after header value
                TUint myRefresh = GetRetryAfter( elems );
                req->SetRetryAfter( myRefresh );
                
                TInt retCode = ConvertSipCode( status, myRefresh ?
                    ETrue: EFalse, isModify );
                req->SetStatus( status );
                req->Request().SetResponseMethod(
                    MSimpleEngineRequest::EStatus );

                // Make response to a client when necessary, i.e. response to
                // the first PUBLISH.
                // This handles STOP publish also, since it's
                // actually a new publish request with expires=0 and ETag.
                const CSIPMessageElements& mElems = elems->MessageElements();
                TInt err( KErrNone );
                TRAP( err, iCallback.HandlePublishRespL( mElems, req ) );
                //  Complete the client request and change the request state
                req->Complete( err ? err : retCode );
                }
            else
                {
                // This is unknown response
                #ifdef _DEBUG
                    TSimpleLogger::Log( _L("SipConnectionObserver: IncomingResponse IGNORED") );
                #endif
                }
            }
        else
            {
            // Request not found
            #ifdef _DEBUG
                TSimpleLogger::Log( _L("SipConnectionObserver: IncomingResponse IGNORED") );
            #endif
            }
        }
    }

// -----------------------------------------------------------------------------
// CSimpleSipConnectionObserver::DoIncomingResponse
// -----------------------------------------------------------------------------
//
void CSimpleSipConnectionObserver::DoIncomingResponse(
    CSIPClientTransaction& aTransaction,
    CSIPDialogAssocBase& /*aDialogAssoc*/)
    {
    // ownership of elems is not transferred.
    const CSIPResponseElements* elems = aTransaction.ResponseElements();
    if ( !elems )
        {
        return;
        }
    // elems ownership is not transferred
    TUint status = elems->StatusCode();          

    if ( status >= KSimpleOK )
        {
        // Final response received

        // search the right request
        CSimpleRequest* req = iCallback.GetRequest( aTransaction );
        if ( ! req )
            {
#ifdef _DEBUG
           TSimpleLogger::Log(_L("SipConnectionObserver: Unknown response **" ));
#endif
            // This is unknown response
            return;
            }

        req->SetStatus( status );

        // update response method
        req->Request().SetResponseMethod( MSimpleEngineRequest::EStatus );
        req->Complete( ConvertSipCode( status, EFalse, EFalse ) );

        }
    }

// ----------------------------------------------------------
// CSimpleSipConnectionObserver::ConvertSipCode
// ----------------------------------------------------------
//
TInt CSimpleSipConnectionObserver::ConvertSipCode(
    TUint aSip, TBool aRetryAfter, TBool aPublishModify )
    {
    // The SIP status codes are converted to engine error codes
    if ( aSip < KSimpleOK )
        {
        return KErrNone;
        }
    if ( aPublishModify )
        {
        return ConvertSipModifyCode( aSip );
        }
    switch ( aSip )
        {
        case KSimpleOK: // 200
            return KErrNone;
        case KSimpleAccepted: // 202
            return KSimpleErrPending;
        case 404:  // Not Found
            return KSimpleErrNotFound;
        case 401: //  Unauthorized
        case 407: //  Proxy Authentication Required
            return KSimpleErrAuthorization;
        case 302: // Moved Temporarily
        case 408: // Request Timeout
        case 412: // Conditional Request Failed, RFC3903
        case 480: // Temporarily Unavailable
        case 481: // Call/Transaction Does Not Exist
        case 482: // Loop Detected
        case 483: // Too Many Hops
        case 486: // Busy Here
        case 487: // Request Terminated (BYE/CANCEL) ** should not happen
        case 491: // Request Pending
        case 503: // Service Unavailable
        case 504: // Server Time-out
            return KSimpleErrTemporary;
        case 413: // Request Entity Too Large
            if ( aRetryAfter )
                {
                return KSimpleErrTemporary;
                }
            else
                {
                return KSimpleErrPermanent;
                }
        default:
            return KSimpleErrPermanent;
        }
    }

// ----------------------------------------------------------
// CSimpleSipConnectionObserver::ConvertSipModifyCode
// ----------------------------------------------------------
//
TInt CSimpleSipConnectionObserver::ConvertSipModifyCode( 
    TUint aSip )
    {
    // The SIP status codes are converted to engine error codes
    if ( aSip < KSimpleOK )
        {
        return KErrNone;
        }
    switch ( aSip )
        {
        case KSimpleOK: // 200
            return KErrNone;
        case KSimpleAccepted: // 202
            return KSimpleErrPending;
        case 404:  // Not Found
            return KSimpleErrNotFound;
        case 401: //  Unauthorized
        case 407: //  Proxy Authentication Required
            return KSimpleErrAuthorization;
        case 400: // Bad request
        case 408: // Request Time Out
        case 415: // Unsupported media type
        case 508: // Server Timed-Out
            return KSimpleErrTemporary;
        default:
            return KSimpleErrPermanent;
        }
    }
// ----------------------------------------------------------
// CSimpleSipConnectionObserver::GetRetryAfter
// ----------------------------------------------------------
//
TUint CSimpleSipConnectionObserver::GetRetryAfter( const CSIPResponseElements* aElems )
    {
    TUint ret(0);

    // aElems must exist
    const CSIPMessageElements& mes = aElems->MessageElements();

    // Scan user headers
    const RPointerArray<CSIPHeaderBase>& userHs = mes.UserHeaders();
    for ( TInt i = 0; i < userHs.Count(); i++ )
        {
        const CSIPHeaderBase* header = userHs[ i ];
        if ( header->Name() == SIPStrings::StringF( SipStrConsts::ERetryAfterHeader ))
            {
            CSIPRetryAfterHeader* retryH = (CSIPRetryAfterHeader*)header;
            ret = retryH->RetryAfter();
            }
        else
            {
            // We ignore other than Retry-After header
            }
        }

    return ret;
    }
