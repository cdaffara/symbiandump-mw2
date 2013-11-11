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
* Description:  Declaration of CCCETester class.
*
*/



#ifndef CCCETESTER_H
#define CCCETESTER_H

// INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <ccce.h>
#include <mcceextensioninterface.h>
#include <mccedtmfinterface.h>
#include "ccetesterdefs.h"
#include "cdelay.h"
#include "ccetesterdefs.h"
#include "ctestobserver.h"
#include "ctestcallobserver.h"
#include "ctestdtmfobserver.h"
#include "ceventstorage.h"
#include "ctestssobserver.h"
#include "ctestextobserver.h"

// CONSTANTS
// Call states

_LIT( KStateIdle, "idle" );
_LIT( KStateRinging, "ringing" );
_LIT( KStateConnecting, "connecting" );
_LIT( KStateConnected, "connected" );
_LIT( KStateDisconnecting, "disconnecting" );
_LIT( KStateHold, "hold" );
_LIT( KStateTransferring, "transferring" );
_LIT( KStateForwarding, "forwarding" );
_LIT( KStateQueued, "queued" );
_LIT( KStateAnswering, "answering" );
_LIT( KStateDialling, "dialling" );
_LIT( KStateDisconnectingWithInband, "disconnectingwithinband" );
_LIT( KStateReconnectPending, "reconnectpending" );
_LIT( KStateWaitingAlternatingCallSwitch, "waitingalternatingcallswitch" );

// Call direction
_LIT( KDirectionMo, "MoCall" );
_LIT( KDirectionMt, "MtCall" );

// MACROS

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

class CCCETester;
class CTestConfCallObserver;
class CTestObserver;
class CTestSsObserver;
class CTestExtObserver;
class CTestMediator;

// DATA TYPES

// CLASS DECLARATION

/**
*  CCCCETester test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCCETester) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCCETester* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCCETester();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        CCCETester( CTestModuleIf& aTestModuleIf );

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

		TInt InitL( TFileName& aIniFile, TBool aFirstTime );
		
		void CheckCallIdL( TInt aCallID, TBool aIsNewCallId=EFalse );
		void CheckDtmf();
		void CheckExtIfL();
		void CheckConferenceCall();
		void SetEventName( TEvent& aEvent, const TDesC* aEventName );
		TInt SetPluginCommand( const TDesC& aPluginCommand );
		TBool IsNumber(TPtrC& aInput);
		TInt GetValue(TPtrC& aInput);
		TInt CheckPluginCommand( TPtrC& aItem );
		TInt UnhandledEvents();
		void RemoveComma( TPtrC& aEventParam );
		TBool CheckComma( TPtrC aEventParam ) const;
						
	public:
	
		TInt InsertCall( const MCCECall& aCall );
		
		TInt InsertCallObserver( const CTestCallObserver& newCallObs );
       
        /////////////////////////////////////////////////////////////////////
		// SCRIPTER TEST METHODS, ADD NEW DECLARATIONS HERE
		/////////////////////////////////////////////////////////////////////
		
	private:

        virtual TInt WaitEvent(CStifItemParser& aItem);
        virtual TInt WaitEvents(CStifItemParser& aItem);
        virtual TInt Delay( CStifItemParser& aItem );
        virtual TInt VerifyTest(CStifItemParser& aItem);
        virtual TInt SendPluginRequest( CStifItemParser& aItem );
        virtual TInt ConfigurePlugin( CStifItemParser& aItem );
        virtual TInt InitIncomingCall( CStifItemParser& aItem );
        virtual TInt ResetCallArray( CStifItemParser& aItem );
  		virtual TInt SetVerificationMode(CStifItemParser& aItem);
        virtual TInt Create_CCE( CStifItemParser& aItem );
        virtual TInt Create_CCE_2( CStifItemParser& aItem );
        virtual TInt Delete_CCE( CStifItemParser& aItem );
        virtual TInt OpenNewCall( CStifItemParser& aItem );
        virtual TInt Release(CStifItemParser& aItem);
        virtual TInt Dial(CStifItemParser& aItem);
        virtual TInt HangUp(CStifItemParser& aItem);
      	virtual TInt Hold( CStifItemParser& aItem );
      	virtual TInt Resume( CStifItemParser& aItem );
      	virtual TInt Swap( CStifItemParser& aItem );
  		virtual TInt Answer( CStifItemParser& aItem );
  		virtual TInt InitializeEmergencyCall( CStifItemParser& aItem );
  		virtual TInt DialEmergencyCall( CStifItemParser& aItem );
      	virtual TInt StopDtmfTone( CStifItemParser& aItem );
      	virtual TInt StartDtmfTone( CStifItemParser& aItem );
     	virtual TInt SendDtmfToneString( CStifItemParser& aItem );
      	virtual TInt ContinueStringSending( CStifItemParser& aItem );
      	virtual TInt CancelDtmfStringSending( CStifItemParser& aItem );      	
  		virtual TInt AddToConference( CStifItemParser& aItem );
  		virtual TInt RemoveFromConference( CStifItemParser& aItem );
  		virtual TInt SwapConference( CStifItemParser& aItem );
  		virtual TInt GoOneToOne( CStifItemParser& aItem );
  		virtual TInt ReleaseConference( CStifItemParser& aItem );
  		virtual TInt HangupConference( CStifItemParser& aItem );
  		virtual TInt UnattendedTransfer( CStifItemParser& aItem );
  		virtual TInt AttendedTransfer( CStifItemParser& aItem );
  		virtual TInt AcceptTransfer( CStifItemParser& aItem );
  		virtual TInt GetForwardAddressChoices( CStifItemParser& aItem );
  		virtual TInt ForwardToAddress( CStifItemParser& aItem );
  		virtual TInt TerminateAllButRingingAndQueuedCalls( CStifItemParser& aItem );
  		virtual TInt GetAirTimeDuration( CStifItemParser& aItem );
  		virtual TInt TerminateCall( CStifItemParser& aItem );
  		virtual TInt RequestExtIfCommand( CStifItemParser& aItem );
  		virtual TInt GetMobileCallInfo( CStifItemParser& aItem );
  		virtual TInt TerminateAllCalls( CStifItemParser& aItem );
  		virtual TInt MiscTests( CStifItemParser& aItem );
  		virtual TInt ReplaceActive(CStifItemParser& aItem);
  		virtual TInt OOM_Tests_1( CStifItemParser& aItem );
  		virtual TInt ExpectMediatorEvent( CStifItemParser& aItem );
  		virtual TInt VerifyMediatorEvents( CStifItemParser& aItems );
  		virtual TInt CallIndexForCall( CStifItemParser& aItems );

		/////////////////////////////////////////////////////////////////////
		
	public: // Data
	
        // iCallArray.Find cannot be used if private
        // Call array
        RPointerArray<MCCECall> iCallArray;
    
        // Call observer array
        RPointerArray <CTestCallObserver> iCallObserverArray;
	
		MCCECall* iPluginInitiatedCall;	
		
	private: // Data

		CCCE* iCceTest;
		
		MCCEConferenceCall* iConferenceCall;
		
		MCCEDtmfInterface* iDtmfTest;
		MCCEExtensionInterface* iExtIfTest;
		
		// Observers
		CTestObserver* iObserver;
		const CTestConfCallObserver* iConfCallObserver;
		CTestDtmfObserver* iDtmfObserver;
		CTestSsObserver* iSsObserver;
		CTestExtObserver* iExtIfObserver;
		CTestMediator* iMediatorObserver;
		
		// Holds event received from Observers
		CEventStorage* iEventStorage;
		
		CDelay* iTestTimer;
		
		// Forward address choices
		CDesC8Array* iForwardAddressArray;
				
		TInt iTestStep;

		TInt iSkipVerification;
		
		CCCECallParameters* iCallParams;
		
    };

#endif      // CCCETESTER_H

// End of File
