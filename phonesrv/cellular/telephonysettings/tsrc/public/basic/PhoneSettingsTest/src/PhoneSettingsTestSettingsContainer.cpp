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
* Description: Implementation of CPhoneSettingsTestSettingsContainer class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <BADESCA.H>            
#include <PsetContainer.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestCallBarringObserver.h"
#include "PhoneSettingsTestCallDivertingObserver.h"
#include "PhoneSettingsTestCallWaitingObserver.h"
#include "PhoneSettingsTestCliObserver.h"
#include "PhoneSettingsTestNetworkModeObserver.h"
#include "PhoneSettingsTestNetworkInfoObserver.h"
#include "PhoneSettingsTestSettingsContainer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::CPhoneSettingsTestSettingsContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestSettingsContainer::CPhoneSettingsTestSettingsContainer(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestSettingsContainer::ConstructL()
    {
	iPsetContainer = CPsetContainer::NewL();
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestSettingsContainer* CPhoneSettingsTestSettingsContainer::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler )
    {
    CPhoneSettingsTestSettingsContainer* self = new (ELeave) CPhoneSettingsTestSettingsContainer(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestSettingsContainer::~CPhoneSettingsTestSettingsContainer()
    {          		
    delete iPsetContainer; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCPsetContainerConstructAndDestructL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCPsetContainerConstructAndDestructL(
    TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerConstructAndDestruct() );
	
	// Initialize test 
	CPsetContainer* psetContainer = CPsetContainer::NewL();
	CleanupStack::PushL( psetContainer );	
	
	// Do tests here 
    ASSERT( psetContainer != NULL ); 
		
    // Uninitialize test             
	CleanupStack::PopAndDestroy( psetContainer );	
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerConstructAndDestruct );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateCBObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateCBObjectL(
    TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateCBObjectL() );
	
	// Initialize test 
	CPhoneSettingsTestCallBarringObserver* phoneSettingsTestCallBarringObserver = 
	    CPhoneSettingsTestCallBarringObserver::NewL();
	CleanupStack::PushL( phoneSettingsTestCallBarringObserver );	

	// Do tests here 
	CPsetCallBarring* psetCallBarring= 
	    iPsetContainer->CreateCBObjectL( *phoneSettingsTestCallBarringObserver ); 
    CleanupStack::PushL( psetCallBarring );  
    
    ASSERT( psetCallBarring != NULL ); 
    		
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateCBObjectL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateCFObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateCFObjectL(
    TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateCFObjectL() );
	
	// Initialize test 
    CPhoneSettingsTestCallDivertingObserver* phoneSettingsTestCallDivertingObserver = 
        CPhoneSettingsTestCallDivertingObserver::NewL(); 
    CleanupStack::PushL( phoneSettingsTestCallDivertingObserver );     
    
	// Do tests here 
	CPsetCallDiverting* psetCallDiverting = 
	    iPsetContainer->CreateCFObjectL( *phoneSettingsTestCallDivertingObserver ); 
	CleanupStack::PushL( psetCallDiverting ); 
	
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateCFObjectL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;       
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateCWObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateCWObjectL(
    TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateCWObjectL() );
	
	// Initialize test 
    CPhoneSettingsTestCallWaitingObserver* phoneSettingsTestCallWaitingObserver = 
        CPhoneSettingsTestCallWaitingObserver::NewL(); 
    CleanupStack::PushL( phoneSettingsTestCallWaitingObserver );     
    
	// Do tests here 
	CPsetCallWaiting* psetCallWaiting = 
	    iPsetContainer->CreateCWObjectL( *phoneSettingsTestCallWaitingObserver ); 
	CleanupStack::PushL( psetCallWaiting ); 
	
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 );
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateCWObjectL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;           
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateCliObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateCliObjectL(
    TTestResult& aResult)
    {
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateCliObjectL() );
	
	// Initialize test 
    CPhoneSettingsTestCliObserver* phoneSettingsTestCliObserver = 
        CPhoneSettingsTestCliObserver::NewL(); 
    CleanupStack::PushL( phoneSettingsTestCliObserver );     
    
	// Do tests here 
	CPsetCli* psetCli = 
	    iPsetContainer->CreateCliObjectL( *phoneSettingsTestCliObserver ); 
	CleanupStack::PushL( psetCli ); 
	
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateCliObjectL );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;               
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateNetworkModeObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateNetworkModeObjectL(
    TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateNetworkModeObjectL() );
	
	// Initialize test 
    CPhoneSettingsTestNetworkModeObserver* phoneSettingsTestNetworkModeObserver = 
        CPhoneSettingsTestNetworkModeObserver::NewL(); 
    CleanupStack::PushL( phoneSettingsTestNetworkModeObserver );     
    
	// Do tests here 
	CPsetNetwork* psetNetwork = 
	    iPsetContainer->CreateNetworkModeObjectL( *phoneSettingsTestNetworkModeObserver ); 
	CleanupStack::PushL( psetNetwork  ); 
	
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateNetworkModeObjectL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;        
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateNetworkObjectL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateNetworkObjectL(
    TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateNetworkObjectL() );
	
	// Initialize test 
    CPhoneSettingsTestNetworkInfoObserver* phoneSettingsTestNetworkInfoObserver = 
        CPhoneSettingsTestNetworkInfoObserver::NewL(); 
    CleanupStack::PushL( phoneSettingsTestNetworkInfoObserver );     
    
	// Do tests here 
	CPsetNetwork* psetNetwork = 
	    iPsetContainer->CreateNetworkObjectL( *phoneSettingsTestNetworkInfoObserver ); 
	CleanupStack::PushL( psetNetwork  ); 
	
    // Uninitialize test             
    CleanupStack::PopAndDestroy( 2 ); 
    
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateNetworkObjectL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;    
    } 
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestSettingsContainer::TestCreateRefreshHandlerL
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSettingsContainer::TestCreateRefreshHandlerL(
    TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KContainerCreateRefreshHandlerL() );
	
	// Initialize test 
    CPSetRefreshHandler* cPSetRefreshHandler = 
        iPsetContainer->CreateRefreshHandlerL(); 
	CleanupStack::PushL( cPSetRefreshHandler ); 
    
	// Do tests here 
    ASSERT( cPSetRefreshHandler != NULL ); 
    
    // Uninitialize test             
	CleanupStack::PopAndDestroy( cPSetRefreshHandler );                                       
	
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KContainerCreateRefreshHandlerL );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;    
    }     

		    					
//  End of File
