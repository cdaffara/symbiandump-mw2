/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneSettingsTestCallDiverting class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <PsetContainer.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <BADESCA.H>            
#include <PsetConstants.h>
#include <PsetCallDiverting.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestPsetRequestObserver.h" 
#include "PhoneSettingsTestCallDivertingObserver.h"
#include "PhoneSettingsTestCallDiverting.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::CPhoneSettingsTestCallDiverting
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallDiverting::CPhoneSettingsTestCallDiverting(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDiverting::ConstructL()
    {

    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallDiverting* CPhoneSettingsTestCallDiverting::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler)
    {
    CPhoneSettingsTestCallDiverting* self = new (ELeave) CPhoneSettingsTestCallDiverting(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallDiverting::~CPhoneSettingsTestCallDiverting()
    {          		
    delete iPsetCallDiverting;     
    delete iPhoneSettingsTestCallDivertingObserver;             
    delete iPsetContainer;
    delete iPhoneSettingsTestPsetRequestObserver; 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDiverting::WaitTimer(TInt aWaitTime)
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();
    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::DoCallBackL(TAny* aAny)
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestCallDiverting*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDiverting::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDiverting::SetupL()
    {
    iPsetContainer = CPsetContainer::NewL();        
    iPhoneSettingsTestCallDivertingObserver = 
        CPhoneSettingsTestCallDivertingObserver::NewL();
	iPsetCallDiverting = iPsetContainer->CreateCFObjectL(
	    *iPhoneSettingsTestCallDivertingObserver );
	iPhoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();    
    iPsetCallDiverting->SetRequestObserver( 
        iPhoneSettingsTestPsetRequestObserver );	    
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDiverting::TearDownL()
    {
    delete iPsetCallDiverting;     
    iPsetCallDiverting = NULL; 
    delete iPhoneSettingsTestCallDivertingObserver;             
    iPhoneSettingsTestCallDivertingObserver = NULL; 
    delete iPsetContainer;
    iPsetContainer = NULL; 
    delete iPhoneSettingsTestPsetRequestObserver; 
    iPhoneSettingsTestPsetRequestObserver = NULL; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestCPsetCallDivertingConstructAndDestruct
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestCPsetCallDivertingConstructAndDestruct(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingConstructAndDestruct() );
	
	// Initialize test 
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    
	
	// Do tests here 
	CPsetCallDiverting* psetCallDiverting;	
	psetCallDiverting = CPsetCallDiverting::NewL( 
	    *iPhoneSettingsTestCallDivertingObserver, phone );
	ASSERT( psetCallDiverting != NULL ); 
		
    // Uninitialize test     
    delete psetCallDiverting;      
    CPhoneSettingsTestUtilities::ClosePhoneL( telServer, phone, line ); 	    
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingConstructAndDestruct );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestGetUsedDataNumberLC
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestGetUsedDataNumberLC(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingGetUsedDataNumberLC() );
	
	// Initialize test 
	SetupL();
	
	// Do tests here 
	HBufC* usedDataNumber = iPsetCallDiverting->GetUsedDataNumberLC(); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KUsedDataNumber() );
	iTestMessageHandler.PrintMessage( usedDataNumber->Des() ); 
	
    // Uninitialize test     
    TearDownL();
    CleanupStack::PopAndDestroy( usedDataNumber );     
            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingGetUsedDataNumberLC );			
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestGetUsedFaxNumberLC
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestGetUsedFaxNumberLC(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingGetUsedFaxNumberLC() );
	
	// Initialize test 
	SetupL();
	
	// Do tests here 
	HBufC* usedFaxNumber = iPsetCallDiverting->GetUsedFaxNumberLC(); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KUsedFaxNumber() );
	iTestMessageHandler.PrintMessage( usedFaxNumber->Des() ); 
	
    // Uninitialize test     
    TearDownL(); 
    CleanupStack::PopAndDestroy( usedFaxNumber );     
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingGetUsedFaxNumberLC );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetUsedDataNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetUsedDataNumberL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetUsedDataNumberL() );
	
	// Initialize test 	
	SetupL();
	
	TTelNumber telNumber( KTestDataNumber );

	// Do tests here 
	iPsetCallDiverting->SetUsedDataNumberL(telNumber); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KUsedDataNumber() );
	iTestMessageHandler.PrintMessage( telNumber ); 
	
    // Uninitialize test     
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetUsedDataNumberL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetUsedFaxNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetUsedFaxNumberL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetUsedFaxNumberL() );
	
	// Initialize test 	
	SetupL(); 
	
	TTelNumber telNumber( KTestFaxNumber );

	// Do tests here 
	iPsetCallDiverting->SetUsedFaxNumberL(telNumber); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KUsedFaxNumber() );
	iTestMessageHandler.PrintMessage( telNumber ); 
	
    // Uninitialize test     
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetUsedFaxNumberL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetTimerValueL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetTimerValueL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetTimerValueL() );
	
	// Initialize test 	
	SetupL(); 
		
	TInt timerValue = 1000; 
    
	// Do tests here 
	iPsetCallDiverting->SetTimerValueL(timerValue); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KTimerValue() );
    TBuf<KPhoneSettingsTestMaxTimerValueLength> timerValueBuf;
    timerValueBuf.Num( timerValue );
	iTestMessageHandler.PrintMessage( timerValueBuf ); 
	
    // Uninitialize test     
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetTimerValueL );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestGetTimerValueL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestGetTimerValueL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingGetTimerValueL() );
	
	// Initialize test 		
    SetupL(); 
    
	// Do tests here 
	TInt timerValue = iPsetCallDiverting->GetTimerValueL(); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KTimerValue() );
	TBuf<KPhoneSettingsTestMaxTimerValueLength> timerValueBuf;
	timerValueBuf.Num( timerValue );
	iTestMessageHandler.PrintMessage( timerValueBuf ); 
	
    // Uninitialize test     
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingGetTimerValueL );		
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetNewDefaultNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetNewDefaultNumberL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetNewDefaultNumberL() );
	
	// Initialize test 		
	SetupL(); 
	
	TBuf<KPhoneSettingsTestMaxPhoneNumberLength> defaultNumber; 
	defaultNumber.Copy( KTestDefaultNumber );
    
	// Do tests here 
	iPsetCallDiverting->SetNewDefaultNumberL( defaultNumber ); 
	
	// Print result data
	iTestMessageHandler.PrintMessage( KNewDefaultNumber() ); 	
	iTestMessageHandler.PrintMessage( defaultNumber ); 	
	
    // Uninitialize test     
    TearDownL();  
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetNewDefaultNumberL );		
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestGetDefaultNumbersL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestGetDefaultNumbersL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingGetDefaultNumbersL() );
	
	// Initialize test 	
	SetupL(); 
		
    CDesC16ArrayFlat* descArrayFlat16;
    descArrayFlat16 = new(ELeave) CDesC16ArrayFlat(KPhoneSettingsTestMaxNumbers);        
    CleanupStack::PushL( descArrayFlat16 );
    
	// Do tests here 
	TInt resultValue = iPsetCallDiverting->GetDefaultNumbersL( *descArrayFlat16 ); 
	
	// Print result data
	TBuf<KPhoneSettingsTestMaxPhoneNumberLength> defaultNumber; 
	iTestMessageHandler.PrintMessage( KDefaultNumbers() ); 

	if( resultValue >= 0 )
	    {
	    for( TInt i=0; i<resultValue; i++ )
	        {
	        defaultNumber = (*descArrayFlat16)[i];    
	        iTestMessageHandler.PrintMessage( defaultNumber ); 
	        }
	    }
	else
	    {
	    TBuf<KMessageHandlerMaxMessageSize> errorMessage; 
	    errorMessage.Num(resultValue);

	    iTestMessageHandler.PrintMessage( KErrorDetected() ); 
	    iTestMessageHandler.PrintMessage( KErrorCode() ); 
	    iTestMessageHandler.PrintMessage( errorMessage ); 
	    }
	
    // Uninitialize test  
    TearDownL();            
    CleanupStack::PopAndDestroy( descArrayFlat16 );
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingGetDefaultNumbersL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}		

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestCancelCurrentRequest
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestCancelCurrentRequest(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingCancelCurrentRequest() );

    // Initialize test 
    SetupL(); 
    
    TServiceGroup serviceGroup = EServiceGroupVoice; 
    TCallDivertingCondition callDivertingCondition = EDivertConditionAllCalls; 
    TBasicServiceGroups basicServiceGroups = EAllTele;      

	// Do tests here 
	iPsetCallDiverting->GetDivertingStatusL( 
	    serviceGroup, callDivertingCondition, basicServiceGroups );	    
	    
	TInt err = iPsetCallDiverting->CancelCurrentRequest();		
	
	if( KErrNone != err )
	    {
	    TBuf<KMessageHandlerMaxMessageSize> errorMessage; 
	    errorMessage.Num(err); 
	    iTestMessageHandler.PrintMessage( KErrorDetected() );
	    iTestMessageHandler.PrintMessage( KErrorCode() );   	     
	    iTestMessageHandler.PrintMessage( errorMessage ); 	     
	    }
	
	// Uninitialize test 
    TearDownL(); 
    	
	// Set result for STIF framework
	aResult.SetResult( err, KCallDivertingCancelCurrentRequest );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetDivertingL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetDivertingL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetDivertingL() );
	
	// Initialize test
	SetupL(); 
	
	TCallDivertSetting divertSetting; 
	divertSetting.Initialize(); 
    divertSetting.iSetting = EActivateDiverting; 

	TBasicServiceGroups basicServiceGroups = EAllTele;  
	TBool voiceMailbox = EFalse; 
	
	// Do tests here 
	iPsetCallDiverting->SetDivertingL( 
	    divertSetting, basicServiceGroups, voiceMailbox );	
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    

    // Uninitialize test 
    TearDownL(); 
    
	// Parse result data
	iTestParser.ParseTCallDivertSetting( divertSetting );            	
	iTestParser.ParseTBasicServiceGroups( basicServiceGroups );      
	iTestParser.ParseTBool( voiceMailbox ); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetDivertingL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestGetDivertingStatusL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestGetDivertingStatusL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingGetDivertingStatusL() );
    
    // Initialize test
    SetupL(); 
    
    TServiceGroup serviceGroup = EServiceGroupVoice; 
    TCallDivertingCondition callDivertingCondition = EDivertConditionUnconditional; 
    TBasicServiceGroups basicServiceGroups = ETelephony;  

	// Do tests here 
	iPsetCallDiverting->GetDivertingStatusL( serviceGroup, 
	    callDivertingCondition, basicServiceGroups );
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
            
	// Parse result data	
    iTestParser.ParseTServiceGroup( serviceGroup ); 
	iTestParser.ParseTCallDivertingCondition( callDivertingCondition ); 
	iTestParser.ParseTBasicServiceGroups( basicServiceGroups );      
	     
    // Uninitialize test 
    TearDownL(); 
	            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingGetDivertingStatusL );			
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSetRequestObserver(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSetRequestObserver() );
	
	// Initialize test
    CPsetContainer* psetContainer = CPsetContainer::NewL();        
	CleanupStack::PushL( psetContainer );
	    
    CPhoneSettingsTestCallDivertingObserver* phoneSettingsTestCallDivertingObserver = 
        CPhoneSettingsTestCallDivertingObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestCallDivertingObserver );
        
	CPsetCallDiverting* psetCallDiverting = psetContainer->CreateCFObjectL(
	    *phoneSettingsTestCallDivertingObserver );
	CleanupStack::PushL( psetCallDiverting );
	    	
	CPhoneSettingsTestPsetRequestObserver* phoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestPsetRequestObserver );
	
    // Do tests here 	
    psetCallDiverting->SetRequestObserver( 
        phoneSettingsTestPsetRequestObserver );
    
    // Uninitialize test 
    CleanupStack::PopAndDestroy( 4 );  
                
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSetRequestObserver );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestSwapDefaultNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestSwapDefaultNumberL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingSwapDefaultNumberL() );
	
	// Initialize test
	SetupL(); 
	
	TInt usedNumberLocation = 2; 

    // Do tests here 	    
    iPsetCallDiverting->SwapDefaultNumberL( usedNumberLocation ); 
    
    // Uninitialize test 
    TearDownL(); 
	            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingSwapDefaultNumberL );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
		    	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestVoiceMailQueryL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestVoiceMailQueryL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingVoiceMailQueryL() );
	
	// Initialize test
	SetupL(); 
	
    TBuf<KPhoneSettingsTestMaxVmbxNumberLength> voiceMailNumber;  

    // Do tests here 	    
    iPsetCallDiverting->VoiceMailQueryL(voiceMailNumber); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
    
    // Print result data 
    iTestMessageHandler.PrintMessage( KVoiceMailNumber() );
    iTestMessageHandler.PrintMessage( voiceMailNumber ); 
    
    // Uninitialize test 
    TearDownL(); 
	            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingVoiceMailQueryL );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
		    			    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestVideoMailQueryL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestVideoMailQueryL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingVideoMailQueryL() );
	
	// Initialize test
	SetupL(); 
	
    TBuf<KPhoneSettingsTestMaxVmbxNumberLength> videoMailNumber;  

    // Do tests here 	    
    iPsetCallDiverting->VideoMailQueryL(videoMailNumber); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
    
    // Print result data 
    iTestMessageHandler.PrintMessage( KVideoMailNumber() );
    iTestMessageHandler.PrintMessage( videoMailNumber ); 
    
    // Uninitialize test 
    TearDownL(); 
	            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingVideoMailQueryL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDiverting::TestTCallDivertSettingInitialize
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallDiverting::TestTCallDivertSettingInitialize(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallDivertingTCallDivertSettingInitialize() );
	
	// Initialize test 
    SetupL(); 

	TCallDivertSetting divertSetting; 
	
    // Do tests here 
	divertSetting.Initialize(); 
        
    // Uninitialize test 
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallDivertingTCallDivertSettingInitialize );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	
				
//  End of File
