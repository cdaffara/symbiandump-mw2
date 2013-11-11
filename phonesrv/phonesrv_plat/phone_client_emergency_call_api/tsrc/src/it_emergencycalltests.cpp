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

#include "IT_EmergencyCallTests.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <cphcltemergencycall.h>

#include <etelmm.h>
#include <mmtsy_names.h>


class CEtel : public CActive
    {
public:
    
    static CEtel* NewL() { 
        CEtel* self = CEtel::NewLC();
        CleanupStack::Pop( self );
        return self;
    }
    
    static CEtel* NewLC()
        {
        CEtel* self = new( ELeave )CEtel( );
        CleanupStack::PushL( self );
        self->ConstructL();
        return self;
        }
    
    ~CEtel() {

        delete iNumber;
        delete iWait;
        iCall.Close();
        iLine.Close();
        iTelServer.Close();
        }   
    
    void WaitForEmergencyCallToConnectL()
        {
        iLine.NotifyStatusChange(iStatus, iLineStatus );
        SetActive();
        WaitForRequest();
        }
    
    const TDesC& GetNumberOfEmergencyCall() const
        {
        return *iNumber;
        }

    void Hangup() {
        iCall.HangUp(iStatus);
        User::WaitForRequest( iStatus );
        }
    
    void WaitForRequest() {
        if ( IsActive() )
            {
            iWait->Start();
            }
        }
    
protected:
    
    void RunL() {
        if( iLineStatus == RCall::EStatusConnected )
            {
            iWait->AsyncStop();
            RLine::TCallInfo callInfo;
            User::LeaveIfError( iLine.GetCallInfo(0, callInfo ) );
            User::LeaveIfError( iCall.OpenExistingCall( iLine, callInfo.iCallName ) );
            RMobileCall::TMobileCallInfoV1 info;
            RMobileCall::TMobileCallInfoV1Pckg pckg( info );            
            User::LeaveIfError( iCall.GetMobileCallInfo( pckg ) );
            
            iNumber = info.iDialledParty.iTelNumber.AllocL();
            }
        else
            {
            iLine.NotifyStatusChange(iStatus, iLineStatus );
            SetActive();
            }

    }
    
    void DoCancel() { 
         }
    
private:
    
    void ConstructL() { 
        User::LeaveIfError( iTelServer.Connect() );
        User::LeaveIfError( iTelServer.LoadPhoneModule(KMmTsyModuleName) );

        TBuf<100> name;
        name.Append( KMmTsyPhoneName );
        name.Append( _L("::") );
        name.Append( KMmTsyVoice1LineName );
        User::LeaveIfError( iLine.Open( iTelServer, name ) );
        iWait = new( ELeave )CActiveSchedulerWait(); 
        }
    
    CEtel() : CActive( EPriorityStandard ) 
        { CActiveScheduler::Add( this ); }
    
private:
    RTelServer iTelServer;
    RMobileLine iLine;
    RMobileCall iCall;
    CActiveSchedulerWait* iWait;
    RCall::TStatus iLineStatus;
    HBufC* iNumber;
    };

// - Construction -----------------------------------------------------------

IT_EmergencyCallTests* IT_EmergencyCallTests::NewL()
    {
    IT_EmergencyCallTests* self = IT_EmergencyCallTests::NewLC();
    CleanupStack::Pop();
    return self;
    }

IT_EmergencyCallTests* IT_EmergencyCallTests::NewLC()
    {
    IT_EmergencyCallTests* self = new( ELeave ) IT_EmergencyCallTests();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

IT_EmergencyCallTests::~IT_EmergencyCallTests()
    {
    }

IT_EmergencyCallTests::IT_EmergencyCallTests()
    {
    }

void IT_EmergencyCallTests::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

void IT_EmergencyCallTests::HandleEmergencyDialL( const TInt aStatus )
    {
    iReceivedDialStatus = aStatus;
    }

// - Test methods ----------------------------------------------------------

void IT_EmergencyCallTests::DoTestValidEmergencyNumberL( const TDesC& aNumber )
    {
    TBool isEmergency;
    EUNIT_PRINT( aNumber );
    EUNIT_ASSERT_EQUALS( iEmergency->IsEmergencyPhoneNumber( aNumber, isEmergency ), KErrNone );
    EUNIT_ASSERT( isEmergency );
    }

void IT_EmergencyCallTests::DoTestInvalidEmergencyNumberL( const TDesC& aNumber )
    {
    TBool isEmergency;
    EUNIT_PRINT( aNumber );
    EUNIT_ASSERT_EQUALS( iEmergency->IsEmergencyPhoneNumber( aNumber, isEmergency ), KErrNone );
    EUNIT_ASSERT( !isEmergency );
    }

void IT_EmergencyCallTests::DoTestFindValidEmergencyNumberL( const TDesC& aNumber, const TDesC& aExpectedFoundEmergencyNumber )
    {
    TBool isEmergency;
    HBufC* number = aNumber.AllocLC();
    TPtr numberPtr = number->Des();
    EUNIT_PRINT( aNumber );
    EUNIT_ASSERT_EQUALS( iEmergency->FindEmergencyPhoneNumber( numberPtr, isEmergency ), KErrNone );
    EUNIT_ASSERT( isEmergency );
    
    EUNIT_ASSERT( numberPtr.Match( aExpectedFoundEmergencyNumber ) == 0 );
    CleanupStack::PopAndDestroy( number );
    }

void IT_EmergencyCallTests::DoTestFindInvalidEmergencyNumberL( const TDesC& aNumber )
    {
    TBool isEmergency;
    HBufC* number = aNumber.AllocLC();
    TPtr numberPtr = number->Des();
    EUNIT_PRINT( aNumber );
    EUNIT_ASSERT_EQUALS( iEmergency->FindEmergencyPhoneNumber( numberPtr, isEmergency ), KErrNone );
    EUNIT_ASSERT( !isEmergency );
    
    CleanupStack::PopAndDestroy( number );
    }

void IT_EmergencyCallTests::SetupL(  )
    {
    iEmergency = CPhCltEmergencyCall::NewL( this );
    }

void IT_EmergencyCallTests::SetupWithEtelL()
    {
    iReceivedDialStatus = -1000; // Non default value 
    iEtel = CEtel::NewL();
    iEmergency = CPhCltEmergencyCall::NewL( this );
    }
    
void IT_EmergencyCallTests::Teardown(  )
    {
    delete iEmergency;
    delete iEtel; iEtel = NULL;
    }
        
void IT_EmergencyCallTests::TestConstructionL()
    {
    // SetupL and Teardown does the job.
    }

void IT_EmergencyCallTests::Test112L()
    {
    DoTestValidEmergencyNumberL( _L("112") );
    }

void IT_EmergencyCallTests::TestHash31Hash112L()
    {
    DoTestValidEmergencyNumberL( _L("#31#112") );
    }

void IT_EmergencyCallTests::TestStar31Hash112L()
    {
    DoTestValidEmergencyNumberL( _L("*31#112") );
    }

void IT_EmergencyCallTests::Test112WithDtmfL()
    {
    DoTestValidEmergencyNumberL( _L("112p234p1234#") );
    }

void IT_EmergencyCallTests::Test112WithDtmf_2L()
    {
    DoTestValidEmergencyNumberL( _L("112p454") );
    }

void IT_EmergencyCallTests::TestPlusSignBefore112L()
    {
    DoTestValidEmergencyNumberL( _L("+112") );
    }

void IT_EmergencyCallTests::TestCLIR112DTMFL()
    {
    DoTestValidEmergencyNumberL( _L("*31#112p123w123") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_1L()
    {
    DoTestInvalidEmergencyNumberL( _L("*132#112") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_2L()
    {
    DoTestInvalidEmergencyNumberL( _L("#112") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_3L()
    {
    DoTestInvalidEmergencyNumberL( _L("112#") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_4L()
    {
    DoTestInvalidEmergencyNumberL( _L("*34#112p52##5w256") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_5L()
    {
    DoTestInvalidEmergencyNumberL( _L("p112") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_6L()
    {
    DoTestInvalidEmergencyNumberL( _L("w112p") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_7L()
    {
    DoTestInvalidEmergencyNumberL( _L("p112w") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_8L()
    {
    DoTestInvalidEmergencyNumberL( _L("112+") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_9L()
    {
    DoTestInvalidEmergencyNumberL( _L("112+p112") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_10L()
    {
    DoTestInvalidEmergencyNumberL( _L("p") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_11L()
    {
    DoTestInvalidEmergencyNumberL( _L("w") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_12L()
    {
    DoTestInvalidEmergencyNumberL( _L("pw") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_13L()
    {
    DoTestInvalidEmergencyNumberL( _L("*#+112p+") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_14L()
    {
    DoTestInvalidEmergencyNumberL( _L("*#+112p+#") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_15L()
    {
    DoTestInvalidEmergencyNumberL( _L("05001234567#112") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_16L()
    {
    DoTestInvalidEmergencyNumberL( _L("*31#") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_17L()
    {
    DoTestInvalidEmergencyNumberL( _L("*31#p123w12") );
    }

void IT_EmergencyCallTests::TestInvalidEmergencyNumber_18L()
    {
    DoTestInvalidEmergencyNumberL( KNullDesC() );
    }

void IT_EmergencyCallTests::TestFindingEmergencyNumber_1L()
    {
    DoTestFindValidEmergencyNumberL(_L("112"), _L("112") );
    }


// The feature flag __EN_POLICY_DOS must not be defined when running this test in emulator.
// In emulator we have stub DOS
void IT_EmergencyCallTests::TestFindingEmergencyNumber_2L()
    {
    DoTestFindValidEmergencyNumberL(_L("555*#112"), _L("112") );
    }

// The feature flag __EN_POLICY_DOS must not be defined when running this test in emulator.
// In emulator we have stub DOS
void IT_EmergencyCallTests::TestFindingEmergencyNumber_3L()
    {
    DoTestFindInvalidEmergencyNumberL(_L("1120") );
    }

void IT_EmergencyCallTests::TestCallingEmergencyCallWithInvalidEmergencyNumberL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iEmergency->DialEmergencyCallL( _L("030444") ), KErrArgument );
    }

// The feature flag __EN_POLICY_DOS must not be defined when running this test in emulator.
// In emulator we have stub DOS
void IT_EmergencyCallTests::TestEmergencyCallL()
    {
    EUNIT_ASSERT_NO_LEAVE( iEmergency->DialEmergencyCallL( _L("*31#112p123w123") ) );
    
    iEtel->WaitForEmergencyCallToConnectL();
    EUNIT_ASSERT( iReceivedDialStatus == KErrNone );
    
    // Verify that the Etel has got the emergency number without any extra chars.
    EUNIT_ASSERT( iEtel->GetNumberOfEmergencyCall().Match( _L("112") ) == 0 );
    
    iEtel->Hangup();
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    IT_EmergencyCallTests,
    "API tests for Emergency call api",
    "MODULE" )

EUNIT_TEST(
    "Consruction",
    "CPhCltEmergencyCall",
    "NewL",
    "FUNCTIONALITY",
    SetupL, TestConstructionL, Teardown)
    
EUNIT_TEST(
    "Valid e number 112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, Test112L, Teardown)
    
EUNIT_TEST(
    "Valid em number  #31#112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestHash31Hash112L, Teardown)
    
EUNIT_TEST(
    "Valid em number *31#112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestStar31Hash112L, Teardown)
    
EUNIT_TEST(
    "Valid em number 112p234p1234#",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, Test112WithDtmfL, Teardown)
    
EUNIT_TEST(
    "Valid em number 112p454",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, Test112WithDtmf_2L, Teardown)
    
EUNIT_TEST(
    "Valid em number +112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestPlusSignBefore112L, Teardown)
    
EUNIT_TEST(
    "Valid em number *31#112p123w123",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestCLIR112DTMFL, Teardown)
    

    
EUNIT_TEST(
    "Invalid em number  *132#112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_1L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  #112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_2L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  112#",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_3L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  *34#112p52##5w256",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_4L, Teardown)

EUNIT_TEST(
    "Invalid em number  p112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_5L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  w112p",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_6L, Teardown)
    

EUNIT_TEST(
    "Invalid em number  p112w",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_7L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  112+",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_8L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  112+p112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_9L, Teardown)
    
EUNIT_TEST(
    "Invalid em number  p",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_10L, Teardown)   

EUNIT_TEST(
    "Invalid em number  w",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_11L, Teardown)
   
EUNIT_TEST(
    "Invalid em number  pw",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_12L, Teardown)
    
EUNIT_TEST(
    "Invalid em number *#+112p+",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_13L, Teardown)
    
EUNIT_TEST(
    "Invalid em number *#+112p+#",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_14L, Teardown)
    
EUNIT_TEST(
    "Invalid em number 05001234567#112",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_15L, Teardown)
    
EUNIT_TEST(
    "Invalid em number *31#",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_16L, Teardown)
    
EUNIT_TEST(
    "Invalid em number *31#p123w12",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_17L, Teardown)
    

EUNIT_TEST(
    "Invalid em number ",
    "CPhCltEmergencyCall",
    "IsEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestInvalidEmergencyNumber_18L, Teardown)
    
EUNIT_TEST(
    "Valid em number found from 112",
    "CPhCltEmergencyCall",
    "FindEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestFindingEmergencyNumber_1L, Teardown)
    
EUNIT_TEST(
    "Valid em number found from 555*#112",
    "CPhCltEmergencyCall",
    "FindEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestFindingEmergencyNumber_2L, Teardown)
    
EUNIT_TEST(
    "Invalid em number not found from 1120",
    "CPhCltEmergencyCall",
    "FindEmergencyPhoneNumber",
    "FUNCTIONALITY",
    SetupL, TestFindingEmergencyNumber_3L, Teardown)

EUNIT_TEST(
    "Dial with invalid em number",
    "CPhCltEmergencyCall",
    "DialEmergency",
    "FUNCTIONALITY",
    SetupWithEtelL, TestCallingEmergencyCallWithInvalidEmergencyNumberL, Teardown)
    
EUNIT_TEST(
    "Dial emergency call",
    "CPhCltEmergencyCall",
    "DialEmergency",
    "FUNCTIONALITY",
    SetupWithEtelL, TestEmergencyCallL, Teardown)
   


EUNIT_END_TEST_TABLE
