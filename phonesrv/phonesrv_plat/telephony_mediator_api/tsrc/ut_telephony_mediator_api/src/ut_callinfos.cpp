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
* Description:
*
*/

#include "ut_callinfos.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include "callinformation.h"




// - Construction -----------------------------------------------------------

ut_callinfos* ut_callinfos::NewL()
    {
    ut_callinfos* self = ut_callinfos::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_callinfos* ut_callinfos::NewLC()
    {
    ut_callinfos* self = new( ELeave ) ut_callinfos();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_callinfos::~ut_callinfos()
    {
    }

ut_callinfos::ut_callinfos()
    {
    }

void ut_callinfos::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------

void ut_callinfos::AddInfoL( TInt aCallIndex )
    {
    TCallInfo info;
    info.iState = CCPCall::EStateDialling;
    info.iType = CCPCall::ECallTypeCSVoice;
    info.iDirection = CCPCall::EMobileOriginated;    
    info.iIndex = aCallIndex;
    info.iServiceId = 5;
    info.iEmergencyCall = EFalse;
    
    iInfos->AddL( info );
    }

void ut_callinfos::SetupL(  )
    {
    iInfos = CCallInfos::NewL();
    }
    

void ut_callinfos::Teardown(  )
    {
    delete iInfos;
    }
    

void ut_callinfos::TestEmptyInfoL(  )
    {
    EUNIT_ASSERT_EQUALS( iInfos->CallInfos().Count(), 0 );
    
    HBufC8* data = iInfos->ExternalizeL();
    CleanupStack::PushL( data );

    
    CCallInfos* info = CCallInfos::NewLC();
    info->InternalizeL( *data );
    EUNIT_ASSERT_EQUALS( info->CallInfos().Count(), 0 );
    CleanupStack::PopAndDestroy( 2, data );
    }

void ut_callinfos::TestInfoHasContentL()
    {
    AddInfoL( 0 );
    AddInfoL( 1 );
    AddInfoL( 2 );
    
    EUNIT_ASSERT_EQUALS( iInfos->CallInfos().Count(), 3 );
    
    HBufC8* data = iInfos->ExternalizeL();
    CleanupStack::PushL( data );

    CCallInfos* infos = CCallInfos::NewLC();
    infos->InternalizeL( *data );
    EUNIT_ASSERT_EQUALS( infos->CallInfos().Count(), 3 );
    
    for( TInt i = 0; i < infos->CallInfos().Count(); i++ )
        {                
        TCallInfo* callInfo = infos->CallInfos()[i];        
        EUNIT_ASSERT_EQUALS( callInfo->CallState(), CCPCall::EStateDialling );
        EUNIT_ASSERT_EQUALS( callInfo->CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( callInfo->CallIndex(), i );
        EUNIT_ASSERT_EQUALS( callInfo->CallDirection(), CCPCall::EMobileOriginated );        
        EUNIT_ASSERT_EQUALS( callInfo->ServiceId(), 5 );
        EUNIT_ASSERT_EQUALS( callInfo->IsEmergency(), EFalse );
        }
    
    CleanupStack::PopAndDestroy( 2, data );
    }
    
// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_callinfos,
    "Unittests for CCallInfos",
    "UNIT" )

EUNIT_TEST(
    "Empty info",
    "CCallInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestEmptyInfoL, Teardown)
    
EUNIT_TEST(
    "Info has content",
    "CCallInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestInfoHasContentL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Info has content",
    "CCallInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestInfoHasContentL, Teardown)
    

EUNIT_END_TEST_TABLE
