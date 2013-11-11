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
* Description: Implementation of CPhoneSettingsTestNetwork class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <BADESCA.H>            
#include <PsetContainer.h>
#include <PsetConstants.h>
#include <PsetNetwork.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestPsetRequestObserver.h" 
#include "PhoneSettingsTestNetworkInfoObserver.h" 
#include "PhoneSettingsTestNetworkModeObserver.h" 
#include "psetsaobserver.h" 
#include "PhoneSettingsTestNetwork.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::CPhoneSettingsTestNetwork
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetwork::CPhoneSettingsTestNetwork(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetwork::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetwork* CPhoneSettingsTestNetwork::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler)
    {
    CPhoneSettingsTestNetwork* self = new (ELeave) CPhoneSettingsTestNetwork(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestNetwork::~CPhoneSettingsTestNetwork()
    {          		
    delete iPsetNetwork;             
    delete iPsetContainer; 
    delete iPhoneSettingsTestNetworkModeObserver; 
    delete iPhoneSettingsTestNetworkInfoObserver;  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::WaitTimer
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetwork::WaitTimer(TInt aWaitTime)
    {
    CPeriodic* timer = CPeriodic::NewL( CActive::EPriorityStandard );
    timer->Start( aWaitTime, aWaitTime, TCallBack( DoCallBackL, this ) );
    iWaitTimer.Start();
    delete timer;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::DoCallBackL
// ----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::DoCallBackL(TAny* aAny)
    {
    // Can't call iWaitAny.AsyncStop() in static function
    REINTERPRET_CAST( CPhoneSettingsTestNetwork*, aAny )->Stop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::Stop
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetwork::Stop()
    {
    iWaitTimer.AsyncStop();  
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::SetupL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetwork::SetupL()
    {
    iPsetContainer = CPsetContainer::NewL();        
	iPhoneSettingsTestNetworkInfoObserver = 
	    CPhoneSettingsTestNetworkInfoObserver::NewL(); 
	iPhoneSettingsTestNetworkModeObserver = 
	    CPhoneSettingsTestNetworkModeObserver::NewL();    
	iPsetNetwork  = iPsetContainer->CreateNetworkObjectL(
	    *iPhoneSettingsTestNetworkInfoObserver );
    iPsetNetwork->SetNetworkModeObserver( 
        *iPhoneSettingsTestNetworkModeObserver ); 
    }

// ----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TearDownL
// ----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetwork::TearDownL()
    {
    delete iPsetNetwork;             
    iPsetNetwork = NULL; 
    delete iPsetContainer; 
    iPsetContainer = NULL; 
    delete iPhoneSettingsTestNetworkModeObserver; 
    iPhoneSettingsTestNetworkModeObserver = NULL; 
    delete iPhoneSettingsTestNetworkInfoObserver; 
    iPhoneSettingsTestNetworkInfoObserver = NULL;     
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestCPsetNetworkConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestCPsetNetworkConstructAndDestructL(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkConstructAndDestruct() );
	
	// Initialize test 
    RMobilePhone phone; 
	RTelServer telServer; 
	RMobileLine line; 
    CPhoneSettingsTestUtilities::OpenPhoneL( telServer, phone, line ); 	    
	
	CPhoneSettingsTestNetworkInfoObserver* phoneSettingsTestNetworkInfoObserver = 
	    CPhoneSettingsTestNetworkInfoObserver::NewL(); 
	CleanupStack::PushL( phoneSettingsTestNetworkInfoObserver ); 
	
	CPhoneSettingsTestNetworkModeObserver* phoneSettingsTestNetworkModeObserver = 
	    CPhoneSettingsTestNetworkModeObserver::NewL(); 
	CleanupStack::PushL( phoneSettingsTestNetworkModeObserver ); 
	
	CPsetNetwork* psetNetwork; 
	
	// Do tests here 
	// 1.) Test construction with Network Info observer 
	iTestMessageHandler.PrintMessage( 
	    KNetworkConstructAndDestructUsingNetworkInfoObserver() );	     
	psetNetwork = CPsetNetwork::NewL( phone, *phoneSettingsTestNetworkInfoObserver );
	CleanupStack::PushL( psetNetwork ); 
	ASSERT( psetNetwork != NULL ); 
    CleanupStack::PopAndDestroy( psetNetwork ); 
    psetNetwork = NULL; 

	// 2.) Test construction with Network Mode observer 
    iTestMessageHandler.PrintMessage( 
        KNetworkConstructAndDestructUsingNetworkModeObserver() );     
	psetNetwork = CPsetNetwork::NewL( phone, *phoneSettingsTestNetworkModeObserver );
	CleanupStack::PushL( psetNetwork ); 
	ASSERT( psetNetwork != NULL ); 
    CleanupStack::PopAndDestroy( psetNetwork ); 
	psetNetwork = NULL; 	
		
    CleanupStack::PopAndDestroy( 2 );     
    CPhoneSettingsTestUtilities::ClosePhoneL( telServer, phone, line ); 	    
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkConstructAndDestruct );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestGetCurrentNetworkInfo
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestGetCurrentNetworkInfo(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkGetCurrentNetworkInfo() );
	
	// Initialize test 
    SetupL(); 
	
    MPsetNetworkSelect::TCurrentNetworkInfo networkInfo; 
    
	// Do tests here 
	TInt err = iPsetNetwork->GetCurrentNetworkInfo( networkInfo ); 	
    WaitTimer( KPhoneSettingsTestAsyncTimeout );    
    
	// GetCurrentNetworkInfo is deprecated and should return KErrNotSupported 		
	ASSERT( err == KErrNotSupported ); 
			
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkGetCurrentNetworkInfo );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestGetNetworkSelectMode
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestGetNetworkSelectMode(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkGetNetworkSelectMode() );
	
	// Initialize test 
    SetupL(); 
	
    MPsetNetworkSelect::TSelectMode selectMode; 
    
	// Do tests here 
	TInt err = iPsetNetwork->GetNetworkSelectMode( selectMode ); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    

	// Parse result data 
	iTestParser.ParseTSelectMode( selectMode ); 
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( err, KNetworkGetNetworkSelectMode );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;         
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestIsCallActive
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestIsCallActive(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkIsCallActive() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	TInt retValue = iPsetNetwork->IsCallActive(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
	
	ASSERT( retValue>= 0 ); 
	
	// Print result data
	if( retValue > 0 )
	    {
	    iTestMessageHandler.PrintMessage( KCallIsActive() );
	    }
	else 
	    {
	    iTestMessageHandler.PrintMessage( KCallIsNotActive() );
	    }
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkIsCallActive );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;             
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestIsGPRSConnected
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestIsGPRSConnected(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkIsGPRSConnected() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	TInt retValue = iPsetNetwork->IsGPRSConnected(); 
	WaitTimer( KPhoneSettingsTestAsyncTimeout );    
	
	ASSERT( retValue >= 0 ); 
	
	// Print result data 
	switch( retValue )
	    {
	    case CPsetSAObserver::EPSetGPRSConnectionActive: 
	        iTestMessageHandler.PrintMessage( KEPSetGPRSConnectionActive() ); 
	        break; 
	    case CPsetSAObserver::EPSetGPRSNotConnected: 
	        iTestMessageHandler.PrintMessage( KEPSetGPRSNotConnected() ); 
	        break; 

        default: 
            break; 
        }
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkIsGPRSConnected );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;        
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestResetNetworkSearch
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestResetNetworkSearch(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkResetNetworkSearch() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	TInt retValue = iPsetNetwork->ResetNetworkSearch(); 
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( retValue, KNetworkResetNetworkSearch );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;          
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestCancelProcess
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestCancelProcess(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkCancelProcess() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	iPsetNetwork->CancelProcess(); 
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkCancelProcess );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;             
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestGetAvailableNetworksL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestGetAvailableNetworksL(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkGetAvailableNetworksL() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	iPsetNetwork->GetAvailableNetworksL(); 
	
	// Uninitialize test 
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkGetAvailableNetworksL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;       
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestGetCurrentNetworkModeSelectionL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestGetCurrentNetworkModeSelectionL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkGetCurrentNetworkModeSelectionL() );
	
	// Initialize test 
    SetupL(); 
    
	// Do tests here 
	iPsetNetwork->GetCurrentNetworkModeSelectionL(); 	
		
	// Uninitialize test     
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkGetCurrentNetworkModeSelectionL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;        
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestSelectNetworkL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestSelectNetworkL(TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkSelectNetworkL() );
	
	// Initialize test 
    SetupL(); 
	
    MPsetNetworkSelect::TNetworkInfo networkInfo; 
        networkInfo.iId.iCountryCode = KTestCountryCode; 
        networkInfo.iId.iNetworkCode = KTestNetworkCode; 
        networkInfo.iStatus = MPsetNetworkSelect::ENetStatAvailable;
        networkInfo.iMode = MPsetNetworkSelect::ENetSelectModeAutomatic;
        networkInfo.iAccess = MPsetNetworkSelect::ENetNetworkWCDMA; 
        networkInfo.iShortName = KNetworkShortName; 
        networkInfo.iLongName = KNetworkLongName;     
    
	// Do tests here 	
	iPsetNetwork->SelectNetworkL( networkInfo ); 
	
	// Parse result data	
	iTestParser.ParseTNetworkInfo( networkInfo ); 
	
	// Uninitialize test     
	TearDownL(); 
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkSelectNetworkL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;          
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestSetNetSAObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestSetNetSAObserver(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkSetNetSAObserver() );
	
	// Initialize test 
    CPsetContainer* psetContainer = CPsetContainer::NewL();        
    CleanupStack::PushL( psetContainer ); 
	
    CPhoneSettingsTestNetworkInfoObserver* phoneSettingsTestNetworkInfoObserver = 
        CPhoneSettingsTestNetworkInfoObserver::NewL();   
    CleanupStack::PushL( phoneSettingsTestNetworkInfoObserver );      

    CPsetNetwork* psetNetwork  = psetContainer->CreateNetworkObjectL(
	    *phoneSettingsTestNetworkInfoObserver );
    CleanupStack::PushL( psetNetwork );   
        
	// Do tests here 
	psetNetwork->SetNetSAObserver( *phoneSettingsTestNetworkInfoObserver ); 
	
	// Uninitialize test 	
	CleanupStack::PopAndDestroy( 3 );   
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkSetNetSAObserver );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;           
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestSetNetworkModeObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestSetNetworkModeObserver(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkSetNetworkModeObserver() );
	
	// Initialize test 
    CPsetContainer* psetContainer = CPsetContainer::NewL();        
    CleanupStack::PushL( psetContainer ); 
	
    CPhoneSettingsTestNetworkModeObserver* phoneSettingsTestNetworkModeObserver = 
        CPhoneSettingsTestNetworkModeObserver::NewL();   
    CleanupStack::PushL( phoneSettingsTestNetworkModeObserver );      

    CPhoneSettingsTestNetworkInfoObserver* phoneSettingsTestNetworkInfoObserver = 
        CPhoneSettingsTestNetworkInfoObserver::NewL();   
    CleanupStack::PushL( phoneSettingsTestNetworkInfoObserver );      

    CPsetNetwork* psetNetwork  = psetContainer->CreateNetworkObjectL(
	    *phoneSettingsTestNetworkInfoObserver );
    CleanupStack::PushL( psetNetwork );   
    
	// Do tests here 
	psetNetwork->SetNetworkModeObserver( *phoneSettingsTestNetworkModeObserver ); 
	
	// Uninitialize test 	
	CleanupStack::PopAndDestroy( 4 );   
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkSetNetworkModeObserver );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;       
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetwork::TestSetNetworkModeSelectionL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestNetwork::TestSetNetworkModeSelectionL(TTestResult& aResult)
    {
 	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KNetworkSetNetworkModeSelectionL() );
	
	// Initialize test     
    SetupL(); 
	
    TUint32 networkModeCapsUint = RMmCustomAPI::KCapsNetworkModeDual;      
    
	// Do tests here 
	iPsetNetwork->SetNetworkModeSelectionL( networkModeCapsUint ); 
		
	// Parse result data
	RMmCustomAPI::TNetworkModeCaps networkModeCaps = 
	    static_cast<RMmCustomAPI::TNetworkModeCaps>(networkModeCapsUint); 
	iTestParser.ParseTNetworkModeCaps( networkModeCaps ); 

	// Uninitialize test     
	TearDownL(); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KNetworkSetNetworkModeSelectionL );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;           
    }

		    					
//  End of File
