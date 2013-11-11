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
* Description: Implementation of T_CSPSettingsEngine class.
*
*/

#include "T_CSPSettingsEngine.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <spentry.h>
#include <spproperty.h>
#include "spsettingsengine.h"

// - Construction -----------------------------------------------------------

T_CSPSettingsEngine* T_CSPSettingsEngine::NewL()
    {
    T_CSPSettingsEngine* self = T_CSPSettingsEngine::NewLC();
    CleanupStack::Pop();
    return self;
    }

T_CSPSettingsEngine* T_CSPSettingsEngine::NewLC()
    {
    T_CSPSettingsEngine* self = new( ELeave ) T_CSPSettingsEngine();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_CSPSettingsEngine::~T_CSPSettingsEngine()
    {
    }

T_CSPSettingsEngine::T_CSPSettingsEngine()
    {
    }

void T_CSPSettingsEngine::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void T_CSPSettingsEngine::SetupL(  )
    {
    iCSPSettingsEngine = CSPSettingsEngine::NewL();
    }
    

void T_CSPSettingsEngine::Teardown(  )
    {
    delete iCSPSettingsEngine;
    iCSPSettingsEngine = NULL;
    }

    
void T_CSPSettingsEngine::T_CSPSettingsEngine_BeginTransactionLCL(  )
    {
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->BeginTransactionLC( ) );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_CommitTransactionL(  )
    {
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->CommitTransaction() );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_RollbackTransactionL(  )
    {
    iCSPSettingsEngine->RollbackTransaction( );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_AddEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    TServiceId id = entry->GetServiceId();

    EUNIT_ASSERT_SPECIFIC_LEAVE( iCSPSettingsEngine->AddEntryL( *entry ), KErrArgument ) ;
    
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    iCSPSettingsEngine->AddEntryL( *entry );
    EUNIT_ASSERT( id != entry->GetServiceId() );

    CleanupStack::PopAndDestroy( entry );    
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    TInt id = entry->GetServiceId();
    
    EUNIT_ASSERT( KErrNotFound == iCSPSettingsEngine->FindEntryL( id, *entry ) );
    
    iCSPSettingsEngine->AddEntryL( *entry );
    id = entry->GetServiceId();
    
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->FindEntryL( id, *entry ) );

    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_UpdateEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    
    EUNIT_ASSERT_LEAVE( iCSPSettingsEngine->UpdateEntryL( *entry ) );
    
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    iCSPSettingsEngine->AddEntryL( *entry );
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->UpdateEntryL( *entry ) );
    
    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_DeleteEntryLL(  )
    {
    CSPEntry* entry = CSPEntry::NewLC();
    User::LeaveIfError( entry->SetServiceName( _L("Nimi") ) );
    TInt id = entry->GetServiceId();
    
    EUNIT_ASSERT( KErrNotFound == iCSPSettingsEngine->DeleteEntryL( id ) );
    
    iCSPSettingsEngine->AddEntryL( *entry );
    id = entry->GetServiceId();
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->DeleteEntryL( id ) );
    CleanupStack::PopAndDestroy( entry );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindPropertyLL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();
    property->SetName( EPropertyVoIPSubServicePluginId );
    TInt profId( 15 );
    User::LeaveIfError( property->SetValue( profId ));

    EUNIT_ASSERT( KErrNotFound == iCSPSettingsEngine->FindPropertyL(
        0, EPropertyVoIPSubServicePluginId, *property ) );
    
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->FindPropertyL(
        profId, EPropertyVoIPSubServicePluginId, *property ) );
    
    CleanupStack::PopAndDestroy( property );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_AddOrUpdatePropertiesLL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();
    property->SetName( EPropertyVoIPSubServicePluginId );
    TInt profId( 15 );
    User::LeaveIfError( property->SetValue( profId ));
    
    RPropertyArray propertyArray;
    propertyArray.Append( property );
    CleanupClosePushL( propertyArray );

    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->AddOrUpdatePropertiesL( profId, propertyArray ) );
    
    CleanupStack::PopAndDestroy( &propertyArray );
    CleanupStack::PopAndDestroy( property );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_SettingsCountLL(  )
    {
    EUNIT_ASSERT( !iCSPSettingsEngine->SettingsCountL() );
    
    // Note that stub file is set to increase the count for this
    // test when calling SettingsCountL method again
    EUNIT_ASSERT( !iCSPSettingsEngine->SettingsCountL() );
    EUNIT_ASSERT( iCSPSettingsEngine->SettingsCountL() );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindServiceIdsLL(  )
    {
    RIdArray serviceIds;
    CleanupClosePushL( serviceIds );
    
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->FindServiceIdsL( serviceIds ) );
    EUNIT_ASSERT( serviceIds.Count() );
    
    CleanupStack::PopAndDestroy( &serviceIds );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindServiceNamesLL(  )
    {
    RIdArray ids;
    CleanupClosePushL( ids );
    iCSPSettingsEngine->FindServiceIdsL( ids );
    
    CDesCArrayFlat* names = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( names );
    iCSPSettingsEngine->FindServiceNamesL( ids, *names );
    TInt count = names->MdcaCount();
    EUNIT_ASSERT( count );
    
    CleanupStack::PopAndDestroy( names );
    CleanupStack::PopAndDestroy( &ids );
    }

void T_CSPSettingsEngine::T_CSPSettingsEngine_PropertyNameArrayFromItemTypeLL(  )
    {
    RPropertyArray* propertyArray = new (ELeave) RPropertyArray( 3 );
    CleanupStack::PushL( propertyArray );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypeNotDefined,
        *propertyArray ), KErrArgument );
    iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypeVoIPSubProperty, *propertyArray );
    iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypePresenceSubProperty, *propertyArray );
    iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypeIMSubProperty, *propertyArray );
    iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypeVMBXSubProperty, *propertyArray );
    
    CleanupStack::PopAndDestroy( propertyArray );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindSubServicePropertiesLL(  )
    {
    RPropertyArray* propertyArray = new (ELeave) RPropertyArray( 3 );
    CleanupStack::PushL( propertyArray );
    
    iCSPSettingsEngine->FindSubServicePropertiesL( 0, EItemTypeVMBXSubProperty, *propertyArray );
    TInt count = propertyArray->Count();
    EUNIT_ASSERT( !count );

    CleanupStack::PopAndDestroy( propertyArray );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_DeleteServicePropertiesLL(  )
    {
    RPropertyNameArray names;
    CleanupClosePushL( names );
    
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->DeleteServicePropertiesL( 0, names ) );
    
    names.AppendL( EPropertyPCSPluginId );
    names.AppendL( ESubPropertyVoIPSettingsId );
    EUNIT_ASSERT( KErrNone == iCSPSettingsEngine->DeleteServicePropertiesL( 0, names ) );
    
    CleanupStack::PopAndDestroy( &names );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_FindServiceIdsFromPropertiesLL(  )
    {
    RIdArray serviceIds;
    CleanupClosePushL( serviceIds );
    RPropertyArray properties;
    CleanupClosePushL( properties );
    
    iCSPSettingsEngine->FindServiceIdsFromPropertiesL( properties, serviceIds );
    EUNIT_ASSERT( serviceIds.Count() );
    
    CleanupStack::PopAndDestroy( &properties );
    CleanupStack::PopAndDestroy( &serviceIds );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_IsFeatureSupportedLL(  )
    {
    EUNIT_ASSERT( !iCSPSettingsEngine->IsFeatureSupportedL( ESupportInternetCallFeature, EFalse ) );
    EUNIT_ASSERT_LEAVE( iCSPSettingsEngine->IsFeatureSupportedL( ESupportInternetCallFeature, ETrue ) );
    
    EUNIT_ASSERT( !iCSPSettingsEngine->IsFeatureSupportedL( ESupportCallOutFeature, EFalse ) );
    EUNIT_ASSERT_LEAVE( iCSPSettingsEngine->IsFeatureSupportedL( ESupportCallOutFeature, ETrue ) );
    
    EUNIT_ASSERT( !iCSPSettingsEngine->IsFeatureSupportedL( ESupportVoIPSSFeature, EFalse ) );
    EUNIT_ASSERT_LEAVE( iCSPSettingsEngine->IsFeatureSupportedL( ESupportVoIPSSFeature, ETrue ) );
    
    EUNIT_ASSERT( iCSPSettingsEngine->IsFeatureSupportedL( ESupportVoIPFeature, EFalse ) );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_UpdateSupportFeaturePSKeyLL(  )
    {
    EUNIT_ASSERT_LEAVE( iCSPSettingsEngine->UpdateSupportFeaturePSKeyL( ) );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_IsVoIPServiceLL(  )
    {
    EUNIT_ASSERT( !iCSPSettingsEngine->IsVoIPServiceL( 0 ) );
    EUNIT_ASSERT( iCSPSettingsEngine->IsVoIPServiceL( 2 ) );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_HasCallPropertyNameL(  )
    {
    RPropertyNameArray names;
    CleanupClosePushL( names );
    EUNIT_ASSERT( !iCSPSettingsEngine->HasCallPropertyName( names ) );
    
    names.AppendL( EPropertyUnknown );
    names.AppendL( EPropertyCTIPluginId );
    EUNIT_ASSERT( iCSPSettingsEngine->HasCallPropertyName( names ) );
    
    CleanupStack::PopAndDestroy( &names );
    }
    
void T_CSPSettingsEngine::T_CSPSettingsEngine_HasVoIPAndVmbxPropertyNameL(  )
    {
    RPropertyNameArray names;
    CleanupClosePushL( names );
    EUNIT_ASSERT( !iCSPSettingsEngine->HasVoIPAndVmbxPropertyName( names ) );
    
    names.AppendL( EPropertyUnknown );
    names.AppendL( EPropertyVoIPSubServicePluginId );
    EUNIT_ASSERT( iCSPSettingsEngine->HasVoIPAndVmbxPropertyName( names ) );
    
    CleanupStack::PopAndDestroy( &names );
    }

void T_CSPSettingsEngine::T_CSPSettingsEngine_NameArrayFromPropertyArrayLL(  )
    {
    RPropertyNameArray names;
    CleanupClosePushL( names );
    
    RPropertyArray properties;
    CleanupClosePushL( properties );
    
    iCSPSettingsEngine->NameArrayFromPropertyArrayL( names, properties );
    EUNIT_ASSERT( !names.Count() );
    
    CSPProperty* property = CSPProperty::NewLC();
    property->SetName( EPropertyVoIPSubServicePluginId );
    properties.AppendL( property );
    
    iCSPSettingsEngine->NameArrayFromPropertyArrayL( names, properties );
    EUNIT_ASSERT( names.Count() );
    
    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( &properties );
    CleanupStack::PopAndDestroy( &names );
    }


// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    T_CSPSettingsEngine,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "BeginTransactionLC - test1",
    "CSPSettingsEngine",
    "BeginTransactionLC - test1",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_BeginTransactionLCL, Teardown)

EUNIT_TEST(
    "CommitTransaction - test",
    "CSPSettingsEngine",
    "CommitTransaction - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_CommitTransactionL, Teardown)

EUNIT_TEST(
    "RollbackTransaction - test",
    "CSPSettingsEngine",
    "RollbackTransaction - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_RollbackTransactionL, Teardown)

EUNIT_TEST(
    "AddEntryL - test",
    "CSPSettingsEngine",
    "AddEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_AddEntryLL, Teardown)

EUNIT_TEST(
    "FindEntryL - test",
    "CSPSettingsEngine",
    "FindEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindEntryLL, Teardown)

EUNIT_TEST(
    "UpdateEntryL - test",
    "CSPSettingsEngine",
    "UpdateEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_UpdateEntryLL, Teardown)

EUNIT_TEST(
    "DeleteEntryL - test",
    "CSPSettingsEngine",
    "DeleteEntryL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_DeleteEntryLL, Teardown)

EUNIT_TEST(
    "FindPropertyL - test",
    "CSPSettingsEngine",
    "FindPropertyL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindPropertyLL, Teardown)

EUNIT_TEST(
    "AddOrUpdatePropertiesL - test",
    "CSPSettingsEngine",
    "AddOrUpdatePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_AddOrUpdatePropertiesLL, Teardown)

EUNIT_TEST(
    "SettingsCountL - test",
    "CSPSettingsEngine",
    "SettingsCountL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_SettingsCountLL, Teardown)

EUNIT_TEST(
    "FindServiceIdsL - test",
    "CSPSettingsEngine",
    "FindServiceIdsL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindServiceIdsLL, Teardown)

EUNIT_TEST(
    "FindServiceNamesL - test",
    "CSPSettingsEngine",
    "FindServiceNamesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindServiceNamesLL, Teardown)

EUNIT_TEST(
    "PropertyNameArrayFromItemTypeL - test",
    "CSPSettingsEngine",
    "PropertyNameArrayFromItemTypeL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_PropertyNameArrayFromItemTypeLL, Teardown)

EUNIT_TEST(
    "FindSubServicePropertiesL - test",
    "CSPSettingsEngine",
    "FindSubServicePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindSubServicePropertiesLL, Teardown)

EUNIT_TEST(
    "DeleteServicePropertiesL - test",
    "CSPSettingsEngine",
    "DeleteServicePropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_DeleteServicePropertiesLL, Teardown)

EUNIT_TEST(
    "FindServiceIdsFromPropertiesL - test",
    "CSPSettingsEngine",
    "FindServiceIdsFromPropertiesL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_FindServiceIdsFromPropertiesLL, Teardown)

EUNIT_TEST(
    "IsFeatureSupportedL - test",
    "CSPSettingsEngine",
    "IsFeatureSupportedL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_IsFeatureSupportedLL, Teardown)

EUNIT_TEST(
    "UpdateSupportFeaturePSKeyL - test",
    "CSPSettingsEngine",
    "UpdateSupportFeaturePSKeyL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_UpdateSupportFeaturePSKeyLL, Teardown)

EUNIT_TEST(
    "IsVoIPServiceL - test",
    "CSPSettingsEngine",
    "IsVoIPServiceL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_IsVoIPServiceLL, Teardown)

EUNIT_TEST(
    "HasCallPropertyName - test",
    "CSPSettingsEngine",
    "HasCallPropertyName - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_HasCallPropertyNameL, Teardown)

EUNIT_TEST(
    "HasVoIPAndVmbxPropertyName - test",
    "CSPSettingsEngine",
    "HasVoIPAndVmbxPropertyName - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_HasVoIPAndVmbxPropertyNameL, Teardown)

EUNIT_TEST(
    "NameArrayFromPropertyArrayL - test",
    "CSPSettingsEngine",
    "NameArrayFromPropertyArrayL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsEngine_NameArrayFromPropertyArrayLL, Teardown)

EUNIT_END_TEST_TABLE
