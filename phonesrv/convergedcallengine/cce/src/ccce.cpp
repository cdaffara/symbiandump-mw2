/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Main CCE control class
*
*/


//  INCLUDE FILES
#include "ccce.h"
#include "mcceobserver.h"
#include "cccecall.h"
#include "ccceconferencecall.h"
#include "cccedtmf.h"
#include "ccceutility.h"
#include "cccelogger.h"
#include "cccecallcontainer.h"
#include "ccceobserverasynchroniser.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCE::CCCE()
    {
    
    }

    
// ---------------------------------------------------------------------------
// Construct object
// ---------------------------------------------------------------------------
//
void CCCE::ConstructL(MCCEObserver* aObserver)
    {
    CCELOGSTRING("*** CCCE::ConstructL Started ***");
    iObserver = CCCEObserverAsynchroniser::NewL();
    iObserver->SetMCCEObserver(aObserver);
    iCallContainer = CCCECallContainer::NewL( iObserver->GetMCCEObserver() );
    CCELOGSTRING("*** CCCE::ConstructL Completed! ***");
    }


// ---------------------------------------------------------------------------
// Default Symbian constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCCE* CCCE::NewL()
    {
    CCCE* self = CCCE::NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Default Symbian constructor (object is left into cleanupstack)
// ---------------------------------------------------------------------------
//
EXPORT_C CCCE* CCCE::NewLC()
    {
    CCCE* self = new( ELeave ) CCCE();
    CleanupStack::PushL( self );
    self->ConstructL(NULL);
    return self;
    }

// ---------------------------------------------------------------------------
// Default Symbian constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCCE* CCCE::NewL( MCCEObserver& aObserver )
    {
    CCCE* self = CCCE::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// Default Symbian constructor (object is left into cleanupstack)
// ---------------------------------------------------------------------------
//
EXPORT_C CCCE* CCCE::NewLC( MCCEObserver& aObserver )
    {
    CCCE* self = new( ELeave ) CCCE();
    CleanupStack::PushL( self );
    self->ConstructL(&aObserver);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCE::~CCCE()
    {
    CCELOGSTRING("*** CCCE::::~CCCE Started ***");
    delete iCallContainer;
    delete iObserver;
    CCELOGSTRING("*** CCCE::::~CCCE Completed! ***");
    }

// ---------------------------------------------------------------------------
// CCCE::SetObserver( MCCEObserver& aObserver )
// ---------------------------------------------------------------------------
//
EXPORT_C void CCCE::SetObserver( MCCEObserver& aObserver,
        MCCESsObserver& aSsObserver )
    {
    iObserver->SetMCCEObserver(&aObserver);
    iObserver->SetMCCESsObserver(&aSsObserver);
    iCallContainer->SetObserver( iObserver->GetMCCEObserver(), iObserver->GetMCCESsObserver() );
    }

// ---------------------------------------------------------------------------
// Open new call. 
// ---------------------------------------------------------------------------
//
EXPORT_C MCCECall& CCCE::OpenNewCallL(
    const TDesC&                        aNumber,
    const CCCECallParameters& aCallParameters,
    const MCCECallObserver&             aCallObserver )
    {
    return iCallContainer->OpenNewCallL( 
        aNumber, 
        aCallParameters, 
        aCallObserver );
    }

// -----------------------------------------------------------------------------
// InitializeEmergencyCallL()
// -----------------------------------------------------------------------------
//
EXPORT_C MCCECall& CCCE::InitializeEmergencyCall( 
    const MCCECallObserver& aCallObserver )
    {
    return iCallContainer->EmergencyCall( aCallObserver );
    }
    
// -----------------------------------------------------------------------------
// ConferenceCallL()
// -----------------------------------------------------------------------------
//
EXPORT_C MCCEConferenceCall& CCCE::ConferenceCallL( 
    const MCCEConferenceCallObserver& aObserver )
    {
    CCELOGSTRING("CCCE::ConferenceCallL()");
    iObserver->SetMCCEConferenceCallObserver(&aObserver);
    return iCallContainer->ConferenceCallL( iObserver->GetMCCEConferenceCallObserver() );
    }
    
// -----------------------------------------------------------------------------
// DtmfInterface()
// -----------------------------------------------------------------------------
//
EXPORT_C MCCEDtmfInterface& CCCE::DtmfInterfaceL(
    const MCCEDtmfObserver& aObserver )
    {
    CCELOGSTRING("CCCE::DtmfInterfaceL()");
    iObserver->SetMCCEDtmfObserver(&aObserver);
    return iCallContainer->DtmfInterfaceL(iObserver->GetMCCEDtmfObserver() );
    }
    
// -----------------------------------------------------------------------------
// ExtensionInterfaceL()
// -----------------------------------------------------------------------------
//
EXPORT_C MCCEExtensionInterface& CCCE::ExtensionInterfaceL( 
    const MCCEExtensionObserver& aObserver )
    {
    CCELOGSTRING("CCCE::ExtensionInterfaceL()");
    iObserver->SetMCCEExtensionObserver(&aObserver);
    return iCallContainer->ExtensionInterfaceL(iObserver->GetMCCEExtensionObserver());
    }
   
// -----------------------------------------------------------------------------
// TerminateAllCalls()
// -----------------------------------------------------------------------------
// 
EXPORT_C void CCCE::TerminateAllCalls()
    {
    CCELOGSTRING("CCCE::TerminateAllCalls():IN");
    iCallContainer->TerminateAllCalls();
    CCELOGSTRING("CCCE::TerminateAllCalls():OUT");
    }


// -----------------------------------------------------------------------------
// TerminateCall()
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt CCCE::TerminateCall( TName& /*aCallName*/ )
    {
    CCELOGSTRING("CCCE::TerminateCall(TName):IN");
    CCELOGSTRING("CCCE::TerminateCall(TName):OUT");
    return KErrNotSupported; // TODO
    }

// -----------------------------------------------------------------------------
// TerminateCall()
// -----------------------------------------------------------------------------
// 
EXPORT_C void CCCE::TerminateCall( TInt& /*aStatus*/,
                             TName& /*aCallName*/ )
    {
    CCELOGSTRING("CCCE::TerminateCall(TRequestStatus,TName):IN");
    // TODO
    CCELOGSTRING("CCCE::TerminateCall(TRequestStatus,TName):OUT");
    }

// -----------------------------------------------------------------------------
// TerminateAllButRingingAndQueuedCalls()
// -----------------------------------------------------------------------------
// 
EXPORT_C void CCCE::TerminateAllButRingingAndQueuedCalls()
    {
    CCELOGSTRING("CCCE::TerminateAllButRingingAndQueuedCalls():IN");
    iCallContainer->TerminateAllButRingingAndQueuedCalls();
    CCELOGSTRING("CCCE::TerminateAllButRingingAndQueuedCalls():OUT");
    }


// -----------------------------------------------------------------------------
// GetAirTimeDuration()
// -----------------------------------------------------------------------------
// 
EXPORT_C void CCCE::GetAirTimeDuration( TTimeIntervalSeconds& aCSDuration, 
                                        TTimeIntervalSeconds& aPSDuration )
    {
    CCELOGSTRING("CCCE::GetAirTimeDuration()");
    iCallContainer->GetAirTimeDuration( aCSDuration, aPSDuration );
    }


// -----------------------------------------------------------------------------
// GetLifeTime()
// -----------------------------------------------------------------------------
// 
EXPORT_C TBool CCCE::GetLifeTime( TDes8& aLifeTimeInfo )
    {
    CCELOGSTRING("CCCE::GetLifeTime()");
    return iCallContainer->GetLifeTime(aLifeTimeInfo);
    }

// -----------------------------------------------------------------------------
// GetCSInfo()
// -----------------------------------------------------------------------------
// 
EXPORT_C TBool CCCE::GetCSInfo( CSInfo& aCSInfo )
    {
    CCELOGSTRING("CCCE::GetCSInfo()");
    return iCallContainer->GetCSInfo(aCSInfo);
    }


