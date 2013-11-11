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
* Description: Implementation of CPhoneSettingsTestCli class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <BADESCA.H>            
#include <PsetContainer.h>
#include <PsetConstants.h>
#include <PsetCli.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestPsetRequestObserver.h" 
#include "PhoneSettingsTestCliObserver.h" 
#include "PhoneSettingsTestCli.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::CPhoneSettingsTestCli
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCli::CPhoneSettingsTestCli(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCli::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCli* CPhoneSettingsTestCli::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler )
    {
    CPhoneSettingsTestCli* self = new (ELeave) CPhoneSettingsTestCli(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCli::~CPhoneSettingsTestCli()
    {          		
    delete iPsetContainer; 
    delete iPsetCli;         
    delete iPhoneSettingsTestCliObserver; 
    delete iPhoneSettingsTestPsetRequestObserver; 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCli::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCli::WaitTimer( TInt aWaitTime )
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();
    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCli::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::DoCallBackL( TAny* aAny )
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestCli*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCli::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCli::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCli::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCli::SetupL()
    {
 	iPhoneSettingsTestCliObserver = CPhoneSettingsTestCliObserver::NewL(); 
	iPsetContainer = CPsetContainer::NewL();
	iPsetCli = iPsetContainer->CreateCliObjectL( *iPhoneSettingsTestCliObserver );	
	iPhoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	iPsetCli->SetRequestObserver( iPhoneSettingsTestPsetRequestObserver );       
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCli::TearDownL()
    {    
    delete iPsetCli; 
    iPsetCli = NULL; 
    delete iPsetContainer;
    iPsetContainer = NULL; 
    delete iPhoneSettingsTestCliObserver; 
    iPhoneSettingsTestCliObserver = NULL;     
    delete iPhoneSettingsTestPsetRequestObserver; 
    iPhoneSettingsTestPsetRequestObserver = NULL; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestCPsetCliConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestCPsetCliConstructAndDestructL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliConstructAndDestruct() );
	
	// Initialize test 
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    
	
	CPhoneSettingsTestCliObserver* phoneSettingsTestCliObserver; 
	phoneSettingsTestCliObserver = CPhoneSettingsTestCliObserver::NewL(); 
	CleanupStack::PushL( phoneSettingsTestCliObserver ); 
	
	// Do tests here 
	CPsetCli* psetCli;	
	psetCli= CPsetCli::NewL( phone, *phoneSettingsTestCliObserver );
	CleanupStack::PushL( psetCli ); 
	
	ASSERT( psetCli != NULL ); 
		
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); // psetCli, phoneSettingsTestCliObserver  
    CPhoneSettingsTestUtilities::ClosePhoneL( telServer, phone, line ); 	    
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliConstructAndDestruct );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestCancelAll
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestCancelAll(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliCancelAll() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetClipModeL(); 	
    iPsetCli->CancelAll(); 
    WaitTimer( KPhoneSettingsTestAsyncTimeout );    
    
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliCancelAll );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestGetClipModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestGetClipModeL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliGetClipModeL() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetClipModeL(); 	
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
		
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliGetClipModeL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
	
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestGetClirModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestGetClirModeL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliGetClirModeL() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetClirModeL(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
		
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliGetClirModeL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestGetCnapL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestGetCnapL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliGetCnapL() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetCnapL(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
		
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliGetCnapL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestGetColpModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestGetColpModeL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliGetColpModeL() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetColpModeL(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
		
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliGetColpModeL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestGetColrModeL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestGetColrModeL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliGetColrModeL() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 
	iPsetCli->GetColrModeL(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
		
    // Uninitialize test             
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliGetColrModeL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCli::TestSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCli::TestSetRequestObserver(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCliSetRequestObserver() );
	
	// Initialize test
 	CPhoneSettingsTestCliObserver* phoneSettingsTestCliObserver = 
 	    CPhoneSettingsTestCliObserver::NewL(); 	
	CleanupStack::PushL( phoneSettingsTestCliObserver );
	
	CPsetContainer* psetContainer = CPsetContainer::NewL();	
	CleanupStack::PushL( psetContainer );
	
	CPsetCli* psetCli = psetContainer->CreateCliObjectL( *phoneSettingsTestCliObserver );	
    CleanupStack::PushL( psetCli );

	CPhoneSettingsTestPsetRequestObserver* phoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestPsetRequestObserver );
	
    // Do tests here 	
    psetCli->SetRequestObserver( phoneSettingsTestPsetRequestObserver );
    
    // Uninitialize test 
    CleanupStack::PopAndDestroy( 4 );   
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCliSetRequestObserver );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

		    					
//  End of File
