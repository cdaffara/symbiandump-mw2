/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of T_CSPSettings class.
*
*/

#include "T_CSPSettings.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <spentry.h>
#include <spproperty.h>
#include "spsettings.h"
#include "TestData.h"

// - Construction -----------------------------------------------------------

T_CSPSettings* T_CSPSettings::NewL()
    {
    T_CSPSettings* self = T_CSPSettings::NewLC();
    CleanupStack::Pop();
    return self;
    }

T_CSPSettings* T_CSPSettings::NewLC()
    {
    T_CSPSettings* self = new( ELeave ) T_CSPSettings();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_CSPSettings::~T_CSPSettings()
    {
    }

T_CSPSettings::T_CSPSettings()
    {
    }

void T_CSPSettings::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void T_CSPSettings::SetupL(  )
    {
    iCSPSettings = CSPSettings::NewL();
    }

void T_CSPSettings::Setup2L(  )
    {
    TestData::iCheckSupportedCase = ETrue;
    iCSPSettings = CSPSettings::NewL();
    }
    

void T_CSPSettings::Teardown(  )
    {
    delete iCSPSettings;
    iCSPSettings = NULL;
    }

void T_CSPSettings::T_CSPSettings_AddEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );

    EUNIT_ASSERT( KErrNone == iCSPSettings->AddEntryL( *entry ) );
    
    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettings::T_CSPSettings_FindEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    TInt id = entry->GetServiceId();
    
    EUNIT_ASSERT( KErrNotFound == iCSPSettings->FindEntryL( id, *entry ) );
    
    iCSPSettings->AddEntryL( *entry );
    id = entry->GetServiceId();
    EUNIT_ASSERT( KErrNone == iCSPSettings->FindEntryL( id, *entry ) );
    
    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettings::T_CSPSettings_UpdateEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    
    EUNIT_ASSERT_LEAVE( iCSPSettings->UpdateEntryL( *entry ) );
    
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    iCSPSettings->AddEntryL( *entry );
    EUNIT_ASSERT( KErrNone == iCSPSettings->UpdateEntryL( *entry ) );
    
    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettings::T_CSPSettings_DeleteEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    TInt id = entry->GetServiceId();
    
    EUNIT_ASSERT( KErrNotFound == iCSPSettings->DeleteEntryL( id ) );
    
    iCSPSettings->AddEntryL( *entry );
    id = entry->GetServiceId();
    EUNIT_ASSERT( KErrNone == iCSPSettings->DeleteEntryL( id ) );
    CleanupStack::PopAndDestroy( entry );
    }

void T_CSPSettings::T_CSPSettings_FindPropertyLL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();
    property->SetName( EPropertyVoIPSubServicePluginId );
    TInt profId( 15 );
    User::LeaveIfError( property->SetValue( profId ));

    EUNIT_ASSERT( KErrNotFound == iCSPSettings->FindPropertyL(
        0, EPropertyVoIPSubServicePluginId, *property ) );
    
    EUNIT_ASSERT( KErrNone == iCSPSettings->FindPropertyL(
        profId, EPropertyVoIPSubServicePluginId, *property ) );
    
    CleanupStack::PopAndDestroy( property );
    }
    
void T_CSPSettings::T_CSPSettings_AddOrUpdatePropertiesLL(  )
    {
    // iCSPSettings->AddOrUpdatePropertiesL( <add parameters here > );
     EUNIT_ASSERT_DESC( EFalse, "Generated assert, replace with real");
    }
    
void T_CSPSettings::T_CSPSettings_AddOrUpdatePropertyLL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();
    property->SetName( EPropertyVoIPSubServicePluginId );
    TInt profId( 15 );
    User::LeaveIfError( property->SetValue( profId ));
    
    EUNIT_ASSERT( KErrNone == iCSPSettings->AddOrUpdatePropertyL( profId, *property ) );
     
    CleanupStack::PopAndDestroy( property );
    }
    
void T_CSPSettings::T_CSPSettings_SettingsCountLL(  )
    {
    TInt count = iCSPSettings->SettingsCountL( );
    EUNIT_ASSERT( count );
    }
    
void T_CSPSettings::T_CSPSettings_FindServiceIdsLL(  )
    {
    RIdArray ids;
    CleanupClosePushL( ids );
    
    EUNIT_ASSERT( KErrNone == iCSPSettings->FindServiceIdsL( ids ) );
    TInt count( ids.Count() );
    EUNIT_ASSERT( count );
    
    CleanupStack::PopAndDestroy( &ids );
    }
    
void T_CSPSettings::T_CSPSettings_FindServiceNamesLL(  )
    {
    RIdArray ids;
    CleanupClosePushL( ids );
    iCSPSettings->FindServiceIdsL( ids );
    
    CDesCArrayFlat* names = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( names );
    EUNIT_ASSERT( KErrNone == iCSPSettings->FindServiceNamesL( ids, *names ) );
    TInt count = names->MdcaCount();
    EUNIT_ASSERT( count );
    
    CleanupStack::PopAndDestroy( names );
    CleanupStack::PopAndDestroy( &ids );
    }

void T_CSPSettings::T_CSPSettings_FindSubServicePropertiesLL(  )
    {
    RPropertyArray* propertyArray = new (ELeave) RPropertyArray( 3 );
    CleanupStack::PushL( propertyArray );
    
    iCSPSettings->FindSubServicePropertiesL( 0, EItemTypeVMBXSubProperty, *propertyArray );
    TInt count = propertyArray->Count();
    EUNIT_ASSERT( !count );

    CleanupStack::PopAndDestroy( propertyArray );
    }
    
void T_CSPSettings::T_CSPSettings_DeleteServicePropertiesLL(  )
    {
    RPropertyNameArray names;
    CleanupClosePushL( names );
    
    EUNIT_ASSERT( KErrNone == iCSPSettings->DeleteServicePropertiesL( 0, names ) );
    
    CleanupStack::PopAndDestroy( &names );
    }
    
void T_CSPSettings::T_CSPSettings_FindServiceIdsFromPropertiesLL(  )
    {
    RIdArray serviceIds;
    CleanupClosePushL( serviceIds );
    RPropertyArray properties;
    CleanupClosePushL( properties );
    
    EUNIT_ASSERT( KErrNone == iCSPSettings->FindServiceIdsFromPropertiesL( properties, serviceIds ) );
    
    CleanupStack::PopAndDestroy( &properties );
    CleanupStack::PopAndDestroy( &serviceIds );
    }
    
void T_CSPSettings::T_CSPSettings_GetSIPVoIPSWVersionL(  )
    {
    EUNIT_ASSERT( _L("3.1") == iCSPSettings->GetSIPVoIPSWVersion() );
    }
    
void T_CSPSettings::T_CSPSettings_IsFeatureSupportedL(  )
    {
    EUNIT_ASSERT( !iCSPSettings->IsFeatureSupported( ESupportInternetCallFeature ) );
    }

void T_CSPSettings::T_CSPSettings_CheckSupportedLsL(  )
    {    
    // Test CheckSupportedL( TServiceId aServiceId, const RPropertyNameArray& aNameArray )
    
    CSPProperty* property = CSPProperty::NewLC();
    iCSPSettings->FindPropertyL( 0, EPropertyVoIPSubServicePluginId, *property );
    iCSPSettings->FindPropertyL( 2, EPropertyVoIPSubServicePluginId, *property );
    CleanupStack::PopAndDestroy( property );

    
    // Test CheckSupportedL( TServiceId aServiceId )
    
    CSPEntry* entry = CSPEntry::NewLC();
    iCSPSettings->FindEntryL( 0, *entry );
    entry->SetServiceId( 2 );
    iCSPSettings->FindEntryL( 2, *entry );
    CleanupStack::PopAndDestroy( entry );
    
    
    // Test CheckSupportedL( TSPItemType aPropertyType )
    
    RPropertyArray* propertyArray = new (ELeave) RPropertyArray( 3 );
    CleanupStack::PushL( propertyArray );
    iCSPSettings->FindSubServicePropertiesL( 0, EItemTypeVMBXSubProperty, *propertyArray );

    
    // Test CheckSupportedL( const RPropertyArray& aPropertyArray )
    
    RIdArray serviceIds;
    CleanupClosePushL( serviceIds );
    iCSPSettings->FindServiceIdsFromPropertiesL( *propertyArray, serviceIds );

    CleanupStack::PopAndDestroy( &serviceIds );    
    CleanupStack::PopAndDestroy( propertyArray );
    }
    

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    T_CSPSettings,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddEntryL - test",
    "CSPSettings",
    "AddEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_AddEntryLL, Teardown)
    
EUNIT_TEST(
    "FindEntryL - test",
    "CSPSettings",
    "FindEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindEntryLL, Teardown)

EUNIT_TEST(
    "UpdateEntryL - test",
    "CSPSettings",
    "UpdateEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_UpdateEntryLL, Teardown)

EUNIT_TEST(
    "DeleteEntryL - test",
    "CSPSettings",
    "DeleteEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_DeleteEntryLL, Teardown)

EUNIT_TEST(
    "FindPropertyL - test",
    "CSPSettings",
    "FindPropertyL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindPropertyLL, Teardown)
/*
EUNIT_TEST(
    "AddOrUpdatePropertiesL - test",
    "CSPSettings",
    "AddOrUpdatePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_AddOrUpdatePropertiesLL, Teardown)
*/ 
EUNIT_TEST(
    "AddOrUpdatePropertyL - test",
    "CSPSettings",
    "AddOrUpdatePropertyL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_AddOrUpdatePropertyLL, Teardown)

EUNIT_TEST(
    "SettingsCountL - test",
    "CSPSettings",
    "SettingsCountL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_SettingsCountLL, Teardown)

EUNIT_TEST(
    "FindServiceIdsL - test",
    "CSPSettings",
    "FindServiceIdsL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindServiceIdsLL, Teardown)

EUNIT_TEST(
    "FindServiceNamesL - test",
    "CSPSettings",
    "FindServiceNamesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindServiceNamesLL, Teardown)

EUNIT_TEST(
    "FindSubServicePropertiesL - test",
    "CSPSettings",
    "FindSubServicePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindSubServicePropertiesLL, Teardown)

EUNIT_TEST(
    "DeleteServicePropertiesL - test",
    "CSPSettings",
    "DeleteServicePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_DeleteServicePropertiesLL, Teardown)
    
EUNIT_TEST(
    "FindServiceIdsFromPropertiesL - test",
    "CSPSettings",
    "FindServiceIdsFromPropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_FindServiceIdsFromPropertiesLL, Teardown)

EUNIT_TEST(
    "GetSIPVoIPSWVersion - test",
    "CSPSettings",
    "GetSIPVoIPSWVersion - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_GetSIPVoIPSWVersionL, Teardown)

EUNIT_TEST(
    "IsFeatureSupported - test",
    "CSPSettings",
    "IsFeatureSupported - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettings_IsFeatureSupportedL, Teardown)

EUNIT_TEST(
    "CheckSupportedLs - test",
    "CSPSettings",
    "CheckSupportedLs - test",
    "FUNCTIONALITY",
    Setup2L, T_CSPSettings_CheckSupportedLsL, Teardown)

EUNIT_END_TEST_TABLE
