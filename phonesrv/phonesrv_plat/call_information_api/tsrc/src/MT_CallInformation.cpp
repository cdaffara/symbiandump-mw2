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

#include "MT_CallInformation.h"

#ifdef __WINSCW__
#include "CPhoneEngineCreator.h"
#include "CPEMessageWaiter.h"
#endif

#include <eunitmacros.h>
#include <eunitdecorators.h>

#include <etelmm.h>

#include <mmtsy_names.h>
#include <mcall.h>
#include <ccallinfoiter.h>
#include <cphcltemergencycall.h>
#include <mphcltemergencycallobserver.h>
#include <cphcltcommandhandler.h>

//Speaking clock number when calling from NTN network.
_LIT (KNumber, "99901");     
_LIT( KEmergencyNumber, "112" );
_LIT( KOtherNumber, "99999" );

class CDialer : public CActive, protected MPhCltEmergencyCallObserver
    {
public:
    
    static CDialer* NewL( RTelServer& aTelServer ) { 
        CDialer* self = CDialer::NewLC( aTelServer );
        CleanupStack::Pop( self );
        return self;
    }
    
    static CDialer* NewLC( RTelServer& aTelServer )
        {
        CDialer* self = new( ELeave )CDialer( aTelServer );
        CleanupStack::PushL( self );
        self->ConstructL();
        return self;
        }
    
    ~CDialer() {
        if( IsActive() )
            {
            WaitForRequest();
            }
        delete iEmergency;
        delete iCmdHandler;
        delete iWait;
        iCall.Close();
        }
    
    void Dial( const TDesC& aNumber ) {
        iCall.Dial( iStatus, aNumber );
        SetActive();
        }
    
    void Dial() {
        Dial( KNumber() );
        }
    
    void DialEmergency() {
        iEmergency->DialEmergencyCallL( KEmergencyNumber );
        }
    
    void Hangup() {
        iCall.HangUp(iStatus);
        SetActive();
        }
    
    void HangupEmergencyCall()
        {
        iCmdHandler->Chup( iStatus );
        SetActive();
        }
    
    void WaitForRequest() {
        if ( IsActive() || iEmergency->IsActive() )
            {
            iWait->Start();
            }
        }
    
    void WaitL( TInt aTimeoutInSeconds ) {
        RTimer timer;
        User::LeaveIfError( timer.CreateLocal() ); 
        CleanupClosePushL( timer );        
        timer.After( iStatus, aTimeoutInSeconds * 1000000 );
        SetActive();
        WaitForRequest();
        CleanupStack::PopAndDestroy( 1, &timer ); 
        }
    
protected:
    
    void RunL() {
        if( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
    }
    
    void DoCancel() { 
         }
    
    void HandleEmergencyDialL( const TInt /*aStatus*/ ) {
        if ( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
    }
private:
    
    void ConstructL() { 
        TBuf<100> name;
        name.Append( KMmTsyPhoneName );
        name.Append( _L("::") );
        name.Append( KMmTsyVoice1LineName );
        name.Append( _L("::") );
        TBuf<100> callName;
        User::LeaveIfError( iCall.OpenNewCall( iTelServer, name, callName  ) );
        iEmergency = CPhCltEmergencyCall::NewL( this );
        iCmdHandler = CPhCltCommandHandler::NewL();
        iWait = new( ELeave )CActiveSchedulerWait(); 
        }
    
    CDialer( RTelServer& aTelServer ) : CActive( EPriorityStandard ), iTelServer( aTelServer ) 
        { CActiveScheduler::Add( this ); }
    
private:
    RTelServer iTelServer;
    RMobileCall iCall;
    CActiveSchedulerWait* iWait;
    CPhCltEmergencyCall* iEmergency;
    CPhCltCommandHandler* iCmdHandler;
    };

// - Construction -----------------------------------------------------------

MT_CallInformation* MT_CallInformation::NewL()
    {
    MT_CallInformation* self = MT_CallInformation::NewLC();
    CleanupStack::Pop();
    return self;
    }

MT_CallInformation* MT_CallInformation::NewLC()
    {
    MT_CallInformation* self = new( ELeave ) MT_CallInformation();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

MT_CallInformation::~MT_CallInformation()
    {
#ifdef __WINSCW__
    delete iPECreator;
#endif    
    }

MT_CallInformation::MT_CallInformation()
    {
    }

void MT_CallInformation::ConstructL()
    {
#ifdef __WINSCW__
    iPECreator = CPhoneEngineCreator::NewL();    
#endif    
    CEUnitTestSuiteClass::ConstructL();
    }


void MT_CallInformation::CallInformationChanged()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }


// - Test methods -----------------------------------------------------------
void MT_CallInformation::SetupL(  )
    {
    EUNIT_PRINT( _L( "MT_CallInformation::SetupL in" ));
#ifdef __WINSCW__
    iMsgWaiter = CPEMessageWaiter::NewL();
    iPhoneModel = iPECreator->CreatePhoneEngineL( *iMsgWaiter );
    
    RProperty::Define( KPSUidNetworkInfo,
                       KNWRegistrationStatus,
                       RProperty::EInt );
        
    User::LeaveIfError( RProperty::Set( KPSUidNetworkInfo,
                        KNWRegistrationStatus,
                        ENWStatusRegisteredOnHomeNetwork ) ); 
    iMsgWaiter->WaitL(1);
#endif    
    User::LeaveIfError( iTelServer.Connect() );
    iCallInfos = CCallInformation::NewL();
    iCallInfos->NotifyCallInformationChanges( *this );
    iDialer = CDialer::NewL( iTelServer );
    iWait = new( ELeave )CActiveSchedulerWait();
    EUNIT_PRINT( _L( "MT_CallInformation::SetupL out" ));
    }
    

void MT_CallInformation::Teardown(  )
    {
    EUNIT_PRINT( _L( "MT_CallInformation::Teardown in" ));
    delete iWait;
    delete iCallInfos;
    delete iDialer;
    iTelServer.Close();
#ifdef __WINSCW__    
    delete iPhoneModel;
    iPhoneModel = NULL;
    delete iMsgWaiter;
    iMsgWaiter = NULL;    
#endif   
    EUNIT_PRINT( _L( "MT_CallInformation::Teardown out" ));
    }
    

void MT_CallInformation::TestNoCallsL(  )
    {
    CCallInfoIter& infos( iCallInfos->GetCallsL() );
    EUNIT_ASSERT_EQUALS( infos.Count(), 0 );    
    }

void MT_CallInformation::TestActiveCallL()
    {
    iDialer->Dial();
    // Wait for dial to complete, timing might be different in HW and emulator
    iDialer->WaitForRequest();   
    
    CCallInformation* callInfos = CCallInformation::NewL();
    CleanupStack::PushL( callInfos );
    
    TBool dataCecked (EFalse);
    while ( !dataCecked )
        {
        CCallInfoIter& infos( callInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 1 );
        
        if ( callInfos->IsCallInState( CCPCall::EStateConnected ) == ETrue )
            {
            CCPCall::TCallState state = call.CallState();
            EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateConnected );
            dataCecked = ETrue;
            }
        else
            {
            iDialer->WaitL( 1 );            
            }
        }
    
    CleanupStack::PopAndDestroy( callInfos );
    iDialer->Hangup();
    iDialer->WaitForRequest();
    
    }

void MT_CallInformation::TestSingleCallL()
    {
    iDialer->Dial();
    EUNIT_PRINT( _L( "MT_CallInformation: Dial returns" ));
    
    EUNIT_PRINT( _L( "MT_CallInformation: wait for EStateDialling" ));
    iWait->Start(); // Wait for state notification.    
    
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateDialling );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 1 );
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), EFalse );
    }
    EUNIT_PRINT( _L( "MT_CallInformation: wait for EStateConnecting" ));
    iWait->Start();    
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateConnecting );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 1 );
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), EFalse );
    }
    EUNIT_PRINT( _L( "MT_CallInformation: wait for EStateConnected" ));
    iWait->Start();    
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateConnected );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 1 );
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), EFalse );
    }
    if( iDialer->IsActive() )
        {
        // Wait for dial to complete
        iDialer->WaitForRequest();
        EUNIT_PRINT( _L( "MT_CallInformation: Dial completes" ));
        }
    
    iDialer->Hangup();
    EUNIT_PRINT( _L( "MT_CallInformation: Hangup returns" ));

    EUNIT_PRINT( _L( "MT_CallInformation: wait for EStateDisconnecting" ));
    iWait->Start();    
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateDisconnecting );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 1 );
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), EFalse );
    }
    EUNIT_PRINT( _L( "MT_CallInformation: wait for Idle" ));
    iWait->Start();    
        {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 0 );
        }
    if ( iDialer->IsActive() )
        {
        iDialer->WaitForRequest();
        EUNIT_PRINT( _L( "MT_CallInformation: Hangup completes" ));
        }
    }

void MT_CallInformation::TestEmergencyCallL()
    {
    iDialer->DialEmergency();
    iWait->Start(); // Wait for state notification.
    
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateDialling );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 0 ); 
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), ETrue );
    }
    iWait->Start();
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateConnecting );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 0 ); 
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), ETrue );
    }
    iWait->Start();
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateConnected );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 0 ); 
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), ETrue );
    }
    if( iDialer->IsActive() )
        {
        // Wait for dial to complete
        iDialer->WaitForRequest();
        }
    iDialer->HangupEmergencyCall();

    iWait->Start();
    {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 1 );
        const MCall& call( infos.Current() );
        EUNIT_ASSERT_EQUALS( call.CallState(), CCPCall::EStateDisconnecting );
        EUNIT_ASSERT_EQUALS( call.CallType(), CCPCall::ECallTypeCSVoice );
        EUNIT_ASSERT_EQUALS( call.CallIndex(), 1 );
        EUNIT_ASSERT_EQUALS( call.ServiceId(), 0 ); 
        EUNIT_ASSERT_EQUALS( call.IsEmergency(), ETrue );
    }
    iWait->Start();
        {
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        EUNIT_ASSERT_EQUALS( infos.Count(), 0 );
        }
    if ( iDialer->IsActive() )
        {
        iDialer->WaitForRequest();
        }
    }

void MT_CallInformation::TestMultiCallL()
    {
    EUNIT_PRINT( _L( "MT_CallInformation: TestMultiCallL" ));
    iDialer->Dial();
    iDialer->WaitForRequest(); // Waits call to go connected state
    
    CDialer* otherDialer = CDialer::NewLC( iTelServer );
    otherDialer->Dial( KNumber );
    
    while ( iCallInfos->IsCallInState( CCPCall::EStateHold ) != ETrue &&
            iCallInfos->IsCallInState( CCPCall::EStateConnected ) != ETrue )
        {
        iWait->Start(); // Wait for state notifications
        CCallInfoIter& infos( iCallInfos->GetCallsL() );
        
        if( infos.Count() == 2 )
            {
            const MCall& call_1( infos.Current() );
            infos.Next();
            const MCall& call_2( infos.Current() );
         
            EUNIT_ASSERT_EQUALS( call_1.CallIndex(), 1 );
            EUNIT_ASSERT_EQUALS( call_2.CallIndex(), 2 );
            }
        }
    EUNIT_PRINT( _L( "MT_CallInformation: held and connected call exist" ));
    
    
    if ( otherDialer->IsActive() )
        {
        otherDialer->WaitForRequest();
        }
    iDialer->Hangup();
    iDialer->WaitForRequest();
    otherDialer->Hangup();
    otherDialer->WaitForRequest();
    CleanupStack::PopAndDestroy( otherDialer );
    }


void MT_CallInformation::TwoCallsExistL()
    {
    EUNIT_PRINT( _L( "MT_CallInformation: TwoCallsExistL" ));
    iDialer->Dial();
    iDialer->WaitForRequest(); // Waits call to go connected state
        
    CDialer* otherDialer = CDialer::NewLC( iTelServer );
    otherDialer->Dial( KNumber );  
    
    TBool TwoCallsExist = EFalse;
    while ( !TwoCallsExist )
        {
        iWait->Start(); // Wait for state notifications            
        CCallInfoIter& infos( iCallInfos->GetCallsL() );        
            
        TInt callIndex(1);                
        for( infos.First(); !infos.IsDone(); infos.Next() )
            {
            const MCall& call( infos.Current() );
            EUNIT_ASSERT_EQUALS( call.CallIndex(), callIndex );
            EUNIT_PRINT( _L( "MT_CallInformation: CallIndex()" ));
            callIndex++;
            if ( infos.Count() == 2 )
                {
                EUNIT_PRINT( _L( "MT_CallInformation: Count()" ));
                TwoCallsExist = ETrue;
                }
            }    
        }
    EUNIT_PRINT( _L( "MT_CallInformation: TwoCallsExist == ETrue" ));
        
    if ( otherDialer->IsActive() )
        {
        otherDialer->WaitForRequest();
        }
    iDialer->Hangup();
    iDialer->WaitForRequest();
    otherDialer->Hangup();
    otherDialer->WaitForRequest();
    CleanupStack::PopAndDestroy( otherDialer );    
    }

    

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    MT_CallInformation,
    "API tests for Call information api",
    "MODULE" )

EUNIT_TEST(
    "No calls",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestNoCallsL, Teardown)
    
EUNIT_TEST(
    "Constructing API when call available",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestActiveCallL, Teardown)
    
EUNIT_TEST(
    "Single call states",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestSingleCallL, Teardown)
    
/* testing emergency call should be done only in special lab    
EUNIT_TEST(
    "Emergency call states",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestEmergencyCallL, Teardown)
*/
    
EUNIT_TEST(
    "Multicall states",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TestMultiCallL, Teardown)
    
EUNIT_TEST(
    "Two calls exist",
    "CCallInformation",
    "GetCallsL",
    "FUNCTIONALITY",
    SetupL, TwoCallsExistL, Teardown)    

EUNIT_END_TEST_TABLE
