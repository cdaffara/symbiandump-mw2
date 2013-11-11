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
* Description:  CCommonTestClass implementation for STIF Test Framework TestScripter.
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "commontestclass.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCommonTestClass::CCommonTestClass
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCommonTestClass::CCommonTestClass( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ), 
        iTestModuleIf(aTestModuleIf)
    {
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCommonTestClass::ConstructL()
    {
    iLog = CStifLogger::NewL( KCommonTestClassLogPath, 
                          KCommonTestClassLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
	iTimeoutController = CSimpleTimeout::NewL (this, iLog);
    // Print title of the test case
    TName title;
    TestModuleIf().GetTestCaseTitleL(title);
    iLog->Log(_L("[Title] %S"), &title);
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCommonTestClass* CCommonTestClass::NewL(CTestModuleIf& aTestModuleIf )
    {
    CCommonTestClass* self = new (ELeave) CCommonTestClass( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCommonTestClass::~CCommonTestClass()
    { 
    Delete();
    // Delete logger
    delete iLog; 
	delete iTimeoutController;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CCommonTestClass::EventName( TInt aKey )
    {
	TText* const badKeyword = (TText*)L"BadKeyword";
	TText* const keywords[] =
	    {
		(TText*)L"ERequestComplete"
	    };
	
	if( (TUint)aKey >= (sizeof( keywords )/sizeof(TText*)) )
    	{
		iLog->Log(_L("Keyword out of bounds"));
		TPtrC keyword( badKeyword );
		return keyword;
	    }
	else
	    {
		TPtrC keyword( keywords[aKey] );
		return keyword;
	    }
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CCommonTestClass::AddExpectedEvent(TMPXTestExpectedEvent event, TInt ms)
    {
	FTRACE(FPrint(_L("CCommonTestClass::AddExpectedEvent")));
	iExpectedEvents.Append(event);
	TPtrC eventName = EventName(event);
	iLog->Log(_L("Adding expected event:(0x%02x)%S Total=%d"), event, &eventName, iExpectedEvents.Count() );

	if ( iTimeoutController && !iTimeoutController->IsActive() )
	    {
		if (ms > 0)
		    {
			iTimeoutController->Start( TTimeIntervalMicroSeconds(ms * 1000) );
		    }
		else
		    {
			iLog->Log(_L("Timeout with default value (1s)"));
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
		    }
	    }
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool CCommonTestClass::RemoveExpectedEvent(TMPXTestExpectedEvent aEvent)
    {
	FTRACE(FPrint(_L("CCommonTestClass::RemoveExpectedEvent")));
	TBool match = EFalse;
	for (TUint i=0; i < iExpectedEvents.Count() ; i++)
	    {
		if (iExpectedEvents[i] == aEvent)
		    {
			iExpectedEvents.Remove(i);
			match = ETrue;
			break;
		    }
	    }
	return match;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CCommonTestClass::RemoveAllExpectedEvents()
    {
	FTRACE(FPrint(_L("CCommonTestClass::RemoveAllExpectedEvents")));
	iLog->Log(_L("Removing all expected events"));

	iExpectedEvents.Reset();
	iOcurredEvents.Reset();
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::ProcessEvent
// Process events.
// -----------------------------------------------------------------------------
void CCommonTestClass::ProcessEvent(TMPXTestExpectedEvent aEvent, TInt aError)
    {
	FTRACE(FPrint(_L("CCommonTestClass::ProcessExpectedEvent")));
	TPtrC nameEvent = EventName(aEvent);

	// Check for error
	if (aError == KErrNone)
	    {
		// Remove the event
		if (RemoveExpectedEvent(aEvent))
		    {
			iLog->Log(_L("Expected Event: (0x%02x)%S has ocurred Total=%d"),
			    aEvent, &nameEvent,iExpectedEvents.Count());
		    }
		else
		    {
			iLog->Log(_L("Event: (0x%02x)%S has ocurred"), aEvent, &nameEvent);
			return;
		    }

		// All expected events have ocurred
		if (iExpectedEvents.Count() == 0 )
		    {
			Signal();
			iTimeoutController->Cancel();
		    }
	    }
	else
	    {
		iLog->Log(_L("[Error] Event: (0x%02x)%S return with error code=%d"), aEvent, &nameEvent, aError);
		if (iExpectedEvents.Count() != 0 )
		    {
			RemoveExpectedEvent(aEvent);
		    }
		iTimeoutController->Cancel();
		Signal(KErrCallbackErrorCode);
	    }
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------
void CCommonTestClass::HandleTimeout(TInt aError)
    {
	FTRACE(FPrint(_L("CCommonTestClass::HandleTimeout")));
	// All expected events have ocurred
	if (aError != KErrNone)
	    {
		if (iExpectedEvents.Count() == 0 )
		    {
			iLog->Log(_L("Timing out but events have ocurred"));
			Signal();
		    }
		else
		    {
			RemoveAllExpectedEvents();
			iLog->Log(_L("Timing out and events still pending"));
			Signal(KErrEventPending);
		    }
	    }
	else
	    {
		iLog->Log(_L("Timing out return a error %d"), aError);
		Signal(aError);
	    }
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetTimeout( CStifItemParser& aItem )
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetTimeout")));
	TInt timeout=0;
	TInt error = aItem.GetNextInt(timeout) ;
	if ( iTimeoutController )
	    {
		if ( timeout > 0 )
		    {
			iTimeoutController->Start( TTimeIntervalMicroSeconds(timeout*1000) );
		    }
		else
		    {
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
		    }
	    }
	else
	    {
		iLog->Log(_L("Timeout Controller doesn't exist"));
		error = KErrTimeoutController;
	    }
	return error;
    }


// -----------------------------------------------------------------------------
// Uses the TestModuleBase API to allow a panic as exit reason for a test case
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetAllowedPanic( CStifItemParser& aItem )
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetAllowedPanic")));
	TInt error = KErrNone;
	TInt panicCode;
	TPtrC panicType;
	if ( ( KErrNone == aItem.GetNextString(panicType) ) 
	    && ( KErrNone == aItem.GetNextInt(panicCode) ) )
	    {
		iLog->Log(_L("Allowing panic: %S %d"), &panicType, panicCode);
		iTestModuleIf.SetExitReason( CTestModuleIf::EPanic, panicCode );
		iNormalExitReason = EFalse;
	    }
	else
	    {
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
	    }
	return error;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SetExpectedEvents()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SetExpectedEvents( CStifItemParser& aItem )
    {
	FTRACE(FPrint(_L("CCommonTestClass::SetExpectedEvents")));
	TInt error = KErrNone;
	TInt event=0;
	while ( KErrNone == aItem.GetNextInt(event))
	    {
		AddExpectedEvent(static_cast<TMPXTestExpectedEvent>(event), 0); // Default timeout value
	    }
	return error;
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
    return ( CScriptBase* ) CCommonTestClass::NewL( aTestModuleIf );
    }

//  End of File
