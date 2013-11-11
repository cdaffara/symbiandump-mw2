/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base Class Scripted STIF cases
*
*/

// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "basetest.h"


// -----------------------------------------------------------------------------
// CBaseTest::CCollectionHelperTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CBaseTest::CBaseTest( CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ), 
        iTestModuleIf(aTestModuleIf)
    {   
    }
    
// Destructor
EXPORT_C CBaseTest::~CBaseTest()
    { 
    // Delete logger
    delete iLog; 
    delete iTimeoutController;
    }

// -----------------------------------------------------------------------------
// CBaseTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
EXPORT_C void CBaseTest::ConstructL()
{
    iTimeoutController = CSimpleTimeout::NewL (this, iLog);
}

// -----------------------------------------------------------------------------
// CBaseTest::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
EXPORT_C TPtrC CBaseTest::EventName( TInt aKey )
{
    static TText* const badKeyword = (TText*)L"BadKeyword";
    static TText* const keywords[] = {
        (TText*)L"EColHandleCollectionMessage",
        (TText*)L"EColHandleOpen",
        (TText*)L"EColHandleAddFileComplete",
        (TText*)L"EColHandleRemove",
        (TText*)L"EColHandleFindAllComplete",
        (TText*)L"EPlaHandlePlaybackMessage1",
        (TText*)L"EPlaHandlePlaybackMessage2"
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
// CBaseTest::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
EXPORT_C void CBaseTest::AddExpectedEvent(TExpectedEvent event, TInt ms)
{
    FTRACE(FPrint(_L("CBaseTest::AddExpectedEvent")));
    iExpectedEvents.Append(event);
    TPtrC eventName = EventName(event);
    iLog->Log(_L("Adding expected event:(0x%02x)%S Total=%d"), event, 
            &eventName, iExpectedEvents.Count() );

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
// CBaseTest::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
EXPORT_C TBool CBaseTest::RemoveExpectedEvent(TExpectedEvent aEvent)
{
    FTRACE(FPrint(_L("CBaseTest::RemoveExpectedEvent")));
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
// CBaseTest::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
EXPORT_C void CBaseTest::RemoveAllExpectedEvents()
{
    FTRACE(FPrint(_L("CBaseTest::RemoveAllExpectedEvents")));
    iLog->Log(_L("Removing all expected events"));

    iExpectedEvents.Reset();
    iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CBaseTest::ProcessEvent
// Process events.
// -----------------------------------------------------------------------------
EXPORT_C void CBaseTest::ProcessEvent(TExpectedEvent aEvent, TInt aError)
{
    FTRACE(FPrint(_L("CBaseTest::ProcessExpectedEvent")));
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
        iLog->Log(_L("[Error] Event: (0x%02x)%S return with error code=%d"), 
                aEvent, &nameEvent, aError);
        
        if (iExpectedEvents.Count() != 0 )
        {
            RemoveExpectedEvent(aEvent);
        }
        
        iTimeoutController->Cancel();
        Signal(KErrCallbackErrorCode);
    }
}

// -----------------------------------------------------------------------------
// CBaseTest::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------
EXPORT_C void CBaseTest::HandleTimeout(TInt aError)
{
    FTRACE(FPrint(_L("CBaseTest::HandleTimeout")));
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
// CBaseTest::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
EXPORT_C TInt CBaseTest::SetTimeout( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CBaseTest::SetTimeout")));
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
EXPORT_C TInt CBaseTest::SetAllowedPanic( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CBaseTest::SetAllowedPanic")));
    TInt error = KErrNone;
    TInt panicCode;
    TPtrC panicType;
    if (  ( KErrNone == aItem.GetNextString(panicType) ) &&
    ( KErrNone == aItem.GetNextInt(panicCode) )  )
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
// CBaseTest::SetExpectedEvents()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
EXPORT_C TInt CBaseTest::SetExpectedEvents( CStifItemParser& aItem )
{
    FTRACE(FPrint(_L("CBaseTest::SetExpectedEvents")));
    TInt error = KErrNone;
    TInt event=0;
    while ( KErrNone == aItem.GetNextInt(event))
    {
        AddExpectedEvent(static_cast<TExpectedEvent>(event), 0); // Default timeout value
    }
    return error;
}
