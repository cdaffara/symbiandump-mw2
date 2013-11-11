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
#include "simpleconnection.h"
#include "simplecommon.h"
#include "simpleenginerequest.h"
#include "simplesipconnection.h"
#include "simpleengineutils.h"
#include "simpleclient.h"
#include "simpleerrors.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleClient::CSimpleClient
// ----------------------------------------------------------
//
CSimpleClient::CSimpleClient(
    MSimpleConnection& aConn )
: iConn( aConn ),
  iOpId(0),
  iSipStatus(0),
  iRetryAfterTime(0),
  iRequestList( CSimpleEngineRequest::LinkOffset()), 
  iRequest( MSimpleEngineRequest::ENone ),
  iDestroyedPtr( NULL )
    {
    }

// ----------------------------------------------------------
// CSimpleClient::~CSimpleClient
// ----------------------------------------------------------
//
CSimpleClient::~CSimpleClient()
    {
    DeleteRequests();
    
    if ( iDestroyedPtr )
        {
        // We are called inside callback
        *iDestroyedPtr = ETrue;
        iDestroyedPtr = NULL;
        }
    
    }

// ----------------------------------------------------------
// CSimpleClient::BaseConstructL
// ----------------------------------------------------------
//
void CSimpleClient::BaseConstructL()
    {
    iLimitLowerValue = TSimpleEngineUtils::OpIdRange();
    iLimitUpperValue = iLimitLowerValue + KRangeSize;
    iOpId = iLimitLowerValue;
    }

// ----------------------------------------------------------------------------
// CSimpleClient::IncreaseOpId
// ----------------------------------------------------------------------------
void CSimpleClient::IncreaseOpId( )
    {
    iOpId++;
    if( iOpId >= iLimitUpperValue )
        {
        iOpId = iLimitLowerValue;
        }
    }

// -----------------------------------------------------------------------------
// CSimpleClient::DeleteRequests
// -----------------------------------------------------------------------------
void CSimpleClient::DeleteRequests()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleClient: DeleteRequests this=%d" ), (TInt)this );
#endif
    // Delete all buffered transaction requests
    TDblQueIter<CSimpleEngineRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleEngineRequest* req = rIter;
        rIter++; //lint !e1757
        // delete corresponding request from another DLL too.
        req->ModifyType( MSimpleEngineRequest::EDestroy );
        TRAP_IGNORE( SendReqL( *req ) );
        // delete request from this DLL.
        req->Destroy();
        }
    }

// -----------------------------------------------------------------------------
// CSimpleClient::SearchRequests
// -----------------------------------------------------------------------------
CSimpleEngineRequest* CSimpleClient::SearchRequests( TInt aOpId )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("SimpleClient: SearchRequests this=%d" ), (TInt)this );
#endif
    // Delete all buffered transaction requests
    TDblQueIter<CSimpleEngineRequest> rIter( iRequestList );
    rIter.SetToFirst();

    while ( rIter )
        {
        CSimpleEngineRequest* req = rIter;
        rIter++; //lint !e1757
        if ( req->OpId() == aOpId )
            {
            return req;
            }
        }
    return (CSimpleEngineRequest*)NULL;
    }

// -----------------------------------------------------------------------------
// CSimpleClient::SendReqL
// -----------------------------------------------------------------------------
void CSimpleClient::SendReqL( MSimpleEngineRequest& aReq )
    {
    CSimpleConnection* conn = STATIC_CAST( CSimpleConnection*, &iConn); //lint !e826
    CSimpleSipConnection* engine = conn->Connection();
    engine->DispatchReqL( aReq );
    }

// ----------------------------------------------------------
// CSimpleClient::ResponseReason
// ----------------------------------------------------------
//
TInt CSimpleClient::ResponseReason(
    MSimpleEngineRequest& aR )
    {
    switch ( aR.ResponseReason() )
        {
        case MSimpleEngineRequest::ENoReason:
            return KErrNone;

        case MSimpleEngineRequest::ERejected:
            return KSimpleErrRejected;

        case MSimpleEngineRequest::ETimeout:
            return KSimpleErrTimeout;

        case MSimpleEngineRequest::EDeactivated:
            return KSimpleErrDeactivated;

        case MSimpleEngineRequest::EProbation:
            return KSimpleErrProbation;

        case MSimpleEngineRequest::ENoresource:
            return KSimpleErrNoresource;

        case MSimpleEngineRequest::EGiveup:
            return KSimpleErrGiveup;

        default:
            return KErrNone;
        }; //lint !e960
    }
    
// ----------------------------------------------------------
// CSimpleClient::DoSIPStatus
// ----------------------------------------------------------
//
TUint CSimpleClient::DoSIPStatus( )
    {
    return iSipStatus;    
    }
    
// ----------------------------------------------------------
// CSimpleClient::DoRetryAfter
// ----------------------------------------------------------
//
TUint CSimpleClient::DoRetryAfter( )
    {
    return iRetryAfterTime;
    }    
    
// ----------------------------------------------------------
// CSimpleClient::GetSIPStatus
// ----------------------------------------------------------
//
void CSimpleClient::GetSIPStatus( TInt aOpId )
    {    
    CSimpleConnection* conn = STATIC_CAST( CSimpleConnection*, &iConn); //lint !e826
    CSimpleSipConnection* engine = conn->Connection();
    iSipStatus = engine->SIPStatus( aOpId, iRetryAfterTime );
    }    
