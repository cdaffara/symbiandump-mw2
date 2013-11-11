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

#include "IT_RemotePartyInformation.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <CCallInformation.h>
#include <ccallinfoiter.h>
#include <MCallInformationObserver.h>
#include <CCallRemotePartyInformation.h>
#include <MCallRemotePartyInfo.h>
#include <ccallremotepartyinfoiter.h>

#include "cdialer.h"

//Speaking clock number when calling from NTN network.
_LIT (KNumber, "99901");

// - Construction -----------------------------------------------------------

IT_RemotePartyInformation* IT_RemotePartyInformation::NewL()
    {
    IT_RemotePartyInformation* self = IT_RemotePartyInformation::NewLC();
    CleanupStack::Pop();
    return self;
    }

IT_RemotePartyInformation* IT_RemotePartyInformation::NewLC()
    {
    IT_RemotePartyInformation* self = new( ELeave ) IT_RemotePartyInformation();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

IT_RemotePartyInformation::~IT_RemotePartyInformation()
    {
    }

IT_RemotePartyInformation::IT_RemotePartyInformation()
    {
    }

void IT_RemotePartyInformation::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------

void IT_RemotePartyInformation::CallInformationChanged()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

void IT_RemotePartyInformation::CallRemotePartyInformationChanged()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }

void IT_RemotePartyInformation::SetupL(  )
    {    
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::SetupL in" ));
    User::LeaveIfError( iTelServer.Connect() );    
    iCallInfos = CCallInformation::NewL();
    iRemotePartyInfo = CCallRemotePartyInformation::NewL();
    iDialer = CDialer::NewL( iTelServer );
    iWait = new( ELeave )CActiveSchedulerWait();
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::SetupL out" ));
    }
    

void IT_RemotePartyInformation::Teardown(  )
    {
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::Teardown in" ));
    delete iWait;    
    delete iDialer;
    delete iRemotePartyInfo;
    delete iCallInfos;
    iTelServer.Close();
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::Teardown out" ));
    }
    

void IT_RemotePartyInformation::TestNoCallsL(  )
    {    
    const MCallRemotePartyInfo* remoteInfo = iRemotePartyInfo->GetRemotePartyInformationL( 1 ); 
    EUNIT_ASSERT( remoteInfo == NULL );
    
    }

void IT_RemotePartyInformation::TestSingleCallNoContactL(  )
    {
    // Listening notification from Call Info API for getting remote party info is not
    // good idea since remote party info readiness is not tied to a call state change.
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::TestSingleCallNoContactL in" ));
    iRemotePartyInfo->NotifyChanges( *this );
    iDialer->Dial(KNumber);    
    iWait->Start(); // Wait for notification from Remote Party Info API.    
    iRemotePartyInfo->CancelNotification();        
        
    CCallInfoIter& infos( iCallInfos->GetCallsL() );
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: GetCallsL" ));
    EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: Count" ));
    const MCall& call( infos.Current() );    
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: Current" ));
    TInt callIndex = call.CallIndex();   
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: CallIndex"));
        
    //check the data, option 1
    const MCallRemotePartyInfo* remoteInfo = iRemotePartyInfo->GetRemotePartyInformationL( callIndex );
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: GetRemotePartyInformationL" ));
    EUNIT_ASSERT( remoteInfo != NULL );    
    const TDesC& number = remoteInfo->Address();     
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: Address" ));
    EUNIT_ASSERT_EQUALS( number, KNumber );    
    EUNIT_ASSERT_EQUALS( remoteInfo->RemoteIdentity(), MCallRemotePartyInfo::ERemoteIdentityAvailable );
    EUNIT_ASSERT( remoteInfo->SATCall() == EFalse );
        
    //check the data, option 2    
    CCallRemotePartyInfoIter& iter = iRemotePartyInfo->GetRemotePartyInformationL( );
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: GetRemotePartyInformationL" ));
    EUNIT_ASSERT_EQUALS( iter.Count(), 1 );   
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: Count" ));
    const MCallRemotePartyInfo& duplicatedInfo = iter.Current();
    EUNIT_PRINT( _L( "IT_RemotePartyInformation: Current" ));
    EUNIT_ASSERT( (*remoteInfo == duplicatedInfo) == ETrue );    
        
    iDialer->Hangup();
    EUNIT_PRINT( _L( "IT_RemotePartyInformation::TestSingleCallNoContactL out" ));
    }



// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    IT_RemotePartyInformation,
    "API tests for Remote party information api",
    "MODULE" )

EUNIT_TEST(
    "No calls",
    "CCallRemotePartyInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestNoCallsL, Teardown)    
   
EUNIT_TEST(
    "One call, no contact information found",
    "CCallRemotePartyInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestSingleCallNoContactL, Teardown)    

EUNIT_END_TEST_TABLE
