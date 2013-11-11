/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  STIF test scripter implementation
*
*/


// Include files
#include <stiftestinterface.h>
#include <e32property.h>		// RProperty
#include <icmapi.h>
#include "cccetester.h"
#include "ccetesterdefs.h"
#include "ctestconfcallobserver.h"
#include "ctestssobserver.h"
#include "e32debug.h"
#include "ccetesterlogger.h"
#include "ctestmediator.h"

// Forward declarations

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCCETester::CCCETester
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCCETester::CCCETester( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    iTestStep = 0;
    
    iSkipVerification = 0;
    }



// -----------------------------------------------------------------------------
// CCCETester::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCCETester::ConstructL()
    {
    /* Check if the property has a value (left from previously failed/crashed test case)
    *  and delete if found
    */
    CCETESTERLOG("CCCETester::ConstructL entered");
    
    RProperty test;
    TInt val;
    
    if ( test.Get( KPSUidICMIncomingCall, KPropertyKeyICMPluginUID, val ) == KErrNone )
    	{
    	CCETESTERLOG("CCCETester::ConstructL, deleting p&s key");
    	RProperty::Delete(KPSUidICMIncomingCall, KPropertyKeyICMPluginUID);
    	}
    test.Close();
    
    CCETESTERLOG("CCCETester::ConstructL, p&s key handled");
    
    iLog = CStifLogger::NewL( KCCCETesterLogPath, 
                          KCCCETesterLogFile //,
                          //optional params, set in .ini file
                          //CStifLogger::ETxt,
                          //CStifLogger::ERDebug
                          //CStifLogger::EFile,
                          //EFalse				//overwrite
                          );
	
	iEventStorage = CEventStorage::NewL();
	
	iObserver = CTestObserver::NewL( iEventStorage, this );
	
	iConfCallObserver = CTestConfCallObserver::NewL( iEventStorage, this );
	
	iDtmfObserver = CTestDtmfObserver::NewL( iEventStorage );
	
	iSsObserver = CTestSsObserver::NewL( iEventStorage );
	
	iExtIfObserver = CTestExtObserver::NewL( iEventStorage );
	
	iTestTimer = CDelay::NewL();
	
    iCallParams = CCCECallParameters::NewL();
	
#ifndef __DONT_CREATE_CCE_IN_CONSTRUCTOR

	CCETESTERLOG("CCCETester::ConstructL, creating CCE");
	
	iCceTest = CCCE::NewL(); //needs Active Scheduler
	
	CCETESTERLOG("CCCETester::ConstructL, created CCE");
     
    __ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("CCCE::NewL() failed"), KErrGeneral ) );
    
    iCceTest->SetObserver(*iObserver, *iSsObserver);
    
    CCETESTERLOG("CCCETester::ConstructL, finished");
    
    // Has to be created after cce, because mediator event subscription fails if no provider.
    iMediatorObserver = CTestMediator::NewL();
        
        
#endif
    
    }



// -----------------------------------------------------------------------------
// CCCETester::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCCETester* CCCETester::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CCETESTERLOG("CCCETester::NewL called");
    
    CCCETester* self = new (ELeave) CCCETester( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }



// -----------------------------------------------------------------------------
// CCCETester destructor
// -----------------------------------------------------------------------------
//
CCCETester::~CCCETester()
    { 
    CCETESTERLOG("CCCETester destructor called");
    
    RProperty::Delete(KPSUidICMIncomingCall, KPropertyKeyICMPluginUID);
    
    CCETESTERLOG("CCCETester destructor, ps key deleted");
    
    iCallArray.Reset();
    iCallArray.Close();
	iCallObserverArray.ResetAndDestroy();
    iCallObserverArray.Close();    
	iTestTimer->Cancel();
    
#ifndef __DONT_CREATE_CCE_IN_CONSTRUCTOR

	delete iCceTest;
	CCETESTERLOG("CCCETester destructor, cce deleted");

#endif

	delete iTestTimer;
	delete iSsObserver;
	delete iDtmfObserver;
	delete iConfCallObserver;
	delete iObserver;
	delete iEventStorage;
	delete iExtIfObserver;
	delete iCallParams;
	delete iMediatorObserver;
	
    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
    }
    


// -----------------------------------------------------------------------------
// CCCETester::InitL
// -----------------------------------------------------------------------------
//
TInt CCCETester::InitL( TFileName& /*aIniFile*/, 
                         TBool /*aFirstTime*/ )
    {
    //CCETESTERLOG("CCCETester::InitL called");
    return KErrNone;
    }
    


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CCCETester::NewL( aTestModuleIf );

    }


//  End of File
