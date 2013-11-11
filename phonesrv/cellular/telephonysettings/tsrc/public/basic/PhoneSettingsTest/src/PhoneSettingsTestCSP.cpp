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
* Description: Implementation of CPhoneSettingsTestCSP class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <PsetConstants.h>
#include <psetcsp.h>
#include "PhoneSettingsTest.hrh"
#include "PhoneSettingsTest.h"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestCSP.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::CPhoneSettingsTestCSP
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCSP::CPhoneSettingsTestCSP(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCSP::ConstructL()
    {    	
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCSP* CPhoneSettingsTestCSP::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler )
    {
    CPhoneSettingsTestCSP* self = new (ELeave) CPhoneSettingsTestCSP(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCSP::~CPhoneSettingsTestCSP()
    {          		
    delete iPsetCustomerServiceProfile;     
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCSP::WaitTimer(TInt aWaitTime)
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();
    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::DoCallBackL(TAny* aAny)
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestCSP*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCSP::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCSP::SetupL()
    {
	iPsetCustomerServiceProfile = CPsetCustomerServiceProfile::NewL();
    iPsetCustomerServiceProfile->OpenCSProfileL(); 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCSP::TearDownL()
    {
    delete iPsetCustomerServiceProfile;
    iPsetCustomerServiceProfile = NULL; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCPsetCustomerServiceProfileConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCPsetCustomerServiceProfileConstructAndDestructL(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPConstructAndDestruct() );
	
	// Do tests here 
	CPsetCustomerServiceProfile * psetCustomerServiceProfile = 
	    CPsetCustomerServiceProfile::NewL();
	CleanupStack::PushL( psetCustomerServiceProfile ); 
	
	ASSERT( psetCustomerServiceProfile != NULL ); 
		
    // Uninitialize test             
    CleanupStack::PopAndDestroy( psetCustomerServiceProfile  ); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPConstructAndDestruct );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestIsALSSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestIsALSSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPIsALSSupported() );
	
	// Initialize test 	
	SetupL(); 
    
    TBool alsSupport; 
    	
	// Do tests here 
    iPsetCustomerServiceProfile->IsALSSupported( alsSupport ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   
    
    // Parse result data     
    iTestParser.ParseTBool( alsSupport ); 

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPIsALSSupported );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestIsCBSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestIsCBSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPIsCBSupported() );
	
	// Initialize test 	
	SetupL(); 

    TBool cbsSupport; 
    	
	// Do tests here 
    iPsetCustomerServiceProfile->IsCBSupported( cbsSupport ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   
        
    // Parse result data     
    iTestParser.ParseTBool( cbsSupport ); 
    
    // Uninitialize test             
	TearDownL(); 
            
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPIsCBSupported );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestIsCFSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestIsCFSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPIsCFSupported() );
	
	// Initialize test 	
	SetupL(); 

    TBool cfSupport; 
    	
	// Do tests here 
    iPsetCustomerServiceProfile->IsCFSupported( cfSupport ); 

    // Parse result data     
    iTestParser.ParseTBool( cfSupport );
    
    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPIsCFSupported );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestIsCWSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestIsCWSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPIsCWSupported() );
	
	// Initialize test 	
	SetupL(); 

    TBool cwSupport; 
    	
	// Do tests here 
    iPsetCustomerServiceProfile->IsCWSupported( cwSupport );  
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   
    
    // Parse result data     
    iTestParser.ParseTBool( cwSupport );

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPIsCWSupported );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestIsNetworkSelectionSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestIsNetworkSelectionSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPIsNetworkSelectionSupported() );
	
	// Initialize test 	
	SetupL(); 

    TBool networkSelectionSupport; 

	// Do tests here 
    iPsetCustomerServiceProfile->IsNetworkSelectionSupported( networkSelectionSupport ); 

    // Parse result data     
    iTestParser.ParseTBool( networkSelectionSupport );

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPIsNetworkSelectionSupported );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestOpenCSProfileL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestOpenCSProfileL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPOpenCSProfileL() );
	
	// Initialize test 	
	SetupL(); 
    	
	// Do tests here 
    TInt err = iPsetCustomerServiceProfile->OpenCSProfileL(); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( err, KCSPOpenCSProfileL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCheckIfCWSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCheckIfCWSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPCheckIfCWSupported() );
	
#ifndef PHONESETTINGSTEST_LIBRARY_ONLY
	
	// Initialize test 	
	SetupL(); 

    RMobilePhone::TCspCallCompletion cspCallCompletion; 
    TBool isCspCWSupported; 	
    TBool isCspCCBSSupported; 	
    TBool isCspHoldSupported; 	
    	
	// Do tests here 
	cspCallCompletion = RMobilePhone::KCspCCBS; 	 
    isCspCCBSSupported = iPsetCustomerServiceProfile->CheckIfCWSupported( cspCallCompletion ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   
	    
	cspCallCompletion = RMobilePhone::KCspCW; 	 
    isCspCWSupported = iPsetCustomerServiceProfile->CheckIfCWSupported( cspCallCompletion ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );  
    
	cspCallCompletion = RMobilePhone::KCspHOLD; 	 
    isCspHoldSupported = iPsetCustomerServiceProfile->CheckIfCWSupported( cspCallCompletion ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   
    
    // Parse result data 
    iTestMessageHandler.PrintMessage( KKCspCCBS() );
    iTestParser.ParseTBool( isCspCCBSSupported );     
    iTestMessageHandler.PrintMessage( KKCspCW() );
    iTestParser.ParseTBool( isCspCWSupported );     
    iTestMessageHandler.PrintMessage( KKCspHOLD() );
    iTestParser.ParseTBool( isCspHoldSupported ); 
    
    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPCheckIfCWSupported );	

#else // PHONESETTINGSTEST_LIBRARY_ONLY

	aResult.SetResult( KErrNotSupported, KCSPCheckIfCWSupported );	

#endif // PHONESETTINGSTEST_LIBRARY_ONLY	

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCheckIfCBSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCheckIfCBSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPCheckIfCBSupported() );

#ifndef PHONESETTINGSTEST_LIBRARY_ONLY
	
	// Initialize test 	
	SetupL(); 
	
    RMobilePhone::TCspCallRestriction callRestriction; 
    TBool isSupportedBICRoam; 
    TBool isSupportedBAIC; 
    TBool isSupportedBOICexHC; 
    TBool isSupportedBOIC; 
    TBool isSupportedBOAC; 
    	
	// Do tests here 
	callRestriction = RMobilePhone::KCspBICRoam; 
    isSupportedBICRoam = iPsetCustomerServiceProfile->CheckIfCBSupported( callRestriction ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callRestriction = RMobilePhone::KCspBAIC; 
    isSupportedBAIC = iPsetCustomerServiceProfile->CheckIfCBSupported( callRestriction ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callRestriction = RMobilePhone::KCspBOICexHC; 
    isSupportedBOICexHC = iPsetCustomerServiceProfile->CheckIfCBSupported( callRestriction ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callRestriction = RMobilePhone::KCspBOIC; 
    isSupportedBOIC = iPsetCustomerServiceProfile->CheckIfCBSupported( callRestriction ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callRestriction = RMobilePhone::KCspBOAC; 
    isSupportedBOAC = iPsetCustomerServiceProfile->CheckIfCBSupported( callRestriction ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

    // Parse result data
    iTestMessageHandler.PrintMessage( KKCspBICRoam() );
    iTestParser.ParseTBool( isSupportedBICRoam ); 
    iTestMessageHandler.PrintMessage( KKCspBAIC() );
    iTestParser.ParseTBool( isSupportedBAIC ); 
    iTestMessageHandler.PrintMessage( KKCspBOICexHC() );
    iTestParser.ParseTBool( isSupportedBOICexHC ); 
    iTestMessageHandler.PrintMessage( KKCspBOIC() );
    iTestParser.ParseTBool( isSupportedBOIC ); 
    iTestMessageHandler.PrintMessage( KKCspBOAC() );
    iTestParser.ParseTBool( isSupportedBOAC ); 

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPCheckIfCBSupported );		    
	
#else // PHONESETTINGSTEST_LIBRARY_ONLY	

	aResult.SetResult( KErrNotSupported, KCSPCheckIfCBSupported );		    

#endif // PHONESETTINGSTEST_LIBRARY_ONLY

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }


// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCheckIfCFSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCheckIfCFSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPCheckIfCFSupported() );

#ifndef PHONESETTINGSTEST_LIBRARY_ONLY
	
	// Initialize test 	
	SetupL(); 

    RMobilePhone::TCspCallOffering callOffering; 
    TBool isSupportedCFU; 
    TBool isSupportedCFB; 
    TBool isSupportedCFNRc; 
    TBool isSupportedCFNRy; 
        	
	// Do tests here 
	callOffering = RMobilePhone::KCspCFU; 
    isSupportedCFU = iPsetCustomerServiceProfile->CheckIfCFSupported( callOffering ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callOffering = RMobilePhone::KCspCFB; 
    isSupportedCFB = iPsetCustomerServiceProfile->CheckIfCFSupported( callOffering ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callOffering = RMobilePhone::KCspCFNRc; 
    isSupportedCFNRc = iPsetCustomerServiceProfile->CheckIfCFSupported( callOffering ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

	callOffering = RMobilePhone::KCspCFNRy; 
    isSupportedCFNRy = iPsetCustomerServiceProfile->CheckIfCFSupported( callOffering ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

    // Parse result data
    iTestMessageHandler.PrintMessage( KKCspCFU() );
    iTestParser.ParseTBool( isSupportedCFU ); 
    iTestMessageHandler.PrintMessage( KKCspCFB() );
    iTestParser.ParseTBool( isSupportedCFB ); 
    iTestMessageHandler.PrintMessage( KKCspCFNRc() );
    iTestParser.ParseTBool( isSupportedCFNRc ); 
    iTestMessageHandler.PrintMessage( KKCspCFNRy() );
    iTestParser.ParseTBool( isSupportedCFNRy ); 

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPCheckIfCFSupported );		    

#else // PHONESETTINGSTEST_LIBRARY_ONLY

    aResult.SetResult( KErrNotSupported, KCSPCheckIfCFSupported );		    

#endif // PHONESETTINGSTEST_LIBRARY_ONLY

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCheckIfAlsSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCheckIfAlsSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPCheckIfAlsSupported() );
	
#ifndef PHONESETTINGSTEST_LIBRARY_ONLY	

	// Initialize test 	
	SetupL(); 

    RMobilePhone::TCspCPHSTeleservices cPHSTeleservices = RMobilePhone::KCspALS; 
    TBool alsSupported; 	
    	
	// Do tests here 
    alsSupported = iPsetCustomerServiceProfile->CheckIfAlsSupported( cPHSTeleservices ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

    // Parse result data
    iTestMessageHandler.PrintMessage( KKCspALS() );
    iTestParser.ParseTBool( cPHSTeleservices ); 

    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPCheckIfAlsSupported );	

#else // PHONESETTINGSTEST_LIBRARY_ONLY

    aResult.SetResult( KErrNotSupported, KCSPCheckIfAlsSupported );	
    
#endif // PHONESETTINGSTEST_LIBRARY_ONLY

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCSP::TestCheckIfNetworkSelectionSupported
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCSP::TestCheckIfNetworkSelectionSupported(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCSPCheckIfNetworkSelectionSupported() );
	
#ifndef PHONESETTINGSTEST_LIBRARY_ONLY
	
	// Initialize test 	
	SetupL(); 

    RMobilePhone::TCspValueAdded valueAdded = RMobilePhone::KCspPLMNMode; 
    TBool pLMNModeSupported; 
    	
	// Do tests here 
    pLMNModeSupported = iPsetCustomerServiceProfile->CheckIfNetworkSelectionSupported(
        valueAdded ); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );   

    // Parse result data
    iTestMessageHandler.PrintMessage( KKCspPLMNMode() );
    iTestParser.ParseTBool( pLMNModeSupported ); 
    
    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCSPCheckIfNetworkSelectionSupported );		    

#else // PHONESETTINGSTEST_LIBRARY_ONLY

    aResult.SetResult( KErrNotSupported, KCSPCheckIfNetworkSelectionSupported );		    

#endif // PHONESETTINGSTEST_LIBRARY_ONLY

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

		    					
//  End of File
