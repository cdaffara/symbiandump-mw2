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
* Description: Contains phonesettings test case definitions.
*
*/



// INCLUDE FILES
#include <e32math.h>
#include <s32file.h>
#include "PhoneSettingsTest.h"
#include "PhoneSettingsTestParser.h"
#include "PhoneSettingsTestCallBarring.h"
#include "PhoneSettingsTestCallDiverting.h"
#include "PhoneSettingsTestCallWaiting.h"
#include "PhoneSettingsTestCli.h"
#include "PhoneSettingsTestNetwork.h"
#include "PhoneSettingsTestCSP.h"
#include "PhoneSettingsTestSAObserver.h"
#include "PhoneSettingsTestSettingsContainer.h"
#include "PhoneSettingsTestRefreshHandler.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo CPhoneSettingsTest::Case ( 
    const TInt aCaseNumber) const 
     {
    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    PhoneSettingsTest.cpp file and to PhoneSettingsTest.h 
    *    header file.
    *
    * 2) Add entry to following KCases array either by using:
    *
    * 2.1: FUNCENTRY or ENTRY macro
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    *
    * FUNCENTRY macro takes only test case function name as a parameter and
    * uses that as a test case name and test case function name.
    *
    * Or
    *
    * 2.2: OOM_FUNCENTRY or OOM_ENTRY macro. Note that these macros are used
    * only with OOM (Out-Of-Memory) testing!
    *
    * OOM_ENTRY macro takes five parameters: test case name, test case 
    * function name, TBool which specifies is method supposed to be run using
    * OOM conditions, TInt value for first heap memory allocation failure and 
    * TInt value for last heap memory allocation failure.
    * 
    * OOM_FUNCENTRY macro takes test case function name as a parameter and uses
    * that as a test case name, TBool which specifies is method supposed to be
    * run using OOM conditions, TInt value for first heap memory allocation 
    * failure and TInt value for last heap memory allocation failure. 
    */ 

    static TCaseInfoInternal const KCases[] =
        {
        // Tests for CPsetCallBarring library functions                 
        ENTRY( "CPsetCallBarring::~CPsetCallBarring", TestCallBarringCPsetCallBarringConstructAndDestructL ), 
        ENTRY( "CPsetCallBarring::ChangePasswordL", TestCallBarringChangePasswordL ), 
        ENTRY( "CPsetCallBarring::SetBarringL", TestCallBarringSetBarringL ), 
        ENTRY( "CPsetCallBarring::GetBarringStatusL", TestCallBarringGetBarringStatusL ), 
        ENTRY( "CPsetCallBarring::SetRequestObserver", TestCallBarringSetRequestObserver ), 
        ENTRY( "CPsetCallBarring::CancelCurrentRequest", TestCallBarringCancelCurrentRequest ),
        // Tests for CPsetCallBarring public functions         
        ENTRY( "CPsetCallBarring::NewL", TestCallBarringCPsetCallBarringNewL ), 
        ENTRY( "CPsetCallBarring::PhoneSettingChanged", TestCallBarringPhoneSettingChanged ),         
        
        // Tests for CPsetCallDiverting library functions        
        ENTRY( "CPsetCallDiverting::NewL", TestCallDivertingCPsetCallDivertingConstructAndDestruct ),
        ENTRY( "CPsetCallDiverting::~CPsetCallDiverting", TestCallDivertingCPsetCallDivertingConstructAndDestruct ),        
        ENTRY( "CPsetCallDiverting::SetUsedDataNumberL", TestCallDivertingSetUsedDataNumberL ),        
        ENTRY( "CPsetCallDiverting::GetUsedDataNumberLC", TestCallDivertingGetUsedDataNumberLC ),
        ENTRY( "CPsetCallDiverting::SetUsedFaxNumberL", TestCallDivertingSetUsedFaxNumberL ),        
        ENTRY( "CPsetCallDiverting::GetUsedFaxNumberLC", TestCallDivertingGetUsedFaxNumberLC ),        
        ENTRY( "CPsetCallDiverting::SetTimerValueL", TestCallDivertingSetTimerValueL ),        
        ENTRY( "CPsetCallDiverting::GetTimerValueL", TestCallDivertingGetTimerValueL ),        	            
        ENTRY( "CPsetCallDiverting::SetNewDefaultNumberL", TestCallDivertingSetNewDefaultNumberL ),        	            	            
        ENTRY( "CPsetCallDiverting::GetDefaultNumbersL", TestCallDivertingGetDefaultNumbersL ),        	            
        ENTRY( "CPsetCallDiverting::CancelCurrentRequest", TestCallDivertingCancelCurrentRequest ), 	            
        ENTRY( "CPsetCallDiverting::SetDivertingL", TestCallDivertingSetDivertingL ), 	                    
        ENTRY( "CPsetCallDiverting::SetRequestObserver", TestCallDivertingSetRequestObserver ), 
        ENTRY( "CPsetCallDiverting::SwapDefaultNumberL", TestCallDivertingSwapDefaultNumberL ),                 
        // Tests for CPsetCallDiverting public functions
        ENTRY( "TCallDivertSetting::Initialize", TestCallDivertingTCallDivertSettingInitialize ), 
        
        // Tests for CPsetCallWaiting library functions        
        ENTRY( "CPsetCallWaiting::~CPsetCallWaiting", TestCallWaitingCPsetCallWaitingConstructAndDestructL ), 
        ENTRY( "CPsetCallWaiting::NewL", TestCallWaitingCPsetCallWaitingConstructAndDestructL ), 
        ENTRY( "CPsetCallWaiting::SetCallWaitingL", TestCallWaitingSetCallWaitingL ), 
        ENTRY( "CPsetCallWaiting::GetCallWaitingStatusL", TestCallWaitingGetCallWaitingStatusL ),         
        ENTRY( "CPsetCallWaiting::SetRequestObserver", TestCallWaitingSetRequestObserver ), 
        ENTRY( "CPsetCallWaiting::PhoneSettingChanged", TestCallWaitingPhoneSettingChanged ), 

        // Tests for CPsetCli library functions        
        ENTRY( "CPsetCli::~CPsetCli", TestCliCPsetCliConstructAndDestructL ), 
        ENTRY( "CPsetCli::NewL", TestCliCPsetCliConstructAndDestructL ),         
        ENTRY( "CPsetCli::GetClipModeL", TestCliGetClipModeL ), 
        ENTRY( "CPsetCli::GetClirModeL", TestCliGetClirModeL ), 
        ENTRY( "CPsetCli::GetCnapL", TestCliGetCnapL ), 
        ENTRY( "CPsetCli::GetColpModeL", TestCliGetColpModeL ), 
        ENTRY( "CPsetCli::GetColrModeL", TestCliGetColrModeL ), 
        ENTRY( "CPsetCli::SetRequestObserver", TestCliSetRequestObserver ), 

        // Tests for CPsetNetwork library functions        
        ENTRY( "CPsetNetwork::NewL - NetworkModeObserver", TestNetworkCPsetNetworkConstructAndDestructL ),         
        ENTRY( "CPsetNetwork::NewL - NetworkInfoObserver", TestNetworkCPsetNetworkConstructAndDestructL ),         
        ENTRY( "CPsetNetwork::~CPsetNetwork", TestNetworkCPsetNetworkConstructAndDestructL ),         
        ENTRY( "CPsetNetwork::GetCurrentNetworkInfo", TestNetworkGetCurrentNetworkInfo ),         
        ENTRY( "CPsetNetwork::GetNetworkSelectMode", TestNetworkGetNetworkSelectMode ),         
        ENTRY( "CPsetNetwork::IsCallActive", TestNetworkIsCallActive ),         
        ENTRY( "CPsetNetwork::IsGPRSConnected", TestNetworkIsGPRSConnected ),         
        ENTRY( "CPsetNetwork::ResetNetworkSearch", TestNetworkResetNetworkSearch ),         
        ENTRY( "CPsetNetwork::CancelProcess", TestNetworkCancelProcess ),         
        ENTRY( "CPsetNetwork::GetAvailableNetworksL", TestNetworkGetAvailableNetworksL ),         
        ENTRY( "CPsetNetwork::GetCurrentNetworkModeSelectionL", TestNetworkGetCurrentNetworkModeSelectionL ),         
        ENTRY( "CPsetNetwork::SelectNetworkL", TestNetworkSelectNetworkL ),         
        ENTRY( "CPsetNetwork::SetNetSAObserver", TestNetworkSetNetSAObserver ),         
        ENTRY( "CPsetNetwork::SetNetworkModeObserver", TestNetworkSetNetworkModeObserver ),         
        ENTRY( "CPsetNetwork::SetNetworkModeSelectionL", TestNetworkSetNetworkModeSelectionL ),         
        
        // Tests for CPsetCustomerServiceProfile library functions        
        ENTRY( "CPsetCustomerServiceProfile::NewL", 
            TestCSPCPsetCustomerServiceProfileConstructAndDestructL ), 
        ENTRY( "CPsetCustomerServiceProfile::~CPsetCustomerServiceProfile", 
            TestCSPCPsetCustomerServiceProfileConstructAndDestructL ),         
        ENTRY( "CPsetCustomerServiceProfile::IsALSSupported",TestCSPIsALSSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::IsCBSupported",TestCSPIsCBSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::IsCFSupported",TestCSPIsCFSupported ),         
        ENTRY( "CPsetCustomerServiceProfile:IsCWSupported",TestCSPIsCWSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::IsNetworkSelectionSupported",TestCSPIsNetworkSelectionSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::OpenCSProfileL",TestCSPOpenCSProfileL ),         
        ENTRY( "CPsetCustomerServiceProfile::CheckIfCWSupported",TestCSPCheckIfCWSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::CheckIfCBSupported",TestCSPCheckIfCBSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::CheckIfCFSupported",TestCSPCheckIfCFSupported),         
        ENTRY( "CPsetCustomerServiceProfile::CheckIfAlsSupported",TestCSPCheckIfAlsSupported ),         
        ENTRY( "CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported",TestCSPCheckIfNetworkSelectionSupported ),         
        
        // Tests for CPsetContainer library functions        
        ENTRY( "CPsetContainer::~CPsetContainer", TestSettingsContainerCPsetContainerConstructAndDestructL ), 
        ENTRY( "CPsetContainer::NewL", TestSettingsContainerCPsetContainerConstructAndDestructL ), 
        ENTRY( "CPsetContainer::CreateCBObjectL", TestSettingsContainerCreateCBObjectL ), 
        ENTRY( "CPsetContainer::CreateCFObjectL", TestSettingsContainerCreateCFObjectL ), 
        ENTRY( "CPsetContainer::CreateCWObjectL", TestSettingsContainerCreateCWObjectL ), 
        ENTRY( "CPsetContainer::CreateCliObjectL", TestSettingsContainerCreateCliObjectL ), 
        ENTRY( "CPsetContainer::CreateNetworkModeObjectL", TestSettingsContainerCreateNetworkModeObjectL ), 
        ENTRY( "CPsetContainer::CreateNetworkObjectL", TestSettingsContainerCreateNetworkObjectL ),         
     
        // Tests for CPsetSAObserver library functions             
        ENTRY( "CPsetSAObserver::NewL", TestSAOCPsetSAObserverConstructAndDestructL ), 
        ENTRY( "CPsetSAObserver::~CPsetSAObserver", TestSAOCPsetSAObserverConstructAndDestructL ), 
        ENTRY( "CPsetSAObserver::GetCurrentDivertStatus", TestSAOGetCurrentDivertStatus ), 
        ENTRY( "CPsetSAObserver::IsCallActive", TestSAOIsCallActive ), 
        ENTRY( "CPsetSAObserver::IsGPRSConnected", TestSAOIsGPRSConnected ), 
        ENTRY( "CPsetSAObserver::NotifyDivertChange", TestSAONotifyDivertChange ), 
        ENTRY( "CPsetSAObserver::SetNetObserver", TestSAOSetNetObserver ), 
        ENTRY( "CPsetSAObserver::Initialize", TestSAOInitialize ), 
     
        // Tests for CPSetRefreshHandler Pure Virtual functions (test derivation of class)  
        ENTRY( "CPSetRefreshHandler::NotifyFileChange (PureVirtual)", TestCPSetRefreshHandlerConstructAndDestructL ), 
        ENTRY( "CPSetRefreshHandler::CancelNotify (PureVirtual)", TestCPSetRefreshHandlerConstructAndDestructL ), 
        ENTRY( "CPSetRefreshHandler::ChangedCspSettings (PureVirtual)", TestCPSetRefreshHandlerConstructAndDestructL ),
        
        // Unstable test cases (either HW or emulator), run last to prevent test execution problems 
        ENTRY( "CPsetCallWaiting::CancelProcess", TestCallWaitingCancelProcess ), 
        ENTRY( "CPsetCli::CancelAll", TestCliCancelAll ), 
        ENTRY( "CPsetCallDiverting::GetDivertingStatusL", TestCallDivertingGetDivertingStatusL ), 	            	      
        ENTRY( "CPsetCallDiverting::VideoMailQueryL", TestCallDivertingVideoMailQueryL ), 
        ENTRY( "CPsetContainer::CreateRefreshHandlerL", TestSettingsContainerCreateRefreshHandlerL ), 
        ENTRY( "CPsetCallDiverting::VoiceMailQueryL", TestCallDivertingVoiceMailQueryL ),         
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        null.iIsOOMTest = EFalse;
        null.iFirstMemoryAllocation = 0;
        null.iLastMemoryAllocation = 0;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    tmp.iIsOOMTest = KCases[ aCaseNumber ].iIsOOMTest;
    tmp.iFirstMemoryAllocation = KCases[ aCaseNumber ].iFirstMemoryAllocation;
    tmp.iLastMemoryAllocation = KCases[ aCaseNumber ].iLastMemoryAllocation;
    
    return tmp;
    }

/*** Phone Settings Call Barring tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringCPsetCallBarringConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringCPsetCallBarringConstructAndDestructL(
    TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestCPsetCallBarringConstructAndDestructL( 
	    aResult ); 
	}
		
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringChangePasswordL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringChangePasswordL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestChangePasswordL( aResult );
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringSetBarringL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringSetBarringL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestSetBarringL( aResult );
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringGetBarringStatusL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringGetBarringStatusL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestGetBarringStatusL( aResult );
	}
		
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringSetRequestObserver	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringSetRequestObserver(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestSetRequestObserver( aResult );	
	}
		
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringCancelCurrentRequest	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringCancelCurrentRequest(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestCancelCurrentRequest( aResult );	
	}
		
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringCPsetCallBarringNewL	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringCPsetCallBarringNewL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestCPsetCallBarringNewL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallBarringPhoneSettingChanged	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallBarringPhoneSettingChanged(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallBarring->TestPhoneSettingChanged( aResult );	
	}

/*** Phone Settings Call Diverting tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingCPsetCallDivertingConstructAndDestruct	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingCPsetCallDivertingConstructAndDestruct(
    TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestCPsetCallDivertingConstructAndDestruct( 
	    aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingGetUsedDataNumberLC	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingGetUsedDataNumberLC(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestGetUsedDataNumberLC( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingGetUsedDataNumberLC	
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingGetUsedFaxNumberLC(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestGetUsedFaxNumberLC( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetUsedDataNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetUsedDataNumberL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestSetUsedDataNumberL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetUsedDataNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetUsedFaxNumberL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestSetUsedFaxNumberL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetTimerValueL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetTimerValueL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestSetTimerValueL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingGetTimerValueL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingGetTimerValueL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestGetTimerValueL( aResult );	
	}
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetNewDefaultNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetNewDefaultNumberL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestSetNewDefaultNumberL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingGetDefaultNumbersL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingGetDefaultNumbersL(TTestResult& aResult)
	{
	return iPhoneSettingsTestCallDiverting->TestGetDefaultNumbersL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingCancelCurrentRequest
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingCancelCurrentRequest(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestCancelCurrentRequest( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetDivertingL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetDivertingL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestSetDivertingL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingGetDivertingStatus
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingGetDivertingStatusL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestGetDivertingStatusL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSetRequestObserver(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestSetRequestObserver( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingSwapDefaultNumberL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingSwapDefaultNumberL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestSwapDefaultNumberL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingVoiceMailQueryL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingVoiceMailQueryL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestVoiceMailQueryL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingVideoMailQueryL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingVideoMailQueryL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestVideoMailQueryL( aResult );	
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallDivertingTCallDivertSettingInitialize
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallDivertingTCallDivertSettingInitialize(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallDiverting->TestTCallDivertSettingInitialize(
	    aResult );	
	}	

/*** Phone Settings Call Diverting tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingCPsetCallWaitingConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingCPsetCallWaitingConstructAndDestructL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestCPsetCallWaitingConstructAndDestructL(
	    aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingSetCallWaitingL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingSetCallWaitingL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestSetCallWaitingL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingGetCallWaitingStatusL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingGetCallWaitingStatusL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestGetCallWaitingStatusL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingCancelProcess
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingCancelProcess(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestCancelProcess( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingSetRequestObserver(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestSetRequestObserver( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCallWaitingPhoneSettingChanged
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCallWaitingPhoneSettingChanged(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCallWaiting->TestPhoneSettingChanged( aResult );	
	}	

/*** Phone Settings Cli tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliCPsetCliConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliCPsetCliConstructAndDestructL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestCPsetCliConstructAndDestructL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliCancelAll
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliCancelAll(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestCancelAll( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliGetClipModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliGetClipModeL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestGetClipModeL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliGetClirModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliGetClirModeL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestGetClirModeL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliGetCnapL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliGetCnapL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestGetCnapL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliGetColpModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliGetColpModeL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestGetColpModeL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliGetColrModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliGetColrModeL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestGetColrModeL( aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCliSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCliSetRequestObserver(TTestResult& aResult)    
	{
	return iPhoneSettingsTestCli->TestSetRequestObserver( aResult );	
	}	

/*** Phone Settings Container tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCPsetContainerConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCPsetContainerConstructAndDestructL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCPsetContainerConstructAndDestructL( 
	    aResult );	
	}	
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateCBObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateCBObjectL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateCBObjectL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateCFObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateCFObjectL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateCFObjectL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateCWObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateCWObjectL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateCWObjectL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateCliObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateCliObjectL(TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateCliObjectL( aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateNetworkModeObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateNetworkModeObjectL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateNetworkModeObjectL( 
	    aResult );	
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateNetworkObjectL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateNetworkObjectL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateNetworkObjectL( aResult );	
	}	
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSettingsContainerCreateRefreshHandlerL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSettingsContainerCreateRefreshHandlerL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestContainer->TestCreateRefreshHandlerL( aResult );	
	}	
	
/*** Phone Settings Network tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkCPsetNetworkConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkCPsetNetworkConstructAndDestructL(
    TTestResult& aResult)    
	{
	return iPhoneSettingsTestNetwork->TestCPsetNetworkConstructAndDestructL( 
	    aResult );	
	}	

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkGetCurrentNetworkInfo
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkGetCurrentNetworkInfo(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestGetCurrentNetworkInfo( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkGetNetworkSelectMode
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkGetNetworkSelectMode(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestGetNetworkSelectMode( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkIsCallActive
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkIsCallActive(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestIsCallActive( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkIsGPRSConnected
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkIsGPRSConnected(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestIsGPRSConnected( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkResetNetworkSearch
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkResetNetworkSearch(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestResetNetworkSearch( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkCancelProcess
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkCancelProcess(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestCancelProcess( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkGetAvailableNetworksL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkGetAvailableNetworksL(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestGetAvailableNetworksL( aResult );  
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkGetCurrentNetworkModeSelectionL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkGetCurrentNetworkModeSelectionL(
    TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestGetCurrentNetworkModeSelectionL( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkSelectNetworkL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkSelectNetworkL(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestSelectNetworkL( aResult );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkSetNetSAObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkSetNetSAObserver(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestSetNetSAObserver( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkSetNetworkModeObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkSetNetworkModeObserver(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestSetNetworkModeObserver( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestNetworkSetNetworkModeSelectionL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestNetworkSetNetworkModeSelectionL(TTestResult& aResult)
    {
    return iPhoneSettingsTestNetwork->TestSetNetworkModeSelectionL( aResult ); 
    }

/*** Phone Settings CSP tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCPsetCustomerServiceProfileConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCPsetCustomerServiceProfileConstructAndDestructL(
    TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCPsetCustomerServiceProfileConstructAndDestructL(
        aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPIsALSSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPIsALSSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestIsALSSupported( aResult );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPIsCBSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPIsCBSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestIsCBSupported( aResult );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPIsCFSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPIsCFSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestIsCFSupported( aResult );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPIsCWSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPIsCWSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestIsCWSupported( aResult );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPIsNetworkSelectionSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPIsNetworkSelectionSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestIsNetworkSelectionSupported( aResult );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPOpenCSProfileL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPOpenCSProfileL(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestOpenCSProfileL( aResult );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCheckIfCWSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCheckIfCWSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCheckIfCWSupported( aResult );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCheckIfCBSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCheckIfCBSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCheckIfCBSupported( aResult );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCheckIfCFSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCheckIfCFSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCheckIfCFSupported( aResult );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCheckIfAlsSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCheckIfAlsSupported(TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCheckIfAlsSupported( aResult );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCSPCheckIfNetworkSelectionSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCSPCheckIfNetworkSelectionSupported(
    TTestResult& aResult)
    {
    return iPhoneSettingsTestCSP->TestCheckIfNetworkSelectionSupported( aResult );    
    }

/*** Phone Settings SA Observer tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOCPsetSAObserverConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOCPsetSAObserverConstructAndDestructL(
    TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestCPsetSAObserverConstructAndDestructL(
        aResult ); 
    }
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOGetCurrentDivertStatus
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOGetCurrentDivertStatus(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestGetCurrentDivertStatus( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOIsCallActive
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOIsCallActive(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestIsCallActive( aResult ); 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOIsGPRSConnected
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOIsGPRSConnected(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestIsGPRSConnected( aResult );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAONotifyDivertChange
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAONotifyDivertChange(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestNotifyDivertChange( aResult );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOSetNetObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOSetNetObserver(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestSetNetObserver( aResult );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestSAOInitialize
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestSAOInitialize(TTestResult& aResult)
    {
    return iPhoneSettingsTestSAObserver->TestInitialize( aResult ); 
    }
	
/*** Phone Settings Refresh handler tests ***/ 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::TestCPSetRefreshHandlerConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::TestCPSetRefreshHandlerConstructAndDestructL(
    TTestResult& aResult)
    {
    // Test construction and destruction of object derived from 
    // CPSetRefreshHandlerConstruct 
    // (Pure Virtuals "tested" i.e. the interface checked) 
    iPhoneSettingsTestRefreshHandler = CPhoneSettingsTestRefreshHandler::NewL();
    delete iPhoneSettingsTestRefreshHandler;     
    iPhoneSettingsTestRefreshHandler = NULL; 

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCPSetRefreshHandlerConstructAndDestructL );			

    return KErrNone; 
    }	
	
	
//  End of File
