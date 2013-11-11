/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Call Audio Control
*
*/



#ifndef CCallAudioControlTestClass_H
#define CCallAudioControlTestClass_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32svr.h>
#include <e32base.h>
#include <e32cons.h>
#include "CallAudioControl.h"
#include "RoutingObserver.h"
#include "UplinkGainObserver.h"
#include "DownlinkVolumeObserver.h"

#include "TimeoutController.h"


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting
const TInt KErrTimeoutController = -2007;       // Unexpected notification

const TInt KShortTimeout = 2000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 10000;




_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");


_LIT( KTagNotActive, "NotActive");
_LIT( KTagNone, "None");
_LIT( KTagHandset, "Handset");
_LIT( KTagLoudspeaker, "Loudspeaker");
_LIT( KTagWiredAudioAccessory, "WiredAudioAccessory");
_LIT( KTagBTAudioAccessory, "BTAudioAccessory");
_LIT( KTagTTY, "TTY");
_LIT( KTagTrue, "True");
_LIT( KTagFalse, "False");
_LIT( KTagMin, "Min");
_LIT( KTagMax, "Max");
_LIT( KTagMedium, "Medium");


// MACROS
//#define ?macro ?macro_def
// Logging path
#ifdef __WINSCW__
_LIT( KCallAudioControlTestClassLogPath, "\\logs\\testframework\\" );
#else
_LIT( KCallAudioControlTestClassLogPath, "e:\\testing\\Log\\" );
#endif

// Log file
_LIT( KCallAudioControlTestClassLogFile, "CCallAudioControlTestClass.txt" );

// FORWARD DECLARATIONS
class CCallAudioControlTestClass;



// CLASS DECLARATION

/**
*  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since Series60_3_2
*/
NONSHARABLE_CLASS(CCallAudioControlTestClass) :
                        public CScriptBase,
                        public MTimeoutObserver,
                        public CDownlinkVolumeObserver,
                        public CUplinkGainObserver,
                        public CRoutingObserver


    {


    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCallAudioControlTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCallAudioControlTestClass();

        enum TCallControlExpectedEvent
        {
            EDownlinkVolumeChanged,
         //   EDownlinkMuteStateChanged,
            EUplinkMuteStateChanged,
            EOutputChanged,
            ESetOutputComplete

        };

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

// For RoutingObserver:
	void RoutingEvent(CCallAudioControl& aCallAudioControl, TUint aEvent);
	void RoutingEvent(CCallAudioControl& aCallAudioControl, TUint aEvent, TInt aError);

	// For UplinkGainObserver:
	void UplinkGainEvent(CCallAudioControl& aCallAudioControl, TUint aEvent);

	// For DownlinkVolumeObserver:
	void DownlinkVolumeEvent(CCallAudioControl& aCallAudioControl, TUint aEvent);



        // From MTimeoutObserver

        /**
        * @since ?Series60_version
        * @param none
        * Review if all the expected events have ocurred once the time is over
        */
        void HandleTimeout(TInt error);



    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CCallAudioControlTestClass( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below.
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */

        /**
        * Set an event as expected and set default timeout
        * @since ?Series60_version
        */
        void AddExpectedEvent(TCallControlExpectedEvent event, TInt ms);


        /**
        * Unset an event as expected
        * @since ?Series60_version
        */
        TBool RemoveExpectedEvent(TCallControlExpectedEvent event);


        /**
        * @since ?Series60_version
        * @param none
        * Removes all expected events
        */
        void RemoveAllExpectedEvents();

        /**
        * Verify that the event was expected, removes it from the list
        * Signal the TestScripter with the returned error code
        * @since ?Series60_version
        */
        void ProcessEvent(TCallControlExpectedEvent aEvent, TInt aError);

        /**
        * Maps a event with a descriptor with its name
        * @since ?Series60_version
        */
        TPtrC EventName( TInt aKey );

        /*
        * Test methods are listed below.
        */

        /**
        * Sets a timeout different since the default
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetTimeout( CStifItemParser& aItem );

        /**
        *
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetExpectedEvents( CStifItemParser& aItem );

        /**
        *
        * @since Series60_3_2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt SetAllowedPanic( CStifItemParser& aItem );

        /**
        * Connect to the default client to the server
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */

        TInt ExampleL( CStifItemParser& aItem );

        TInt CallAudioControl( CStifItemParser& aItem );
        TInt AppendRoutingObserver( CStifItemParser& aItem );
        TInt AppendUplinkGainObserver( CStifItemParser& aItem );
        TInt AppendDownlinkVolumeObserver( CStifItemParser& aItem );
        TInt RemoveRoutingObserver( CStifItemParser& aItem );
        TInt RemoveUplinkGainObserver( CStifItemParser& aItem );
        TInt RemoveDownlinkVolumeObserver( CStifItemParser& aItem );
        TInt GetAvailableOutputs( CStifItemParser& aItem );
        TInt SetOutput( CStifItemParser& aItem );
        TInt GetOutput( CStifItemParser& aItem );
        TInt PreviousOutput( CStifItemParser& aItem );
        TInt SetDownlinkVolume( CStifItemParser& aItem );
        TInt GetMinDownlinkVolume( CStifItemParser& aItem );
        TInt GetDownlinkVolume( CStifItemParser& aItem );
        TInt GetMaxDownlinkVolume( CStifItemParser& aItem );
        TInt GetUplinkMute( CStifItemParser& aItem );
        TInt SetUplinkMute( CStifItemParser& aItem );

        void DisplayOutput(CCallAudioControl::TAudioOutput aOutput);
        CCallAudioControl::TAudioOutput iCurrentAudioOutput;
				CCallAudioControl::TAudioOutput iPreviousAudioOutput;

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data

    	// reference to TestModuleIf
    	CTestModuleIf& 				iTestModuleIf;




	    // Active object with a timer to timeout the test case
	    CSimpleTimeout * iTimeoutController;

	    // Indicates if the test case use a normal exit reason
	    TBool iNormalExitReason;

	    // List of expected events
	    RArray<TCallControlExpectedEvent> iExpectedEvents;

	    // List of notification event (by the callback)
	    RArray<TCallControlExpectedEvent> iOcurredEvents;

	    CActiveSchedulerWait* iActive;
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
      CCallAudioControl* iCallAudioControl;


    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // CCallAudioControlTestClass_H

// End of File
