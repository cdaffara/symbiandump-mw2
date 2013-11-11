/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Updates call information to mediator clients.
*
*/


#include "cccecallinfomediatorupdater.h"

#include <callinformation.h>
#include <MediatorDomainUIDs.h> 

#include "mccecallinfomediator.h"
#include "cccecall.h"
#include "mccecallobserver.h"
#include "mccecallarray.h"
#include "mccecallinfo.h"
#include "cccecallinfoconverter.h"
#include "cccelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediatorUpdater::CCCECallInfoMediatorUpdater(
    MCCECallInfoMediator& aMediator,
    MCCECallArray& aCallArray,
    CCCECallInfoConverter* aConverter ) :
    iMediator( aMediator ),
    iCallArray( aCallArray ),
    iConverter( aConverter )
    {
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediatorUpdater::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Gets snapshot of call infos and packs them
// ---------------------------------------------------------------------------
//
HBufC8* CCCECallInfoMediatorUpdater::GetSnapShotOfCallInfosL()
    {
    CCallInfos* infos = CCallInfos::NewLC();
            
    for ( TInt i = 0; i < iCallArray.MaxNumberOfCalls(); i++ )
        {
        MCCECallInfo* info = iCallArray.CallInfo( i );
        AddInfoL( info, *infos );
        }
    MCCECallInfo* emergencyInfo = iCallArray.EmergencyCallInfo();
    AddInfoL( emergencyInfo, *infos );
    
    HBufC8* packedInfos = infos->ExternalizeL();
    CleanupStack::PopAndDestroy( infos );
    return packedInfos;
    }

// ---------------------------------------------------------------------------
// Converts call info to mediator info and adds it to the infos.
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediatorUpdater::AddInfoL( 
    MCCECallInfo* aInfo, 
    CCallInfos& aInfos )
    {
    if ( aInfo )
        {
        // Idle calls are not reported to mediator clients
        if ( aInfo->State() != CCPCall::EStateIdle )
            {
            TCallInfo mediatorInfo = iConverter->Convert( *aInfo );
            aInfos.AddL( mediatorInfo );
            }
        }
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediatorUpdater* CCCECallInfoMediatorUpdater::NewL(
    MCCECallInfoMediator& aMediator,
    MCCECallArray& aCallArray,
    CCCECallInfoConverter* aConverter )
    {
    CCCECallInfoMediatorUpdater* self = 
        CCCECallInfoMediatorUpdater::NewLC(
            aMediator, aCallArray, aConverter );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediatorUpdater* CCCECallInfoMediatorUpdater::NewLC(
    MCCECallInfoMediator& aMediator,
    MCCECallArray& aCallArray,
    CCCECallInfoConverter* aConverter )
    {
    CCCECallInfoMediatorUpdater* self = 
        new( ELeave ) CCCECallInfoMediatorUpdater( 
            aMediator, aCallArray, aConverter );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediatorUpdater::~CCCECallInfoMediatorUpdater()
    {
    delete iConverter;
    }

// ---------------------------------------------------------------------------
// Updates the call infos to mediator clients.
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediatorUpdater::UpdateL()
    {            
    HBufC8* packedInfos = GetSnapShotOfCallInfosL();
    iMediator.RaiseEvent( EChangesInCallStates, *packedInfos );
    
    delete packedInfos;
    }

// ---------------------------------------------------------------------------
// From base class MMediatorCommandObserver
// Handles the command of getting all call infos.
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediatorUpdater::MediatorCommandL( 
    TUid aDomain,
    TUid aCategory, 
    TInt aCommandId,
    TVersion /*aVersion*/, 
    const TDesC8& /*aData*/ )
    {
    CCELOGSTRING( "CCCECallInfoMediatorUpdater::MediatorCommandL(): In" );
    if ( aDomain != KMediatorTelephonyDomain || 
         aCategory != KCatCallInformation ||
         aCommandId != EGetAllCallStates )
        {
        User::Leave( KErrArgument );
        }
    
    HBufC8* packedInfos = GetSnapShotOfCallInfosL();
    iMediator.SendResponse( EGetAllCallStates, *packedInfos );
    
    CCELOGSTRING( "CCCECallInfoMediatorUpdater::MediatorCommandL(): Out" );
    delete packedInfos;
    }

// ---------------------------------------------------------------------------
// From base class MMediatorCommandObserver
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediatorUpdater::CancelMediatorCommand( 
    TUid /*aDomain*/,
    TUid /*aCategory*/, 
    TInt /*aCommandId*/ )
    {
    
    }
