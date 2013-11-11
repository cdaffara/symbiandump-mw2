/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings API implementation file.
*
*/

#include <e32property.h>
#include <e32cmn.h>
#include <featmgr.h>

#include "spsettings.h"
#include "spsettingsengine.h"
#include "spentry.h"
#include "spproperty.h"
#include "spdefinitions.h"
#include "spdefaultvalues.h"
#include "spsbufferedpublisher.h"
#include "spsapilogger.h" // For logging

_LIT( KSIPVoIPSWVersion, "3.1" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSettings::CSPSettings() : iSettingsWriteAllowed( ETrue )
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPSettings::ConstructL()
    {
    XSPSLOGSTRING( "CSPSettings::ConstructL() - IN" );

    iSettingsEngine = CSPSettingsEngine::NewL();
    
    static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);

    TInt err =  RProperty::Define( KUidSystemCategory, KSPSupportInternetCallKey, 
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
    if( err == KErrNone )
        {
        RProperty::Set( KUidSystemCategory, KSPSupportInternetCallKey, 
            KSPInitFeatureValue );
        }
    else if ( KErrPermissionDenied == err )
        {
        iSettingsWriteAllowed = EFalse;
        }
    else if( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }

    err =  RProperty::Define( KUidSystemCategory, KSPSupportCallOutKey,
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
    if( err == KErrNone )
        {
        RProperty::Set( KUidSystemCategory, KSPSupportCallOutKey, 
            KSPInitFeatureValue );
        }
    else if ( KErrPermissionDenied == err )
        {
        iSettingsWriteAllowed = EFalse;
        }
    else if( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }

    err =  RProperty::Define( KUidSystemCategory, KSPSupportVoIPSSKey, 
        RProperty::EInt, KAllowAllPolicy, KAllowAllPolicy);
    if( err == KErrNone )
        {
        RProperty::Set( KUidSystemCategory, KSPSupportVoIPSSKey, 
            KSPInitFeatureValue );
        }
    else if ( KErrPermissionDenied == err )
        {
        iSettingsWriteAllowed = EFalse;
        }
    else if( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }
        
    FeatureManager::InitializeLibL();
    iFeatureManagerSupportVoIP = FeatureManager::FeatureSupported( 
        KFeatureIdCommonVoip );
    FeatureManager::UnInitializeLib();

    XSPSLOGSTRING( "CSPSettings::ConstructL() - OUT" );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings* CSPSettings::NewL()
    {
    CSPSettings* self = CSPSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings* CSPSettings::NewLC()
    {
    CSPSettings* self = new( ELeave ) CSPSettings;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings::~CSPSettings()
    {
    XSPSLOGSTRING( "CSPSettings::~CSPSettings() - IN" );

    delete iSettingsEngine;

    XSPSLOGSTRING( "CSPSettings::~CSPSettings() - OUT" );
    }


// ---------------------------------------------------------------------------
// Stores new service provider settings entry
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::AddEntryL( CSPEntry& aEntry )
    {
    XSPSLOGSTRING( "CSPSettings::AddEntryL() - IN" );
    
    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {        
        const RPropertyArray& propertyArray = aEntry.GetAllProperties();
        err = CheckSupportedL( propertyArray );
        
        // continue 
        if( err == KErrNone )
            {
            TInt err( iSettingsEngine->BeginTransactionLC() );
            if( err != KErrNone )
                {
                return err;
                }
                
            iSettingsEngine->AddEntryL( aEntry );
            err = iSettingsEngine->CommitTransaction();
            
            if( err == KErrNone )
                {
                // update p&s key for feature checking
                iSettingsEngine->UpdateSupportFeaturePSKeyL();

                // update p&s key for service change notify
                CSpsBufferedPublisher::SetL( 
                    KUidSystemCategory, KSPNotifyChangeKey, aEntry.GetServiceId() );

                }
            }
        }
        
    XSPSLOGSTRING( "CSPSettings::AddEntryL() - OUT" );
    return err;
    }


// ---------------------------------------------------------------------------
// Find service provider settings entry by service ID
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindEntryL( TServiceId aServiceId, 
    CSPEntry& aEntry )
    {
    XSPSLOGSTRING2( "CSPSettings::FindEntryL( %d ) - IN", aServiceId );
    
    TInt err = CheckSupportedL( aServiceId );

    // continue        
    if( err == KErrNone )
        {
        err = iSettingsEngine->FindEntryL( aServiceId, aEntry );
        }

    XSPSLOGSTRING2( "CSPSettings::FindEntryL( %d ) - OUT", aServiceId );
    return err;
    }


// ---------------------------------------------------------------------------
// Update existing service provider settings entry
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::UpdateEntryL( const CSPEntry& aEntry )
    {
    XSPSLOGSTRING( "CSPSettings::UpdateEntry() - IN" );
    
    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {
        const RPropertyArray& propertyArray = aEntry.GetAllProperties();
        RPropertyNameArray nameArray;
        CleanupClosePushL( nameArray );
        iSettingsEngine->NameArrayFromPropertyArrayL( 
            nameArray, propertyArray );

        err = CheckSupportedL( aEntry.GetServiceId(), nameArray );
        CleanupStack::PopAndDestroy( &nameArray );
        
        // continue
        if( err == KErrNone )
            {
            err = iSettingsEngine->BeginTransactionLC();
            if( err != KErrNone )
                {
                return err;
                }
            
            TInt result( iSettingsEngine->UpdateEntryL( aEntry ) );
            err = CommitOrRollbackL( result, aEntry.GetServiceId() );
            }
        }
    XSPSLOGSTRING( "CSPSettings::UpdateEntry() - OUT" );
    return err;
    }


// ---------------------------------------------------------------------------
// Deletes service provider settings entry by service ID
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::DeleteEntryL( TServiceId aServiceId )
    {
    XSPSLOGSTRING2( "CSPSettings::DeleteEntry(%d) - IN", aServiceId );

    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {    
        err = CheckSupportedL( aServiceId );
        
        if( err == KErrNone )
            {
            err = iSettingsEngine->BeginTransactionLC();
            if( err != KErrNone )
                {
                return err;
                }

            TInt result( iSettingsEngine->DeleteEntryL( aServiceId ) );
            err = CommitOrRollbackL( result, aServiceId );
            }
        }        
    XSPSLOGSTRING2( "CSPSettings::DeleteEntry(%d) - OUT", aServiceId );
    return err;
    }


// ---------------------------------------------------------------------------
// Search property of service provider settings entry by property name
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindPropertyL( TServiceId aServiceId, 
    TServicePropertyName aPropertyName, CSPProperty& aProperty )
    {
    XSPSLOGSTRING2( "CSPSettings::FindPropertyL(%d) - IN", aServiceId );

    RPropertyNameArray nameArray;
    CleanupClosePushL( nameArray );
    
    User::LeaveIfError( nameArray.Append( aPropertyName ) );
    
    TInt err = CheckSupportedL( aServiceId, nameArray );
    CleanupStack::PopAndDestroy( &nameArray );
    
    if( err == KErrNone )
        {
        err = iSettingsEngine->FindPropertyL( aServiceId, aPropertyName,
            aProperty );
        }
    
    XSPSLOGSTRING2( "CSPSettings::FindPropertyL(%d) - OUT", aServiceId );
    return err;
    }


// ---------------------------------------------------------------------------
// Add or update properties
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::AddOrUpdatePropertiesL( TServiceId aServiceId,
    const RPropertyArray& aPropertyArray )
    {
    XSPSLOGSTRING2( "CSPSettings::AddOrUpdateProperties(%d) - IN",
        aServiceId );
    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {
        RPropertyNameArray nameArray;
        CleanupClosePushL( nameArray );
        iSettingsEngine->NameArrayFromPropertyArrayL( 
            nameArray, aPropertyArray );

        err = CheckSupportedL( aServiceId, nameArray );
        CleanupStack::PopAndDestroy( &nameArray );
        
        // continue
        if( err == KErrNone )
            {
            TInt err( iSettingsEngine->BeginTransactionLC() );
            if( err != KErrNone )
                {
                return err;
                }

            TInt result( iSettingsEngine->AddOrUpdatePropertiesL( 
                aServiceId, aPropertyArray ) );
            err = CommitOrRollbackL( result, aServiceId );
            }
        }
    XSPSLOGSTRING2( "CSPSettings::AddOrUpdateProperties(%d) - OUT",
        aServiceId );
    return err;
    }

// ---------------------------------------------------------------------------
// Add or update property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::AddOrUpdatePropertyL( TServiceId aServiceId,
    const CSPProperty& aProperty )
    {
    XSPSLOGSTRING2( "CSPSettings::AddOrUpdatePropertyL(%d) - IN", aServiceId );
    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {
        RPropertyArray array;
        CleanupClosePushL( array );
        User::LeaveIfError( array.Append( &aProperty ) );
        err = AddOrUpdatePropertiesL( aServiceId, array );
        
        CleanupStack::PopAndDestroy( &array );
        }
    XSPSLOGSTRING2( "CSPSettings::AddOrUpdatePropertyL(%d) - OUT", aServiceId );
    return err;
    }


// ---------------------------------------------------------------------------
// Returns count of stored service provider settings entries
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::SettingsCountL()
    {
    XSPSLOGSTRING( "CSPSettings::SettingsCount() - IN" );

    TInt result( iSettingsEngine->SettingsCountL() );
    
    XSPSLOGSTRING2( "CSPSettings::SettingsCount(%d) - OUT", result );
    
    return result;
    }


// ---------------------------------------------------------------------------
// Search stored service IDs of service provider settings entries
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindServiceIdsL( RIdArray& aServiceIds )
    {
    XSPSLOGSTRING( "CSPSettings::FindServiceIdsL() - IN" );

    TInt err( iSettingsEngine->FindServiceIdsL( aServiceIds ) );
    
    XSPSLOGSTRING( "CSPSettings::FindServiceIdsL() - OUT" );
    
    return err;
    }


// ---------------------------------------------------------------------------
// Search service names by given service IDs
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindServiceNamesL( RIdArray& aServiceIds, 
    CDesCArray& aServiceNames )
    {
    XSPSLOGSTRING( "CSPSettings::FindServiceNames() - IN" );

    iSettingsEngine->FindServiceNamesL( aServiceIds, aServiceNames );

    XSPSLOGSTRING( "CSPSettings::FindServiceNames() - OUT" );

    return KErrNone;
    }




// ---------------------------------------------------------------------------
// Search certain subservice properties
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindSubServicePropertiesL( TServiceId aServiceId,
    TSPItemType aPropertyType, RPropertyArray& aPropertyArray )
    {
    XSPSLOGSTRING( "CSPSettings::FindSubServiceProperties() - IN" );

    TInt err = CheckSupportedL( aPropertyType );

    // continue        
    if( err == KErrNone )
        {
        err = iSettingsEngine->FindSubServicePropertiesL( aServiceId, 
            aPropertyType, aPropertyArray );
        }

    XSPSLOGSTRING( "CSPSettings::FindSubServiceProperties() - OUT" );
    return err;
    }



// ---------------------------------------------------------------------------
// Deletes service properties by service ID and property name
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::DeleteServicePropertiesL( TServiceId aServiceId, 
    const RPropertyNameArray& aNameArray )
    {
    XSPSLOGSTRING( "CSPSettings::DeleteServiceProperties() - IN" );
    
    TInt err = KErrPermissionDenied;
    if( iSettingsWriteAllowed )
        {
        err = CheckSupportedL( aServiceId, aNameArray );
        
        // continue
        if( err == KErrNone )
            {
            err = iSettingsEngine->BeginTransactionLC();
            if( err != KErrNone )
                {
                return err;
                }
            
            TInt result( iSettingsEngine->DeleteServicePropertiesL( aServiceId, aNameArray ) );
            err = CommitOrRollbackL( result, aServiceId );
            }
        }
    XSPSLOGSTRING( "CSPSettings::DeleteServiceProperties() - OUT" );
    return err;
    }

// ---------------------------------------------------------------------------
// Find service IDs with same properties
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindServiceIdsFromPropertiesL( 
    const RPropertyArray& aPropertyArray, RIdArray& aServiceIds )
    {
    XSPSLOGSTRING( "CSPSettings::FindServiceIdsFromProperties() - IN" );
    
    TInt err = CheckSupportedL( aPropertyArray );
        
    // continue 
    if( err == KErrNone )
        {
        iSettingsEngine->FindServiceIdsFromPropertiesL( aPropertyArray, 
            aServiceIds );
        }
        
    XSPSLOGSTRING( "CSPSettings::FindServiceIdsFromProperties() - OUT" );
    return err;
    }
    
// ---------------------------------------------------------------------------
// Get SIP VoIP software version
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CSPSettings::GetSIPVoIPSWVersion()
    {
    return KSIPVoIPSWVersion;
    }

// ---------------------------------------------------------------------------
// Commit or rollback transaction
// ---------------------------------------------------------------------------
//
TInt CSPSettings::CommitOrRollbackL( TInt aError, TServiceId aServiceId )
    {
    TInt err = KErrNone;
    
    if( aError != KErrNone )
        {
        iSettingsEngine->RollbackTransaction();
        err = aError;
        }
    else
        {
        err = iSettingsEngine->CommitTransaction();
        if( err == KErrNone )
            {
            // update p&s key for feature checking
            iSettingsEngine->UpdateSupportFeaturePSKeyL();

            // update p&s key for service change notify
            CSpsBufferedPublisher::SetL(
                KUidSystemCategory, KSPNotifyChangeKey, aServiceId );
            }
        
        }
        
    return err;
    }

// ---------------------------------------------------------------------------
// Check feature is supported or not
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSPSettings::IsFeatureSupported( TSPServiceFeature aFeature )
    {
    XSPSLOGSTRING( "CSPSettings::IsFeatureSupported() - IN" );

    TBool ret = EFalse;
    TRAP_IGNORE( ret = iSettingsEngine->IsFeatureSupportedL( 
        aFeature, iSettingsWriteAllowed ) );
    
    XSPSLOGSTRING( "CSPSettings::IsFeatureSupported() - OUT" );
    
    return ret;
    }

    
// ---------------------------------------------------------------------------
// Check feature is supported or not
// ---------------------------------------------------------------------------
//
TInt CSPSettings::CheckSupportedL( TServiceId aServiceId, 
    const RPropertyNameArray& aNameArray )
    {
    TInt err ( KErrNone );
    if( !iFeatureManagerSupportVoIP )
        {
        TBool bIsVoIP = iSettingsEngine->IsVoIPServiceL( aServiceId );

        if( bIsVoIP )
            {
            TBool bRestricted = 
                iSettingsEngine->HasCallPropertyName( aNameArray ) || 
                iSettingsEngine->HasVoIPAndVmbxPropertyName( aNameArray );
            
            if( bRestricted )
                {
                err = KErrNotSupported;
                }
            }
        else
            {
            TBool bHasVoIPAndVmbx = 
                iSettingsEngine->HasVoIPAndVmbxPropertyName( aNameArray );
            if( bHasVoIPAndVmbx )
                {
                err = KErrNotSupported;
                }
            }
        }
        
    return err;
    }
    

// ---------------------------------------------------------------------------
// Check feature is supported or not 
// ---------------------------------------------------------------------------
//
TInt CSPSettings::CheckSupportedL( TServiceId aServiceId )
    {
    TInt err = KErrNone;

    if( !iFeatureManagerSupportVoIP )
        {
        TBool bIsVoIP = iSettingsEngine->IsVoIPServiceL( aServiceId );
        if( bIsVoIP )
            {
            err = KErrNotSupported;
            }
        }
        
    return err;
    }

// ---------------------------------------------------------------------------
// Check feature is supported or not 
// ---------------------------------------------------------------------------
//
TInt CSPSettings::CheckSupportedL( TSPItemType aPropertyType )
    {
    TInt err = KErrNone;

    if ( !iFeatureManagerSupportVoIP ) 
        {
        if ( aPropertyType == EItemTypeVoIPSubProperty || 
            aPropertyType == EItemTypeVMBXSubProperty )
            {
            err = KErrNotSupported;
            }
        }
        
    return err;
    }
    

// ---------------------------------------------------------------------------
// Check feature is supported or not 
// ---------------------------------------------------------------------------
//
TInt CSPSettings::CheckSupportedL( const RPropertyArray& aPropertyArray )
    {
    TInt err ( KErrNone );
    
    if( !iFeatureManagerSupportVoIP )
        {
        RPropertyNameArray nameArray;
        CleanupClosePushL( nameArray );
        iSettingsEngine->NameArrayFromPropertyArrayL( 
            nameArray, aPropertyArray );
        
        TBool bHasVoIPAndVmbx = 
            iSettingsEngine->HasVoIPAndVmbxPropertyName( nameArray );
        if( bHasVoIPAndVmbx )
            {
            err = KErrNotSupported;
            }
        CleanupStack::PopAndDestroy( &nameArray );
        }

    return err;
    }


