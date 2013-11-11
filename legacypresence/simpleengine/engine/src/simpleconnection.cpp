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

// own simple
#include "simplesipconnection.h"
#include "simplecommon.h"
#include "simpleenginerequest.h"
#include "msimpleconnectionobserver.h"
#include "simpleconnection.h"
#include "simpleengineutils.h"
#include "msimpleenginerequest.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleConnection::CSimpleConnection
// ----------------------------------------------------------
//
CSimpleConnection::CSimpleConnection( MSimpleConnectionObserver& aObserver )
: iState( EInactive ),
  iObserver( aObserver ),
  iOpId(0),
  iRequestList( CSimpleEngineRequest::LinkOffset())
    {
    }

// ----------------------------------------------------------
// CSimpleConnection::~CSimpleConnection
// ----------------------------------------------------------
//
CSimpleConnection::~CSimpleConnection()
    {
    delete iConnection;
    DeleteRequests();
    }

// ----------------------------------------------------------
// CSimpleConnection::ConstructL
// ----------------------------------------------------------
//
void CSimpleConnection::ConstructL()
    {
    // Create CSimpleSipConnection entity in simplesiputils.dll
    iConnection = CSimpleSipConnection::NewL();
    // Set operation id ranges
    iLimitLowerValue = TSimpleEngineUtils::OpIdRange();
    iLimitUpperValue = iLimitLowerValue + KRangeSize;
    iOpId = iLimitLowerValue;

    // start to listen to status events
    IncreaseOpId();
    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::EListenEvents, iOpId );
    CleanupStack::PushL( req );
    iConnection->DispatchReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );
    }

// ----------------------------------------------------------
// CSimpleConnection::NewL
// ----------------------------------------------------------
//
CSimpleConnection* CSimpleConnection::NewL(
    MSimpleConnectionObserver& aObserver )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: NewL" ));
#endif
    CSimpleConnection* self = new (ELeave) CSimpleConnection( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleConnection::Connection
// ----------------------------------------------------------
//
CSimpleSipConnection* CSimpleConnection::Connection()
    {
    return iConnection;
    }

// ----------------------------------------------------------
// CSimpleConnection::Close
// ----------------------------------------------------------
//
void CSimpleConnection::Close( )
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleConnection::LoginL
// ----------------------------------------------------------
//
TInt CSimpleConnection::LoginL(  )
    {
    return LoginL( 0 );
    }

// ----------------------------------------------------------
// CSimpleConnection::LoginL
// ----------------------------------------------------------
//
TInt CSimpleConnection::LoginL( TInt aId  )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: LoginL aId=%d" ), aId);
#endif
    // check state
    if ( iState != EInactive )
        {
        User::Leave(KErrNotReady);
        }

    IncreaseOpId();

    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::ERegister, iOpId );
    req->SetAux( aId );
    CleanupStack::PushL( req );
    iConnection->DispatchReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );
    SetConnectionStatus( EIdle );
    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleConnection::ConnectionStatus
// ----------------------------------------------------------
//
MSimpleConnection::TSimpleState CSimpleConnection::ConnectionStatus()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: ConnectionStatus returns %d" ), iState);
#endif
    return iState;
    }
    
// ----------------------------------------------------------
// CSimpleConnection::CurrentSIPIdentityL
// ----------------------------------------------------------
//    
TPtrC8 CSimpleConnection::CurrentSIPIdentityL()
    {
    return iConnection->CurrentSIPIdentityL();    
    }

// ----------------------------------------------------------
// CSimpleConnection::Complete
// ----------------------------------------------------------
//
void CSimpleConnection::Complete(
    TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: Complete opid=%d status=%d" ),
        aOpId, aStatus );
#endif
    // check the SIP state from another DLL
    TSimpleSipState st = iConnection->SIPState();
    MSimpleConnection::TSimpleState oldState = ConnectionStatus();
    MSimpleConnection::TSimpleState newState = ConvertState( st );
    SetConnectionStatus( newState );

    if ( aReq.RequestType() != MSimpleEngineRequest::EListenEvents )
        {
        // delete corresponding request from another DLL too.
        aReq.ModifyType( MSimpleEngineRequest::EDestroy );
        TRAP_IGNORE( iConnection->DispatchReqL( aReq ));
        // delete request from this DLL.
        aReq.Destroy();
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: call  RequestCompleteL opid=%d status=%d" ),
        aOpId, aStatus );
#endif
        TRAP_IGNORE( iObserver.RequestCompleteL( aOpId, aStatus ) );
        }
    else
        {
        // This is status event
        if ( oldState != newState )
            {
#ifdef _DEBUG
            TSimpleLogger::Log(_L("SimpleConnection: call ConnectionStatusL state=%d" ),
            newState );
#endif
            TRAP_IGNORE( iObserver.ConnectionStatusL( newState ) );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSimpleConnection::DeleteRequests
// -----------------------------------------------------------------------------
void CSimpleConnection::DeleteRequests()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: DeleteRequests" ));
#endif
    // Delete all buffered transaction requests
    TDblQueIter<CSimpleEngineRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleEngineRequest* req = rIter;
        rIter++; //lint !e1757
        req->Destroy();
        }
    }

// ----------------------------------------------------------------------------
// CSimpleConnection::IncreaseOpId
// ----------------------------------------------------------------------------
void CSimpleConnection::IncreaseOpId( )
    {
    iOpId++;
    if( iOpId >= iLimitUpperValue )
        {
        iOpId = iLimitLowerValue;
        }
    }

// ----------------------------------------------------------------------------
// CSimpleConnection::ConvertState
// ----------------------------------------------------------------------------
MSimpleConnection::TSimpleState CSimpleConnection::ConvertState( TSimpleSipState aState )
    {
    switch( aState )
        {
        case ESimpleSipIdle:
            return MSimpleConnection::EIdle;
        case ESimpleSipActive:
            return MSimpleConnection::EActive;
        case ESimpleSipSuspend:
            return ESuspend;
        case ESimpleSipInactive:
            return EInactive;
        case ESimpleSipUnavailable:
        default:
            return EUnavailable;
        }; //lint !e960
    }

// ----------------------------------------------------------------------------
// CSimpleConnection::SetConnectionStatus
// ----------------------------------------------------------------------------
void CSimpleConnection::SetConnectionStatus( TSimpleState aState )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleConnection: SetConnectionStatus %d" ), aState );
#endif
    iState = aState;
    }

