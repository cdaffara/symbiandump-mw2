/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides events to mediator event consumer.
*
*/

#include    <featmgr.h>
#include    <centralrepository.h>
#include    <MediatorDomainUIDs.h>
#include    <MediatorEventProvider.h>
#include    <SecondaryDisplay/secondarydisplaysatapi.h>

#include    "SATPrivateCRKeys.h"
#include    "csatmediatoreventprovider.h"
#include    "SatLog.h"

using namespace MediatorService;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatMediatorEventProvider* CSatMediatorEventProvider::NewL()
    {
    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::NewL calling" )

    CSatMediatorEventProvider* self =
        new ( ELeave ) CSatMediatorEventProvider();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::~CSatMediatorEventProvider
// Destructor.
// -----------------------------------------------------------------------------
//
CSatMediatorEventProvider::~CSatMediatorEventProvider()
    {
    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::\
        ~CSatMediatorEventProvider calling" )

    if ( iEventProvider )
        {
        iEventProvider->UnregisterEvent( KMediatorSecondaryDisplayDomain,
                                         KCatSAT,
                                         EMsgAnySatCommand );
        delete iEventProvider;
        iEventProvider = NULL;
        }

    LOG( NORMAL,
        "SATENGINE: CSatMediatorEventProvider::\
            ~CSatMediatorEventProvider exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::RaiseSatEvent
//
// -----------------------------------------------------------------------------
//
TInt CSatMediatorEventProvider::RaiseSatEvent( const TDesC8& aData )
    {

    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider \
        ::RaiseSatEvent calling" )
    TInt retVal( KErrNone );
    // If SAT Event is registered, raise SAT event
    if ( iRegistered )
        {
        LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider \
        ::RaiseSatEvent iRegistered true" )
#ifdef ENABLE_SAT_LOGGING
        const TSatCommandPckg* eventPckg =
            reinterpret_cast<const TSatCommandPckg*> ( aData.Ptr() );
        const TSatCommandData& eventData =
            reinterpret_cast<const TSatCommandData&> ( *eventPckg );

        LOG2( DETAILED,
        "SATENGINE: CSatMediatorEventProvider::RaiseSatEvent: \
        cmdNumber: 0x%2X", eventData.iPCmdNumber )
        LOG2( DETAILED,
        "SATENGINE: CSatMediatorEventProvider::RaiseSatEvent: AlphaId: %S",
        &eventData.iAlphaId )
        LOG2( DETAILED,
        "SATENGINE: CSatMediatorEventProvider::RaiseSatEvent: \
        duration[s]: 0x%2X", eventData.iDuration.iNumOfUnits )
        LOG2( DETAILED,
        "SATENGINE: CSatMediatorEventProvider::RaiseSatEvent: IconID: 0x%2X",
        eventData.iIconID.iIdentifier )
#endif
        retVal = iEventProvider->RaiseEvent(
                                            KMediatorSecondaryDisplayDomain,
                                            KCatSAT,
                                            EMsgAnySatCommand,
                                            TVersion( KVersionMajor,
                                                      KVersionMinor,
                                                      KVersionBuild ),
                                            aData );
        }

    LOG2( NORMAL, "SATENGINE: CSatMediatorEventProvider\
          ::RaiseSatEvent exiting, retVal: %d", retVal )
    return retVal;
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::CoverUiSupported
//
// -----------------------------------------------------------------------------
//
TBool CSatMediatorEventProvider::CoverUiSupported()
    {
    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider \
        ::CoverUiSupported calling-exiting" )
    return iCoverUiSupported;
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::CSatMediatorEventProvider
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSatMediatorEventProvider::CSatMediatorEventProvider() :
    iRegistered( EFalse ),
    iCoverUiSupported( EFalse ),
    iEventProvider( NULL )
    {
    LOG( NORMAL,
        "SATENGINE: CSatMediatorEventProvider::CSatMediatorEventProvider \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatMediatorEventProvider::ConstructL()
    {
    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::ConstructL calling" )

    // Store Cover UI supported information
    CUiSupportedL();

    // If Cover UI is supported, register SAT Event
    if ( iCoverUiSupported )
        {
        LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::ConstructL \
             iCoverUiSupported true" )
        iEventProvider = CMediatorEventProvider::NewL();

        User::LeaveIfError( iEventProvider->RegisterEvent(
                    KMediatorSecondaryDisplayDomain,
                    KCatSAT,
                    EMsgAnySatCommand,
                    TVersion( KVersionMajor, KVersionMinor, KVersionBuild ),
                    TCapabilitySet( ECapabilitySwEvent ) ) );

        iRegistered = ETrue;
        }

    LOG( NORMAL, "SATENGINE: CSatMediatorEventProvider::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMediatorEventProvider::CUiSupportedL
//
// -----------------------------------------------------------------------------
//
void CSatMediatorEventProvider::CUiSupportedL()
    {
    LOG( NORMAL,
        "SATENGINE: CSatMediatorEventProvider::CoverUiSupported calling" )

    TBool coverui( EFalse );

    // Check if Cover UI presents from Feature Manager
    FeatureManager::InitializeLibL();
    coverui = FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    FeatureManager::UnInitializeLib();

    // Check SAT internal flag for Cover UI
    TInt satCoverUi( 0 );
    CRepository* repository = CRepository::NewL( KCRUidSatServer );
    repository->StartTransaction( CRepository::EReadTransaction );
    repository->Get( KSatSecondaryDisplaySupport, satCoverUi );
    delete repository;
    repository = NULL;

    // Store Supported information
    iCoverUiSupported = TBool( coverui & satCoverUi );
    LOG2( SIMPLE,"CSatMediatorEventProvider::CUiSupportedL \
          KSatSecondaryDisplaySupport:%d", satCoverUi )
    LOG( NORMAL,
        "SATENGINE: CSatMediatorEventProvider::CoverUiSupported exiting" )
    }

