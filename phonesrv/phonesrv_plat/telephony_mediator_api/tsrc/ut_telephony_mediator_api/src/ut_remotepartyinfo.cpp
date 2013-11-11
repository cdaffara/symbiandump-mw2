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

#include "ut_remotepartyinfo.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <S32MEM.h>

#include "callremotepartyinformation.h"

static void ExternalizeL( RDesWriteStream& aStream, CCallRemotePartyInfo& aInfo )
    {
    aStream.PushL();
    aInfo.ExternalizeL( aStream );
    aStream.CommitL();
    aStream.Pop();
    aStream.Close();
    }

static void InternalizeL( RDesReadStream& aStream, CCallRemotePartyInfo& aInfo )
    {
    aStream.PushL();
    
    aInfo.InternalizeL( aStream );
    aStream.Pop();
    aStream.Close();
    }

// - Construction -----------------------------------------------------------

ut_remotepartyinfo* ut_remotepartyinfo::NewL()
    {
    ut_remotepartyinfo* self = ut_remotepartyinfo::NewLC();
    CleanupStack::Pop();
    return self;
    }

ut_remotepartyinfo* ut_remotepartyinfo::NewLC()
    {
    ut_remotepartyinfo* self = new( ELeave ) ut_remotepartyinfo();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

ut_remotepartyinfo::~ut_remotepartyinfo()
    {
    }

ut_remotepartyinfo::ut_remotepartyinfo()
    {
    }

void ut_remotepartyinfo::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------

void ut_remotepartyinfo::SetupL(  )
    {
    iInfo = CCallRemotePartyInfo::NewL();
    }
    

void ut_remotepartyinfo::Teardown(  )
    {
    delete iInfo;
    }
    

void ut_remotepartyinfo::TestEmptyInfoL(  )
    {
    EUNIT_ASSERT( iInfo->MatchedName().Length() == 0 );
    EUNIT_ASSERT( iInfo->NameFromNetwork().Length() == 0 );
    EUNIT_ASSERT( iInfo->CompanyName().Length() == 0 );
    EUNIT_ASSERT( iInfo->Address().Length() == 0 );    
    EUNIT_ASSERT( iInfo->CallIndex() == -1 );
    EUNIT_ASSERT( iInfo->RemoteIdentity() == MCallRemotePartyInfo::ERemoteIdentityUnknown );
    EUNIT_ASSERT( iInfo->SATCall() == EFalse );
    }

void ut_remotepartyinfo::TestInternalizeExternalizeEmptyInfoL()
    {
    HBufC8* data = HBufC8::NewLC( 1024 );
    TPtr8 ptr = data->Des();
    RDesWriteStream stream( ptr );
    ExternalizeL( stream, *iInfo );
    
    CCallRemotePartyInfo* info = CCallRemotePartyInfo::NewLC();
    
    RDesReadStream readStream( ptr );
    InternalizeL( readStream, *info );
    
    EUNIT_ASSERT( iInfo->MatchedName().Length() == 0 );
    EUNIT_ASSERT( iInfo->NameFromNetwork().Length() == 0 );
    EUNIT_ASSERT( iInfo->CompanyName().Length() == 0 );
    EUNIT_ASSERT( iInfo->Address().Length() == 0 ); 
    EUNIT_ASSERT( iInfo->CallIndex() == -1 );
    EUNIT_ASSERT( iInfo->RemoteIdentity() == MCallRemotePartyInfo::ERemoteIdentityUnknown );
    EUNIT_ASSERT( iInfo->SATCall() == EFalse );
    
    CleanupStack::PopAndDestroy( info );
    
    
    CleanupStack::PopAndDestroy( data );
    }

void ut_remotepartyinfo::TestInfoL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
    
    iInfo->SetMatchedNameL( KMatchedName );
    iInfo->SetNameFromNetworkL(KNameFromNetwork);
    iInfo->SetCompanyNameL(KCompanyName);
    iInfo->SetAddressL( KAddress );    
    iInfo->SetCallIndex( 1 );    
    iInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
    iInfo->SetSATCall( ETrue );
        
    EUNIT_ASSERT( iInfo->MatchedName().Match( KMatchedName ) == KErrNone );
    EUNIT_ASSERT( iInfo->NameFromNetwork().Match( KNameFromNetwork ) == KErrNone );
    EUNIT_ASSERT( iInfo->CompanyName().Match( KCompanyName ) == KErrNone );
    EUNIT_ASSERT( iInfo->Address().Match( KAddress ) == KErrNone );    
    EUNIT_ASSERT( iInfo->CallIndex() == 1 );
    EUNIT_ASSERT( iInfo->RemoteIdentity() == MCallRemotePartyInfo::ERemoteIdentityAvailable );
    EUNIT_ASSERT( iInfo->SATCall() == ETrue );
    }

void ut_remotepartyinfo::TestComparisonOperatorL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
    
    iInfo->SetMatchedNameL( KMatchedName );
    iInfo->SetNameFromNetworkL(KNameFromNetwork);
    iInfo->SetCompanyNameL(KCompanyName);
    iInfo->SetAddressL( KAddress );    
    iInfo->SetCallIndex( 1 );
    iInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
    
    CCallRemotePartyInfo* duplicatedInfo = CCallRemotePartyInfo::NewLC();
    _LIT( KMatchedName2, "Mr Smith" );
    _LIT( KNameFromNetwork2, "John Smith" );
    _LIT( KCompanyName2, "Nokia" );
    _LIT( KAddress2, "555555" );    
    
    duplicatedInfo->SetMatchedNameL( KMatchedName2 );
    duplicatedInfo->SetNameFromNetworkL( KNameFromNetwork2 );
    duplicatedInfo->SetCompanyNameL( KCompanyName2 );
    duplicatedInfo->SetAddressL( KAddress2 );    
    duplicatedInfo->SetCallIndex( 1 );
    duplicatedInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
        
    EUNIT_ASSERT( (*iInfo == *duplicatedInfo) == ETrue );
    CleanupStack::PopAndDestroy(duplicatedInfo);    
    }

void ut_remotepartyinfo::TestComparisonOperatorNoMatchL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
    
    iInfo->SetMatchedNameL( KMatchedName );
    iInfo->SetNameFromNetworkL(KNameFromNetwork);
    iInfo->SetCompanyNameL(KCompanyName);
    iInfo->SetAddressL( KAddress );    
    iInfo->SetCallIndex( 1 );
    iInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
    
    CCallRemotePartyInfo* duplicatedInfo = CCallRemotePartyInfo::NewLC();
    _LIT( KMatchedName2, "Mr Smith" );
    _LIT( KNameFromNetwork2, "Jack Smith" );
    _LIT( KCompanyName2, "Nokia" );
    _LIT( KAddress2, "555555" );    
    
    duplicatedInfo->SetMatchedNameL( KMatchedName2 );
    duplicatedInfo->SetNameFromNetworkL( KNameFromNetwork2 );
    duplicatedInfo->SetCompanyNameL( KCompanyName2 );
    duplicatedInfo->SetAddressL( KAddress2 );    
    duplicatedInfo->SetCallIndex( 1 );
    duplicatedInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
        
    EUNIT_ASSERT( (*iInfo == *duplicatedInfo) == EFalse );
    CleanupStack::PopAndDestroy(duplicatedInfo);    
    }

void ut_remotepartyinfo::TestInternalizeExternalizeInfoL()
    {
    _LIT( KMatchedName, "Mr Smith" );
    _LIT( KNameFromNetwork, "John Smith" );
    _LIT( KCompanyName, "Nokia" );
    _LIT( KAddress, "555555" );
    
    iInfo->SetMatchedNameL( KMatchedName );
    iInfo->SetNameFromNetworkL(KNameFromNetwork);
    iInfo->SetCompanyNameL(KCompanyName);
    iInfo->SetAddressL( KAddress );    
    iInfo->SetCallIndex( 2 );
    iInfo->SetRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityAvailable );
    iInfo->SetSATCall( ETrue );
    
    HBufC8* data = HBufC8::NewLC( 1024 );
    TPtr8 ptr = data->Des();
    
    RDesWriteStream stream( ptr );
    ExternalizeL( stream, *iInfo );
    
    CCallRemotePartyInfo* info = CCallRemotePartyInfo::NewLC();
    RDesReadStream readStream( ptr );
    InternalizeL( readStream, *info );
    
    EUNIT_ASSERT( info->MatchedName().Match( KMatchedName ) == KErrNone );
    EUNIT_ASSERT( info->NameFromNetwork().Match( KNameFromNetwork ) == KErrNone );
    EUNIT_ASSERT( info->CompanyName().Match( KCompanyName ) == KErrNone );
    EUNIT_ASSERT( info->Address().Match( KAddress ) == KErrNone );    
    EUNIT_ASSERT( info->CallIndex() == 2 );
    EUNIT_ASSERT( iInfo->RemoteIdentity() == MCallRemotePartyInfo::ERemoteIdentityAvailable );
    EUNIT_ASSERT( iInfo->SATCall() == ETrue );
    
    CleanupStack::PopAndDestroy( 2, data ); // info, data
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    ut_remotepartyinfo,
    "Unittests for CCallRemotePartyInfo",
    "UNIT" )

EUNIT_ALLOC_TEST(
    "Empty info",       //Textual description of the test case.
    "CCallRemotePartyInfo", // The class to be tested.
    "",                 // The method to be tested.
    "FUNCTIONALITY",    // What is tested: "FUNCTIONALITY", "FAILURE" or "ERRORHANDLING".
    SetupL, TestEmptyInfoL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Empty info externalize/internalize",
    "CCallRemotePartyInfo",
    "",
    "FUNCTIONALITY",
    SetupL, TestInternalizeExternalizeEmptyInfoL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Info",
    "CCallRemotePartyInfo",
    "",
    "FUNCTIONALITY",
    SetupL, TestInfoL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Test comparison operator",
    "CCallRemotePartyInfo",
    "",
    "FUNCTIONALITY",
    SetupL, TestComparisonOperatorL, Teardown)
    
EUNIT_ALLOC_TEST(
    "Test comparison operator, no match",
    "CCallRemotePartyInfo",
    "",
    "FUNCTIONALITY",
    SetupL, TestComparisonOperatorNoMatchL, Teardown)  
    
EUNIT_ALLOC_TEST(
    "Internalize/Externalize Info",
    "CCallRemotePartyInfo",
    "",
    "FUNCTIONALITY",
    SetupL, TestInternalizeExternalizeInfoL, Teardown)
 
EUNIT_END_TEST_TABLE
