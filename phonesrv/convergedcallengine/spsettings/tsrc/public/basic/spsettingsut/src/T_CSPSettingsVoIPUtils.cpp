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
* Description: Implementation of T_CSPSettingsVoIPUtils class.
*
*/

#include "T_CSPSettingsVoIPUtils.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <centralrepository.h>
#include <SettingsInternalCRKeys.h>

#include "spsettingsvoiputils.h"
#include "TestData.h"

// - Construction -----------------------------------------------------------

T_CSPSettingsVoIPUtils* T_CSPSettingsVoIPUtils::NewL()
    {
    T_CSPSettingsVoIPUtils* self = T_CSPSettingsVoIPUtils::NewLC();
    CleanupStack::Pop();
    return self;
    }

T_CSPSettingsVoIPUtils* T_CSPSettingsVoIPUtils::NewLC()
    {
    T_CSPSettingsVoIPUtils* self = new( ELeave ) T_CSPSettingsVoIPUtils();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

T_CSPSettingsVoIPUtils::~T_CSPSettingsVoIPUtils()
    {
    }

T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils()
    {
    }

void T_CSPSettingsVoIPUtils::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void T_CSPSettingsVoIPUtils::SetupL(  )
    {
    iCSPSettingsVoIPUtils = CSPSettingsVoIPUtils::NewL();
    }

void T_CSPSettingsVoIPUtils::Setup2L(  )
    {
    iCSPSettingsVoIPUtils = CSPSettingsVoIPUtils::NewL();
    
    CRepository* repository = CRepository::NewLC( KCRUidTelephonySettings );
    User::LeaveIfError( repository->Set( KDynamicVoIP, EFalse ) );
    CleanupStack::PopAndDestroy( repository );
    
    repository = CRepository::NewLC( KCRUidRichCallSettings );
    User::LeaveIfError( repository->Set( KRCSPSPreferredService, 0 ) );
    CleanupStack::PopAndDestroy( repository );
    }

void T_CSPSettingsVoIPUtils::Setup3L(  )
    {
    TestData::iCheckSupportedCase = ETrue;
    iCSPSettingsVoIPUtils = CSPSettingsVoIPUtils::NewL();
    }

void T_CSPSettingsVoIPUtils::Teardown(  )
    {    
    delete iCSPSettingsVoIPUtils;
    iCSPSettingsVoIPUtils = NULL;
    }
    
void T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils_VoIPProfilesExistLL(  )
    {
    iCSPSettingsVoIPUtils->VoIPProfilesExistL();
    }

void T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils_IsVoIPSupportedL(  )
    {
    EUNIT_ASSERT( !iCSPSettingsVoIPUtils->IsVoIPSupported( ) );
    TestData::iCheckSupportedCase = EFalse;
    }
    
void T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils_IsPreferredTelephonyVoIPL(  )
    {
    EUNIT_ASSERT( !iCSPSettingsVoIPUtils->IsPreferredTelephonyVoIP() );
    }
    
void T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils_GetPreferredServiceL(  )
    {
    TUint id( 0 );
    EUNIT_ASSERT( KErrNotSupported == iCSPSettingsVoIPUtils->GetPreferredService( id ) );
    
    CRepository* repository = CRepository::NewLC( KCRUidTelephonySettings );
    User::LeaveIfError( repository->Set( KDynamicVoIP, ETrue ) );
    CleanupStack::PopAndDestroy( repository );
    
    EUNIT_ASSERT( KErrNotFound == iCSPSettingsVoIPUtils->GetPreferredService( id ) );
    
    TInt testValue( 1 );
    repository = CRepository::NewLC( KCRUidRichCallSettings );
    User::LeaveIfError( repository->Set( KRCSPSPreferredService, testValue ) );
    CleanupStack::PopAndDestroy( repository );
    EUNIT_ASSERT( KErrNone == iCSPSettingsVoIPUtils->GetPreferredService( id ) );
    EUNIT_ASSERT( testValue == id );
    }

void T_CSPSettingsVoIPUtils::T_CSPSettingsVoIPUtils_DoGetPreferredServiceLL(  )
    {
    
    }


// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    T_CSPSettingsVoIPUtils,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "VoIPProfilesExistL - test",
    "CSPSettingsVoIPUtils",
    "VoIPProfilesExistL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsVoIPUtils_VoIPProfilesExistLL, Teardown)

EUNIT_TEST(
    "IsVoIPSupported - test",
    "CSPSettingsVoIPUtils",
    "IsVoIPSupported - test",
    "FUNCTIONALITY",
    Setup3L, T_CSPSettingsVoIPUtils_IsVoIPSupportedL, Teardown)

EUNIT_TEST(
    "IsPreferredTelephonyVoIP - test",
    "CSPSettingsVoIPUtils",
    "IsPreferredTelephonyVoIP - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsVoIPUtils_IsPreferredTelephonyVoIPL, Teardown)
    
EUNIT_TEST(
    "GetPreferredService - test",
    "CSPSettingsVoIPUtils",
    "GetPreferredService - test",
    "FUNCTIONALITY",
    Setup2L, T_CSPSettingsVoIPUtils_GetPreferredServiceL, Teardown)
/*
EUNIT_TEST(
    "DoGetPreferredServiceL - test",
    "CSPSettingsVoIPUtils",
    "DoGetPreferredServiceL - test",
    "FUNCTIONALITY",
    SetupL, T_CSPSettingsVoIPUtils_DoGetPreferredServiceLL, Teardown)*/


EUNIT_END_TEST_TABLE
