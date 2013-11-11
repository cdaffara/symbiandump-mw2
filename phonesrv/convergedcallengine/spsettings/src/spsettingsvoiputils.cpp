/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings API, VoIP utilities 
*                implementation file.
*
*/


#include "spsettingsvoiputils.h"

#include "spsettingsengine.h"
#include "spproperty.h"
#include "spdefinitions.h"
#include "spdefaultvalues.h"

#include <featmgr.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h> 

#include "spsapilogger.h" // For logging

// Preferred Telephony value is PS call
const TInt KRCSEInternetCallPreferred = 1;

// Dynamic VoIP off
const TInt KVoIPOFF = 0;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSettingsVoIPUtils::CSPSettingsVoIPUtils()
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPSettingsVoIPUtils::ConstructL()
    {
    XSPSLOGSTRING( "CSPSettingsVoIPUtils::ConstructL() - IN" );
    iSettingsEngine = CSPSettingsEngine::NewL();
    XSPSLOGSTRING( "CSPSettingsVoIPUtils::ConstructL() - OUT" );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettingsVoIPUtils* CSPSettingsVoIPUtils::NewL()
    {
    CSPSettingsVoIPUtils* self = CSPSettingsVoIPUtils::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettingsVoIPUtils* CSPSettingsVoIPUtils::NewLC()
    {
    CSPSettingsVoIPUtils* self = new( ELeave ) CSPSettingsVoIPUtils;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettingsVoIPUtils::~CSPSettingsVoIPUtils()
    {
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::~CSPSettingsVoIPUtils() - IN" );
    delete iSettingsEngine;
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::~CSPSettingsVoIPUtils() - OUT" );
    }

// ---------------------------------------------------------------------------
// VoIPProfilesExistL - API method
// Returns ETrue if VoIP sub service plugin ID have been set for some service.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSPSettingsVoIPUtils::VoIPProfilesExistL() const
    {
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::VoIPProfilesExistL() - IN" );

    TBool profileExists( EFalse );

    // Get all service IDs
    RArray<TServiceId> services;
    CleanupClosePushL( services );
    iSettingsEngine->FindServiceIdsL( services );
    const TInt count( services.Count() );

    // Check if some service has VoIP sub service plug-in ID
    TInt err( 0 );
    CSPProperty* property = CSPProperty::NewLC();
    for( TInt i = 0; i < count && !profileExists; ++i )
        {
        TServiceId id = services[ i ];
        if( id == KSPDefaultVoIPServiceId )
            {
            continue;
            }
            
        err = iSettingsEngine->FindPropertyL( id,
                                              EPropertyVoIPSubServicePluginId,
                                              *property );

        profileExists = ( KErrNone == err ? ETrue : EFalse );
        }

    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( &services );


    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::VoIPProfilesExistL() - OUT" );
    return profileExists;
    }

// ---------------------------------------------------------------------------
// IsVoIPSupported - API method
// Returns ETrue if following items are ON:
//      Common Voip (Feature Manager)
//      Dynamic VoIP (Telephony settings)
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSPSettingsVoIPUtils::IsVoIPSupported( ) const
    {
    XSPSLOGSTRING( "CSPSettingsVoIPUtils::IsVoIPSupported( ) - IN" );

    TBool voipSupport( EFalse );
    TRAPD( err, voipSupport = IsVoIPSupportedL() );
    
    if ( err != KErrNone )
        {
        voipSupport = EFalse;
        }

    XSPSLOGSTRING( "CSPSettingsVoIPUtils::IsVoIPSupported( ) - OUT" );
    return voipSupport;
    }

// ---------------------------------------------------------------------------
// IsPreferredTelephonyVoIP - API method
// Returns ETrue if following items are ON:
//      Common Voip (Feature Manager)
//      Dynamic VoIP (Telephony settings)
//      Preferred telephony (Rich call settings)
//  + there is at least one SIP-VoIP profile stored
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSPSettingsVoIPUtils::IsPreferredTelephonyVoIP() const
    {
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::IsPreferredTelephonyVoIP() - IN" );

    // Check that VoIP is supported
    TBool voipSupported( IsVoIPSupported() );
    
    XSPSLOGSTRING2( 
        "CSPSettingsVoIPUtils::VoipSupported: %d",voipSupported );

    // Check the preferred telephony
    TBool voipPreferred( EFalse );
    TRAP_IGNORE( voipPreferred = IsPreferredTelephonyVoIPL() );
    
    XSPSLOGSTRING2( 
        "CSPSettingsVoIPUtils::VoipPreferred: %d", voipPreferred );

    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::IsPreferredTelephonyVoIP() - OUT" );
    return voipPreferred && voipSupported;
    }

// ---------------------------------------------------------------------------
// Get id of the preferred service
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettingsVoIPUtils::GetPreferredService( TUint& aServiceId )
    const
    {
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::GetPreferredService() - IN" );

    TRAPD( error, DoGetPreferredServiceL( aServiceId ) );
    return error;
    }

// ---------------------------------------------------------------------------
// Is VoIP supported - Actual implementation
// ---------------------------------------------------------------------------
//
TBool CSPSettingsVoIPUtils::IsVoIPSupportedL( ) const
    {
    FeatureManager::InitializeLibL();
    TBool voipSupportByFM = FeatureManager::FeatureSupported( KFeatureIdCommonVoip );
    FeatureManager::UnInitializeLib();

    if ( voipSupportByFM )
        {
        TInt voipSupport( KVoIPOFF );
        CRepository* repository     
            = CRepository::NewLC( KCRUidTelephonySettings );
        repository->Get( KDynamicVoIP, voipSupport );
        CleanupStack::PopAndDestroy( repository );
        return KVoIPOFF != voipSupport;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// Is VoIP preferred telephony - Actual implementation
// ---------------------------------------------------------------------------
//
TBool CSPSettingsVoIPUtils::IsPreferredTelephonyVoIPL() const
    {
    TBool voipPreferred( EFalse );

    CRepository* repository = CRepository::NewLC( KCRUidRichCallSettings );
    TInt value(0);
    repository->Get( KRCSEPreferredTelephony, value );
    voipPreferred = ( KRCSEInternetCallPreferred == value ? ETrue : EFalse );
    CleanupStack::PopAndDestroy( repository );

    return voipPreferred && VoIPProfilesExistL();
    }

// ---------------------------------------------------------------------------
// Get Preferred Service - Actual implementation
// ---------------------------------------------------------------------------
//
void CSPSettingsVoIPUtils::DoGetPreferredServiceL( 
    TUint& aServiceId ) const
    {
    XSPSLOGSTRING( 
        "CSPSettingsVoIPUtils::DoGetPreferredServiceL() - IN" );
    
    if ( !IsVoIPSupportedL() )
        {
        // Voip not supported
        User::Leave( KErrNotSupported );
        }
    
    // Get the preferred service id
    CRepository* repository = CRepository::NewLC( KCRUidRichCallSettings );
    TInt value(0);
    User::LeaveIfError( repository->Get( KRCSPSPreferredService, value ) );
    CleanupStack::PopAndDestroy( repository );
    
    
    XSPSLOGSTRING2( 
        "CSPSettingsVoIPUtils::DoGetPreferredServiceL() service:%d",value );
    if ( 0 == value )
        {
        // Preferred service not setted.
        User::Leave( KErrNotFound );
        }

    aServiceId = value;
    }

