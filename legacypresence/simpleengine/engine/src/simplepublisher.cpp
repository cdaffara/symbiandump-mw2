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
#include "msimpledocument.h"
#include "msimplepublishobserver.h"
#include "msimpleetagobserver.h"
#include "simplepublisher.h"


#include "simplesipconnection.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif

const TInt KSimpleExpandSize = 512;

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimplePublisher::CSimplePublisher
// ----------------------------------------------------------
//
CSimplePublisher::CSimplePublisher(
    MSimpleConnection& aConn,
    MSimplePublishObserver& aObserver )
: CSimpleClient( aConn ),
  iPublished( EFalse ),
  iObserver( aObserver ), iBuffer(NULL), iETagObserver( NULL )
    {
    }

// ----------------------------------------------------------
// CSimplePublisher::~CSimplePublisher
// ----------------------------------------------------------
//
CSimplePublisher::~CSimplePublisher()
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: Destructor this=%d" ), (TInt)this );
#endif    
    delete iBuffer;
    delete iETag;
    }

// ----------------------------------------------------------
// CSimplePublisher::ConstructL
// ----------------------------------------------------------
//
void CSimplePublisher::ConstructL()
    {
    BaseConstructL();
    iBuffer = CBufFlat::NewL(KSimpleExpandSize);
    }

// ----------------------------------------------------------
// CSimplePublisher::NewL
// ----------------------------------------------------------
//
CSimplePublisher* CSimplePublisher::NewL(
    MSimpleConnection& aConn,
    MSimplePublishObserver& aObserver )
    {
    CSimplePublisher* self = new (ELeave) CSimplePublisher(
        aConn, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: NewL this=%d" ), (TInt)self );
#endif    
    return self;
    }

// ----------------------------------------------------------
// CSimplePublisher::SIPStatus
// ----------------------------------------------------------
//
TUint CSimplePublisher::SIPStatus()
    {
    return DoSIPStatus();
    }
    
// ----------------------------------------------------------
// CSimplePublisher::SIPRetryAfter
// ----------------------------------------------------------
//
TUint CSimplePublisher::SIPRetryAfter()
    {
    return DoRetryAfter();
    }    

// ----------------------------------------------------------
// CSimplePublisher::Connection
// ----------------------------------------------------------
//
const MSimpleConnection& CSimplePublisher::Connection()
    {
    return iConn;
    }

// ----------------------------------------------------------
// CSimplePublisher::StartPublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::StartPublishL( MSimpleDocument& aDocument,
    TBool aRefresh )
    {

    IncreaseOpId();

#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: StartPublishL opid=%d" ), iOpId );
#endif

    if ( iRequest != MSimpleEngineRequest::ENone )
        {
        User::Leave( KErrInUse );
        }
        
    return DoStartPublishL( aDocument, aRefresh, KNullDesC8 );        
    }
    
// ----------------------------------------------------------
// CSimplePublisher::ContinuePublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::ContinuePublishL( MSimpleDocument& aDocument,
    TBool aRefresh, const TDesC8& aETag )
    {
    
    IncreaseOpId();

#ifdef _DEBUG
    TBuf<100> myETag;
    myETag.Copy( aETag );
    TSimpleLogger::Log(_L("Publisher: ContinuePublishL opid=%d ETag=%S" ), iOpId, &myETag );
#endif 

    return DoStartPublishL( aDocument, aRefresh, aETag ); 
    
    }

// ----------------------------------------------------------
// CSimplePublisher::ModifyPublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::ModifyPublishL( MSimpleDocument& aDocument )
    {

#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: ModifyPublishL opid=%d" ), iOpId );
#endif

    // use the old opid and request
    CSimpleEngineRequest* req = SearchRequests( iOpId );
    if ( !req )
        {
        User::Leave( KErrNotFound );
        }
    req->ModifyType( MSimpleEngineRequest::EPublishModify );

    // add request data
    StreamDocumentL( *req, aDocument );
    
    // Set MIME type of the request
    RPointerArray<MSimpleContent> contents;
    CleanupClosePushL( contents );     
    aDocument.GetDirectContentsL( contents );
    if ( contents.Count() )
        {                                            
        req->SetRequestContentTypeL( KSimpleMultipartType );
        }
    else
        {
        req->SetRequestContentTypeL( KSimpleDocumentType );
        }
    CleanupStack::PopAndDestroy( &contents );    

    // send a request to engine DLL
    SendReqL( *req );

    iRequest = MSimpleEngineRequest::EPublishModify;
    iSipStatus = 0;
    return iOpId;
    }

// ----------------------------------------------------------
// CSimplePublisher::StopPublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::StopPublishL()
    {

#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: StopPublishL opid=%d" ), iOpId );
#endif

    // use the old opid and request
    CSimpleEngineRequest* req = SearchRequests( iOpId );
    if ( !req )
        {
        User::Leave( KErrNotFound );
        }
    req->ModifyType( MSimpleEngineRequest::EStopPublish );

    // send a request to engine DLL
    SendReqL( *req );

    iRequest = MSimpleEngineRequest::EStopPublish;
    iPublished = EFalse;
    iSipStatus = 0;
    return iOpId;
    }
    
// ----------------------------------------------------------
// CSimplePublisher::StopPublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::StopPublishL( const TDesC8& aETag )
    {

#ifdef _DEBUG
    TBuf<100> myETag;
    myETag.Copy( aETag );
    TSimpleLogger::Log(_L("Publisher: StopPublishL opid=%d ETag=%S" ), iOpId, &myETag );
#endif

    // use the old opid and request
    CSimpleEngineRequest* req = SearchRequests( iOpId );

    if ( iRequest != MSimpleEngineRequest::ENone && !req )
        {
        User::Leave( KErrNotFound );
        }
    else if ( !req )
        {
        IncreaseOpId();
        // create a new request when starting from scratch
        req = CSimpleEngineRequest::NewL(
            *this, MSimpleEngineRequest::EStopPublish, iOpId );
        iRequestList.AddLast( *req );              
        }
        
    req->ModifyType( MSimpleEngineRequest::EStopPublish );
    req->SetETagL( aETag );

    // send a request to engine DLL
    SendReqL( *req );

    iRequest = MSimpleEngineRequest::EStopPublish;
    iPublished = EFalse;
    iSipStatus = 0;
    return iOpId;
    }    
    
// ----------------------------------------------------------
// CSimplePublisher::SIPETag
// ----------------------------------------------------------
TPtrC8 CSimplePublisher::SIPETag()
    {
    return iETag ? iETag->Des() : TPtrC8();
    }
    
// ----------------------------------------------------------
// CSimplePublisher::SetSIPETagObserver
// ----------------------------------------------------------
void CSimplePublisher::SetSIPETagObserver( MSimpleETagObserver* aObs )
    {
    iETagObserver = aObs;     
    }

// ----------------------------------------------------------
// CSimplePublisher::Close
// ----------------------------------------------------------
//
void CSimplePublisher::Close( )
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimplePublisher::Complete
// ----------------------------------------------------------
//
void CSimplePublisher::Complete(
    TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq )
    {
#ifdef _DEBUG
    TSimpleLogger::Log(_L("Publisher: Complete opid=%d, status=%d" ),
    aOpId, aStatus );
#endif

    MSimpleEngineRequest::TSimpleRequest origRequest = iRequest;
    TBool myTerminated( EFalse );

    GetSIPStatus( aOpId );
    
    // Allocate ETag when needed and call callback
    if ( SIPETag().CompareF( aReq.ETag() ))
        {
        delete iETag;
        iETag = NULL;
        TRAPD( errMem, iETag = aReq.ETag().AllocL() );  
        if ( iETagObserver && !errMem )
            {
            TRAP_IGNORE( iETagObserver->NewETagL( iETag->Des() ));                                  
            }
        }
        
    if ( aReq.ResponseMethod() == MSimpleEngineRequest:: EStatusETag )
        {
        // If only ETag was updated then all is done this time.
        return;
        }

    // Reset data buffer
    iBuffer->Reset();

    // Set the member to point to stack variable
    TBool   destroyed( EFalse );
    iDestroyedPtr = &destroyed;

    if ( !aStatus && origRequest == MSimpleEngineRequest::EStartPublish )
        {
        iPublished = ETrue;
        }

    // Select the right callback method
    if ( iPublished &&
         origRequest == MSimpleEngineRequest::EStartPublish &&
         aStatus )
        {
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Publisher: call PublishTerminatedL opid=%d" ), aOpId );
#endif
        myTerminated = ETrue;
        TRAP_IGNORE( iObserver.PublishTerminatedL( aOpId ));
        // Check whether an application has called destructor in callback method.
        // Destructor will handle deletion of all the open requests.
        if ( destroyed )
            {
            return;
            }
        }
    else
        {
        // If modify fails then it does not always mean the whole
        // publish is failed.
#ifdef _DEBUG
        TSimpleLogger::Log(_L("Publisher: call PublishReqCompleteL opid=%d status=%d" ),
            aOpId, aStatus );
#endif
        TRAP_IGNORE( iObserver.PublishReqCompleteL( aOpId, aStatus ) );
        // Check whether an application has called destructor in callback method.
        // Destructor will handle deletion of all the open requests.
        if ( destroyed )
            {
            return;
            }
        if ( aStatus == KErrCompletion || aStatus == KErrDisconnected )
            {
            myTerminated = ETrue;            
            // KErrCompletion error code is used when publication is terminated.
            TRAP_IGNORE( iObserver.PublishTerminatedL( aOpId ) );
            // Check whether an application has called destructor in callback method.
            // Destructor will handle deletion of all the open requests.
            if ( destroyed )
                {
                return;
                }
            }
        }

    // Delete a request when not needed anymore.
    if (( aStatus != KErrNone && origRequest == MSimpleEngineRequest::EStartPublish ) ||
          origRequest == MSimpleEngineRequest::EStopPublish  ||
          myTerminated )
        {
        iRequest = MSimpleEngineRequest::ENone;
        iPublished = EFalse;

        // delete corresponding request from another DLL too.
        aReq.ModifyType( MSimpleEngineRequest::EDestroy );
        TRAP_IGNORE( SendReqL( aReq ));
        // delete request from this DLL.
        aReq.Destroy();
        }

    // Set iRequest back to EStartPublish after Modify request.
    // This helps to handle error situations above.
    else if ( origRequest == MSimpleEngineRequest::EPublishModify )
        {
        iRequest = MSimpleEngineRequest::EStartPublish;
        aReq.ModifyType( MSimpleEngineRequest::EStartPublish );        
        }

    iDestroyedPtr = NULL; 
    }

// ----------------------------------------------------------
// CSimplePublisher::StreamDocumentL
// ----------------------------------------------------------
//
void CSimplePublisher::StreamDocumentL(
    CSimpleEngineRequest& aReq,
    MSimpleDocument& aDocument )
    {            
    // add request data
    // externalize the document a stream
    iBuffer->Reset();
    RBufWriteStream stream( *iBuffer );
    stream.Open( *iBuffer );
    aDocument.ExternalizeL( stream );
    stream.Close();
    aReq.SetRequestData( iBuffer->Ptr(0) );
    }
    
// ----------------------------------------------------------
// CSimplePublisher::DoStartPublishL
// ----------------------------------------------------------
//
TInt CSimplePublisher::DoStartPublishL( MSimpleDocument& aDocument,
    TBool aRefresh, const TDesC8& aETag )
    {
    if ( iRequest != MSimpleEngineRequest::ENone )
        {
        User::Leave( KErrInUse );
        }           

    CSimpleEngineRequest* req = CSimpleEngineRequest::NewL(
        *this, MSimpleEngineRequest::EStartPublish, iOpId );
    CleanupStack::PushL( req );
    req->SetRefresh( aRefresh );
    // add remote uri
    const TDesC8* p8 = aDocument.EntityURI();
    req->SetRemoteURIL( *p8 );
    // set ETag
    req->SetETagL( aETag );

    // add request data
    StreamDocumentL( *req, aDocument );
    
    // Set MIME type of the request
    RPointerArray<MSimpleContent> contents;
    CleanupClosePushL( contents );     
    aDocument.GetDirectContentsL( contents );
    if ( contents.Count() )
        {                                            
        req->SetRequestContentTypeL( KSimpleMultipartType );
        }
    else
        {
        req->SetRequestContentTypeL( KSimpleDocumentType );
        }
    CleanupStack::PopAndDestroy( &contents );        

    // send a request to engine DLL
    SendReqL( *req );
    iRequestList.AddLast( *req );
    CleanupStack::Pop( req );

    iRequest = MSimpleEngineRequest::EStartPublish;
    iSipStatus = 0;
    return iOpId;
    }    


