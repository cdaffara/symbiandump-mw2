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
* Description: Implementation of CPhoneSettingsTestCallWaiting class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <BADESCA.H>            
#include <PsetContainer.h>
#include <PsetConstants.h>
#include <PsetCallWaiting.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestCallWaitingObserver.h"
#include "PhoneSettingsTestPsetRequestObserver.h" 
#include "PhoneSettingsTestCallWaiting.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::CPhoneSettingsTestCallWaiting
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallWaiting::CPhoneSettingsTestCallWaiting(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaiting::ConstructL()
    {

    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallWaiting* CPhoneSettingsTestCallWaiting::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler)
    {
    CPhoneSettingsTestCallWaiting* self = new (ELeave) CPhoneSettingsTestCallWaiting(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallWaiting::~CPhoneSettingsTestCallWaiting()
    {          		
	delete iPsetContainer; 
	delete iPhoneSettingsTestCallWaitingObserver;
    delete iPsetCallWaiting;
	delete iPhoneSettingsTestPsetRequestObserver; 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaiting::WaitTimer(TInt aWaitTime)
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();
    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::DoCallBackL(TAny* aAny)
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestCallWaiting*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaiting::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaiting::SetupL()
    {
	iPsetContainer = CPsetContainer::NewL();  	
	iPhoneSettingsTestCallWaitingObserver = 
	    CPhoneSettingsTestCallWaitingObserver::NewL(); 
    iPsetCallWaiting = iPsetContainer->CreateCWObjectL( 
        *iPhoneSettingsTestCallWaitingObserver ); 
	iPhoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();    
    iPsetCallWaiting->SetRequestObserver( iPhoneSettingsTestPsetRequestObserver ); 	
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaiting::TearDownL()
    {
	delete iPsetContainer; 
	iPsetContainer = NULL; 	
	delete iPhoneSettingsTestCallWaitingObserver;
	iPhoneSettingsTestCallWaitingObserver = NULL;
    delete iPsetCallWaiting;
    iPsetCallWaiting = NULL;
	delete iPhoneSettingsTestPsetRequestObserver;     
	iPhoneSettingsTestPsetRequestObserver = NULL;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestCPsetCallWaitingConstructAndDestruct
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestCPsetCallWaitingConstructAndDestructL(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingConstructAndDestruct() );

	// Initialize test 
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
	
	CPhoneSettingsTestCallWaitingObserver* phoneSettingsTestCallWaitingObserver = 
	    CPhoneSettingsTestCallWaitingObserver::NewL(); 	
	CleanupStack::PushL( phoneSettingsTestCallWaitingObserver );  
	
    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    
	
	CPsetCallWaiting* pSetCallWaiting= CPsetCallWaiting::NewL( 
	    phone, *phoneSettingsTestCallWaitingObserver );    
    CleanupStack::PushL( pSetCallWaiting );     	
        	
	// Do tests here 	
	ASSERT( pSetCallWaiting != NULL ); 
    
    // Uninitialize test             
    CleanupStack::PopAndDestroy(2); 
    CPhoneSettingsTestUtilities::ClosePhoneL( telServer, phone, line ); 	            
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingConstructAndDestruct );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestSetCallWaitingL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestSetCallWaitingL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingSetCallWaitingL() );

	// Initialize test 
	SetupL(); 

    MPsetCallWaiting::TSetCallWaiting callWaitingSetting = MPsetCallWaiting::EActivateCallWaiting; 
    TBasicServiceGroups basicServiceGroups = ETelephony; 
    
	// Do tests here 	    
    iPsetCallWaiting->SetCallWaitingL( callWaitingSetting, basicServiceGroups );
    WaitTimer( KPhoneSettingsTestAsyncTimeout );    
    
    // Parse result data
    iTestParser.ParseTSetCallWaiting( callWaitingSetting ); 
    iTestParser.ParseTBasicServiceGroups( basicServiceGroups ); 
    
    // Uninitialize test             
	TearDownL(); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingSetCallWaitingL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestGetCallWaitingStatusL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestGetCallWaitingStatusL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingGetCallWaitingStatusL() );

	// Initialize test 
	SetupL(); 
    
	// Do tests here 	
    iPsetCallWaiting->GetCallWaitingStatusL();
    WaitTimer( KPhoneSettingsTestAsyncTimeout );    
        
    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingGetCallWaitingStatusL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestCancelProcess
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestCancelProcess(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingCancelProcess() );

	// Initialize test 
	SetupL(); 
    
	// Do tests here 		    
    iPsetCallWaiting->GetCallWaitingStatusL();    
    iPsetCallWaiting->CancelProcess();    
        
    // Uninitialize test             
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingCancelProcess );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestSetRequestObserver(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingSetRequestObserver() );
	
	// Initialize test
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
	
	CPhoneSettingsTestCallWaitingObserver* phoneSettingsTestCallWaitingObserver = 
	    CPhoneSettingsTestCallWaitingObserver::NewL(); 	
	CleanupStack::PushL( phoneSettingsTestCallWaitingObserver );  	

    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    	

	CPsetCallWaiting* psetCallWaiting= CPsetCallWaiting::NewL( 
	    phone, *phoneSettingsTestCallWaitingObserver );    	    
    CleanupStack::PushL( psetCallWaiting );     	

	CPhoneSettingsTestPsetRequestObserver* phoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestPsetRequestObserver );
	
    // Do tests here 	
    psetCallWaiting->SetRequestObserver( phoneSettingsTestPsetRequestObserver );
    
    // Uninitialize test 
    CleanupStack::PopAndDestroy( 3 );   
    CPhoneSettingsTestUtilities::ClosePhoneL( 
        telServer, phone, line ); 	    	                                        
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingSetRequestObserver );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaiting::TestPhoneSettingChanged
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallWaiting::TestPhoneSettingChanged(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallWaitingPhoneSettingChanged() );
	
	// Initialize test 
	SetupL(); 
	
	TSSSettingsSetting ssSetting = ESSSettingsAls;  
	TInt newSettingValue = ESSSettingsAlsPrimary; 
	
    // Do tests here 
    iPsetCallWaiting->PhoneSettingChanged( ssSetting, newSettingValue ); 
        
    // Parse result data    
    iTestParser.ParseTSSSettingsSetting( ssSetting );

    // Uninitialize test 
	TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallWaitingPhoneSettingChanged );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	
	
		    					
//  End of File
