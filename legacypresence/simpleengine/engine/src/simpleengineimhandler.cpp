/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  SIMPLE ENGINE IM Handler
*
*/


#include "simpleengineimhandler.h"
#include "simpleenginerequest.h"
#include "simpleinstantmsgobserver.h"
#include "simplesipconnection.h"
#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CSimpleEngineImHandler::CSimpleEngineImHandler
// ---------------------------------------------------------------------------
//
CSimpleEngineImHandler::CSimpleEngineImHandler( MSimpleConnection& aConn,
    MSimpleInstantMsgObserver& aObserver ) : CSimpleClient( aConn ),
    iObserver( aObserver )
    {
    
    }

// ---------------------------------------------------------------------------
// CSimpleEngineImHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CSimpleEngineImHandler::ConstructL()
    {
    BaseConstructL();
    
    // Enable instant message receiving
    ReceiveInstantMessageL();
    }

// ---------------------------------------------------------------------------
// CSimpleEngineImHandler::NewL
// ---------------------------------------------------------------------------
//
CSimpleEngineImHandler* CSimpleEngineImHandler::NewL(
    MSimpleConnection& aConn,
    MSimpleInstantMsgObserver& aObserver )
    {
    CSimpleEngineImHandler* self = new (ELeave) CSimpleEngineImHandler(
        aConn, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log( _L("CSimpleEngineImHandler::NewL this=%d"),
        (TInt)self );
#endif
    return self;
    }

// ---------------------------------------------------------------------------
// CSimpleEngineImHandler::~CSimpleEngineImHandler
// ---------------------------------------------------------------------------
//
CSimpleEngineImHandler::~CSimpleEngineImHandler()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L("CSimpleEngineImHandler::~CSimpleEngineImHandler this=%d"),
        (TInt)this );
#endif
    }

// ---------------------------------------------------------------------------
// From class MSimpleInstantMessage
// Send instant message
// ---------------------------------------------------------------------------
//
TInt CSimpleEngineImHandler::SendInstantMessage( TPtrC8 aMsgContent,
    TPtrC8 aRecipient )
    {
#ifdef _DEBUG
    TSimpleLogger::Log( _L("CSimpleEngineImHandler::SendInstantMessage") );
#endif
    
    IncreaseOpId();
    TRAPD( error, DoSendInstantMessageRequestL( aMsgContent, aRecipient ) )
    
    if ( error )
        {
        return error;
        }
    
    return iOpId;
    }

// ----------------------------------------------------------
// From class MSimpleInstantMessage
// CSimpleEngineImHandler::Close
// ----------------------------------------------------------
//
void CSimpleEngineImHandler::Close()
    {
    delete this;
    }

// ---------------------------------------------------------------------------
// From class CSimpleClient
// Complete the open client request
// ---------------------------------------------------------------------------
//
void CSimpleEngineImHandler::Complete( TInt aOpId, TInt aStatus,
    MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleEngineImHandler::Complete opid=%d, status=%d - Start"),
            aOpId, aStatus );
#endif
    
    if( MSimpleEngineRequest::EReceiveIM == aReq.RequestType() )
        {
        if( !aStatus )
            {
            iObserver.HandleReceiveTextMessage( aReq.Recipient(),
                aReq.ResponseData() );
            }
        // Open receive request was already deleted from SIP Utils side
        // Activate receiving again
        TRAP_IGNORE( ReceiveInstantMessageL() )
        }
    else if( MSimpleEngineRequest::ESendIM == aReq.RequestType() )
        {
        iObserver.SendInstantMsgReqComplete( aOpId, aStatus );
        
        // Delete the request as not needed anymore
        
        iRequest = MSimpleEngineRequest::ENone;
        // Delete corresponding request from another DLL too.
        aReq.ModifyType( MSimpleEngineRequest::EDestroy );
        TRAP_IGNORE( SendReqL( aReq ) );
        // delete request from this DLL.
        aReq.Destroy();
        }
    else
        {
        // Unknown request
        #ifdef _DEBUG
            TSimpleLogger::Log(
                _L( "CSimpleEngineImHandler::Complete - Unknown request") );
        #endif
        }
    
#ifdef _DEBUG
    TSimpleLogger::Log( _L( "CSimpleEngineImHandler::Complete - End") );
#endif
    }

// ---------------------------------------------------------------------------
// Do Send Instant Message request
// ---------------------------------------------------------------------------
//
void CSimpleEngineImHandler::DoSendInstantMessageRequestL( TPtrC8 aMsgContent,
    TPtrC8 aRecipient )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleEngineImHandler::DoSendInstantMessageRequestL - Start") );
#endif
    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL( *this,
        MSimpleEngineRequest::ESendIM, iOpId );
    CleanupStack::PushL( req );
    
    req->SetRefresh( ETrue );
    req->SetRequestData( aMsgContent );
    req->SetRecipientL( aRecipient );
    
    // send a request to engine DLL
    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );
    iRequest = MSimpleEngineRequest::ESendIM;
    iSipStatus = KErrNone;
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleEngineImHandler::DoSendInstantMessageRequestL - End") );
#endif
    }

// ---------------------------------------------------------------------------
// Receive instant message
// ---------------------------------------------------------------------------
//
void CSimpleEngineImHandler::ReceiveInstantMessageL()   
    {
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleEngineImHandler::ReceiveInstantMessageL - Start") );
#endif
    IncreaseOpId();
    
    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL( *this,
        MSimpleEngineRequest::EReceiveIM, iOpId );
    CleanupStack::PushL( req );
    
    req->SetRefresh( ETrue );
    // Send a request to engine DLL
    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );
    iRequest = MSimpleEngineRequest::EReceiveIM;
    iSipStatus = KErrNone;
#ifdef _DEBUG
    TSimpleLogger::Log(
        _L( "CSimpleEngineImHandler::ReceiveInstantMessageL - End") );
#endif 
    }
