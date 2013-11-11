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
* Description: Implementation of T_CSPProperty class.
*
*/

#include "T_CSPProperty.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>



// - Construction -----------------------------------------------------------

T_CSPProperty* T_CSPProperty::NewL()
    {
    T_CSPProperty* self = T_CSPProperty::NewLC();
    CleanupStack::Pop();
    return self;
    }

T_CSPProperty* T_CSPProperty::NewLC()
    {
    T_CSPProperty* self = new( ELeave ) T_CSPProperty();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_CSPProperty::~T_CSPProperty()
    {
    }

T_CSPProperty::T_CSPProperty()
    {
    }

void T_CSPProperty::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void T_CSPProperty::SetupL(  )
    {
    iCSPProperty = CSPProperty::NewL();
    }
    

void T_CSPProperty::Teardown(  )
    {
    delete iCSPProperty;
    iCSPProperty = NULL;
    }

// ---------------------------------------------------------------------------
// T_CSPProperty::PropertyDatatypeTIntSetGetTestL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::PropertyDatatypeTIntSetGetTestL(  )
    {
    TInt profIdSet( 15 );
    EUNIT_ASSERT( KErrArgument == iCSPProperty->SetValue( profIdSet ) );
    
    iCSPProperty->SetName( EPropertyVoIPSubServicePluginId );
    EUNIT_ASSERT( EPropertyVoIPSubServicePluginId == iCSPProperty->GetName() );
    User::LeaveIfError( iCSPProperty->SetValue( profIdSet ));
    EUNIT_ASSERT( EDataTypeInt == iCSPProperty->GetDataType() );
    
    TInt profIdGet( 0 );
    TInt error = iCSPProperty->GetValue( profIdGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( profIdSet == profIdGet );
    }

// ---------------------------------------------------------------------------
// T_CSPProperty::PropertyDatatypeOnOffSetGetTestL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::PropertyDatatypeOnOffSetGetTestL(  )
    {
    TOnOff onOffSet( EOn );
    EUNIT_ASSERT( KErrArgument == iCSPProperty->SetValue( onOffSet ) );
    
    iCSPProperty->SetName( ESubPropertyPresenceEnabled );
    EUNIT_ASSERT( ESubPropertyPresenceEnabled == iCSPProperty->GetName() );
    User::LeaveIfError( iCSPProperty->SetValue( onOffSet ) );
    EUNIT_ASSERT( EDataTypeOnOff == iCSPProperty->GetDataType() );
    
    TOnOff onOffGet( EOONotSet );
    TInt error = iCSPProperty->GetValue( onOffGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( onOffSet == onOffGet );
    }

// ---------------------------------------------------------------------------
// T_CSPProperty::PropertyDatatypeTDesCSetGetTestL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::PropertyDatatypeTDesCSetGetTestL(  )
    {
    iCSPProperty->SetName( EPropertyBrandId );
    EUNIT_ASSERT( EPropertyBrandId == iCSPProperty->GetName() );
    
    TBuf<514> overflower;
    overflower.FillZ( 514 );
    EUNIT_ASSERT( KErrOverflow == iCSPProperty->SetValue( overflower ) );
    
    RBuf brandIdSet;
    brandIdSet.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandIdSet);
    User::LeaveIfError( iCSPProperty->SetValue( brandIdSet ) );
    EUNIT_ASSERT( iCSPProperty->GetDataType() == EDataTypeDes );
    
    RBuf brandIdGet;    
    brandIdGet.CreateL( 9 );
    CleanupClosePushL( brandIdGet );
    brandIdGet.FillZ( 9 );
    EUNIT_ASSERT( KErrOverflow == iCSPProperty->GetValue( brandIdGet ) );
    CleanupStack::PopAndDestroy( &brandIdGet);
    
    brandIdGet.CreateL( KSPMaxDesLength );
    CleanupClosePushL( brandIdGet);
    TInt error = iCSPProperty->GetValue( brandIdGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( brandIdSet.Compare( brandIdGet ) == 0 );

    CleanupStack::PopAndDestroy( &brandIdGet);
    CleanupStack::PopAndDestroy( &brandIdSet);
    }

// ---------------------------------------------------------------------------
// CopyLL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::CopyLL(  )
    {
    iCSPProperty->CopyL( *iCSPProperty );
    
    CSPProperty* property = CSPProperty::NewLC();
    RBuf brandIdSet;
    brandIdSet.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( brandIdSet);
    User::LeaveIfError( property->SetValue( brandIdSet ) );
    
    iCSPProperty->CopyL( *property );
    
    RBuf brandIdGet;
    brandIdGet.CreateL( KSPMaxDesLength );
    CleanupClosePushL( brandIdGet);
    TInt error = iCSPProperty->GetValue( brandIdGet );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( brandIdSet.Compare( brandIdGet ) == 0 );
    
    CleanupStack::PopAndDestroy( &brandIdGet );
    CleanupStack::PopAndDestroy( &brandIdSet );
    CleanupStack::PopAndDestroy( property );
    }

// ---------------------------------------------------------------------------
// CopyLL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::SetValueNoDataTypeCheckL(  )
    {
    RBuf bufToSet;
    bufToSet.CreateL( KSPMaxDesLength + 1 );
    CleanupClosePushL( bufToSet );
    bufToSet.FillZ( KSPMaxDesLength + 1 );
    EUNIT_ASSERT( KErrOverflow == iCSPProperty->SetValueNoDataTypeCheck( bufToSet ) );    
    CleanupStack::PopAndDestroy( &bufToSet );
    
    bufToSet.CreateL( _L( "SomeBrandId" ) );
    CleanupClosePushL( bufToSet );
    TInt error = iCSPProperty->SetValueNoDataTypeCheck( bufToSet );
    EUNIT_ASSERT( KErrNone == error );
    
    CleanupStack::PopAndDestroy( &bufToSet );
    }


// ---------------------------------------------------------------------------
// PropertyTypeL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::PropertyTypeL(  )
    {
    EUNIT_ASSERT( EItemTypeNotDefined == iCSPProperty->PropertyType( EPropertyUnknown ) );
    EUNIT_ASSERT( EItemTypeVoIPSubProperty == iCSPProperty->PropertyType( ESubPropertyVoIPPreferredSNAPId ) );
    EUNIT_ASSERT( EItemTypePresenceSubProperty == iCSPProperty->PropertyType( ESubPropertyPresencePreferredSNAPId  ) );
    EUNIT_ASSERT( EItemTypeIMSubProperty == iCSPProperty->PropertyType( ESubPropertyIMPreferredSNAPId ) );
    EUNIT_ASSERT( EItemTypeVMBXSubProperty  == iCSPProperty->PropertyType( ESubPropertyVMBXPreferredSNAPId ) );
    EUNIT_ASSERT( EItemTypeProperty  == iCSPProperty->PropertyType( EPropertyCTIPluginId ) );
    }

// ---------------------------------------------------------------------------
// PropertyTypeL
// ---------------------------------------------------------------------------
//
void T_CSPProperty::EqualToOperatorOverloadL(  )
    {
    CSPProperty* property = CSPProperty::NewLC();
    TBool objectsEqual = *iCSPProperty == *iCSPProperty;
    EUNIT_ASSERT( objectsEqual );
    
    property->SetName( EPropertyBrandId );
    objectsEqual = *iCSPProperty == *property;
    EUNIT_ASSERT( !objectsEqual );
    
    CleanupStack::PopAndDestroy( property );
    }
    

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    T_CSPProperty,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "PropertyDatatypeTIntSetGetTest - Test",
    "CSPProperty",
    "PropertyDatatypeTIntSetGetTest - Test",
    "FUNCTIONALITY",
    SetupL, PropertyDatatypeTIntSetGetTestL, Teardown)
    
EUNIT_TEST(
    "PropertyDatatypeTIntSetGetTest - Test",
    "CSPProperty",
    "PropertyDatatypeTIntSetGetTest - Test",
    "FUNCTIONALITY",
    SetupL, PropertyDatatypeOnOffSetGetTestL, Teardown)
    
EUNIT_TEST(
    "PropertyDatatypeTIntSetGetTest - Test",
    "CSPProperty",
    "PropertyDatatypeTIntSetGetTest - Test",
    "FUNCTIONALITY",
    SetupL, PropertyDatatypeTDesCSetGetTestL, Teardown)

EUNIT_TEST(
    "CopyL - Test",
    "CSPProperty",
    "CopyL - Test",
    "FUNCTIONALITY",
    SetupL, CopyLL, Teardown)

EUNIT_TEST(
    "SetValueNoDataTypeCheck - Test",
    "CSPProperty",
    "SetValueNoDataTypeCheck - Test",
    "FUNCTIONALITY",
    SetupL, SetValueNoDataTypeCheckL, Teardown)
    
EUNIT_TEST(
    "PropertyType - Test",
    "CSPProperty",
    "PropertyType - Test",
    "FUNCTIONALITY",
    SetupL, PropertyTypeL, Teardown)

EUNIT_TEST(
    "EqualToOperatorRedefinition - Test",
    "CSPProperty",
    "EqualToOperatorRedefinition - Test",
    "FUNCTIONALITY",
    SetupL, EqualToOperatorOverloadL, Teardown)

EUNIT_END_TEST_TABLE
