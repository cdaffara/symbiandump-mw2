/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CVmSPSHandler class.
*
*/

// INCLUDE FILES
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spnotifychange.h>
#include <featmgr.h>
#include "vmspshandler.h"
#include "voicemailboxdefsinternal.h"

#include "vmbxlogger.h"


// CONSTANTS
/**  Maximum length of descriptor data */
const TInt KVmbxMaxDesLength = 512;


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmSPSHandler::CVmSPSHandler
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CVmSPSHandler::CVmSPSHandler()
    {
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::ConstructL =>" );
    iSettings = CSPSettings::NewL();
    iSpsSettingsVoipUtils = CSPSettingsVoIPUtils::NewL();

    iNotifier = CSPNotifyChange::NewL( *this );

    // Subscribe to service change notifies
    // Empty array so that new services will be notified also
    RIdArray array;
    CleanupClosePushL( array );
    // Commented out because voip is not supported yet.
    //iNotifier->NotifyChangeL( iServiceIds );
    iNotifier->NotifyChangeL( array );
    CleanupStack::PopAndDestroy( &array );
     VMBLOGSTRING( "VMBX: CVmSPSHandler::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmSPSHandler* CVmSPSHandler::NewL( )
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NewL =>" );
    CVmSPSHandler* self = CVmSPSHandler::NewLC();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmSPSHandler* CVmSPSHandler::NewLC()
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NewLC =>" );
    CVmSPSHandler* self = new (ELeave) CVmSPSHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NewLC <=" );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CVmSPSHandler::~CVmSPSHandler
// Destructor
// ---------------------------------------------------------------------------
//
CVmSPSHandler::~CVmSPSHandler()
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::~CVmSPSHandler =>" );
    iObserver = NULL;

    if ( iNotifier )
        {
        iNotifier->Cancel();
        delete iNotifier;
        }

    delete iSettings;

    delete iSpsSettingsVoipUtils;
    VMBLOGSTRING( "VMBX: CVmSPSHandler::~CVmSPSHandler <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::GetServiceIdsL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::GetServiceIdsL( RArray<TUint>& aServiceIds ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceIdsL =>" );
    RArray<TUint> serviceIds;
    CleanupClosePushL( serviceIds );
    // Get all service Ids and..
    iSettings->FindServiceIdsL( serviceIds );
    // ..remove service if VMBX is not defined or if VMBX address is empty
    for ( TInt i( 0 ); i <  serviceIds.Count(); i++ )
        {
        if ( IsVbmxSupportedL( serviceIds[ i ] ) )
            {
            aServiceIds.Append( serviceIds[ i ] );
            }
        }

    VMBLOGSTRING2( "VMBX: CVmSPSHandler::GetServiceIdsL: found %d \
        Voip mailbox services", serviceIds.Count() );

    CleanupStack::PopAndDestroy( &serviceIds );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceIdsL <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::IsVbmxSupportedL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CVmSPSHandler::IsVbmxSupportedL( TUint32 aServiceId ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::IsVbmxSupportedL =>" );
    TBool supported( EFalse );
    // Get entry
    CSPEntry* entry = CSPEntry::NewLC();
    iSettings->FindEntryL( aServiceId, *entry );

    // Get all service properties a.k.a subservices
    RPropertyArray properties;
    properties = entry->GetAllProperties();

    TServicePropertyName propertyName;

    for ( TInt i( 0 ); i < properties.Count() && !supported; i++ )
        {
        propertyName = properties[ i ]->GetName();

        supported = propertyName == EPropertyVMBXSubServicePluginId;

        if ( supported )
            {
            TVmbxServiceInfo serviceInfo;
            TRAPD( found, GetVmAddressL( aServiceId, serviceInfo.iAddress ) );

            supported = KErrNotFound != found;
            }
        }

    CleanupStack::PopAndDestroy( entry );
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::IsVbmxSupportedL: %d<=", supported );
    return supported;
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::GetServiceInfo
// (other items were commented in a header).
// ---------------------------------------------------------------------------
TInt CVmSPSHandler::GetServiceInfo( TVmbxServiceInfo& aServiceInfo ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceInfo =>" );
    TInt ret( KErrNone );
    TRAP( ret,
        {
        GetServiceNameL( aServiceInfo.iServiceId, aServiceInfo.iName );

        GetVmAddressL( aServiceInfo.iServiceId, aServiceInfo.iAddress );

        TInt snap( 0 );
        GetSNAPIdL( aServiceInfo.iServiceId, snap );
        aServiceInfo.iSnapId = static_cast<TUint32> ( snap );
        } );

    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceInfo <=" );
    return ret;
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::GetVmAddressL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::GetVmAddressL( const TUint32 aServiceId,
    TDes& aAddress ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetVmAddressL =>" );
    RBuf buffer;
    CleanupClosePushL( buffer );
    buffer.ReAllocL( KVmbxMaxDesLength );

    CSPEntry* entry = CSPEntry::NewLC();
    const CSPProperty* property = NULL;

    iSettings->FindEntryL( aServiceId, *entry );

    // Find correct subservice
    TInt err = entry->GetProperty( property, ESubPropertyVMBXListenAddress );
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::GetVmAddressL: err%d", err );
    User::LeaveIfError( err );

    property->GetValue( buffer );
    buffer.Length() > aAddress.MaxLength() ?
        User::Leave( KErrOverflow ) : aAddress.Copy( buffer );

    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( &buffer );

    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetVmAddressL <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::GetServiceNameL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::GetServiceNameL( const TUint32 aServiceId,
    TDes& aName ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceNameL: =>" );
    RBuf buffer;
    CleanupClosePushL( buffer );
    buffer.ReAllocL( KVmbxMaxDesLength );

    // Get entry
    CSPEntry* entry = CSPEntry::NewLC();

    iSettings->FindEntryL( aServiceId, *entry );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceNameL: 1" );
    // Read service id and service name
    buffer.Copy( entry->GetServiceName() );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceNameL: 2" );
    buffer.Length() > aName.MaxLength() ?
        User::Leave( KErrOverflow ) : aName.Copy( buffer );

    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( &buffer );

    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetServiceNameL: <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::GetSNAPIdL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::GetSNAPIdL( TUint32 aServiceId, TInt& aSNAPId ) const
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetSNAPIdL: =>" );
    CSPEntry* entry = CSPEntry::NewLC();
    const CSPProperty* property = NULL;
    iSettings->FindEntryL( aServiceId, *entry );
    // Find correct subservice
    TInt error(
        entry->GetProperty( property, ESubPropertyVMBXPreferredSNAPId ) );
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::GetSNAPIdL: error%d", error );

    if ( KErrNotFound != error )
        {
        property->GetValue( aSNAPId );
        }

    CleanupStack::PopAndDestroy( entry );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::GetSNAPIdL: <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::BrandIdL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::BrandIdL( TInt aServiceId, TDes8& aBrandId )
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::BrandIdL: =>" );
    CSPEntry* entry = CSPEntry::NewLC();
    const CSPProperty* property = NULL;;
    TBuf<KSPMaxDesLength> brandId( KNullDesC );

    TInt err = iSettings->FindEntryL( aServiceId, *entry );
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::BrandIdL: err%d", err );
    User::LeaveIfError( err );

    err = entry->GetProperty( property, EPropertyBrandId );
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::BrandIdL: err%d", err );
    User::LeaveIfError( err );

    if ( property )
        {
        property->GetValue( brandId );
        }

    if ( brandId.Length() )
        {
        aBrandId.Copy( brandId );
        }
    CleanupStack::PopAndDestroy( entry );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::BrandIdL: <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::IsActivatedL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CVmSPSHandler::IsActivatedL( TInt aServiceId )
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::IsActivatedL: =>" );
    TBool enabled( EFalse );
    CSPEntry* entry = CSPEntry::NewLC();
    const CSPProperty* property = NULL;

    iSettings->FindEntryL( aServiceId, *entry );

    // Find correct subservice
    TInt error(
        entry->GetProperty( property, ESubPropertyVMBXEnabled ) );

    if ( KErrNotFound != error )
        {
        TOnOff onOff( EOONotSet );
        property->GetValue( onOff );
        enabled = onOff == EOn;
        }

    CleanupStack::PopAndDestroy( entry );

    VMBLOGSTRING2( "VMBX: CVmSPSHandler::IsActivatedL: %d", enabled );
    VMBLOGSTRING( "VMBX: CVmSPSHandler::IsActivatedL: <=" );
    return enabled;
    }

// ----------------------------------------------------------------------------
// CVmSPSHandler::NotifyServiceChange
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CVmSPSHandler::NotifyServiceChange( MServiceNotifyHandler* aHandler )
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NotifyServiceChange: =>" );
    iObserver = aHandler;
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NotifyServiceChange: <=" );
    }

// ----------------------------------------------------------------------------
// CVmSPSHandler::NotifyServiceChangeCancel
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CVmSPSHandler::NotifyServiceChangeCancel()
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NotifyServiceChangeCancel: =>" );
    iObserver = NULL;
    VMBLOGSTRING( "VMBX: CVmSPSHandler::NotifyServiceChangeCancel: <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::HandleNotifyChange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::HandleNotifyChange( TServiceId /*ServiceId*/ )
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::HandleNotifyChange: =>" );
    if ( iObserver )
        {
        TRAP_IGNORE( iObserver->HandleServiceNotifyL() );
        }
    VMBLOGSTRING( "VMBX: CVmSPSHandler::HandleNotifyChange: <=" );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::HandleError
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVmSPSHandler::HandleError( TInt aError )
    {
    // Avoid warning
    aError = aError;
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::HandleError: aError %d", aError );
    }

// ---------------------------------------------------------------------------
// CVmSPSHandler::IsVoIPProfilesFound
// 
// ---------------------------------------------------------------------------
//
TBool CVmSPSHandler::IsVoIPProfilesFound()
    {
    VMBLOGSTRING( "VMBX: CVmSPSHandler::IsVoIPProfilesFound" );
    TBool ret( EFalse );
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        TRAP_IGNORE( ret = iSpsSettingsVoipUtils->VoIPProfilesExistL() );
        }
    VMBLOGSTRING2( "VMBX: CVmSPSHandler::IsVoIPProfilesFound: return %I", ret );
    return ret;
    }

//  End of File
