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
* Description:  Service Provider Settings API's engine.
*
*/

// The entire file is taken from 3.2.3 baseline

#include <centralrepository.h>
#include <e32property.h>
#include <FeatMgr.h>
#include <SettingsInternalCRKeys.h>
#include <cenrepdatabaseutil.h>

#include "spsettingsengine.h"
#include "spentry.h"
#include "spproperty.h"
#include "spcrkeys.h"
#include "spdefinitions.h"
#include "spdefaultvalues.h"


#include "spsapilogger.h" // For logging

const TInt KSPPredefinedCount = 2;
TInt counter( 0 );



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSettingsEngine::CSPSettingsEngine()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::ConstructL()
    {
    XSPSLOGSTRING( "CSPSettingsEngine::ConstructL() - IN" );
    
    iCenRepUtils = CCenRepDatabaseUtil::NewL( KCRUidSPSettings, 
                                            KSPStartKey,
                                            KSPColIncrement,         
                                            KSPColMask, 
                                            KServiceIdCounter,
                                            KSPColCount );

    XSPSLOGSTRING( "CSPSettingsEngine::ConstructL() - OUT" );
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSettingsEngine* CSPSettingsEngine::NewL()
    {
    CSPSettingsEngine* self = CSPSettingsEngine::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPSettingsEngine* CSPSettingsEngine::NewLC()
    {
    CSPSettingsEngine* self = new( ELeave ) CSPSettingsEngine;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSPSettingsEngine::~CSPSettingsEngine()
    {
    XSPSLOGSTRING( "CSPSettingsEngine::~CSPSettingsEngine() - IN" );
    
    delete iCenRepUtils;

    XSPSLOGSTRING( "CSPSettingsEngine::~CSPSettingsEngine() - OUT" );
    }


// ---------------------------------------------------------------------------
// Begins transaction in repository.
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::BeginTransactionLC()
    {
    XSPSLOGSTRING( "CSPSettingsEngine::BeginTransaction() - IN" );

    iCenRepUtils->BeginTransactionL();
    
    XSPSLOGSTRING( "CSPSettingsEngine::BeginTransaction() - OUT" );
    
    return KErrNone;
    
    }


// ---------------------------------------------------------------------------
// Commits changes in repository.
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::CommitTransaction()
    {
    XSPSLOGSTRING( 
        "CSPSettingsEngine::CommitTransaction() - IN" );

    iCenRepUtils->CommitTransaction();
    
    XSPSLOGSTRING( "CSPSettingsEngine::CommitTransaction() - OUT" );

    return KErrNone;
    }


// ---------------------------------------------------------------------------
// Rollback changes in repository.
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::RollbackTransaction()
    {
    XSPSLOGSTRING( 
        "CSPSettingsEngine::RollbackTransaction() - IN" );

    iCenRepUtils->RollbackTransaction();
    
    XSPSLOGSTRING( "CSPSettingsEngine::RollbackTransaction() - OUT" );

    }

void CSPSettingsEngine::ConvertSpEntryToCenRepArrayL( const CSPEntry& aEntry, 
                                                     RIpAppPropArray& aArray )
    {
    CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
    
    property->SetName( EServiceName );
    property->SetValue( aEntry.GetServiceName() );
    User::LeaveIfError( aArray.Append( property ) );
    CleanupStack::Pop( property );
    
    TInt count = aEntry.PropertyCount();
    const RPropertyArray& entryArray = aEntry.GetAllProperties();
    
    ConvertSpArrayToCenRepArrayL( entryArray, aArray );
    }
    
void CSPSettingsEngine::ConvertSpArrayToCenRepArrayL( const RPropertyArray& aSpArray,
                                                      RIpAppPropArray& aCenRepArray )
    {
    TInt count = aSpArray.Count();
    for( TInt i = 0; i < count; i++ )
        {
        CSPProperty* spProperty = aSpArray[i];
        CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
        property->SetName( spProperty->GetName() );
        
        RBuf value;
        value.CreateL( KSPMaxDesLength );
        CleanupClosePushL( value);
        User::LeaveIfError( spProperty->GetValue( value ) );
        User::LeaveIfError( property->SetValue( value ) );
        CleanupStack::PopAndDestroy( &value );
        
        User::LeaveIfError( aCenRepArray.Append( property ) );
        CleanupStack::Pop( property );
        }
    }
    
void CSPSettingsEngine::CleanupPointerArray(  TAny* aPointer )
	{
	RIpAppPropArray* array = static_cast<RIpAppPropArray*>( aPointer );
	array->ResetAndDestroy();
	}

// ---------------------------------------------------------------------------
// Stores new service provider settings entry
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::AddEntryL( CSPEntry& aEntry )
    {
    XSPSLOGSTRING( "CSPSettingsEngine::AddEntryL() - IN" );
    
    // check the service name is not empty
    if( aEntry.GetServiceName().Length() == 0 )
    	{
    	User::LeaveIfError( KErrArgument );
    	}

    // construct RIpAppPropArray
    RIpAppPropArray array;
	TCleanupItem cleanup( CSPSettingsEngine::CleanupPointerArray, &array );
	CleanupStack::PushL( cleanup );
    
    ConvertSpEntryToCenRepArrayL( aEntry, array );

    // Create new unique service ID
    TInt newId = 0;
    iCenRepUtils->AddEntryL( newId, array );
    aEntry.SetServiceId( newId );
    
    CleanupStack::PopAndDestroy( &array );
    
    XSPSLOGSTRING( "CSPSettingsEngine::AddEntryL() - OUT" );
    }

// ---------------------------------------------------------------------------
// Find service provider settings entry by service ID
// ---------------------------------------------------------------------------
//
 TInt CSPSettingsEngine::FindEntryL( TServiceId aServiceId, CSPEntry& aEntry )
    {
    XSPSLOGSTRING2( "CSPSettingsEngine::FindEntryL( %d ) - IN", aServiceId );
    
    RIpAppPropArray array;
	TCleanupItem cleanup( CSPSettingsEngine::CleanupPointerArray, &array );
	CleanupStack::PushL( cleanup );

    TInt err = iCenRepUtils->FindEntryL( aServiceId, array );
    
    if( err == KErrNone )
        {
        TInt count = array.Count();

        for( TInt i = 0; i < count; i++ )
            {
            CCenRepDatabaseProperty* property = array[i];
            
            if( property->GetName() == EServiceName )
                {
                User::LeaveIfError( aEntry.SetServiceName( property->GetDesValue() ) );
                }
            else
                {
                CSPProperty* spProperty = CSPProperty::NewLC();
                User::LeaveIfError( spProperty->SetName( (TServicePropertyName)( property->GetName() )));
                User::LeaveIfError( spProperty->SetValue( property->GetDesValue() ));
                
                User::LeaveIfError( aEntry.AddPropertyL( *spProperty ));
                CleanupStack::PopAndDestroy( spProperty );
                }
            }
        }
    CleanupStack::PopAndDestroy( &array );
    
    aEntry.SetServiceId( aServiceId );
    
    XSPSLOGSTRING2( "CSPSettingsEngine::FindEntryL( %d ) - OUT", aServiceId );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Update existing service provider settings entry
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::UpdateEntryL( const CSPEntry& aEntry )
    {
    XSPSLOGSTRING( "CSPSettingsEngine::UpdateEntry() - IN" );

    TServiceId serviceId( aEntry.GetServiceId() );

    // Leave if there is no service ID 
    if ( KSPNoId == serviceId || aEntry.GetServiceName().Length() == 0 )
        {
        User::Leave( KErrArgument );
        }

    // construct RIpAppPropArray
    RIpAppPropArray array;
	TCleanupItem cleanup( CSPSettingsEngine::CleanupPointerArray, &array );
	CleanupStack::PushL( cleanup );
    
    ConvertSpEntryToCenRepArrayL( aEntry, array );

    TInt err = iCenRepUtils->UpdateEntryL( (TInt) serviceId, array );
    
    CleanupStack::PopAndDestroy( &array );
	
    XSPSLOGSTRING( "CSPSettingsEngine::UpdateEntry() - OUT" );
    
    return err;
    }

	
// ---------------------------------------------------------------------------
// Deletes service provider settings entry by service ID
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::DeleteEntryL( TServiceId aServiceId )
    {
    XSPSLOGSTRING2( "CSPSettingsEngine::DeleteEntryL(%d) - IN", aServiceId );

    TInt err = iCenRepUtils->DeleteEntryL( aServiceId );
    
    XSPSLOGSTRING2( "CSPSettingsEngine::DeleteEntryL(%d) - OUT", aServiceId );
    
    return err;
    }


// ---------------------------------------------------------------------------
// Search property of service provider settings entry by property name
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::FindPropertyL( TServiceId aServiceId, 
    								   TServicePropertyName aPropertyName, 
    								   CSPProperty& aProperty )
    {
    XSPSLOGSTRING2( "CSPSettingsEngine::FindPropertyL(%d) - IN", aServiceId );

    CCenRepDatabaseProperty* cenrepProperty = CCenRepDatabaseProperty::NewLC();
    TInt err = iCenRepUtils->FindPropertyL( aServiceId, aPropertyName, *cenrepProperty );
    
    if( err == KErrNone )
        {
        User::LeaveIfError( aProperty.SetName( aPropertyName ) );
        User::LeaveIfError( aProperty.SetValue( cenrepProperty->GetDesValue()));
        }
        
    CleanupStack::PopAndDestroy( cenrepProperty );
    
    XSPSLOGSTRING2( "CSPSettingsEngine::FindPropertyL(%d) - OUT", aServiceId );
    
/*    if ( !counter )
        {
        counter++;
        }
    else
        {
        err = KErrNone;
        }*/
//    err = KErrNone;
    return err;
    }


// ---------------------------------------------------------------------------
// Add or update properties of service provider settings entry
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::AddOrUpdatePropertiesL( TServiceId aServiceId,
												const RPropertyArray& aPropertyArray )
    {
    XSPSLOGSTRING2( "CSPSettingsEngine::AddOrUpdatePropertiesL(%d) - IN", aServiceId );

    // construct RIpAppPropArray
    RIpAppPropArray array;
	TCleanupItem cleanup( CSPSettingsEngine::CleanupPointerArray, &array );
	CleanupStack::PushL( cleanup );
    
    ConvertSpArrayToCenRepArrayL( aPropertyArray, array );
    
    TInt err = iCenRepUtils->AddOrUpdatePropertiesL( aServiceId, array );    

    CleanupStack::PopAndDestroy( &array );
    
    XSPSLOGSTRING2( 
        "CSPSettingsEngine::AddOrUpdatePropertiesL(%d) - OUT", aServiceId );
        
    return err;
    }


// ---------------------------------------------------------------------------
// Returns count of stored service provider settings entries
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::SettingsCountL()
    {
    XSPSLOGSTRING( "CSPSettingsEngine::SettingsCount() - IN" );

    TInt result( 0 );

    TInt err = iCenRepUtils->EntryCountL( result );
    
    if( err == KErrNone )
        {
      
        if( result >= KSPPredefinedCount )
            {
            result--;
            }
         
        err = result;
        }
    else
        {
        err = 0;
        }
    
    XSPSLOGSTRING2( "CSPSettingsEngine::SettingsCount(%d) - OUT", result );

    return err;
    }



// ---------------------------------------------------------------------------
// Search stored service IDs of service provider settings entries
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::FindServiceIdsL( RIdArray& aServiceIds )
    {
    XSPSLOGSTRING( "CSPSettingsEngine::FindServiceIdsL() - IN" );

    RArray<TInt> cenrepArray;
    CleanupClosePushL( cenrepArray );
    
    TInt err = iCenRepUtils->FindEntryIdsL( cenrepArray );
    
    for( TInt i = 0; i < cenrepArray.Count(); i++ )
        {
        TServiceId id = (TServiceId)cenrepArray[i];
        if( id != KSPDefaultVoIPServiceId )
            {
            User::LeaveIfError( aServiceIds.Append( id ));
            
            if ( i > 2 )
                {
                aServiceIds.Append( 2 );
                break;
                }
            }
        }

    CleanupStack::PopAndDestroy( &cenrepArray );
    
    XSPSLOGSTRING( "CSPSettingsEngine::FindServiceIdsL() - OUT" );
    
    return err;
    }


// ---------------------------------------------------------------------------
// Search service names by given service IDs
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::FindServiceNamesL( RIdArray& aServiceIds,
    CDesCArray& aServiceNames )
    {
    XSPSLOGSTRING( "CSPSettingsEngine::FindServiceNamesL() - IN" );
    
    CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
    TInt count = aServiceIds.Count();
    for( TInt i = 0; i < count; i++ )
        {
        TInt id = ( TInt )aServiceIds[i];
        
        TInt err = iCenRepUtils->FindPropertyL( id, EServiceName, *property );
        if( err == KErrNone )
            {
            aServiceNames.AppendL( property->GetDesValue() );
            }
        else
            {
            aServiceIds.Remove( i ); // Remove non-existent ID
            count--;
            i--; // Do not increment index
            }
        }

    CleanupStack::PopAndDestroy( property );
    
    XSPSLOGSTRING( "CSPSettingsEngine::FindServiceNamesL() - OUT" );
    }

void CSPSettingsEngine::PropertyNameArrayFromItemTypeL( TSPItemType aPropertyType, 
                                                        RPropertyNameArray& aNameArray )
    {
    switch ( aPropertyType )
        {
        case EItemTypeVoIPSubProperty:
            {
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPSettingsId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPPreferredSNAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPPreferredIAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPTemporaryIAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPRelNumber ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPBrandDataUri ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPBrandIconUri ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPAddrScheme ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVoIPEnabled ) );
            break;
            }
        case EItemTypePresenceSubProperty:
            {
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceSettingsId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresencePreferredSNAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresencePreferredIAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresencePresentityIDFieldType ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceLaunchMethod ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceLaunchUid ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceAddrScheme ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceEnabled ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyPresenceRequestPreference ) );
            break;
            }
        case EItemTypeIMSubProperty:
            {
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMSettingsId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMPreferredSNAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMPreferredIAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMLaunchMethod ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMLaunchUid ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMAddrScheme ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyIMEnabled ) );
            break;
            }
        case EItemTypeVMBXSubProperty:
            {
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXSettingsId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXPreferredSNAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXPreferredIAPId ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXMWISubscribeInterval ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXLaunchMethod ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXLaunchUid ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXListenAddress ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXMWIAddress ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXBrandIconUri ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXAddrScheme ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXListenRegister ) );
            User::LeaveIfError( aNameArray.Append( ESubPropertyVMBXEnabled ) );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::FindSubServicePropertiesL( TServiceId aServiceId, 
	TSPItemType aPropertyType,
    RPropertyArray& aPropertyArray )
    {
    XSPSLOGSTRING2( 
        "CSPSettingsEngine::FindSubServicePropertiesL( %d ) - IN", aServiceId );

    RPropertyNameArray nameArray;
    CleanupClosePushL( nameArray );
    
    PropertyNameArrayFromItemTypeL( aPropertyType, nameArray );
    TInt count = nameArray.Count();
    TInt id = (TInt) aServiceId;
    
    CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
    
    for( TInt i = 0; i < count; i++ )
        {
        TUint32 name = nameArray[i];
        TInt err = iCenRepUtils->FindPropertyL( id, name, *property );
        if( err == KErrNone )
            {
            CSPProperty* spProperty = CSPProperty::NewLC();
            User::LeaveIfError( spProperty->SetName( (TServicePropertyName)(property->GetName())) );
            User::LeaveIfError( spProperty->SetValue( property->GetDesValue() ));
            
            User::LeaveIfError( aPropertyArray.Append( spProperty ));
            CleanupStack::Pop( spProperty );
            }
        }
        
    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( &nameArray );
    
    XSPSLOGSTRING( "CSPSettingsEngine::FindSubServicePropertiesL() - OUT" );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Deletes service properties
// ---------------------------------------------------------------------------
//
TInt CSPSettingsEngine::DeleteServicePropertiesL( TServiceId aServiceId,
    											  const RPropertyNameArray& aNameArray  )
    {
    XSPSLOGSTRING2( 
        "CSPSettingsEngine::FindSubServicePropertyL( %d ) - IN", aServiceId );

    RArray<TUint32> nameArray;
    CleanupClosePushL( nameArray );
    TInt count = aNameArray.Count();
    
    for( TInt i = 0; i < count; i++ )
        {
        TUint32 name = (TUint32)( aNameArray[i] );
        User::LeaveIfError( nameArray.Append( name ) );
        }
        
    TInt id = (TInt)aServiceId;
    TInt err = iCenRepUtils->DeletePropertiesL( id, nameArray );
    
    CleanupStack::PopAndDestroy( &nameArray );
    
    XSPSLOGSTRING( "CSPSettingsEngine::FindSubServicePropertyL() - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Find service IDs with same properties
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::FindServiceIdsFromPropertiesL( const RPropertyArray& aPropertyArray, 
													   RIdArray& aServiceIds )
	{
	XSPSLOGSTRING( "CSPSettingsEngine::FindServiceIdsFromPropertiesL() - IN" );
	
    RArray<TInt> entryIds;
    CleanupClosePushL( entryIds );
    
    RIpAppPropArray array;
	TCleanupItem cleanup( CSPSettingsEngine::CleanupPointerArray, &array );
	CleanupStack::PushL( cleanup );
    
    // todo custom cleanup stack
    ConvertSpArrayToCenRepArrayL( aPropertyArray, array );  
    
    TInt err = iCenRepUtils->FindEntryIdsFromPropertiesL( array, entryIds );
    if( err == KErrNone )
        {
        TInt count = entryIds.Count();
        for( TInt i = 0; i < count; i++ )
            {
            TServiceId id = (TServiceId)entryIds[i];
            if( id != KSPDefaultVoIPServiceId )
                {
                User::LeaveIfError( aServiceIds.Append( id ) );
                }
            }
        }
        
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( &entryIds );
    
	XSPSLOGSTRING( "CSPSettingsEngine::FindServiceIdsFromPropertiesL() - OUT" );
	}



// ---------------------------------------------------------------------------
// Check feature is supported or not
// ---------------------------------------------------------------------------
//
TBool CSPSettingsEngine::IsFeatureSupportedL( TSPServiceFeature aFeature,
                                              TBool aWriteAllowed )
	{
    XSPSLOGSTRING( "CSPSettingsEngine::IsFeatureSupportedL() - IN" );

    TBool ret = EFalse;
    
	// First check P&S key, if P&S key is defined, use the value from P&S key.
	// if not defined, then walk through all the sevices and check there is any service match
	TInt mask = 0;
	TInt value = 0;
	TInt psFlag = 0;
	TInt retProperty = 0;
	
    switch( aFeature )
    	{
    	case ESupportInternetCallFeature:
    		{
    		retProperty = RProperty::Get( KUidSystemCategory, KSPSupportInternetCallKey, psFlag );
    		if( retProperty == KErrNone && psFlag != KSPInitFeatureValue )
    			{
    			ret = psFlag;
    			}
    		else
    			{
				mask = ESupportsInternetCall | ESupportsAlphanumericAddressing | EIsVisibleInCallMenu;
				value = mask;
				ret = CheckFeatureL( mask, value );
				if ( aWriteAllowed )
				    {				    
				    User::LeaveIfError( RProperty::Set( KUidSystemCategory, 
				        KSPSupportInternetCallKey, ret ) );
				    }
    			}
    		
    		break;
    		}
    	case ESupportCallOutFeature:
    		{
    		retProperty = RProperty::Get( KUidSystemCategory, KSPSupportCallOutKey, psFlag );
    		if( retProperty == KErrNone && psFlag != KSPInitFeatureValue )
    			{
    			ret = psFlag;
    			}
    		else
    			{
				mask = ESupportsInternetCall | ESupportsMSISDNAddressing | EIsVisibleInCallMenu;
				value = mask;
				ret = CheckFeatureL( mask, value );
				if ( aWriteAllowed )
				    {				    
    				User::LeaveIfError( RProperty::Set( 
    				    KUidSystemCategory, 
    				    KSPSupportCallOutKey, ret ) );
				    }
    			}
    		break;
    		}
    	
    	case ESupportVoIPSSFeature:
    		{
    		retProperty = RProperty::Get( KUidSystemCategory, KSPSupportVoIPSSKey, psFlag );
    		if( retProperty == KErrNone && psFlag != KSPInitFeatureValue )
    			{
    			ret = psFlag;
    			}
    		else
    			{
				mask = ESupportVoIPSS;
				value = mask;
				ret = CheckFeatureL( mask, value );
				if ( aWriteAllowed )
				    {				    
    				User::LeaveIfError( RProperty::Set( 
    				    KUidSystemCategory, 
    				    KSPSupportVoIPSSKey, ret ) );
				    }
    			}
    		break;
    		}
    	
    	case ESupportVoIPFeature:
    		{
    		TBool bFeatureMgr = EFalse;
    		TBool bDynVoIP = EFalse;
    		TBool bSubVoIP = EFalse;
    		
    		//check FeatureManager
    		FeatureManager::InitializeLibL();
    		bFeatureMgr = FeatureManager::FeatureSupported( KFeatureIdCommonVoip );
    		FeatureManager::UnInitializeLib();
    		
    		//check dynamic voip flag
			CRepository* repository = CRepository::NewLC( KCRUidTelephonySettings );
			TInt value = 0;
			TInt err = repository->Get( KDynamicVoIP, value );
			if( value != 0 )
				{
				bDynVoIP = ETrue;
				}
			
			CleanupStack::PopAndDestroy( repository );
    		
    		//check subservice voip
			RIdArray serviceIds;
			CleanupClosePushL( serviceIds );
			FindServiceIdsL( serviceIds );
			CSPProperty* property = CSPProperty::NewLC();
			
			for( TInt i = 0; i < serviceIds.Count(); i++ )
				{
				TServiceId serviceId = serviceIds[i];
				TInt retProperty = FindPropertyL( serviceId, EPropertyVoIPSubServicePluginId, *property );
				if( retProperty == KErrNone )
					{
					bSubVoIP = ETrue;
					break;
					}
				}
			CleanupStack::PopAndDestroy( property );
			CleanupStack::PopAndDestroy( &serviceIds );
			
			ret = bFeatureMgr && bDynVoIP && bSubVoIP;
			
    		break;
    		}
		default:
			{
			User::Leave( KErrNotSupported );    	
			}
    	}
    
    XSPSLOGSTRING( "CSPSettingsEngine::IsFeatureSupportedL() - OUT" );

    return ret;
	}

// ---------------------------------------------------------------------------
// When service table is changed, update the P&S key
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::UpdateSupportFeaturePSKeyL()
	{
	TInt mask = 0;
	TInt value = 0;
	TBool ret = EFalse;
	
	//ESupportInternetCallFeature
	//ESupportsInternetCall + ESupportsAlphanumericAddressing + EIsVisibleInCallMenu
	mask = ESupportsInternetCall | ESupportsAlphanumericAddressing | EIsVisibleInCallMenu;
	value = mask;
	ret = CheckFeatureL( mask, value );
	User::LeaveIfError( RProperty::Set( KUidSystemCategory, KSPSupportInternetCallKey, ret ) );
	
	//ESupportCallOutFeature
	//ESupportsInternetCall + ESupportsMSISDNAddressing + EIsVisibleInCallMenu
	mask = ESupportsInternetCall | ESupportsMSISDNAddressing | EIsVisibleInCallMenu;
	value = mask;
	ret = CheckFeatureL( mask, value );
	User::LeaveIfError( RProperty::Set( KUidSystemCategory, KSPSupportCallOutKey, ret ) );
	
	//ESupportVoIPSSFeature
	//ESupportVoIPSS
	mask = ESupportVoIPSS;
	value = mask;
	ret = CheckFeatureL( mask, value );
	User::LeaveIfError( RProperty::Set( KUidSystemCategory, KSPSupportVoIPSSKey, ret ) );
	}

// ---------------------------------------------------------------------------
// Walk through all the sevices and check there is any service match 
// ---------------------------------------------------------------------------
//
TBool CSPSettingsEngine::CheckFeatureL( TInt aMask, TInt aValue )
	{
	RArray<TInt> entryIds;
	CleanupClosePushL( entryIds );
	iCenRepUtils->FindEntryIdsL( entryIds );
	CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
	TBool ret = EFalse;
	
	for( TInt i = 0; i < entryIds.Count(); i++ )
		{
		TInt entryId = entryIds[i];
		if( entryId == KSPDefaultVoIPServiceId )
		    {
		    continue;
		    }
		    
		TInt retProperty = iCenRepUtils->FindPropertyL( entryId, EPropertyServiceAttributeMask, *property );
		if( retProperty == KErrNone )
			{
			TInt maskValue = 0;
			TInt err = property->GetValue( maskValue );
			if( err == KErrNone )
				{
				if( ( maskValue & aMask ) == (aValue & aMask) )
					{
					// Find matched servcie
					ret = ETrue;
					break;
					}
				}
			}
		}
	
	CleanupStack::PopAndDestroy( property );
	CleanupStack::PopAndDestroy( &entryIds );
	return ret;
	}

// ---------------------------------------------------------------------------
// Check if it is VoIP Service ID
// ---------------------------------------------------------------------------
//
TBool CSPSettingsEngine::IsVoIPServiceL( TServiceId aServiceId )
    {
    TBool bIsVoIP = EFalse;
    
    CSPProperty* property = CSPProperty::NewLC();
    TInt err = FindPropertyL( aServiceId, EPropertyVoIPSubServicePluginId, *property );
    if( err == KErrNone )
        {
        bIsVoIP = ETrue;
        }
    CleanupStack::PopAndDestroy( property );
    
    return bIsVoIP;
    }

// ---------------------------------------------------------------------------
// Check if an array has call related property name in it
// ---------------------------------------------------------------------------
//
TBool CSPSettingsEngine::HasCallPropertyName( const RPropertyNameArray& aNameArray )
    {
    TBool bHasCall = EFalse;
    
    for( TInt i = 0; i < aNameArray.Count(); i++ )
        {
        TServicePropertyName name = aNameArray[i];
        if( name == EPropertyCTIPluginId ||
            name == EPropertyCLIPluginId ||
            name == EPropertyCallLoggingPluginId ||
            name == EPropertyCallStateIndicatorPluginId ||
            name == EPropertyCallMenuHandlerPluginId ||
            name == EPropertyCallProviderPluginId )
            {
            bHasCall = ETrue;
            break;
            }
        }
    
    return bHasCall;
    }
    
// ---------------------------------------------------------------------------
// Check if an array has VoIP or Vmbx related property name in it
// ---------------------------------------------------------------------------
//
TBool CSPSettingsEngine::HasVoIPAndVmbxPropertyName(  const RPropertyNameArray& aNameArray )
    {
    TBool bHasVoIPAndVmbx = EFalse;
    
    for( TInt i = 0; i < aNameArray.Count(); i++ )
        {
        TServicePropertyName name = aNameArray[i];
        if( name == EPropertyVoIPSubServicePluginId ||
            name == EPropertyVMBXSubServicePluginId ||
            name == ESubPropertyVoIPSettingsId ||
            name == ESubPropertyVoIPPreferredSNAPId ||
            name == ESubPropertyVoIPPreferredIAPId ||
            name == ESubPropertyVoIPTemporaryIAPId ||
            name == ESubPropertyVMBXSettingsId ||
            name == ESubPropertyVMBXPreferredSNAPId ||
            name == ESubPropertyVMBXPreferredIAPId ||
            name == ESubPropertyVMBXMWISubscribeInterval ||
            name == ESubPropertyVMBXLaunchMethod ||
            name == ESubPropertyVMBXLaunchUid ||
            name == ESubPropertyVoIPRelNumber ||
            name == ESubPropertyVoIPBrandDataUri ||
            name == ESubPropertyVoIPBrandIconUri ||
            name == ESubPropertyVoIPAddrScheme ||
            name == ESubPropertyVMBXListenAddress ||
            name == ESubPropertyVMBXMWIAddress ||
            name == ESubPropertyVMBXBrandIconUri ||
            name == ESubPropertyVMBXAddrScheme ||
            name == ESubPropertyVoIPEnabled ||
            name == ESubPropertyVMBXListenRegister ||
            name == ESubPropertyVMBXEnabled )
            {
            bHasVoIPAndVmbx = ETrue;
            break;
            }
        }
    
    return bHasVoIPAndVmbx;
    }

// ---------------------------------------------------------------------------
// Get Name array from Property array 
// ---------------------------------------------------------------------------
//
void CSPSettingsEngine::NameArrayFromPropertyArrayL( RPropertyNameArray& aNameArray, 
												const RPropertyArray& aPropertyArray )
    {
    aNameArray.Reset();
    
    for( TInt i = 0; i < aPropertyArray.Count(); i++ )
        {
        CSPProperty* property = aPropertyArray[i];
        TServicePropertyName name = property->GetName();
        User::LeaveIfError( aNameArray.Append( name ) );
        }
    }

