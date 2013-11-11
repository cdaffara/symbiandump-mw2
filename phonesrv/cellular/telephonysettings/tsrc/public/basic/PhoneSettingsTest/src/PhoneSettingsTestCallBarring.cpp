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
* Description: Implementation of CPhoneSettingsTestCallBarring class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <PsetContainer.h>
#include <PsetCallBarring.h>
#include <nwdefs.h> 
#include <etelmm.h> 
#include <MmTsy_names.h> 
#include "PhoneSettingsTest.h"
#include "PhoneSettingsTest.hrh"
#include "PhoneSettingsTestDefs.h"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestCallBarringObserver.h"
#include "PhoneSettingsTestPsetRequestObserver.h" 
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestCallBarring.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::CPhoneSettingsTestCallBarring
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallBarring::CPhoneSettingsTestCallBarring(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarring::ConstructL()
    {   
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallBarring* CPhoneSettingsTestCallBarring::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler)
    {
    CPhoneSettingsTestCallBarring* self = new (ELeave) CPhoneSettingsTestCallBarring(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallBarring::~CPhoneSettingsTestCallBarring()
    {          	
    delete iPsetCallBarring; 
    delete iPhoneSettingsTestPsetRequestObserver; 
    delete iPhoneSettingsTestCallBarringObserver; 
    delete iPsetContainer; 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarring::WaitTimer(TInt aWaitTime)
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );    
    iWaitTimer.Start();

    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::DoCallBackL(TAny* aAny)
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestCallBarring*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarring::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarring::SetupL()
    {
	iPsetContainer = CPsetContainer::NewL();
	iPhoneSettingsTestCallBarringObserver = 
	    CPhoneSettingsTestCallBarringObserver::NewL(); 		
	iPsetCallBarring = iPsetContainer->CreateCBObjectL( 
	    *iPhoneSettingsTestCallBarringObserver );	
	iPhoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	iPsetCallBarring->SetRequestObserver( iPhoneSettingsTestPsetRequestObserver );       
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarring::TearDownL()
    {
    delete iPsetCallBarring; 
    iPsetCallBarring = NULL;     
    delete iPhoneSettingsTestPsetRequestObserver; 
    iPhoneSettingsTestPsetRequestObserver = NULL; 
    delete iPhoneSettingsTestCallBarringObserver; 
    iPhoneSettingsTestCallBarringObserver = NULL; 
    delete iPsetContainer;          
    iPsetContainer = NULL; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestCPsetCallBarringConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestCPsetCallBarringConstructAndDestructL( 
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringConstructAndDestruct() );
	
	// Initialize test 
    SetupL(); 
	
	// Do tests here 	
    ASSERT( iPsetCallBarring != NULL ); 
    ASSERT( iPhoneSettingsTestPsetRequestObserver != NULL ); 
    ASSERT( iPhoneSettingsTestCallBarringObserver != NULL ); 
    ASSERT( iPsetContainer != NULL );     
        
    // Uninitialize test     
    TearDownL(); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringConstructAndDestruct );			    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}
		
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestChangePasswordL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestChangePasswordL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
    iTestMessageHandler.PrintMessage( KCallBarringChangePassword() );

	// Initialize test  
	SetupL();
	
	RMobilePhone::TMobilePhonePasswordChangeV2 pwds1; 
	RMobilePhone::TMobilePhonePasswordChangeV2 pwds2; 
	TBool isBarringPw(ETrue);  
	    
    // Do tests here 	
    
    // 1.) Test ChangePasswordL(RMobilePhone::TMobilePhonePasswordChangeV2&,TBool)
    iPsetCallBarring->ChangePasswordL( pwds1, isBarringPw );    
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
    
    // 2.) Test ChangePasswordL(RMobilePhone::TMobilePhonePasswordChangeV2&)
    iPsetCallBarring->ChangePasswordL( pwds2 );
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
    
    // Parse result data  
    iTestParser.ParseTMobilePhonePasswordChangeV2( pwds1 ); 
    iTestParser.ParseTMobilePhonePasswordChangeV2( pwds2 ); 

    // Uninitialize test 
    TearDownL(); 

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringChangePassword );		
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestSetBarringL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestSetBarringL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringSetBarringL() );

	// Initialize test  		
	SetupL();	
	
	TCallBarringSetting setBarring;    
    setBarring.iServiceGroup = EServiceGroupVoice;
    setBarring.iType = EBarringTypeAllOutgoingServices;
	TBasicServiceGroups basicServiceGroups = EAllTele;  
	
    // Do tests here 	
    iPsetCallBarring->SetBarringL( setBarring, basicServiceGroups ); 
        
    // Parse result data        
    iTestParser.ParseTServiceGroup( setBarring.iServiceGroup );
    iTestParser.ParseTBarringProgram( setBarring.iType );   
    iTestParser.ParseTBasicServiceGroups( basicServiceGroups );            

    // Uninitialize test 
    TearDownL(); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringSetBarringL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestGetBarringStatusL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestGetBarringStatusL(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringGetBarringStatusL() );
	
	// Initialize test  	
    SetupL();			
	TCallBarringSetting setBarring;    
    setBarring.iServiceGroup = EServiceGroupVoice;
    setBarring.iType = EBarringTypeAllBarrings; 
	setBarring.iSetting = EActivateBarring; 
	
    // Do tests here 	
    iPsetCallBarring->GetBarringStatusL( setBarring.iServiceGroup, setBarring.iType );         
    WaitTimer( KPhoneSettingsTestAsyncTimeout );
    
    // Parse result data        
    iTestParser.ParseTServiceGroup( setBarring.iServiceGroup );
    iTestParser.ParseTBarringProgram( setBarring.iType );    

    // Uninitialize test 
    TearDownL();
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringGetBarringStatusL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestSetRequestObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestSetRequestObserver(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringSetRequestObserver() );
	
	// Initialize test	
	CPsetContainer* psetContainer = CPsetContainer::NewL();	
	CleanupStack::PushL( psetContainer );
	
	CPhoneSettingsTestCallBarringObserver* phoneSettingsTestCallBarringObserver = 
	    CPhoneSettingsTestCallBarringObserver::NewL(); 			
	CleanupStack::PushL( phoneSettingsTestCallBarringObserver );	    
	
	CPsetCallBarring* psetCallBarring = 
	    psetContainer->CreateCBObjectL( *phoneSettingsTestCallBarringObserver );	
	CleanupStack::PushL( psetCallBarring );	
		
    // Do tests here 	
	CPhoneSettingsTestPsetRequestObserver* phoneSettingsTestPsetRequestObserver = 
	    CPhoneSettingsTestPsetRequestObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestPsetRequestObserver );
	psetCallBarring->SetRequestObserver( phoneSettingsTestPsetRequestObserver );       
    
    // Uninitialize test 
    CleanupStack::PopAndDestroy( 4 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringSetRequestObserver );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestCancelCurrentRequest
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestCancelCurrentRequest(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringCancelCurrentRequest() );
	
    // Initialize test 
    SetupL(); 
    
	TCallBarringSetting setBarring;    
    setBarring.iServiceGroup = EServiceGroupVoice;
    setBarring.iType = EBarringTypeAllBarrings; 
	setBarring.iSetting = EActivateBarring; 

	// Do tests here 
	iPsetCallBarring->GetBarringStatusL( setBarring.iServiceGroup, setBarring.iType );     
	TInt err = iPsetCallBarring->CancelCurrentRequest();
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
	aResult.SetResult( err, KCallBarringCancelCurrentRequest );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestCPsetCallBarringNewL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestCPsetCallBarringNewL( TTestResult& aResult )
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringCPsetCallBarringNewL() );
	
#ifndef PHONESETTINGSTEST_LIBRARY_ONLY	

    // Initialize test 
	CPhoneSettingsTestCallBarringObserver* phoneSettingsTestCallBarringObserver = 
	    CPhoneSettingsTestCallBarringObserver::NewL(); 			
    CleanupStack::PushL( phoneSettingsTestCallBarringObserver );
        
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    
	
	// Do tests here 
	CPsetCallBarring* psetCallBarring = CPsetCallBarring::NewL(
	    *iPhoneSettingsTestCallBarringObserver, phone ); 	    
	CleanupStack::PushL( psetCallBarring );
	
	ASSERT( psetCallBarring != NULL ); 
	
	// Uninitialize test 		
	CleanupStack::PopAndDestroy( 2 );   
    CPhoneSettingsTestUtilities::ClosePhoneL( telServer, phone, line ); 	    
    		
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringCPsetCallBarringNewL );
	
#else //PHONESETTINGSTEST_LIBRARY_ONLY	

	aResult.SetResult( KErrNotSupported, KCallBarringCPsetCallBarringNewL );	

#endif // PHONESETTINGSTEST_LIBRARY_ONLY

    iTestMessageHandler.PrintMessage( KExitingTestCase() );    

	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallBarring::TestPhoneSettingChanged
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestCallBarring::TestPhoneSettingChanged(TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KCallBarringPhoneSettingChanged() );
	
	// Initialize test 
	SetupL();
	
	TSSSettingsSetting ssSetting = ESSSettingsAls;  
	TInt newSettingValue = ESSSettingsAlsPrimary; 
	
    // Do tests here 
    iPsetCallBarring->PhoneSettingChanged( ssSetting, newSettingValue ); 
        
    // Parse result data    
    iTestParser.ParseTSSSettingsSetting( ssSetting );

    // Uninitialize test 
    TearDownL();
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KCallBarringPhoneSettingChanged );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}	
	
		
//  End of File
