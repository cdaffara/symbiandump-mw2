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
* Description:  Forwards call state changes to Mediator
*
*/


#include "cccecallinfomediator.h"

#include <MediatorDomainUIDs.h> 
#include <callinformation.h>
#include <MediatorCommandResponder.h> 
#include <MediatorEventProvider.h> 

#include "cccecallinfomediatorupdater.h"
#include "cccecallinfoconverter.h"
#include "cccelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediator* CCCECallInfoMediator::NewL(
    MCCECallArray& aCallArray )
    {
    CCCECallInfoMediator* self = CCCECallInfoMediator::NewLC( aCallArray );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediator* CCCECallInfoMediator::NewLC(
    MCCECallArray& aCallArray )
    {
    CCCECallInfoMediator* self = new( ELeave ) CCCECallInfoMediator;
    CleanupStack::PushL( self );
    self->ConstructL( aCallArray );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediator::~CCCECallInfoMediator()
    {
    if ( iEventProvider )
        {
        TInt ignore = iEventProvider->UnregisterEvent( KMediatorTelephonyDomain, 
            KCatCallInformation, EChangesInCallStates );
        }
    delete iEventProvider;
    if ( iResponder )
        {
        TInt ignore = iResponder->UnregisterCommand( KMediatorTelephonyDomain, 
            KCatCallInformation, EGetAllCallStates );
        }
    delete iResponder;   
    delete iUpdater;
    }

// ---------------------------------------------------------------------------
// Updates call infos
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediator::UpdateCallInfos()
    {
    CCELOGSTRING( "CCCECallInfoMediator::UpdateCallInfos(): IN" );
    TInt err;
    TRAP( err, iUpdater->UpdateL() ); 
    CCELOGSTRING2( "CCCECallInfoMediator::UpdateCallInfos err = %d", err );
    CCELOGSTRING( "CCCECallInfoMediator::UpdateCallInfos(): OUT" );
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfoMediator
// Sends the response to mediator
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediator::SendResponse( TInt aCommandId, const TDesC8& aData )
    {
    CCELOGSTRING( "CCCECallInfoMediator::SendResponse(): IN" );
    const TInt ignore( iResponder->IssueResponse( 
        KMediatorTelephonyDomain, KCatCallInformation, 
        aCommandId, KErrNone, aData ) ); 
    CCELOGSTRING2( "CCCECallInfoMediator::SendResponse() err = %d", ignore );
    CCELOGSTRING( "CCCECallInfoMediator::SendResponse(): OUT" );
    }

// ---------------------------------------------------------------------------
// From base class MCCECallInfoMediator
// Raises the mediator event.
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediator::RaiseEvent( TInt aEventId, const TDesC8& aData )
    {
    CCELOGSTRING( "CCCECallInfoMediator::RaiseEvent(): IN" );
    TVersion version( 
        KCallInformationVersionMajor, KCallInformationVersionMinor, 
        KCallInformationVersionBuild );
    const TInt ignore( iEventProvider->RaiseEvent( 
        KMediatorTelephonyDomain, KCatCallInformation, 
        aEventId, version, aData ) );
    CCELOGSTRING2( "CCCECallInfoMediator::RaiseEvent() err = %d", ignore );
    CCELOGSTRING( "CCCECallInfoMediator::RaiseEvent(): OUT" );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCECallInfoMediator::CCCECallInfoMediator()
    {
    }


// ---------------------------------------------------------------------------
// Second phase constructor.
// ---------------------------------------------------------------------------
//
void CCCECallInfoMediator::ConstructL(
    MCCECallArray& aCallArray )
    {           
    // updater takes the converter ownership
    CCCECallInfoConverter* converter = new( ELeave )CCCECallInfoConverter();
    CleanupStack::PushL( converter );
    iUpdater = CCCECallInfoMediatorUpdater::NewL( 
        *this, aCallArray, converter );
    CleanupStack::Pop( converter );
    
    iResponder = CMediatorCommandResponder::NewL( iUpdater );
    TCapabilitySet caps = TCapabilitySet();
    caps.SetEmpty();
    TVersion version( KCallInformationVersionMajor, KCallInformationVersionMinor, 
                      KCallInformationVersionBuild );
    User::LeaveIfError( iResponder->RegisterCommand( 
        KMediatorTelephonyDomain,
        KCatCallInformation, 
        EGetAllCallStates,
        version,
        caps,
        KMediatorTimeoutInfinite ) );
    
    iEventProvider = CMediatorEventProvider::NewL();
    User::LeaveIfError( 
        iEventProvider->RegisterEvent( 
            KMediatorTelephonyDomain, KCatCallInformation, 
            EChangesInCallStates, version, caps ) );

    }

