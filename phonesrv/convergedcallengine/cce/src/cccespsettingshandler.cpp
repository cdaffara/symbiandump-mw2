/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains service settings handling logic
*
*/

#include "cccespsettingshandler.h"
#include "cccepluginmanager.h"
#include "mccespsobserver.h"
#include "cccelogger.h"
#include "cccecallcontainer.h"

#include <spsettingsvoiputils.h>
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spnotifychange.h>

// -----------------------------------------------------------------------------
// CCCESPSettingsHandler
// -----------------------------------------------------------------------------
//
CCCESPSettingsHandler::CCCESPSettingsHandler( MCCESPSObserver& aObserver ) :
    iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// ~CCCESPSettingsHandler
// -----------------------------------------------------------------------------
//
CCCESPSettingsHandler::~CCCESPSettingsHandler()
    {
    delete iSettings;
    delete iNotifier;
    }

// -----------------------------------------------------------------------------
// NewL
// -----------------------------------------------------------------------------
//
CCCESPSettingsHandler* CCCESPSettingsHandler::NewL( MCCESPSObserver& aObserver )
    {
    CCCESPSettingsHandler* self = new (ELeave) CCCESPSettingsHandler( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::ConstructL()
    {
    iNotifier = CSPNotifyChange::NewL( *this );
    iSettings = CSPSettings::NewL();
    StartListeningChangesL();
    }

// -----------------------------------------------------------------------------
// IsPluginSupportedL
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::IsPluginSupportedL( TUid aImplementationUid )
    {
    if( aImplementationUid != KCSProviderUid )
        {
        CSPSettingsVoIPUtils *utils = CSPSettingsVoIPUtils::NewLC();
        if( !utils->IsVoIPSupported() )
            {
            CCELOGSTRING( "CCCESPSettingsHandler::IsPluginSupportedL : False" );
            User::Leave( KErrNotSupported );
            }

        CleanupStack::PopAndDestroy( utils );
        }
    
    CCELOGSTRING( "CCCESPSettingsHandler::IsPluginSupportedL : True" );
    }

// -----------------------------------------------------------------------------
// GetServicesL
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::GetServicesL( RIdArray& aServices )
    {
    iSettings->FindServiceIdsL( aServices );
    }

// -----------------------------------------------------------------------------
// ImplementationUidL
// -----------------------------------------------------------------------------
//
TUid CCCESPSettingsHandler::ImplementationUidL( TInt aServiceId )
    {
    CSPProperty* property = CSPProperty::NewLC();
    
    iSettings->FindPropertyL( aServiceId,
        EPropertyCallProviderPluginId, *property );
    
    TInt pluginId;
    property->GetValue( pluginId );
    
    TUid id(KNullUid);
    id.iUid = pluginId;
    
    CleanupStack::PopAndDestroy( property );
    
    return id;
    }

// -----------------------------------------------------------------------------
// CapablitiesL
// -----------------------------------------------------------------------------
//
TPropertyServiceAttributes CCCESPSettingsHandler::CapabilitiesL( TInt aServiceId )
    {
    CSPProperty* property = CSPProperty::NewLC();
    
    iSettings->FindPropertyL( aServiceId,
        EPropertyServiceAttributeMask, *property );
    
    TInt mask( 0 );
    property->GetValue( mask );
    
    CleanupStack::PopAndDestroy( property );

    return (TPropertyServiceAttributes)mask;
    }

// -----------------------------------------------------------------------------
// IsEnabledL
// -----------------------------------------------------------------------------
//
TBool CCCESPSettingsHandler::IsEnabledL( TInt aServiceId )
    {
    CSPProperty* property = CSPProperty::NewLC();
    
    iSettings->FindPropertyL( aServiceId,
        ESubPropertyVoIPEnabled, *property );
    
    TBool enabled( 0 );
    property->GetValue( enabled );
    
    CleanupStack::PopAndDestroy( property );
    
    return enabled;
    }
    
// -----------------------------------------------------------------------------
// HandleNotifyChange
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::HandleNotifyChange( TServiceId aServiceId )
    {
    CCELOGSTRING( "CCCESPSettingsHandler::HandleNotifyChange" );
#ifdef _DEBUG
    TInt err = KErrNone;
    TRAP( err, HandleServiceChangeL( aServiceId ) )
    CCELOGSTRING2( "CCCESPSettingsHandler::HandleNotifyChange err=%i", err );
#else
    TRAP_IGNORE( HandleServiceChangeL( aServiceId ) );
#endif
    }

// -----------------------------------------------------------------------------
// HandleError
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::HandleError( TInt /*aError*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// HandleServiceChangeL
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::HandleServiceChangeL( TInt aServiceId )
    {
    TUid uid = ImplementationUidL( aServiceId );
            
    if ( IsPluginOnStandbyL( uid ) )
        {
        CCELOGSTRING2( 
                "CCCESPSettingsHandler:: Load call provider plugin uid: 0x%X", 
                uid.iUid );
        iObserver.ServiceEnabledL( uid );
        }
    else
        {
        CCELOGSTRING2( 
                "CCCESPSettingsHandler:: Unload call provider plugin uid: 0x%X", 
                uid.iUid );
        iObserver.ServiceDisabledL( uid );
        }
    }

// -----------------------------------------------------------------------------
// StartListeningChangesL
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::StartListeningChangesL()
    {
    RIdArray array;
    CleanupClosePushL( array );
    // Array is empty because, also new added services are needed to be notified
    iNotifier->NotifyChangeL( array );
    CleanupStack::PopAndDestroy( &array );
    }

// -----------------------------------------------------------------------------
// IsPluginOnStandbyL
// -----------------------------------------------------------------------------
//
TBool CCCESPSettingsHandler::IsPluginOnStandbyL( TUid aImplementationUid )
    {
    CCELOGSTRING( "CCCESPSettingsHandler:: IsPluginOnStandbyL." );
    TBool ret( EFalse );
    
    RIdArray idArray;
    CleanupClosePushL( idArray );
    
    // Create property array
    RPropertyArray* propertyArray = new (ELeave) RPropertyArray( 1 );
    TCleanupItem cleanup( CCCESPSettingsHandler::CleanupPointerArray, propertyArray );
    CleanupStack::PushL( cleanup );
    
    // call provider property
    CSPProperty* callProviderPluginId  = CSPProperty::NewLC();
    callProviderPluginId->SetName( EPropertyCallProviderPluginId );
    callProviderPluginId->SetValue( aImplementationUid.iUid );
    propertyArray->AppendL( callProviderPluginId );
    CleanupStack::Pop( callProviderPluginId );
    
    // Get service's 
    User::LeaveIfError( iSettings->
            FindServiceIdsFromPropertiesL( *propertyArray, idArray ) );
        
    
    TInt count = idArray.Count();
    for ( TInt index( 0 ); index < count && !ret; index++ )
        {
        TInt tmpServiceId = idArray[ index ]; 
        // ETrue if VoIP atribute is enabled or Bootstrap is enabled
        ret = ( IsEnabledL( tmpServiceId ) || 
            ( CapabilitiesL( tmpServiceId ) & EBootstrapCallProvider ) );
        }
    CleanupStack::PopAndDestroy( propertyArray );
    CleanupStack::PopAndDestroy( &idArray );
    CCELOGSTRING2( "CCCESPSettingsHandler:: ret %i ", ret );
    //note: Lint doesn't understand the use of PopAndDestroy and thinks
    //that there is a memory leak for propertyArray, we disable that warning with
    //the following command
    //lint -e429

    return ret; 
    }
//lint +e429
    
// -----------------------------------------------------------------------------
// CleanupPointerArray
// -----------------------------------------------------------------------------
//
void CCCESPSettingsHandler::CleanupPointerArray(  TAny* aPointer )
    {
    CCELOGSTRING( "CCCESPSettingsHandler:: CleanupPointerArray." );
    RPropertyArray* array = static_cast< RPropertyArray* >( aPointer );
    array->ResetAndDestroy(); 
    array->Close();
    delete array;
    }

// end of file
