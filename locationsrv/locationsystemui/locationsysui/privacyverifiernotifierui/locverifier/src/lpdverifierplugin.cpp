/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  Verifier plug-in which derives from CPosPrivacyNotifier.
 *
 */

// INCLUDE FILES

#include "lpdverifierplugin.h"

#include "lpdrequestao.h"
#include "lpdrequestorprocessor.h"
#include "lpdverifierquerylauncher.h"
#include "lpdnotifierquerylauncher.h"
#include "locverifierdlgdebug.h"
#include "locutilsdebug.h"

#include <eiknotapi.h>
#include <lbs/epos_rposrequestorstack.h>
#include <epos_csuplsettingsconstants.h>

// INCLUDE FILES
#include <s32mem.h>



const TInt KPrivSrvSecureId = 0x10281D45;
// CONSTANTS

// Unnamed namespace for local definitions

const MEikSrvNotifierBase2::TNotifierPriority KNotifierPriority =
        MEikSrvNotifierBase2::ENotifierPriorityHigh;
const TUid KNotifierChannel =
    {
    0x100065ac
    };
#ifdef _DEBUG
_LIT( KPanicText, "CLpdVerifierPlugin" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CLpdVerifierPlugin
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdVerifierPlugin::CLpdVerifierPlugin() :
    iCurrentRequest(KPosNullQNRequestId)

    {
    
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::ConstructL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::ConstructL" );
    BaseConstructL(KNotifierChannel, KNotifierPriority);
  
    iRtorProcessor = CLpdRequestorProcessor::NewL();
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::ConstructL" );    	
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdVerifierPlugin* CLpdVerifierPlugin::NewL()
    {
     LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::NewL" );
    CLpdVerifierPlugin* self = new (ELeave) CLpdVerifierPlugin;

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
     LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::NewL" );
    return self;
    }

// Destructor
CLpdVerifierPlugin::~CLpdVerifierPlugin()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::~CLpdVerifierPlugin" );
    // Destruction of this plugin should only occur only in shutdown
    // or in severe problem situation.

    // A very special scenario is that base class construction leaves and
    // this destructor is called. In that case CompleteAllRequests() causes
    // access violation (noticed this by checking source code of base class).
    if (NotifierBase())
        { // base class has been fully constructed, method call is safe
        CompleteAllRequests(KErrGeneral);
        }

    // It is enough to delete queries so when don't get callbacks.
    FreeQueryResources();
    
    delete iRequestActiveObject;
    iRequestActiveObject = NULL; 
    delete iRtorProcessor;
    iRtorProcessor = NULL; 
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::~CLpdVerifierPlugin" );
    
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNewRequestL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNewRequestL(TPosQNRequestId /*aRequestId */ )
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNewRequestL" );
    
    // Check whether the notifier is already handling a request
    // If yes, do nothing for now.
    if (iRequestActiveObject)
        {
        return;
        }
    else
        {
        iRequestActiveObject = CLpdRequestAO::NewL(*this);
        iRequestActiveObject->ScheduleRequest();
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNewRequestL" );
    }


// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleRequestCancelled
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleRequestCancelled(TPosQNRequestId aRequestId)
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleRequestCancelled" );
    // Check whether the request is coming from Uikon Server. 
    // If not reject this request.
    if (!CheckClientSecureId(KPrivSrvSecureId))
        {
        return;
        }

    
    if (aRequestId != iCurrentRequest)
        {
        return;
        }

   
    if (iCurrentRequestType == EQuery)
        { // Verification was cancelled
        __ASSERT_DEBUG( iVerifierQuery, HandleDebugAssertError() );
        LOCVERIFIERDLGDEBUG( "Verification cancelled" );
        iVerifierQuery->Cancel();
        }
    else
        {
        // It must be a notification then, this case is not probable but
        // we can cancel the dialog if this would happen.
        __ASSERT_DEBUG( iCurrentRequestType == ENotification,
                HandleDebugAssertError() );
         __ASSERT_DEBUG( iNotifier, HandleDebugAssertError() );
        LOCVERIFIERDLGDEBUG( "Notification cancelled" );
        iNotifier->Cancel();
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleRequestCancelled" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleAllRequestCancelled
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleAllRequestCancelled()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleAllRequestCancelled" );
    if (iCurrentRequest != KPosNullQNRequestId)
        { // current request requires some specific behavior
        HandleRequestCancelled(iCurrentRequest);
        }
   LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleAllRequestCancelled" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleVerificationResultL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleVerificationResultL(TInt aResultCode)
    {
    iVerifyResult = aResultCode;

        LOCVERIFIERDLGDEBUG1( "+CLpdVerifierPlugin::HandleVerificationResultL(%d)",
                iVerifyResult );

    switch (iVerifyResult)
        {
        case KErrNone: // fall through
        case KErrAccessDenied:
            {
            // No need for rules now in 3.0
            break;
            }
        case KErrTimedOut:
            { // UI's internal timer expired
            break;
            }
        case KErrCancel:
            {
            return; // don't handle next req. yet
            }
        case KErrAbort: // This is used for emergency call support
            {
            CompleteAllRequests(iVerifyResult);
            // ScheduleRequest() allows the current call chain
            // run to completion and resources are released after that.
            iRequestActiveObject->ScheduleRequest();
            return;
            }
        default:
            {
            
            break;
            }
        }

    CompleteCurrentAndContinue(iVerifyResult); // this request was handled
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleVerificationResultL" );
    }
// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNotificationResultL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNotificationResultL(TInt aResultCode)
    {

   LOCVERIFIERDLGDEBUG1( "+CLpdVerifierPlugin::HandleVerificationResultL(%d)",
                aResultCode );
    CompleteRequest(iCurrentRequest, aResultCode);
    iCurrentRequest = KPosNullQNRequestId;

    switch (aResultCode)
        {
        case KErrNone: // fall through
        case KErrTimedOut: // fall through
            {
            break;
            }
        case KErrCancel:
            {
            break;
            }
        case KErrAbort: // This is used for emergency call support
            {
            CompleteAllRequests(aResultCode);
            // ScheduleRequest() -> allows the current call chain
            // run to completion and resources are released after that.
            break;
            }
        default:
            {
            
            break;
            }
        }

    iRequestActiveObject->ScheduleRequest(); // handle next req.
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNotificationResultL" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleLeave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleLeave(TInt /*aError*/)
    {
        LOCVERIFIERDLGDEBUG1( "+CLpdVerifierPlugin::HandleLeave(%d)", aError);
    // In this case user needs feedback about the error situation:
    

    // In case of leave current request is completed with
    // iVerifyResult, but queue handling is still continued.
    // iVerifyResult is better completion code for request than aError.
    CompleteCurrentAndContinue(iVerifyResult);
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleLeave" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextRequest()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNextRequest" );
    TRAPD( err, HandleNextRequestL() );
    if (err)
        {
        // In case of leave current request is completed with
        // error code, but queue handling is still continued.

        // If we couldn't start handling the request it is
        // better not to confuse user with an error note.

        CompleteCurrentAndContinue(iVerifyResult);
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNextRequest" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextRequestL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextRequestL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNextRequestL" );
    // It is better to free previous query resources here, because
    // now all callback methods have finished (active object allowed
    // run to completion)
    this->FreeQueryResources();

    // Read the next request:
    RArray<TPosQNRequestId> requests;
    CleanupClosePushL(requests);
    GetRequestsL(requests);

    if (requests.Count() == 0)
        { // No more requests to handle
        CleanupStack::PopAndDestroy(); // requests
        // This is a very important step, it allows new requests
        // to flow in ( see HandleNewRequestL() ):
        delete iRequestActiveObject;
        iRequestActiveObject = NULL;
        return;
        }
    iCurrentRequest = requests[0];
    CleanupStack::PopAndDestroy(); // requests
    SetCurrentRequestL(iCurrentRequest);

    // Check whether the request is coming from Uikon Server. If not reject this request.
    if (!CheckClientSecureId(KPrivSrvSecureId))
        {
        CompleteCurrentAndContinue(KErrPermissionDenied);
        return;
        }
   
    // Check the request type
    iCurrentRequestType = RequestTypeL(iCurrentRequest);

    iRtorProcessor->ReadRequestorsL(*this);

      
    
    if (iCurrentRequestType == EQuery)
        {
        LOCVERIFIERDLGDEBUG( "New verification request received" );
        HandleNextVerificationL();
        }
    else if (iCurrentRequestType == ENotification)
        {
        LOCVERIFIERDLGDEBUG( "New notification request received" );
        HandleNextNotificationL();
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNextRequestL" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextVerificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextVerificationL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNextVerificationL" );
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    __ASSERT_DEBUG( !iVerifierQuery, HandleAssertErrorL() );
    
    CPosRequestor::TRequestType reqType;
       if (CheckRequestTypeL() == CPosRequestor::ERequestPeriodic)
            reqType = CPosRequestor::ERequestPeriodic;
        else
            reqType = CPosRequestor::ERequestSingleShot;
    
    TPosRequestSource source(RequestSource());
    if (source == EPosRequestSourceNotAvailable)
        {
        CompleteCurrentAndContinue(KErrNone);
        }
    else
        {
        
         TPosRequestDecision decision(QueryTimeoutStrategy());
        
        iRtorProcessor->iRequestType=reqType;
        iRtorProcessor->iRequestDecision=decision;
        iRtorProcessor->iDialogType=CLpdRequestorProcessor::EQuery;
               
        iVerifierQuery = CLpdVerifierQueryLauncher::NewL(*this);
        // Ownership NOT transferred
        iVerifierQuery->SetRequestInfoL(iRtorProcessor);
        
        iVerifierQuery->StartQueryL();    
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNextVerificationL" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextNotificationL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNextNotificationL" );
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    
    // There is no differentiation now for periodic and single shot notifications
    HandleNextNonPeriodicNotificationL();
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNextNotificationL" );
    }
// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL" );
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    __ASSERT_DEBUG( !iNotifier, HandleAssertErrorL() );

    if (RequestSource() != EPosRequestSourceNetwork)
        { // Notifications are supported only for network requests
        User::Leave(KErrNotSupported);
        }

    TPosNotificationReason reason = NotificationReason();
    TPosRequestDecision decision = LocationRequestDecision();
    iRtorProcessor->iNotifReason=reason;
    iRtorProcessor->iDialogType=CLpdRequestorProcessor::ENotification;
    iRtorProcessor->iRequestDecision=decision;
    
    iNotifier = CLpdNotifierQueryLauncher::NewL(*this);
    iNotifier->SetRequestInfoL(iRtorProcessor);
    iNotifier->StartQueryL();
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL" );
    }


// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CompleteCurrentAndContinue
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::CompleteCurrentAndContinue(TInt aResultCode)
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::CompleteCurrentAndContinue" );
    if (iCurrentRequest != KPosNullQNRequestId)
        {
        CompleteRequest(iCurrentRequest, aResultCode);
        }
    iCurrentRequest = KPosNullQNRequestId;

    __ASSERT_DEBUG( iRequestActiveObject, HandleDebugAssertError() );
    iRequestActiveObject->ScheduleRequest(); // handle next req.
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::CompleteCurrentAndContinue" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::FreeQueryResources
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::FreeQueryResources()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::FreeQueryResources" );
    iCurrentRequest = KPosNullQNRequestId;
    delete iVerifierQuery;
    iVerifierQuery = NULL;
    iVerifyResult = KErrGeneral;
    delete iNotifier;
    iNotifier = NULL;
    if (iRtorProcessor)
        { // if already construction fails iRtorProcessor may
        // be NULL, otherwise it points to an instance.
        iRtorProcessor->ResetAndDestroyRequestors();
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::FreeQueryResources" );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleAssertErrorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleAssertErrorL() const
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleAssertErrorL" ); 
#ifdef _DEBUG        
    User::Panic(KPanicText, KLpdErrGeneral);
#else
    User::Leave( KErrCorrupt );
#endif
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleAssertErrorL" ); 
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleDebugAssertError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleDebugAssertError() const
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::HandleDebugAssertError" ); 
#ifdef _DEBUG
    User::Panic(KPanicText, KLpdErrGeneral);
#endif
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::HandleDebugAssertError" ); 
    }


// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CheckRequestTypeL
// Checks the type of request and helps to know
// if a given request is of SUPL periodic type
// @param aCurrentRequest request id
// @return TRequestType type of request enum
// -----------------------------------------------------------------------------
CPosRequestor::TRequestType CLpdVerifierPlugin::CheckRequestTypeL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdVerifierPlugin::CheckRequestTypeL" );
    CPosRequestor::TRequestType reqType = CPosRequestor::ENetworkTypeUnknown;
    if (RequestorCountL() > 0)
        {
        CPosRequestor* requestor = RequestorLC(0);
        reqType = requestor->RequestType();
        CleanupStack::PopAndDestroy(requestor);
        }
    LOCVERIFIERDLGDEBUG( "-CLpdVerifierPlugin::CheckRequestTypeL" );
    return reqType;
    }


//  End of File
