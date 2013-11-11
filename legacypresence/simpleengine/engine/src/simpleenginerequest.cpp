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
* Description:    Simple Engine request
*
*/




// INCLUDE FILES

#include <e32std.h>

// own simple
#include "simplesipconnection.h"
#include "simplecommon.h"
#include "simpleenginerequest.h"
#include "msimpleenginerequest.h"
#include "msimpleowncallback.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleEngineRequest::CSimpleEngineRequest
// ----------------------------------------------------------
//
CSimpleEngineRequest::CSimpleEngineRequest(
    MSimpleOwnCallback& aEngine,
    MSimpleEngineRequest::TSimpleRequest aType,
    TInt aOpId )
: CActive( CActive::EPriorityHigh ), iType( aType ),
  iOpId( aOpId ), iEngine( aEngine ), iRefresh( ETrue ),
  iAux(0), iDestroyedPtr( NULL )
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::~CSimpleEngineRequest
// ----------------------------------------------------------
//
CSimpleEngineRequest::~CSimpleEngineRequest()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: destructor ereq=%d" ), (TInt)this);
#endif

    if ( iDestroyedPtr )
        {
        // We are called inside callback
        *iDestroyedPtr = ETrue;
        iDestroyedPtr = NULL;
        }
        
    Cancel();
    delete iResponseData;   
    delete iRemoteURI;
    delete iRequestContentType;
    delete iResponseContentType;
    delete iResponseBoundary;
    delete iResponseStart;
    delete iETag;
    delete iRecipientId;
    }

// -----------------------------------------------------------------------------
// CSimpleEngineRequest::Destroy
// -----------------------------------------------------------------------------
void CSimpleEngineRequest::Destroy()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: Destroy ereq=%d" ), (TInt)this);
#endif
    iLink.Deque();
    delete this;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::NewL
// ----------------------------------------------------------
//
CSimpleEngineRequest* CSimpleEngineRequest::NewL(
    MSimpleOwnCallback& aEngine,
    TSimpleRequest aType,
    TInt aOpId )
    {
    CSimpleEngineRequest* self = new (ELeave) CSimpleEngineRequest( aEngine, aType, aOpId );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: NewL ereq=%d" ), (TInt)self);
#endif
    return self;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetRequestData
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRequestData( TPtrC8 aData )
    {
    iRequestData.Set(aData);
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetRefreshTime
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRefreshTime( TUint aTime )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: SetRefreshTime %d" ), aTime);
#endif
    iRefreshTime = aTime;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetRemoteURIL
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRemoteURIL( const TDesC8& aURI )
    {
    delete iRemoteURI;
    iRemoteURI = NULL;
    iRemoteURI = aURI.AllocL();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::RequestType
// ----------------------------------------------------------
//
MSimpleEngineRequest::TSimpleRequest CSimpleEngineRequest::RequestType()
    {
    return iType;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ModifyType
// ----------------------------------------------------------
//
void CSimpleEngineRequest::ModifyType( MSimpleEngineRequest::TSimpleRequest aType )
    {
    iType = aType;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::Complete
// ----------------------------------------------------------
//
void CSimpleEngineRequest::Complete( TInt aStatus )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: COMPLETE ereq=%d aStatus=%d" ),
        (TInt)this, aStatus );
#endif
   // Yield to scheduler in order to "break" a call stack before
   // continuing response handling.

    // RunL will handle the response further
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* s = &iStatus;
    User::RequestComplete( s, aStatus );
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::RequestContentType
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::RequestContentType()
    {
    return iRequestContentType ? iRequestContentType->Des() : TPtrC8();
    }
    
// ----------------------------------------------------------
// CSimpleEngineRequest::SetRequestContentTypeL
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRequestContentTypeL( const TDesC8& aData )
    {
    delete iRequestContentType;
    iRequestContentType = NULL;
    iRequestContentType = aData.AllocL(); 
    }    

// ----------------------------------------------------------
// CSimpleEngineRequest::RequestData
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::RequestData()
    {
    return iRequestData;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseContentType(
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetResponseContentType( HBufC8* aData )
    {
    delete iResponseContentType;
    iResponseContentType = NULL;
    iResponseContentType = aData;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseContentType(
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetResponseBoundaryL( const TDesC8& aData )
    {
    delete iResponseBoundary;
    iResponseBoundary = NULL;
    iResponseBoundary = aData.AllocL();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseContentType(
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetResponseStartL( const TDesC8& aData )
    {
    delete iResponseStart;
    iResponseStart = NULL;
    iResponseStart = aData.AllocL();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseDataL
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetResponseDataL( const TDesC8& aData )
    {
    delete iResponseData;
    iResponseData = NULL;
    iResponseData = aData.AllocL();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetRecipientL
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRecipientL( const TDesC8& aRecipientId )
    {
    delete iRecipientId;
    iRecipientId = NULL;
    iRecipientId = aRecipientId.AllocL();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseMethod
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetResponseMethod( TSimpleSIPResponse aMethod )
    {
    iResponseMethod = aMethod;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseMethod
// ----------------------------------------------------------
//
MSimpleEngineRequest::TSimpleSIPResponse CSimpleEngineRequest::ResponseMethod( )
    {
    return iResponseMethod;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseContentType
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::ResponseContentType()
    {
    return iResponseContentType ? iResponseContentType->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseBoundary
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::ResponseBoundary()
    {
    return iResponseBoundary? iResponseBoundary->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseStart
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::ResponseStart()
    {
    return iResponseStart ? iResponseStart->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseData
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::ResponseData()
    {
    return iResponseData ? iResponseData->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::Recipient
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::Recipient()
    {
    return iRecipientId ? iRecipientId->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::RefreshTime
// ----------------------------------------------------------
//
TUint CSimpleEngineRequest::RefreshTime()
    {
    return iRefreshTime;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::RemoteURI
// ----------------------------------------------------------
//
TPtrC8 CSimpleEngineRequest::RemoteURI()
    {
    return iRemoteURI ? iRemoteURI->Des() : TPtrC8();
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::IsRefresh
// ----------------------------------------------------------
//
TBool CSimpleEngineRequest::IsRefresh()
    {
    return iRefresh;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetRefresh
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetRefresh( TBool aRefresh )
    {
    iRefresh = aRefresh;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::OpId
// ----------------------------------------------------------
//
TInt CSimpleEngineRequest::OpId( )
    {
    return iOpId;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::Aux
// ----------------------------------------------------------
//
TInt CSimpleEngineRequest::Aux( )
    {
    return iAux;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetAux
// ----------------------------------------------------------
//
void CSimpleEngineRequest::SetAux( TInt aData )
    {
    iAux = aData;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::SetResponseReason
// ----------------------------------------------------------
void CSimpleEngineRequest::SetResponseReason( TSimpleResponseReason aReason )
    {
    iReason = aReason;
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::ResponseReason
// ----------------------------------------------------------
MSimpleEngineRequest::TSimpleResponseReason CSimpleEngineRequest::ResponseReason()
    {
    return iReason;
    }
    
// -----------------------------------------------------------------------------
// CSimpleEngineRequest::SetETagL
// -----------------------------------------------------------------------------
void CSimpleEngineRequest::SetETagL( const TDesC8& aTag )
    {
    delete iETag;
    iETag = NULL;
    iETag = aTag.AllocL();
    }

// -----------------------------------------------------------------------------
// CSimpleEngineRequest::ETag
// -----------------------------------------------------------------------------
TPtrC8 CSimpleEngineRequest::ETag()
    {
    return iETag ? iETag->Des() : TPtrC8();    
    }

// -----------------------------------------------------------------------------
// CSimpleEngineRequest::RunL
// -----------------------------------------------------------------------------
//
void CSimpleEngineRequest::RunL()
    {
    TInt stat = iStatus.Int();
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: RunL type=%d status=%d ereq=%d"),iType,stat, (TInt)this );
#endif

    // Set the member to point to stack variable
    TBool   destroyed( EFalse );
    iDestroyedPtr = &destroyed;

    // Call client callback method(s)
    iEngine.Complete( iOpId, stat, *this );
    
    // Check whether this entity is destroyed in Complete method.
    if ( destroyed )
        {
        return;
        }
                    
    // Reset response data
    delete iResponseData;
    iResponseData = NULL;
    delete iResponseContentType;
    iResponseContentType = NULL;
    delete iResponseBoundary;
    iResponseBoundary = NULL;
    delete iResponseStart;
    iResponseStart = NULL;
    delete iRecipientId;
    iRecipientId = NULL;
    iResponseMethod = EUnknownResponse;
    iReason = ENoReason;    
    
    iDestroyedPtr = NULL;    
    }

// ----------------------------------------------------------
// CSimpleEngineRequest::DoCancel
// ----------------------------------------------------------
//
void CSimpleEngineRequest::DoCancel()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("EngineRequest: DoCancel ereq=%d" ), (TInt)this );
#endif
    }

