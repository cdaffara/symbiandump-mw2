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
* Description:  test source
*
*/


//  CLASS HEADER
#include "T_xSPSettingsAPIModuleTests.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <FeatMgr.h>
#include <centralrepository.h>


// API headers under test
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <spsettingsvoiputils.h>

#include "StubObserver.h"

// Test definitions

_LIT( KEntryName, "ServiceProvider" );
_LIT( KEntryName2, "ServiceProvider_2" );
_LIT( KBackupName1, "Backup1" );
_LIT( KBackupName2, "Backup2" );
_LIT( KAddDeletePropertyTestName, "AddDeletePropertyTest1" );

const TUid KCRUidSPSettings = { 0x10282E7F };


TInt const KTestEntryPropertyCount = 5;


// CONSTRUCTION
T_xSPSettingsAPIModuleTests* T_xSPSettingsAPIModuleTests::NewL()
    {
    T_xSPSettingsAPIModuleTests* self = T_xSPSettingsAPIModuleTests::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_xSPSettingsAPIModuleTests* T_xSPSettingsAPIModuleTests::NewLC()
    {
    T_xSPSettingsAPIModuleTests* self = new( ELeave ) T_xSPSettingsAPIModuleTests();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_xSPSettingsAPIModuleTests::~T_xSPSettingsAPIModuleTests()
    {
    }

// Default constructor
T_xSPSettingsAPIModuleTests::T_xSPSettingsAPIModuleTests()
    {
    }

// Second phase construct
void T_xSPSettingsAPIModuleTests::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


// ---------------------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::SetupL(  )
    {
    // Reset all settings from previous test runs
    CRepository* repository = CRepository::NewL( KCRUidSPSettings );
    repository->Reset();
    delete repository;
    repository = NULL;

    // Create API instance
    iSettings = CSPSettings::NewL();
    iWait = new (ELeave) CActiveSchedulerWait;
    
    iStubObserver = CStubObserver::NewL( iWait );
    
    iPeriodic1 = CPeriodic::NewL( CActive::EPriorityStandard );
    iPeriodic2 = CPeriodic::NewL( CActive::EPriorityStandard );
    }

// ---------------------------------------------------------------------------
// SETUP2
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::Setup2L(  )
    {
    // Create API instance
    iSettings = CSPSettings::NewL();
    iVoIPUtils = CSPSettingsVoIPUtils::NewL();
    }

// ---------------------------------------------------------------------------
// SETUP3
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::Setup3L(  )
    {
    // Reset all settings from previous test runs
    CRepository* repository = CRepository::NewL( KCRUidSPSettings );
    repository->Reset();
    delete repository;
    repository = NULL;
    
    // Create API instance
    iSettings = CSPSettings::NewL();
    iVoIPUtils = CSPSettingsVoIPUtils::NewL();
    }


// ---------------------------------------------------------------------------
// TEARDOWN
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::Teardown(  )
    {
    delete iSettings;
    iSettings = NULL;
    
    delete iStubObserver;
    iStubObserver = NULL;
    
    delete iWait;

    if ( iPeriodic1 )
        {
        iPeriodic1->Cancel();
        }
    delete iPeriodic1;

    if ( iPeriodic2 )
        {
        iPeriodic2->Cancel();
        }
    delete iPeriodic2;
    
    // Reset all settings from previous test runs
    CRepository* repository = NULL;
    repository = CRepository::NewL( KCRUidSPSettings );
    repository->Reset();
    delete repository;
    repository = NULL;
    }

// ---------------------------------------------------------------------------
// TEARDOWN2
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::Teardown2(  )
    {
    delete iSettings;
    iSettings = NULL;
    
    delete iVoIPUtils;
    iVoIPUtils = NULL;
    }

// ---------------------------------------------------------------------------
// TEARDOWN3
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::Teardown3(  )
    {
    delete iSettings;
    iSettings = NULL;
    
    delete iVoIPUtils;
    iVoIPUtils = NULL;
    
    // Reset all settings from previous test runs
    CRepository* repository = NULL;
    repository = CRepository::NewL( KCRUidSPSettings );
    repository->Reset();
    delete repository;
    repository = NULL;
    }


// ---------------------------------------------------------------------------
// TEST METHODS
// ---------------------------------------------------------------------------
//
//
// ---------------------------------------------------------------------------
// EntryTestL: Test setting and getting of entry data
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::EntryTestL(  )
    {
    CSPEntry* testEntry = NULL;

    testEntry = CSPEntry::NewL();
    EUNIT_ASSERT( NULL != testEntry );
    CleanupStack::PushL( testEntry );
    
   	const TDesC& name = testEntry->GetServiceName();
   	EUNIT_ASSERT( name == KNullDesC );
   	User::LeaveIfError( testEntry->SetServiceName( _L( "TestServiceEntry" ) ) );

   	CleanupStack::PopAndDestroy( testEntry );
    }



// ---------------------------------------------------------------------------
// EntryMisuseTestL: Test setting and getting of entry data
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::EntryMisuseTestL(  )
    {
    CSPEntry* testEntry = NULL;
    
    testEntry = CSPEntry::NewL();
    EUNIT_ASSERT( NULL != testEntry );
    CleanupStack::PushL(testEntry);
    
   	TInt err = testEntry->SetServiceName( _L( "" ) );
   	EUNIT_ASSERT( err == KErrArgument );
   	CleanupStack::PopAndDestroy(testEntry);
    }


// ---------------------------------------------------------------------------
// PropertyTestL: Test setting and getting of property data
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::PropertyTestL(  )
    {
    CSPProperty* property = NULL;
    property = CSPProperty::NewL();
    EUNIT_ASSERT( NULL != property );
    delete property;
    
    // Create property of TInt type
    CSPProperty* propertyInt = CSPProperty::NewLC();
    EUNIT_ASSERT( NULL != propertyInt );
    propertyInt->SetName( EPropertyVoIPSubServicePluginId );
    TInt profIdSet( 15 );
    User::LeaveIfError( propertyInt->SetValue( profIdSet ));
    EUNIT_ASSERT( EDataTypeInt == propertyInt->GetDataType() );
    TInt profIdGet( 0 );
    TInt error = propertyInt->GetValue( profIdGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( profIdSet == profIdGet );
    CleanupStack::PopAndDestroy( propertyInt );

    // Create property of TDesC type
    CSPProperty* propertyDes = CSPProperty::NewLC();
    EUNIT_ASSERT( NULL != propertyDes );
    propertyDes->SetName( EPropertyBrandId );
    RBuf brandIdSet;
    brandIdSet.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandIdSet);
    User::LeaveIfError( propertyDes->SetValue( brandIdSet ) );
    EUNIT_ASSERT( propertyDes->GetDataType() == EDataTypeDes );
    
    RBuf brandIdGet;
    brandIdGet.CreateL( KSPMaxDesLength );
    CleanupClosePushL( brandIdGet);
    error = propertyDes->GetValue( brandIdGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( brandIdSet.Compare( brandIdGet ) == 0 );

    CleanupStack::PopAndDestroy( &brandIdGet);
    CleanupStack::PopAndDestroy( &brandIdSet);
    
    CleanupStack::PopAndDestroy( propertyDes );
    }


// ---------------------------------------------------------------------------
// PropertyMisuseTestL: 
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::PropertyMisuseTestL(  )
    {
    RBuf brandIdSet;
    brandIdSet.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandIdSet);
    RBuf brandIdGet;
    brandIdGet.CreateL( KSPMaxDesLength );
    CleanupClosePushL( brandIdGet);
    TInt profIdSet( 15 );

    // Create property of TInt type
    CSPProperty* propertyInt = CSPProperty::NewLC();
    propertyInt->SetName( EPropertyVoIPSubServicePluginId );
    EUNIT_ASSERT( propertyInt->SetValue( brandIdSet ) == KErrArgument );

    // Create property of TDesC type
    CSPProperty* propertyDes = CSPProperty::NewLC();
    propertyDes->SetName( EPropertyBrandId );
    EUNIT_ASSERT( propertyDes->SetValue( profIdSet ) == KErrArgument );

    CleanupStack::PopAndDestroy( propertyDes );
    CleanupStack::PopAndDestroy( propertyInt );
    CleanupStack::PopAndDestroy( &brandIdGet);
    CleanupStack::PopAndDestroy( &brandIdSet);
    }


// ---------------------------------------------------------------------------
// CreateAPIL
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::NewAPITestL(  )
    {
    CSPSettings* api = NULL;
    api = CSPSettings::NewL();
    EUNIT_ASSERT( NULL != api );
    delete api;
    
    api = CSPSettings::NewLC();
    EUNIT_ASSERT( NULL != api );
    CleanupStack::PopAndDestroy( api );
    }


// ---------------------------------------------------------------------------
// AddEntryTestL: Test adding of new settings entry
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::AddEntryTestL(  )
    {
    // Create and add entry
    CreateAndAddTestEntryL( _L( "TestServiceEntry" ) );
    
    // Create and add 2nd entry
    CreateAndAddTestEntryL( _L( "TestServiceEntry2" ) );
    }


// ---------------------------------------------------------------------------
// FindEntryByIdTestL: Test finding of settings entry by service ID
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::FindEntryByIdTestL(  )
    {
    const TInt addCount( 5 );
    TBuf<25> name;
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    TInt id( 0 );

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        ids.Append( id );
        }

    // Find created entries
    for ( TInt j( addCount-1 ); 0 <= j; j-- )
        {
        CSPEntry* entry = CSPEntry::NewLC();
        TInt err = iSettings->FindEntryL( ids[j], *entry );
        EUNIT_ASSERT( KErrNone == err );

        // There were KTestEntryPropertyCount properties in each entry
        EUNIT_ASSERT_EQUALS( entry->PropertyCount(), KTestEntryPropertyCount );
        CleanupStack::PopAndDestroy( entry );
        }

    CleanupStack::PopAndDestroy( &ids );
    }
    
void T_xSPSettingsAPIModuleTests::FindEntryByIdTest2L()
	{
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( 1, *entry );
    EUNIT_ASSERT( KErrNone == err );
    CleanupStack::PopAndDestroy( entry );
    EUNIT_ASSERT( KErrNone == err );
	}

// ---------------------------------------------------------------------------
// UpdateEntryTestL: Test updating of settings entry
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::UpdateEntryTestL(  )
    {
    // Create entry
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* updateEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *updateEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT( NULL != updateEntry );
    
    // Define new values
    TInt newProfileID( 99 );
    TOnOff newOnoff( EOff );
    RBuf newBrandId;
    newBrandId.CreateL( _L( "NewBrandId" ) );
    CleanupClosePushL( newBrandId );
    
    // Set new values for properties
	const CSPProperty* propProfileId = NULL;
	EUNIT_ASSERT_EQUALS( KErrNone, updateEntry->GetProperty( propProfileId, EPropertyVoIPSubServicePluginId ) );
	EUNIT_ASSERT( NULL != propProfileId );
	User::LeaveIfError( updateEntry->UpdateProperty( propProfileId->GetName(), newProfileID ) );
    
    const CSPProperty* vmbxConn = NULL;
    EUNIT_ASSERT_EQUALS( KErrNone, updateEntry->GetProperty( vmbxConn, ESubPropertyVMBXListenRegister ) );
    EUNIT_ASSERT( NULL != vmbxConn );
    User::LeaveIfError( updateEntry->UpdateProperty( vmbxConn->GetName(), newOnoff ) );
       
    const CSPProperty* propBrandId = NULL;
    EUNIT_ASSERT_EQUALS( KErrNone, updateEntry->GetProperty( propBrandId, EPropertyBrandId ));
    EUNIT_ASSERT( NULL != propBrandId );
    User::LeaveIfError( updateEntry->UpdateProperty( propBrandId->GetName(), newBrandId ) );

    CleanupStack::PopAndDestroy( &newBrandId );

    // Update entry to settings storage
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );

    // Find entry and check that content has updated
    CSPEntry* checkEntry = CSPEntry::NewLC();
    err = iSettings->FindEntryL( id, *checkEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );

    TInt checkCount = checkEntry->PropertyCount();
    TInt updateCount = updateEntry->PropertyCount();

    EUNIT_ASSERT_EQUALS( updateCount, checkCount );

    for( TInt i( 0 ); i < checkCount; i++ )
        {
        const CSPProperty* updatedProperty = NULL;
        User::LeaveIfError( updateEntry->GetProperty( updatedProperty, i ));

        const CSPProperty* checkProperty = NULL;
        User::LeaveIfError( checkEntry->GetProperty( checkProperty, i ));

        EUNIT_ASSERT_EQUALS( updatedProperty->GetPropertyType(), checkProperty->GetPropertyType() ); 
        EUNIT_ASSERT_EQUALS( updatedProperty->GetName(), checkProperty->GetName() );
        EUNIT_ASSERT_EQUALS( updatedProperty->GetDataType(), checkProperty->GetDataType() );
        
        RBuf temp1, temp2;
        temp1.CreateL( KSPMaxDesLength );
        CleanupClosePushL( temp1 );
        
        temp2.CreateL( KSPMaxDesLength );
        CleanupClosePushL( temp2 );
        
        User::LeaveIfError( updatedProperty->GetValue( temp1 ) );
        User::LeaveIfError( checkProperty->GetValue( temp2 ) );
        EUNIT_ASSERT_EQUALS( temp1, temp2 );
        
        CleanupStack::PopAndDestroy( &temp2 );
        CleanupStack::PopAndDestroy( &temp1 );
        }

    CleanupStack::PopAndDestroy( checkEntry );
    CleanupStack::PopAndDestroy( updateEntry );
    }
// ---------------------------------------------------------------------------
// ResetTestL: Test Reset of settings entry
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::ResetTestL()
    {
    // Create entry
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* resetEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *resetEntry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != resetEntry );
    resetEntry->Reset();
    
    EUNIT_ASSERT( 0 == resetEntry->PropertyCount());
    
    CleanupStack::PopAndDestroy( resetEntry );
    }


// ---------------------------------------------------------------------------
// DeleteEntryTestL: Test deletion of settings entry
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::DeleteEntryTestL(  )
    {
    const TInt addCount( 2 );
    TBuf<25> name;
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    
    TInt initCount = iSettings->SettingsCountL();
    
    TInt id( 0 );
    TInt err( 0 );

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        User::LeaveIfError( ids.Append( id ) );
        }

    // Delete every other entry
    for ( TInt j( 0 ); j < ids.Count(); j++ )
        {
        err = iSettings->DeleteEntryL( ids[j] );
        EUNIT_ASSERT( KErrNone == err );
        }

    // Try to find deleted entries
    for ( TInt k( 0 ); k < ids.Count(); k++ )
        {
        CSPEntry* deletedEntry = CSPEntry::NewLC();
        EUNIT_ASSERT_EQUALS( KErrNotFound, iSettings->FindEntryL( ids[k], *deletedEntry ) );
        CleanupStack::PopAndDestroy( deletedEntry );
        }

    // Create some new entries
    TInt newCount( addCount + initCount );
    RArray<TInt> newIds;
    CleanupClosePushL( newIds );

    for ( TInt n( addCount ); n < newCount; n++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( n );
        id = CreateAndAddTestEntryL( name );
        User::LeaveIfError( newIds.Append( id ) );
        }

    // Find new entries
    for ( TInt m( 0 ); m < newIds.Count(); m++ )
        {
        CSPEntry* newEntry = CSPEntry::NewLC();
        err = iSettings->FindEntryL( newIds[m], *newEntry );
        EUNIT_ASSERT( KErrNone == err );
        CleanupStack::PopAndDestroy( newEntry );
        }

    CleanupStack::PopAndDestroy( &newIds );
    CleanupStack::PopAndDestroy( &ids );
    }

// ---------------------------------------------------------------------------
// FindPropertyByIdTestL: Test finding of property by service ID
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::FindPropertyByIdTestL(  )
    {
    const TInt addCount( 5 );
    TBuf<25> name;
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    TInt id( 0 );
    TInt err( 0 );

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        ids.Append( id );
        }

    // Find one property from each created entry
    for ( TInt j( addCount-1 ); 0 <= j; j-- )
        {
        CSPProperty* property = CSPProperty::NewLC();
        err = iSettings->FindPropertyL( ids[j], EPropertyBrandId, *property );
        EUNIT_ASSERT( KErrNone == err );
        CleanupStack::PopAndDestroy( property );
        }
        
    // find CSVoice attribute mask
    CSPProperty* voice = CSPProperty::NewLC();
    err = iSettings->FindPropertyL( 1, EPropertyServiceAttributeMask, *voice );
    EUNIT_ASSERT( KErrNone == err );
    CleanupStack::PopAndDestroy( voice );
    
    CSPProperty* voip = CSPProperty::NewLC();
    err = iSettings->FindPropertyL( 2, EPropertyServiceAttributeMask, *voip );
    EUNIT_ASSERT( KErrNone == err );
    TInt value;
    voip->GetValue( value );
    EUNIT_ASSERT_EQUALS( value, 48 );
    EUNIT_ASSERT_EQUALS( value & EIsVisibleInCallMenu, 0);
    _LIT( KText, "Correct" );
    TBuf<10> buf;
    buf.Copy( KText );
    EUNIT_PRINT( buf );
    CleanupStack::PopAndDestroy( voip );

    CleanupStack::PopAndDestroy( &ids );
    }

// ---------------------------------------------------------------------------
// GetAllPropertiesTestL: get all properties
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::GetAllPropertiesTestL()
    {
    // Create entry
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* testEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *testEntry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != testEntry );
    RPropertyArray array  = testEntry->GetAllProperties();
    
    EUNIT_ASSERT( 0 != array.Count());
    
    CleanupStack::PopAndDestroy( testEntry );
    }

// ---------------------------------------------------------------------------
// UpdatePropertyTestL: Test updating of settings property
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::AddOrUpdatePropertiesTestL(  )
    {
    const TInt addCount( 5 );
    TBuf<25> name;
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    
    TInt id( 0 );
    TInt err( 0 );

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        ids.Append( id );
        }

    // New values for properties
    TInt newPlugId( 500 );
    RBuf newBrandId;
    newBrandId.CreateL( _L( "SomeNewBrandId" ) );
    CleanupClosePushL( newBrandId );

	RPropertyArray propertyArray;
	CleanupClosePushL( propertyArray );
	
    // Find all properties from each created entry and update them
    for ( TInt j( addCount-1 ); 0 <= j; j-- )
        {
        CSPProperty* voipPlugId = CSPProperty::NewLC();
        err = iSettings->FindPropertyL( ids[j], EPropertyVoIPSubServicePluginId, *voipPlugId );
        EUNIT_ASSERT( KErrNone == err );
        User::LeaveIfError( voipPlugId->SetValue( newPlugId ) );
        User::LeaveIfError( propertyArray.Append( voipPlugId ) );
        err = iSettings->AddOrUpdatePropertiesL( ids[j], propertyArray );
        EUNIT_ASSERT( KErrNone == err );
        CleanupStack::Pop( voipPlugId );
        propertyArray.ResetAndDestroy();

        CSPProperty* brandId = CSPProperty::NewLC();
        err = iSettings->FindPropertyL( ids[j], EPropertyBrandId, *brandId );
        EUNIT_ASSERT( KErrNone == err );
        User::LeaveIfError( brandId->SetValue( newBrandId ) );
        User::LeaveIfError( propertyArray.Append( brandId ) );
        err = iSettings->AddOrUpdatePropertiesL( ids[j], propertyArray);
        EUNIT_ASSERT( KErrNone == err );
        CleanupStack::Pop( brandId );
        propertyArray.ResetAndDestroy();
        }
        
    CleanupStack::PopAndDestroy( &propertyArray );

    // Find all properties again and check that values are up to date
    for ( TInt j( addCount-1 ); 0 <= j; j-- )
        {
        CSPProperty* voipPlugId = CSPProperty::NewLC();
        err = iSettings->FindPropertyL( ids[j], EPropertyVoIPSubServicePluginId, *voipPlugId );
        EUNIT_ASSERT( KErrNone == err );
        TInt checkPlugId( 0 );
        voipPlugId->GetValue( checkPlugId );
        EUNIT_ASSERT( newPlugId == checkPlugId );
        CleanupStack::PopAndDestroy( voipPlugId );

        CSPProperty* brandId = CSPProperty::NewLC();
        err = iSettings->FindPropertyL( ids[j], EPropertyBrandId, *brandId );
        EUNIT_ASSERT( KErrNone == err );
        TBuf<100> checkBrandId;
        brandId->GetValue( checkBrandId );
        EUNIT_ASSERT( 0 == newBrandId.Compare( checkBrandId ) );
        CleanupStack::PopAndDestroy( brandId );
        }

    CleanupStack::PopAndDestroy( &newBrandId );
    CleanupStack::PopAndDestroy( &ids );


    }

// ---------------------------------------------------------------------------
// DeletePropertyBynNameTestL: Test Deleting single properties
// ---------------------------------------------------------------------------
// 
void T_xSPSettingsAPIModuleTests::DeletePropertyByNameTestL()
    {
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry4" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *entry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != entry );
    
    RBuf brandId;
    brandId.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandId );
    
    CSPProperty* desProp = CreateTestPropertyL( EPropertyContactStoreId, brandId );
    CleanupStack::PushL( desProp );
    User::LeaveIfError( entry->AddPropertyL( *desProp ) );
    CleanupStack::PopAndDestroy( desProp);
    CleanupStack::PopAndDestroy( &brandId );
    
    const CSPProperty* propListenAddr = NULL;
    err =  entry->GetProperty( propListenAddr, EPropertyContactStoreId );
    EUNIT_ASSERT( KErrNone == err );
    
    err = entry->DeleteProperty(EPropertyContactStoreId);
    EUNIT_ASSERT( KErrNone == err );
    
    err =  entry->GetProperty( propListenAddr, EPropertyContactStoreId );
    EUNIT_ASSERT( KErrNotFound == err );
    
     CleanupStack::PopAndDestroy(entry);
    }
// ---------------------------------------------------------------------------
// SettingsCountTestL: Test of requesting settings count
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::SettingsCountTestL(  )
    {
    const TInt addCount( 5 );
    TBuf<25> name;
    
    TInt initCount = iSettings->SettingsCountL();

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        CreateAndAddTestEntryL( name );
        }

    TInt getCount = iSettings->SettingsCountL();
    EUNIT_ASSERT( addCount + initCount == getCount );
    }


// ---------------------------------------------------------------------------
// FindServiceIdsTestL: Test finding of service IDs
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::FindServiceIdsTestL(  )
    {
    const TInt addCount( 2 );
    TBuf<25> name;
    RArray<TInt> ids;
    CleanupClosePushL( ids );
    TInt id( 0 );
    
    TInt initCount = iSettings->SettingsCountL();

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        User::LeaveIfError( ids.Append( id ) );
        }

    // Fetch service IDs
    RArray<TServiceId> ids2;
    CleanupClosePushL( ids2 );
    TInt err = iSettings->FindServiceIdsL( ids2 );

    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( addCount + initCount == ids2.Count() );

    for ( TInt j( 0 ); j < addCount; j++ )
        {
        EUNIT_ASSERT( ids[j] == ids2[j + initCount] );
        }

    CleanupStack::PopAndDestroy( &ids2 );
    CleanupStack::PopAndDestroy( &ids );
    }

// ---------------------------------------------------------------------------
// FindServiceNamesTestL: Test finding of service names
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::FindServiceNamesTestL(  )
    {
    const TInt addCount( 5 );
    TBuf<25> name;
    RArray<TServiceId> ids;
    CleanupClosePushL( ids );
    TInt id( 0 );

    // Create some entries
    for ( TInt i( 0 ); i < addCount; i++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( i );
        id = CreateAndAddTestEntryL( name );
        ids.Append( id );
        }

    // Append some non-existent service IDs
    ids.Append( 657 );
    ids.Append( 967 );
    ids.Append( 1234 );

    // Find service names
    CDesCArrayFlat* names = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( names );
    TInt err = iSettings->FindServiceNamesL( ids, *names );
    EUNIT_ASSERT( KErrNone == err );

    // Check fetched data
    TInt count = names->MdcaCount();
    EUNIT_ASSERT( addCount == count );       // All requested names found
    EUNIT_ASSERT( addCount == ids.Count() ); // Non-existent IDs removed

    for ( TInt j( 0 ); j < count; j++ )
        {
        name.Copy( KEntryName );
        name.AppendNum( j );

        TPtrC getName = names->MdcaPoint(j);
        TInt compare = getName.Compare( name ); 
        EUNIT_ASSERT( 0 == compare );   // Stored and fetched name matches
        }

    CleanupStack::PopAndDestroy( names );
    CleanupStack::PopAndDestroy( &ids );
    }


// ---------------------------------------------------------------------------
// FindSubServicePropertiesTestL: Test finding of properties from subservice table
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::FindSubServicePropertiesTestL(  )
    {
    // Create entry
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry3" ) ) );
    
    // Fetch entry from storage and modify
    CSPEntry* updateEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *updateEntry );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( NULL != updateEntry );

    // Define new values
    RBuf listenAddr;
    listenAddr.CreateL( _L( "xx@nokia.com" ) );
    CleanupClosePushL( listenAddr );
    
    RBuf wiAddr;
    wiAddr.CreateL( _L( "yy@nokia.com" ) );
    CleanupClosePushL( wiAddr );
    
    // Set new values for properties
    const CSPProperty* propListenAddr = NULL;
    User::LeaveIfError( updateEntry->GetProperty( propListenAddr, ESubPropertyVMBXListenAddress ));
    EUNIT_ASSERT( NULL != propListenAddr );
    User::LeaveIfError( updateEntry->UpdateProperty( propListenAddr->GetName(), listenAddr ) );
    
    const CSPProperty* propWiAddr = NULL;
    User::LeaveIfError( updateEntry->GetProperty( propWiAddr, ESubPropertyVMBXMWIAddress ));
    EUNIT_ASSERT( NULL != propWiAddr );
    User::LeaveIfError( updateEntry->UpdateProperty( propWiAddr->GetName(), wiAddr ) );

    // Update entry to settings storage
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT( err == KErrNone );

	RPropertyArray* propertyArray = new (ELeave) RPropertyArray(3);
	TCleanupItem cleanup( T_xSPSettingsAPIModuleTests::CleanupPointerArray, propertyArray );
	CleanupStack::PushL( cleanup );
	iSettings->FindSubServicePropertiesL( id, EItemTypeVMBXSubProperty, *propertyArray );
	
	EUNIT_ASSERT( propertyArray->Count() == 3 );
	TBuf<30> buf;
	
	for( TInt i = 0; i < 3; i++ )
		{
		CSPProperty* property = (*propertyArray)[i];
		if( property->GetName() == ESubPropertyVMBXListenAddress )
			{
			EUNIT_ASSERT( property->GetValue( buf ) == KErrNone );
			EUNIT_ASSERT( buf == _L( "xx@nokia.com" ) );
			}
		else if( property->GetName() == ESubPropertyVMBXMWIAddress )
			{
			EUNIT_ASSERT( property->GetValue( buf ) == KErrNone );
			EUNIT_ASSERT( buf == _L( "yy@nokia.com" ) );
			}
		else if( property->GetName() == ESubPropertyVMBXListenRegister )
			{
			TOnOff onoff;
			EUNIT_ASSERT( property->GetValue( onoff ) == KErrNone );
			EUNIT_ASSERT( onoff == EOn );
			}
		else
			{
			EUNIT_FAIL_TEST( "test case failed" );	
			}
		}

	CleanupStack::PopAndDestroy( propertyArray );
	CleanupStack::PopAndDestroy( &wiAddr );
	CleanupStack::PopAndDestroy( &listenAddr );

    CleanupStack::PopAndDestroy( updateEntry );
    }
    
void T_xSPSettingsAPIModuleTests::CleanupPointerArray(  TAny* aPointer )
	{
	RPropertyArray* array = static_cast<RPropertyArray*>( aPointer );
	array->ResetAndDestroy();
	delete array;
	}

// ---------------------------------------------------------------------------
// DeleteServicePropertiesTestL: Test finding of properties from subservice table
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::DeleteServicePropertiesTestL(  )
    {
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry4" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *entry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != entry );

    const CSPProperty* propListenAddr = NULL;
    EUNIT_ASSERT( entry->GetProperty( propListenAddr, ESubPropertyVMBXListenAddress ) == KErrNone );
    
    const CSPProperty* propWiAddr = NULL;
    EUNIT_ASSERT( entry->GetProperty( propWiAddr, ESubPropertyVMBXMWIAddress ) == KErrNone );
    
    RPropertyNameArray array;
    CleanupClosePushL( array );
    User::LeaveIfError( array.Append( ESubPropertyVMBXListenAddress ) );
    User::LeaveIfError( array.Append( ESubPropertyVMBXMWIAddress ) );
    
    EUNIT_ASSERT( iSettings->DeleteServicePropertiesL( id, array ) == KErrNone );

    CSPEntry* entry1 = CSPEntry::NewLC();
    err = iSettings->FindEntryL( id, *entry1 );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != entry1 );

    //EUNIT_ASSERT( entry1->GetProperty( propListenAddr, ESubPropertyVMBXListenAddress ) == KErrNotFound );
    EUNIT_ASSERT_EQUALS( KErrNotFound, entry1->GetProperty( propListenAddr, ESubPropertyVMBXListenAddress ));    
    EUNIT_ASSERT( entry1->GetProperty( propWiAddr, ESubPropertyVMBXMWIAddress ) == KErrNotFound );
    
    CleanupStack::PopAndDestroy( entry1 );
	CleanupStack::PopAndDestroy( &array );
	CleanupStack::PopAndDestroy( entry );
    }
    
// ---------------------------------------------------------------------------
// GetSIPVoIPSWVersionTestL: Test get SIP VoIP software version
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::GetSIPVoIPSWVersionTestL()
	{
	const TDesC& version = CSPSettings::GetSIPVoIPSWVersion();
	EUNIT_ASSERT( version == _L("0.1"));
	}

// ---------------------------------------------------------------------------
// GetSIPVoIPSWVersionTestL: Test get SIP VoIP software version
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::NotifyChangeTestL()
	{
    // Create and add entry
    TInt id = CreateAndAddTestEntryL( _L( "TestServiceEntry" ) );
    
    RIdArray idArray;
    CleanupClosePushL( idArray );
    idArray.Append( id );
    iStubObserver->NotifyChangeL( idArray );

    CSPEntry* updateEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *updateEntry );
    EUNIT_ASSERT( KErrNone == err );
    
    // update entry
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT( KErrNone == err );
    iWait->Start();
    
    EUNIT_ASSERT( iStubObserver->GetServiceId() == id );
    
	// call again, update same service twice.
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT( err == KErrNone );
    iWait->Start();
    
    EUNIT_ASSERT( iStubObserver->GetServiceId() == id );
    EUNIT_ASSERT( iStubObserver->GetCalledTimes() == 2 );
    
    // cause Leave
    iStubObserver->LeaveWhenNotified(ETrue);
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_PRINT(_L("Wait->start"));
    iWait->Start();
    iStubObserver->LeaveWhenNotified(EFalse);
    
	RPropertyArray propertyArray;
	CleanupClosePushL( propertyArray );
	
    CSPProperty* voipPlugId = CSPProperty::NewLC();
    err = iSettings->FindPropertyL( id, EPropertyVoIPSubServicePluginId, *voipPlugId );
    EUNIT_ASSERT( KErrNone == err );
    propertyArray.Append( voipPlugId );
    err = iSettings->AddOrUpdatePropertiesL( id, propertyArray );
    EUNIT_ASSERT( KErrNone == err );
    CleanupStack::Pop( voipPlugId );
    propertyArray.ResetAndDestroy();
    CleanupStack::PopAndDestroy( &propertyArray );
    iWait->Start();
    
    EUNIT_ASSERT( iStubObserver->GetServiceId() == id );
    EUNIT_ASSERT_EQUALS( iStubObserver->GetCalledTimes(), 3 );
    
    // delete properties
    RPropertyNameArray nameArray;
    CleanupClosePushL( nameArray );
    nameArray.Append( ESubPropertyVMBXListenAddress );

    err = iSettings->DeleteServicePropertiesL( id, nameArray );
    EUNIT_ASSERT( KErrNone == err );
    CleanupStack::PopAndDestroy( &nameArray );

    iWait->Start();
    EUNIT_ASSERT( iStubObserver->GetServiceId() == id );
    EUNIT_ASSERT_EQUALS( iStubObserver->GetCalledTimes(), 4 );
    

    CleanupStack::PopAndDestroy( updateEntry );
    CleanupStack::PopAndDestroy( &idArray );
	}

// ---------------------------------------------------------------------------
// FindServiceIdsFromPropertiesTestL: 
// ---------------------------------------------------------------------------
//	
void T_xSPSettingsAPIModuleTests::FindServiceIdsFromPropertiesTestL()
	{
	RIdArray idArray;
	CleanupClosePushL( idArray );
    TInt id1 = CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) );
    
    TInt id2 = CreateAndAddTestEntryL( _L( "TestServiceEntry2" ) );
	
	RPropertyArray* propertyArray = new (ELeave) RPropertyArray(3);
	TCleanupItem cleanup( T_xSPSettingsAPIModuleTests::CleanupPointerArray, propertyArray );
	CleanupStack::PushL( cleanup );
	
    CSPProperty* voipPlugId = CSPProperty::NewLC();
    TInt err = iSettings->FindPropertyL( id1, EPropertyVoIPSubServicePluginId, *voipPlugId );
    EUNIT_ASSERT( KErrNone == err );
    User::LeaveIfError( propertyArray->Append( voipPlugId ) );
	CleanupStack::Pop( voipPlugId );
	
    
    CSPProperty* brandId = CSPProperty::NewLC();
    err = iSettings->FindPropertyL( id2, EPropertyBrandId, *brandId );
    EUNIT_ASSERT( KErrNone == err );
    User::LeaveIfError( propertyArray->Append( brandId ) );
	CleanupStack::Pop( brandId );

    err = iSettings->FindServiceIdsFromPropertiesL( *propertyArray, idArray );
    EUNIT_ASSERT( err == KErrNone );
    EUNIT_ASSERT( idArray[0] == id1 );
    EUNIT_ASSERT( idArray[1] == id2 );
	
    CleanupStack::PopAndDestroy( propertyArray );
    CleanupStack::PopAndDestroy( &idArray );
	}
	
	
void T_xSPSettingsAPIModuleTests::FindPropertyTestL()
	{
    TInt id( CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* entry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *entry );
    EUNIT_ASSERT( KErrNone == err );
    EUNIT_ASSERT( NULL != entry );

    CSPProperty* mask = CSPProperty::NewLC();
    mask->SetName( EPropertyServiceAttributeMask );
    TInt maskValue( 0 );
    maskValue |= EIsVisibleInCallMenu;
    User::LeaveIfError( mask->SetValue( maskValue ) );
    User::LeaveIfError( entry->AddPropertyL( *mask ) );
    CleanupStack::PopAndDestroy( mask );
    
    err = iSettings->UpdateEntryL( *entry );

	CSPProperty* property = CSPProperty::NewLC();
	err = iSettings->FindPropertyL( id, EPropertyServiceAttributeMask, *property );
	
	EUNIT_ASSERT( err == KErrNone );
	
	TInt value;
	property->GetValue( value );
	
	EUNIT_ASSERT( value == maskValue );
	
	CleanupStack::PopAndDestroy( property );
	CleanupStack::PopAndDestroy( entry );
	
	}
	
void T_xSPSettingsAPIModuleTests::ConcurrentTestL()
	{
    RIdArray idArray;
    CleanupClosePushL( idArray );
    iStubObserver->NotifyChangeL( idArray );
    
    TInt id = iStubObserver->AddNewEntryL();

	EUNIT_ASSERT_EQUALS( id, iStubObserver->GetServiceId() );
	EUNIT_ASSERT_EQUALS( iStubObserver->GetCalledTimes(), 1 );
	TBool getFound = EFalse;
	getFound = iStubObserver->GetFound();
	EUNIT_ASSERT( ETrue == getFound );

	iTimes1 = 0;
	iTimes2 = 0;
	
	iStubObserver->CancelNotify();
	
    if ( !iPeriodic1->IsActive() )
        {
        iPeriodic1->Start( 10000, 1000000, TCallBack( AddNewEntryCallBack1, static_cast<TAny*>( this ) ) );
        }
    if ( !iPeriodic2->IsActive() )
        {
        iPeriodic2->Start( 10000, 1000000, TCallBack( AddNewEntryCallBack2, static_cast<TAny*>( this ) ) );
        }
    
    iWait->Start();
    CleanupStack::PopAndDestroy( &idArray );
	}
	
// ---------------------------------------------------------------------------
// IsFeatureSupportedTestL: Test feature supported or not
// ---------------------------------------------------------------------------
//
void T_xSPSettingsAPIModuleTests::IsFeatureSupportedTestL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();

    property->SetName( EPropertyServiceAttributeMask );
    User::LeaveIfError( property->SetValue( 0 ) );
	iSettings->AddOrUpdatePropertyL( 1, *property );
    iSettings->AddOrUpdatePropertyL( 2, *property );
    
    TInt id1 = CreateAndAddTestEntryL( _L( "TestServiceEntry1" ) );
    TInt id2 = CreateAndAddTestEntryL( _L( "TestServiceEntry2" ) );
    TInt id3 = CreateAndAddTestEntryL( _L( "TestServiceEntry3" ) );
    
    TBool support = EFalse;
    support = iSettings->IsFeatureSupported( ESupportInternetCallFeature );
    EUNIT_ASSERT( !support );
    
    // ESupportInternetCallFeature
    property->SetName( EPropertyServiceAttributeMask );
    User::LeaveIfError( property->SetValue( 0x0190 ) );
    iSettings->AddOrUpdatePropertyL( id1, *property );
    support = iSettings->IsFeatureSupported( ESupportInternetCallFeature );
    EUNIT_ASSERT( support );
    
    User::LeaveIfError( property->SetValue( 0x0180 ) );
    iSettings->AddOrUpdatePropertyL( id1, *property );
    support = iSettings->IsFeatureSupported( ESupportInternetCallFeature );
    EUNIT_ASSERT( !support );

    // ESupportCallOutFeature
    support = iSettings->IsFeatureSupported( ESupportCallOutFeature );
    EUNIT_ASSERT( !support );

    property->SetName( EPropertyServiceAttributeMask );
    User::LeaveIfError( property->SetValue( 0x0150 ) );
    iSettings->AddOrUpdatePropertyL( id2, *property );
    support = iSettings->IsFeatureSupported( ESupportCallOutFeature );
    EUNIT_ASSERT( support );
    
    // ESupportVoIPSSFeature
    support = iSettings->IsFeatureSupported( ESupportVoIPSSFeature );
    EUNIT_ASSERT( !support );

    property->SetName( EPropertyServiceAttributeMask );
    User::LeaveIfError( property->SetValue( 0x0400 ) );
    iSettings->AddOrUpdatePropertyL( id3, *property );
    support = iSettings->IsFeatureSupported( ESupportVoIPSSFeature );
    EUNIT_ASSERT( support );
    
    User::LeaveIfError( property->SetValue( 0 ) );
    iSettings->AddOrUpdatePropertyL( id3, *property );
    support = iSettings->IsFeatureSupported( ESupportVoIPSSFeature );
    EUNIT_ASSERT( !support );
    
    // ESupportVoIPFeature
    support = iSettings->IsFeatureSupported( ESupportVoIPFeature );
    EUNIT_ASSERT_EQUALS( support, FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) );

    CleanupStack::PopAndDestroy( property );
    }

	
	
TInt T_xSPSettingsAPIModuleTests::AddNewEntryCallBack1( TAny* aPointer )
    {
    static_cast<T_xSPSettingsAPIModuleTests*>( aPointer )->AddNewEntry1();
    return KErrNone;
    }
    
void T_xSPSettingsAPIModuleTests::AddNewEntry1()
    {
    TBuf<30> buf;
    buf.Append( KEntryName );
    iTimes1++;
    buf.AppendNum( iTimes1 );
    EUNIT_PRINT( buf );   
    
    TInt id = CreateAndAddTestEntryL( buf );
    EUNIT_ASSERT( id != 0 );
    
    if( iTimes1 > 5 && iTimes2 > 5)
    	{
    	EUNIT_PRINT( buf );
    	iPeriodic1->Cancel();
    	iPeriodic2->Cancel();
    	iWait->AsyncStop();
    	}
    
    }
        
TInt T_xSPSettingsAPIModuleTests::AddNewEntryCallBack2( TAny* aPointer )
    {
    static_cast<T_xSPSettingsAPIModuleTests*>( aPointer )->AddNewEntry2();
    return KErrNone;
    }
    
void T_xSPSettingsAPIModuleTests::AddNewEntry2()
    {
    TBuf<30> buf;
    buf.Append( KEntryName2 );
    iTimes2++;
    buf.AppendNum( iTimes2 );
    EUNIT_PRINT( buf ); 
    TInt id = CreateAndAddTestEntryL( buf );
    EUNIT_ASSERT( id != 0 );
    
    if( iTimes1 > 5 && iTimes2 > 5)
    	{
    	EUNIT_PRINT( buf );
    	iPeriodic1->Cancel();
    	iPeriodic2->Cancel();
    	iWait->AsyncStop();
    	}
    }
    
void T_xSPSettingsAPIModuleTests::BackupTestL()
	{
	TInt id1 = CreateAndAddTestEntryL( KBackupName1 );
	EUNIT_ASSERT( 0 != id1 );
	TInt id2 = CreateAndAddTestEntryL( KBackupName2 );
	EUNIT_ASSERT( 0 != id2 );
	TBuf<30> buf;
	buf.AppendNum( id1 );
	buf.Append( _L("  ") );
	buf.AppendNum( id2 );
	EUNIT_PRINT( buf );
	}

void T_xSPSettingsAPIModuleTests::RestoreTestL()
	{
    RIdArray services;
    CleanupClosePushL( services );
    TInt err = iSettings->FindServiceIdsL( services );
    TBool found1 = EFalse;
    TBool found2 = EFalse;
	    
    for ( TInt i = 0; services.Count() > i; i++ )
        {
        CSPEntry* entry = CSPEntry::NewLC();
        err = iSettings->FindEntryL( services[i], *entry );
        EUNIT_ASSERT( KErrNone == err );
        TBuf<30> buf;
        buf.AppendNum( entry->GetServiceId() );
        buf.Append( _L(" ") );
        buf.Append( entry->GetServiceName() );
        EUNIT_PRINT( buf );
        
        EUNIT_ASSERT( KErrNone == err );
        
        if( entry->GetServiceName() == KBackupName1 )
        	{
        	found1 = ETrue;
        	}
        else if( entry->GetServiceName() == KBackupName2 )
        	{
        	found2 = ETrue;
        	}
        else
        	{
        	// do nothing
        	}

        CleanupStack::PopAndDestroy( entry );
        }
        
    CleanupStack::PopAndDestroy( &services );
    EUNIT_ASSERT( found1 );
    EUNIT_ASSERT( found2 );
	}

void T_xSPSettingsAPIModuleTests::VoIPProfilesExistTestL()
	{
	TBool ret = EFalse;
	User::LeaveIfError( ret = iVoIPUtils->VoIPProfilesExistL() );
	
	EUNIT_ASSERT( ret );
	}
	
void T_xSPSettingsAPIModuleTests::IsVoIPSupportedTestL()
	{
	TBool ret = EFalse;
	ret = iVoIPUtils->IsVoIPSupported();
	
	EUNIT_ASSERT( ret == EFalse || ret == FeatureManager::FeatureSupported( KFeatureIdCommonVoip ));
	}
	
void T_xSPSettingsAPIModuleTests::IsPreferredTelephonyVoIPTestL()
	{
	TBool ret = EFalse;
	User::LeaveIfError( ret = iVoIPUtils->IsPreferredTelephonyVoIP() );
	
	EUNIT_ASSERT( !ret );
	}
	
void T_xSPSettingsAPIModuleTests::AddDeletePropertyTest1L()
    {
    // Create entry
    TInt id( CreateAndAddTestEntryL( _L( "AddDeletePropertyTest1" ) ) );
    
    // Fetch entry from storage and modify    
    CSPEntry* updateEntry = CSPEntry::NewLC();
    TInt err = iSettings->FindEntryL( id, *updateEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT( NULL != updateEntry );
    
    // delete property
    EUNIT_ASSERT_EQUALS( KErrNone, updateEntry->DeleteProperty( EPropertyBrandId ));
    
    // add property
    RBuf newBrandId;
    newBrandId.CreateL( _L( "NewBrandId" ) );
    CleanupClosePushL( newBrandId );
    CSPProperty* brandProperty = CSPProperty::NewLC();
    brandProperty->SetName( EPropertyBrandId );
    brandProperty->SetValue( newBrandId );
    EUNIT_ASSERT_EQUALS( KErrNone, updateEntry->AddPropertyL( *brandProperty ));
    
    CleanupStack::PopAndDestroy( brandProperty );
    CleanupStack::PopAndDestroy( &newBrandId );

    // Update entry to settings storage
    err = iSettings->UpdateEntryL( *updateEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );

    // Find entry and check that content has updated
    CSPEntry* checkEntry = CSPEntry::NewLC();
    err = iSettings->FindEntryL( id, *checkEntry );
    EUNIT_ASSERT_EQUALS( err, KErrNone );

    const RPropertyArray& array = checkEntry->GetAllProperties();
    EUNIT_ASSERT_EQUALS( array.Count(),KTestEntryPropertyCount );
    
    for( TInt i = 0; i< array.Count(); i++ )
        {
        CSPProperty* property = array[i];
        if( property->GetName() == EPropertyBrandId )
            {
            TBuf<20> value;
            User::LeaveIfError( property->GetValue( value ) );
            EUNIT_ASSERT_EQUALS( value, _L("NewBrandId") );
            break;
            }
        }
    
    CleanupStack::PopAndDestroy( checkEntry );
    CleanupStack::PopAndDestroy( updateEntry );
    }

void T_xSPSettingsAPIModuleTests::AddDeletePropertyTest2L()
	{
    RIdArray services;
    CleanupClosePushL( services );
    TInt err = iSettings->FindServiceIdsL( services );
    TBool found = EFalse;
	    
    for ( TInt i = 0; services.Count() > i; i++ )
        {
        CSPEntry* entry = CSPEntry::NewLC();
        err = iSettings->FindEntryL( services[i], *entry );
        EUNIT_ASSERT( KErrNone == err );
        TBuf<30> buf;
        buf.AppendNum( entry->GetServiceId() );
        buf.Append( _L(" ") );
        buf.Append( entry->GetServiceName() );
        EUNIT_PRINT( buf );
        
        EUNIT_ASSERT( KErrNone == err );
        
        if( entry->GetServiceName() == KAddDeletePropertyTestName )
        	{
        	found = ETrue;
            const RPropertyArray& array = entry->GetAllProperties();
            EUNIT_ASSERT_EQUALS( array.Count(),KTestEntryPropertyCount );

            for( TInt i = 0; i< array.Count(); i++ )
                {
                CSPProperty* property = array[i];
                if( property->GetName() == EPropertyBrandId )
                    {
                    TBuf<20> value;
                    User::LeaveIfError( property->GetValue( value ) );
                    EUNIT_ASSERT_EQUALS( value, _L("NewBrandId") );
                    break;
                    }
                }
        	}

        CleanupStack::PopAndDestroy( entry );
        }
        
    CleanupStack::PopAndDestroy( &services );
    EUNIT_ASSERT( found );
	}
    
// ---------------------------------------------------------------------------
// TEST TABLE
// ---------------------------------------------------------------------------
//
 
EUNIT_BEGIN_TEST_TABLE(
    T_xSPSettingsAPIModuleTests,
    "Add test suite description here.",
    "MODULE" )

// CSPEntry class tests
EUNIT_TEST(
    "Test entry",
    "CSPEntry",
    "-",
    "MODULE",
    SetupL, EntryTestL, Teardown)

EUNIT_TEST(
    "Test entry misuse",
    "CSPEntry",
    "-",
    "MODULE",
    SetupL, EntryMisuseTestL, Teardown)

// CSPProperty class tests
EUNIT_TEST(
    "Test property",
    "CSPProperty",
    "-",
    "MODULE",
    SetupL, PropertyTestL, Teardown)

EUNIT_TEST(
    "Test property misuse",
    "CSPProperty",
    "-",
    "MODULE",
    SetupL, PropertyMisuseTestL, Teardown)

// API class tests
EUNIT_TEST(
    "Create API instance",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, NewAPITestL, Teardown2)

EUNIT_TEST(
    "Add new entry",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, AddEntryTestL, Teardown)

EUNIT_TEST(
    "Find entry by ID",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindEntryByIdTestL, Teardown)

EUNIT_TEST(
    "Find entry by ID 2",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, FindEntryByIdTest2L, Teardown2)

EUNIT_TEST(
    "Update entry",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, UpdateEntryTestL, Teardown)
    
EUNIT_TEST(
    "Reset entry",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, ResetTestL, Teardown)

EUNIT_TEST(
    "Delete entry",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, DeleteEntryTestL, Teardown)

EUNIT_TEST(
    "Find property by ID",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindPropertyByIdTestL, Teardown)
    
EUNIT_TEST(
    "Get All Properties",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, GetAllPropertiesTestL, Teardown)

EUNIT_TEST(
    "Update property",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, AddOrUpdatePropertiesTestL, Teardown)

EUNIT_TEST(
    "Settings count",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, SettingsCountTestL, Teardown)

EUNIT_TEST(
    "Find service IDs",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindServiceIdsTestL, Teardown)

EUNIT_TEST(
    "Find service names",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindServiceNamesTestL, Teardown)

EUNIT_TEST(
    "Find subservice properties",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindSubServicePropertiesTestL, Teardown)

EUNIT_TEST(
    "Delete subservice properties",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, DeleteServicePropertiesTestL, Teardown)

EUNIT_TEST(
    "Test SIP VoIP software version",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, GetSIPVoIPSWVersionTestL, Teardown)

EUNIT_TEST(
    "Test notify change",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, NotifyChangeTestL, Teardown)

EUNIT_TEST(
    "Test find service Ids from properties",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindServiceIdsFromPropertiesTestL, Teardown)

EUNIT_TEST(
    "Test find property",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, FindPropertyTestL, Teardown)
    
EUNIT_TEST(
    "Test property delete",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, DeletePropertyByNameTestL, Teardown)

EUNIT_TEST(
    "Test concurrent",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, ConcurrentTestL, Teardown)

EUNIT_TEST(
    "Test feature check",
    "CSPSettings",
    "-",
    "MODULE",
    SetupL, IsFeatureSupportedTestL, Teardown)

EUNIT_TEST(
    "Test backup",
    "CSPSettings",
    "-",
    "MODULE",
    Setup3L, BackupTestL, Teardown2)

EUNIT_TEST(
    "Test restore",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, RestoreTestL, Teardown2)

EUNIT_TEST(
    "Test VoIPProfilesExistL",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, VoIPProfilesExistTestL, Teardown2)

EUNIT_TEST(
    "Test IsVoIPSupportedTestL",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, IsVoIPSupportedTestL, Teardown2)

EUNIT_TEST(
    "Test IsPreferredTelephonyVoIPTestL",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, IsPreferredTelephonyVoIPTestL, Teardown3)

EUNIT_TEST(
    "Test AddDeletePropertyTest1L",
    "CSPSettings",
    "-",
    "MODULE",
    Setup3L, AddDeletePropertyTest1L, Teardown2)

EUNIT_TEST(
    "Test AddDeletePropertyTest1L",
    "CSPSettings",
    "-",
    "MODULE",
    Setup2L, AddDeletePropertyTest2L, Teardown2)

EUNIT_END_TEST_TABLE



// ---------------------------------------------------------------------------
// HELPER METHODS
// ---------------------------------------------------------------------------
//
//
// ---------------------------------------------------------------------------
// Creates settings entry with property of each data type
// ---------------------------------------------------------------------------
//
CSPEntry* T_xSPSettingsAPIModuleTests::CreateTestEntryLC( const TDesC& aName )
    {
    // Create entry
    CSPEntry* entry = CSPEntry::NewLC();
    EUNIT_ASSERT( NULL != entry );
    User::LeaveIfError( entry->SetServiceName( aName ) );
    
    // Create property of TInt type and append it to entry
    TInt err = KErrNone;
    TInt profId( 15 );
    CSPProperty* intProp = CreateTestPropertyL( EPropertyVoIPSubServicePluginId, profId );
    CleanupStack::PushL( intProp );
    err = entry->AddPropertyL( *intProp );
    if ( KErrNoMemory == err ) User::Leave( err ); // For Alloc failure
    EUNIT_ASSERT( KErrNone == err );    
    CleanupStack::PopAndDestroy( intProp );
    
    // Create property of TOnOff type and append it to entry
    TOnOff onoff(  EOn );
    CSPProperty* onoffProp = CreateTestPropertyL( ESubPropertyVMBXListenRegister, onoff );
    CleanupStack::PushL( onoffProp );
    err = entry->AddPropertyL( *onoffProp );
    if ( KErrNoMemory == err ) User::Leave( err ); // For Alloc failure
    EUNIT_ASSERT( err == KErrNone );    
    CleanupStack::PopAndDestroy( onoffProp );

    // Create property of Des type and append it to entry
    RBuf brandId;
    brandId.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandId );
    CSPProperty* desProp = CreateTestPropertyL( EPropertyBrandId, brandId );
    CleanupStack::PushL( desProp );
    err = entry->AddPropertyL( *desProp );
    if ( KErrNoMemory == err ) User::Leave( err ); // For Alloc failure
    EUNIT_ASSERT( err == KErrNone );
    CleanupStack::PopAndDestroy( desProp);
    CleanupStack::PopAndDestroy( &brandId );

    RBuf listenAddr;
    listenAddr.CreateL( _L( "SomeAddress1" ) );
    CleanupClosePushL( listenAddr );
    CSPProperty* desProp2 = CreateTestPropertyL( ESubPropertyVMBXListenAddress, listenAddr );
    CleanupStack::PushL( desProp2 );
    err = entry->AddPropertyL( *desProp2 );
    if ( KErrNoMemory == err ) User::Leave( err ); // For Alloc failure
    EUNIT_ASSERT( err == KErrNone );
    CleanupStack::PopAndDestroy( desProp2);
    CleanupStack::PopAndDestroy( &listenAddr );

    RBuf wiAddr;
    wiAddr.CreateL( _L( "SomeAddress2" ) );
    CleanupClosePushL( wiAddr );
    CSPProperty* desProp3 = CreateTestPropertyL( ESubPropertyVMBXMWIAddress, wiAddr );
    CleanupStack::PushL( desProp3 );
    err = entry->AddPropertyL( *desProp3 );
    if ( KErrNoMemory == err ) User::Leave( err ); // For Alloc failure
    EUNIT_ASSERT( err == KErrNone );
    CleanupStack::PopAndDestroy( desProp3 );
    CleanupStack::PopAndDestroy( &wiAddr );

    return entry;
    }

// ---------------------------------------------------------------------------
// Creates and returns settings property with given name and value
// ---------------------------------------------------------------------------
//
template <class T>
CSPProperty* T_xSPSettingsAPIModuleTests::CreateTestPropertyL( 
    TServicePropertyName aProperty, const T& aValue )
    {
    CSPProperty* property = CSPProperty::NewLC();
    EUNIT_ASSERT( NULL != property );

    User::LeaveIfError( property->SetName( aProperty ) );
    
    User::LeaveIfError( property->SetValue( aValue ) );
    
    CleanupStack::Pop( property );

    return property;
    }

// ---------------------------------------------------------------------------
// Creates settings entry with given name and adds it to store
// ---------------------------------------------------------------------------
//
TInt T_xSPSettingsAPIModuleTests::CreateAndAddTestEntryL( 
    const TDesC& aName )
    {
    // Create entry
    CSPEntry* entry = CreateTestEntryLC( aName );
    EUNIT_ASSERT( NULL != entry );

    // Add entry to storage
    iSettings->AddEntryL( *entry );
    TInt id = entry->GetServiceId();
    EUNIT_ASSERT( id != 0 );
    CleanupStack::PopAndDestroy( entry );

    return id;
    }

//  END OF FILE
