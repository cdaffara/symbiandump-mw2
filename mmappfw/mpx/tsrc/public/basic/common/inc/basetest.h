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
* Description:  Base Class for Scripted Tests
*
*/

#ifndef BASETEST_H
#define BASETEST_H

#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include "TimeoutController.h"
#include "debug.h"


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KExpectedFrequencyChange = -2003;        // Is not finding more than one station
const TInt KErrTimeoutController = -2007;       // Unexpected notification

const TInt KSmallTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLargeTimeout = 100000;


_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");

_LIT( KTestFile, "c:\\Blackbird.aac");
_LIT( KTestFileMimeType, "audio/aac" );
_LIT( KRenameOldUri, "c:\\Blackbird.aac");
_LIT( KRenameNewUri, "c:\\data\\Blackbird.aac");

/**
*  CBaseTest class for STIF Test Framework TestScripter.
*
*/
class CBaseTest : public CScriptBase, public MTimeoutObserver
    {
    public:
        
        // Update EventName() whenever you update this
        enum TExpectedEvent
        {
            EColHandleCollectionMessage,
            EColHandleOpen,
            EColHandleAddFileComplete, 
            EColHandleRemove,
            EColHandleFindAllComplete,
            
            EPlaHandlePlaybackMessage1,
            EPlaHandlePlaybackMessage2
        };
        
        IMPORT_C CBaseTest( CTestModuleIf& aTestModuleIf );        
        IMPORT_C virtual ~CBaseTest();
        
        IMPORT_C virtual void ConstructL();
        
    public:

        // From MTimeoutObserver

        /**
        * @since 3.2
        * @param none
        * Review if all the expected events have ocurred once the time is over
        */
        IMPORT_C virtual void HandleTimeout(TInt error);
        
    protected:
        
        /**
        * Maps a event with a descriptor with its name
        * @since S60 3.2
        */
        IMPORT_C virtual TPtrC EventName( TInt aKey );
        
        /**
        * Set an event as expected and set default timeout
        * @since S60 3.2
        */
        IMPORT_C virtual void AddExpectedEvent(TExpectedEvent event, TInt ms);


        /**
        * Unset an event as expected
        * @since S60 3.2
        */
        IMPORT_C virtual TBool RemoveExpectedEvent(TExpectedEvent event);


        /**
        * @since S60 3.2
        * @param none
        * Removes all expected events
        */
        IMPORT_C virtual void RemoveAllExpectedEvents();

        /**
        * Verify that the event was expected, removes it from the list
        * Signal the TestScripter with the returned error code
        * @since S60 3.2
        */
        IMPORT_C virtual void ProcessEvent(TExpectedEvent aEvent, TInt aError);

        /*
        * Test methods are listed below.
        */

        /**
        * Sets a timeout different since the default
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        IMPORT_C virtual TInt SetTimeout( CStifItemParser& aItem );

        /**
        *
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        IMPORT_C virtual TInt SetExpectedEvents( CStifItemParser& aItem );

        /**
        *
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        IMPORT_C virtual TInt SetAllowedPanic( CStifItemParser& aItem );
        
    protected:
        
        // reference to TestModuleIf
        CTestModuleIf& iTestModuleIf;
    
        // Active object with a timer to timeout the test case
        CSimpleTimeout * iTimeoutController;

        // Indicates if the test case use a normal exit reason
        TBool iNormalExitReason;

        // List of expected events
        RArray<TExpectedEvent> iExpectedEvents;

        // List of notification event (by the callback)
        RArray<TExpectedEvent> iOcurredEvents;
    };

#endif // BASETEST_H

