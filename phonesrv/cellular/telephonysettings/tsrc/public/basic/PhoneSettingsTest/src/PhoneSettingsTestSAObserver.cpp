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
* Description: Implementation of CPhoneSettingsTestSAObserver class.
*
*/



// INCLUDE FILES
#include <StifTestModule.h>
#include <nwdefs.h>             
#include <etelmm.h> 
#include <PsetConstants.h>
#include <psetsaobserver.h>
#include "PhoneSettingsTest.hrh"
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestUtilities.h" 
#include "PhoneSettingsTestDefs.h"
#include "PhoneSettingsTestNetworkInfoObserver.h" 
#include "PhoneSettingsTestSAObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::CPhoneSettingsTestSAObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestSAObserver::CPhoneSettingsTestSAObserver(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler ):
    iTestParser(aPhoneSettingsTestParser),
    iTestMessageHandler(aPhoneSettingsTestMessageHandler)
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestSAObserver::ConstructL()
    {    
	iPsetSAObserver = CPsetSAObserver::NewL();
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestSAObserver* CPhoneSettingsTestSAObserver::NewL(
    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler )
    {
    CPhoneSettingsTestSAObserver* self = new (ELeave) CPhoneSettingsTestSAObserver(
        aPhoneSettingsTestParser, aPhoneSettingsTestMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestSAObserver::~CPhoneSettingsTestSAObserver()
    {          		
    delete iPsetSAObserver;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestCPsetSAObserverConstructAndDestructL
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestCPsetSAObserverConstructAndDestructL(
    TTestResult& aResult)
	{
	iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOSAObserverConstructAndDestruct() );
	
	// Initialize test 	
	
	// Do tests here 
	CPsetSAObserver * psetSAObserver = 
	    CPsetSAObserver::NewL();
	CleanupStack::PushL( psetSAObserver ); 
	
	ASSERT( psetSAObserver != NULL ); 
		
    // Uninitialize test             
    CleanupStack::PopAndDestroy( psetSAObserver ); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAOSAObserverConstructAndDestruct );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestGetCurrentDivertStatus
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestGetCurrentDivertStatus(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOGetCurrentDivertStatus() );
	
	// Initialize test 	
	TUnconditionalCFStatus unconditionalCFStatus; 

	// Do tests here 
	TInt err = iPsetSAObserver->GetCurrentDivertStatus( unconditionalCFStatus ); 

    // Parse result data
    iTestParser.ParseTUnconditionalCFStatus( unconditionalCFStatus ); 

	// Set result for STIF framework
	aResult.SetResult( err, KSAOGetCurrentDivertStatus );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestIsCallActive
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestIsCallActive(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOIsCallActive() );

	// Do tests here 
	TInt retValue = iPsetSAObserver->IsCallActive(); 
	ASSERT( retValue >= 0 ); 
	
	switch( retValue )
	    {
	    case CPsetSAObserver::EPSetNoCallsActive: 
	        iTestMessageHandler.PrintMessage( KEPSetNoCallsActive() );
	        break; 
	    case CPsetSAObserver::EPSetCallActive: 
	        iTestMessageHandler.PrintMessage( KEPSetCallActive() );
	        break; 
	    
	    default: 
	        iTestMessageHandler.PrintMessage( KErrorUnknownEnumeration() );
	        break; 
	    }

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAOIsCallActive );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestIsGPRSConnected
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestIsGPRSConnected(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOIsGPRSConnected() );

	// Do tests here 
	TInt retValue = iPsetSAObserver->IsGPRSConnected(); 
	ASSERT( retValue >= 0 ); 
	
	switch( retValue )
	    {
	    case CPsetSAObserver::EPSetGPRSNotConnected: 
	        iTestMessageHandler.PrintMessage( KEPSetGPRSNotConnected() );
	        break; 
	    case CPsetSAObserver::EPSetGPRSConnectionActive: 
	        iTestMessageHandler.PrintMessage( KEPSetGPRSConnectionActive() );
	        break; 
	    
	    default: 
	        iTestMessageHandler.PrintMessage( KErrorUnknownEnumeration() );
	        break; 
	    }

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAOIsGPRSConnected );	
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestNotifyDivertChange
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestNotifyDivertChange(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAONotifyDivertChange() );
	
	// Initialize test 	
    TSelectedLine alsStatus = EPrimaryLine; 
    TCallDivertNotifySetting divertNotify;
    divertNotify.Initialize();
    TInt activatorUncertain; 
    
	// Do tests here 
	iPsetSAObserver->NotifyDivertChange( alsStatus, divertNotify, activatorUncertain ); 

    // Parse result data
    iTestParser.ParseTSelectedLine( alsStatus );  
    iTestParser.ParseTCallDivertNotifySetting( divertNotify ); 
    
    iTestMessageHandler.PrintMessage( KActivatorUncertain() ); 
    TBuf<KMessageHandlerMaxMessageSize> messageBuffer; 
    messageBuffer.Num( activatorUncertain ); 

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAONotifyDivertChange );
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestSetNetObserver
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestSetNetObserver(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOSetNetObserver() );
	
	// Initialize test 	
	CPhoneSettingsTestNetworkInfoObserver* phoneSettingsTestNetworkInfoObserver = 
	    CPhoneSettingsTestNetworkInfoObserver::NewL(); 
	CleanupStack::PushL( phoneSettingsTestNetworkInfoObserver ); 

	// Do tests here 
	iPsetSAObserver->SetNetObserver( *phoneSettingsTestNetworkInfoObserver ); 

    // Uninitialize test             
	CleanupStack::PopAndDestroy( phoneSettingsTestNetworkInfoObserver ); 
        
	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAOSetNetObserver );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestSAObserver::TestInitialize
// 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTestSAObserver::TestInitialize(TTestResult& aResult)
    {
    iTestMessageHandler.PrintMessage( KStartingTestCase() );
	iTestMessageHandler.PrintMessage( KSAOInitialize() );
	
	// Initialize test 	
    TCallDivertNotifySetting divertNotify;

	// Do tests here 
    divertNotify.Initialize();

	// Set result for STIF framework
	aResult.SetResult( KErrNone, KSAOInitialize );		    
    iTestMessageHandler.PrintMessage( KExitingTestCase() );    
    
	return KErrNone;     
    }


//  End of File
