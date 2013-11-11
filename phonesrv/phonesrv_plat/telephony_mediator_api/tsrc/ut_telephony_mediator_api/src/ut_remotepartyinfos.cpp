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
* Description: RemotePartyInformation unit tests.
*
*/

#include "ut_remotepartyinfos.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include "callremotepartyinformation.h"



// - Construction -----------------------------------------------------------

ut_remotepartyinfos* ut_remotepartyinfos::NewL()
    {
    ut_remotepartyinfos* self = ut_remotepartyinfos::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_remotepartyinfos* ut_remotepartyinfos::NewLC()
    {
    ut_remotepartyinfos* self = new( ELeave ) ut_remotepartyinfos();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_remotepartyinfos::~ut_remotepartyinfos()
    {
    }

ut_remotepartyinfos::ut_remotepartyinfos()
    {
    }

void ut_remotepartyinfos::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void ut_remotepartyinfos::AddInfoL( const TDesC& aMatchedName,
                                    const TDesC& aNameFromNetwork,
                                    const TDesC& aCompanyName,
                                    const TDesC& aAddress,
                                    TInt aCallIndex )
    {
    CCallRemotePartyInfo* info = CCallRemotePartyInfo::NewLC();
    info->SetMatchedNameL(aMatchedName);
    info->SetNameFromNetworkL(aNameFromNetwork);
    info->SetCompanyNameL(aCompanyName);
    info->SetAddressL( aAddress );
    info->SetCallIndex( aCallIndex );
    info->SetSATCall( EFalse );
    iInfos->AddL( info );
    CleanupStack::Pop( info );
    }

// - Test methods -----------------------------------------------------------

void ut_remotepartyinfos::SetupL(  )
    {
    iInfos = CCallRemotePartyInfos::NewL();
    }
    

void ut_remotepartyinfos::Teardown(  )
    {
    delete iInfos;
    }
    

void ut_remotepartyinfos::TestEmptyInfoL(  )
    {
    EUNIT_ASSERT_EQUALS( iInfos->RemotePartyInfos().Count(), 0 );
    
    HBufC8* data = iInfos->ExternalizeL();
    CleanupStack::PushL( data );
    
    CCallRemotePartyInfos* info = CCallRemotePartyInfos::NewLC();
    info->InternalizeL( *data );
    EUNIT_ASSERT_EQUALS( info->RemotePartyInfos().Count(), 0 );
    CleanupStack::PopAndDestroy( 2, data );
    }

void ut_remotepartyinfos::TestInfoHasContentL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
    
    _LIT( KMatchedName2, "Mr Jones");
    _LIT( KNameFromNetwork2, "Jack Jones" );
    _LIT( KCompanyName2, "Telia" );
    _LIT( KAddress2, "22222");
    
    
    AddInfoL( KMatchedName, KNameFromNetwork, KCompanyName, KAddress, 1 );
    AddInfoL( KMatchedName2,KNameFromNetwork2, KCompanyName2,KAddress2, 2 );
    
    EUNIT_ASSERT_EQUALS( iInfos->RemotePartyInfos().Count(), 2 );
    
    HBufC8* data = iInfos->ExternalizeL();
    CleanupStack::PushL( data );
        
    CCallRemotePartyInfos* infos = CCallRemotePartyInfos::NewLC();
    infos->InternalizeL( *data );
    EUNIT_ASSERT_EQUALS( infos->RemotePartyInfos().Count(), 2 );
    
    
    CCallRemotePartyInfo* newInfo = infos->RemotePartyInfos()[0];
    EUNIT_ASSERT( newInfo->MatchedName().Match( KMatchedName ) == KErrNone );
    EUNIT_ASSERT( newInfo->NameFromNetwork().Match( KNameFromNetwork ) == KErrNone );
    EUNIT_ASSERT( newInfo->CompanyName().Match( KCompanyName ) == KErrNone );
    EUNIT_ASSERT( newInfo->Address().Match( KAddress ) == KErrNone );
    EUNIT_ASSERT( newInfo->CallIndex() == 1 );
    EUNIT_ASSERT( newInfo->SATCall() == EFalse );
    
    newInfo = infos->RemotePartyInfos()[1];
    EUNIT_ASSERT( newInfo->MatchedName().Match( KMatchedName2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->NameFromNetwork().Match( KNameFromNetwork2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->CompanyName().Match( KCompanyName2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->Address().Match( KAddress2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->CallIndex() == 2 );  
    EUNIT_ASSERT( newInfo->SATCall() == EFalse );
    
    CleanupStack::PopAndDestroy( 2, data );
    }

void ut_remotepartyinfos::TestInternalizingOverExistingDataL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
        
    _LIT( KMatchedName2, "Mr Jones");
    _LIT( KNameFromNetwork2, "Jack Jones" );
    _LIT( KCompanyName2, "Telia" );
    _LIT( KAddress2, "22222");
    
    AddInfoL( KMatchedName, KNameFromNetwork, KCompanyName, KAddress, 1 );

            
    CCallRemotePartyInfos* infos = CCallRemotePartyInfos::NewLC();
    CCallRemotePartyInfo* info = CCallRemotePartyInfo::NewLC();
    info->SetMatchedNameL( KMatchedName2 );
    info->SetNameFromNetworkL(KNameFromNetwork2);
    info->SetCompanyNameL(KCompanyName2);
    info->SetAddressL( KAddress2 );
    
    info->SetCallIndex( 2 );
    infos->AddL( info );
    CleanupStack::Pop( info );
    HBufC8* data = infos->ExternalizeL();
    CleanupStack::PopAndDestroy( infos );
    CleanupStack::PushL( data );
    TPtr8 dataPtr = data->Des();
    
    
    // iInfos should contain only the last internalized information
    iInfos->InternalizeL( dataPtr );
    CleanupStack::PopAndDestroy( data );
    EUNIT_ASSERT_EQUALS( iInfos->RemotePartyInfos().Count(), 1 ); 
    CCallRemotePartyInfo* newInfo = iInfos->RemotePartyInfos()[0];    
    
    EUNIT_ASSERT( newInfo->MatchedName().Match( KMatchedName2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->NameFromNetwork().Match( KNameFromNetwork2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->CompanyName().Match( KCompanyName2 ) == KErrNone );
    EUNIT_ASSERT( newInfo->Address().Match( KAddress2 ) == KErrNone );    
    EUNIT_ASSERT( newInfo->CallIndex() == 2 );
    EUNIT_ASSERT( newInfo->SATCall() == EFalse );    
    }

void ut_remotepartyinfos::TestAddingNullL()
    {
    EUNIT_ASSERT_PANIC( iInfos->AddL( NULL ), "RPMA", 0 );
    }



// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_remotepartyinfos,
    "Unittests for CRemotePartyInfos",
    "UNIT" )

EUNIT_ALLOC_TEST(
    "Empty info",
    "CRemotePartyInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestEmptyInfoL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Info has content",
    "CRemotePartyInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestInfoHasContentL, Teardown)
   
EUNIT_ALLOC_TEST(
    "Internalize over existing data",
    "CRemotePartyInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestInternalizingOverExistingDataL, Teardown)
    
EUNIT_TEST(
    "Internalize over existing data",
    "CRemotePartyInfos",
    "",
    "FUNCTIONALITY",
    SetupL, TestAddingNullL, Teardown)


EUNIT_END_TEST_TABLE
