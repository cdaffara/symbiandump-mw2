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
* Description:  STIF test scripter methods
*
*/


// Include files

#include <escapeutils.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <icmapi.h>
#include <e32property.h>		// RProperty
#include <mccecall.h> //MCCECall
#include <rmmcustomapi.h>  // TCustomCallParams
#include <cccecallparameters.h>
#include <ccpdefs.h>
#include "mcceconferencecall.h"
#include "mccedtmfinterface.h"
#include "cccetester.h"
#include "ctestcallobserver.h"
#include "ctestdtmfobserver.h"
#include "ccetestconfigurations.h"
#include "ccetesterlogger.h"
#include "ctestmediator.h"

// Forward declarations

class CCCETester;
class MCCECall;

// Constants

_LIT( KCharSpace, " " );
_LIT(KTransferTarget, "TransferTarget@test.com");
_LIT(KNullID, "NULL");
_LIT(KCCECallTypeCSVoice, "ECCECallTypeCSVoice");
_LIT(KCCECallTypeVideo, "ECCECallTypeVideo");
_LIT(KCCECallTypeFax, "ECCECallTypeFax");
_LIT(KCCECallTypeData, "ECCECallTypeData");
_LIT(KCCECallTypePS, "ECCECallTypePS");
_LIT(KCCELineTypePrimary, "ECCELineTypePrimary");
_LIT(KCCELineTypeAux, "ECCELineTypeAux");
_LIT(KCallName, "Test Address");
_LIT8(KDataCaps, "caps" );
_LIT(KKeywordInband, "inband");

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCCETester::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCCETester::Delete()
    {
	/**
	iCallArray.Reset();
	iCallObserverArray.ResetAndDestroy();
	**/
    }



// -----------------------------------------------------------------------------
// CCCETester::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCCETester::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
      
		// ADD NEW TEST FUNCTIONS HERE (Identifier for script and function name)
		////////////////////////////////////////////////////////////////////////
          	      	
      	ENTRY( "WaitEvent", CCCETester::WaitEvent ),
      	ENTRY( "WaitEvents", CCCETester::WaitEvents ),
		ENTRY( "Delay", CCCETester::Delay ),
		ENTRY( "VerifyTest", CCCETester::VerifyTest ),
		ENTRY( "SendPluginRequest", CCCETester::SendPluginRequest ),
	    ENTRY( "ConfigurePlugin", CCCETester::ConfigurePlugin ),
        ENTRY( "InitIncomingCall", CCCETester::InitIncomingCall ),
	    ENTRY( "ResetCallArray", CCCETester::ResetCallArray ),
	    ENTRY( "SetVerificationMode", CCCETester::SetVerificationMode ),
        ENTRY( "Create_CCE", CCCETester::Create_CCE ),
        ENTRY( "Create_CCE_2", CCCETester::Create_CCE_2 ),
        ENTRY( "Delete_CCE", CCCETester::Delete_CCE ),
        ENTRY( "OpenNewCall", CCCETester::OpenNewCall ),
        ENTRY( "Release", CCCETester::Release ),
        ENTRY( "Dial", CCCETester::Dial ),
        ENTRY( "HangUp", CCCETester::HangUp ),
        ENTRY( "Hold", CCCETester::Hold ),
        ENTRY( "Resume", CCCETester::Resume ),
        ENTRY( "Swap", CCCETester::Swap ),
		ENTRY( "Answer", CCCETester::Answer ),
	    ENTRY( "InitializeEmergencyCall", CCCETester::InitializeEmergencyCall ),
        ENTRY( "DialEmergencyCall", CCCETester::DialEmergencyCall ),
        ENTRY( "StopDtmfTone", CCCETester::StopDtmfTone ),
      	ENTRY( "StartDtmfTone", CCCETester::StartDtmfTone ),
     	ENTRY( "SendDtmfToneString", CCCETester::SendDtmfToneString ),
      	ENTRY( "ContinueStringSending", CCCETester::ContinueStringSending ),
      	ENTRY( "CancelDtmfStringSending", CCCETester::CancelDtmfStringSending ),
		ENTRY( "AddToConference", CCCETester::AddToConference ),
		ENTRY( "RemoveFromConference", CCCETester::RemoveFromConference ),
		ENTRY( "SwapConference", CCCETester::SwapConference ),
		ENTRY( "GoOneToOne", CCCETester::GoOneToOne ),
		ENTRY( "ReleaseConference", CCCETester::ReleaseConference ),
		ENTRY( "HangupConference", CCCETester::HangupConference ),
		ENTRY( "UnattendedTransfer", CCCETester::UnattendedTransfer ),
		ENTRY( "AttendedTransfer", CCCETester::AttendedTransfer ),
		ENTRY( "AcceptTransfer", CCCETester::AcceptTransfer ),
		ENTRY( "GetForwardAddressChoices", CCCETester::GetForwardAddressChoices ),
		ENTRY( "ForwardToAddress", CCCETester::ForwardToAddress ),
		ENTRY( "TerminateCall", CCCETester::TerminateCall ),
		ENTRY( "TerminateAllCalls", CCCETester::TerminateAllCalls ),
		ENTRY( "TerminateAllButRingingAndQueuedCalls", CCCETester::TerminateAllButRingingAndQueuedCalls ),
		ENTRY( "GetAirTimeDuration", CCCETester::GetAirTimeDuration ),
		ENTRY( "GetMobileCallInfo", CCCETester::GetMobileCallInfo ),
		ENTRY( "RequestExtIfCommand", CCCETester::RequestExtIfCommand ),
		ENTRY( "MiscTests", CCCETester::MiscTests ),
		ENTRY( "ReplaceActive", CCCETester::ReplaceActive ),
		ENTRY( "OOM_Tests_1", CCCETester::OOM_Tests_1 ),
		ENTRY( "ExpectMediatorEvent", CCCETester::ExpectMediatorEvent ),
		ENTRY( "VerifyMediatorEvents", CCCETester::VerifyMediatorEvents ),
		ENTRY( "CallIndexForCall", CCCETester::CallIndexForCall ),
		
		////////////////////////////////////////////////////////////////////////

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }



/*-----------------------------------------------------------------------------
    Function:	WaitEvent

    Parameters:	1. Integer (OPTIONAL), callID of the call from the event is expected,
                   omitted if there is no call associated
    			2. String, the name of the event expected
    			3. Integer (OPTIONAL), DTMF error
    			4. Integer (OPTIONAL), DTMF tone
------------------------------------------------------------------------------*/

TInt CCCETester::WaitEvent(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::WaitEvent called");
	
#ifdef __DISABLE_EVENT_VERIFICATION
	iTestTimer->Start(KDelayLength);
	return KErrNone;
#endif
		
    TInt callID = 0;
    TEvent expectedEvent;
      
    TPtrC eventParam;
    
	__ASSERT_ALWAYS( aItem.GetNextString(eventParam) == KErrNone, User::Panic( _L("WaitEvent, Error in parameter: callID/eventname"), -6) );
	
	// Read and set callID parameter. If parameter is not a number, callID will be set to zero (not call specific events)
	if ( IsNumber(eventParam) )
		{
		// The parameter was a number, get value
		callID = (TInt)GetValue(eventParam);
		
		// Check callID
		if ( callID < 0 || callID > iCallArray.Count() )
			{
			CCETESTERLOG2("[CCETEST] WaitEvent, callID was %d, should be between 0 and %d", callID, iCallArray.Count() );
			return KErrArgument;
			}
		
		// Read event name
		__ASSERT_ALWAYS( aItem.GetNextString(eventParam) == KErrNone, User::Panic( _L("WaitEvent, Error in parameter: commandName"), -6) );
		}
		
	expectedEvent.iCallID = callID;
		
	// Convert the keyword in script to event ID
	SetEventName( expectedEvent, &eventParam );
		
	/* DTMF event contains also Error and Tone parameters
	*/
	if ( expectedEvent.iEventType == CEventStorage::EDtmfEvent )
		{
		TInt dtmfParam1;
		__ASSERT_ALWAYS( aItem.GetNextInt(dtmfParam1) == KErrNone, User::Panic( _L("WaitEvent, error in parameter: dtmfParam1"), -6) );
		expectedEvent.iParam1 = dtmfParam1;
		
		TInt dtmfParam2;
		__ASSERT_ALWAYS( aItem.GetNextInt(dtmfParam2) == KErrNone, User::Panic( _L("WaitEvent, error in parameter: dtmfParam2"), -6) );
		expectedEvent.iParam2 = dtmfParam2;
		}
	else if ( expectedEvent.iEventType == CEventStorage::ECallObserverHandleTransfer )
		{
		TInt transferResponse;
		__ASSERT_ALWAYS( aItem.GetNextInt(transferResponse) == KErrNone, User::Panic( _L("WaitEvent, error in parameter: transferResponse"), -6) );
		__ASSERT_ALWAYS( transferResponse == 0 || transferResponse == 1, User::Panic( _L("WaitEvent, error in parameter: transferResponse"), -6) );
		expectedEvent.iParam1 = transferResponse;
		}
		
	/* Conference Call Removed and Added events have referenced call as parameter 1
	*/
	if ( expectedEvent.iEventType == CEventStorage::EConferenceCallEvent 
	     && ( expectedEvent.iEventID == MCCEConferenceCallObserver::ECCEConferenceCallAdded
	       || expectedEvent.iEventID == MCCEConferenceCallObserver::ECCEConferenceCallRemoved )
	   )
	 	{
	 	TInt referencedCall;
	 	__ASSERT_ALWAYS( aItem.GetNextInt(referencedCall) == KErrNone, User::Panic( _L("WaitEvent, error in parameter: referencedCall"), -6) );
	 	expectedEvent.iParam1 = referencedCall;
	 	}
	 	
	/* Call Error Occurred has error code as parameter 1
	*/
	if ( expectedEvent.iEventType == CEventStorage::ECallObserverErrorOccurred )
		{
		TInt errorCode;
	 	__ASSERT_ALWAYS( aItem.GetNextInt(errorCode) == KErrNone, User::Panic( _L("WaitEvent, error in parameter: errorCode"), -6) );
	 	expectedEvent.iParam1 = errorCode;
		}
		
	/* Check if inband keyword is found and
	*  if defined change EventType to CallStateChangedWithInband
	*/
    TPtrC inband;
    if ( aItem.GetNextString(inband) == KErrNone )
    	{
    	// if InBand keyword is found, change Call Observer type
    	TBuf<KMaxKeywordLength> inbandLC(inband);
		inbandLC.LowerCase();
		TPtrC inbandKeyword(inbandLC);
    	
    	if ( inbandKeyword.CompareF(KKeywordInband) == 0 )
    		{
    		expectedEvent.iEventType = CEventStorage::ECallObserverCallStateChangedWithInband;
    		}
    	
    	//No more parameters after this
    	TPtrC check;
    	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("WaitEvent, Too many parameters"), -6) );
    	}
    		
	// Wait until a message is received
    iEventStorage->GetEvent();
	
	// Compare the result
	if ( iEventStorage->iReceivedEvents[iTestStep] == expectedEvent )
		{
		// Event was verified, move to the next slot
		CCETESTERLOG2("[CCETEST] CCCETester::Event %S verified, Step %d", &eventParam, iTestStep+1 );
		
		iTestStep++;
		return KErrNone;
		}
	else
		{
		// Event didn't match, test case failed. Print event information
		CCETESTERLOG("************************************************************************************" );
		
		CCETESTERLOG1("[CCETEST] CCCETester::Wrong event, test case failed in step %d",iTestStep+1 );
		CCETESTERLOG5("[CCETEST] CCCETester::Expected event: call=%d type=%d eventid=%d par1=%d par2=%d",expectedEvent.iCallID,
	                              expectedEvent.iEventType, expectedEvent.iEventID, expectedEvent.iParam1, expectedEvent.iParam2 );
		CCETESTERLOG5("[CCETEST] CCCETester::Received event: call=%d type=%d eventid=%d par1=%d par2=%d",
		              iEventStorage->iReceivedEvents[iTestStep].iCallID,
	                  iEventStorage->iReceivedEvents[iTestStep].iEventType,
	                  iEventStorage->iReceivedEvents[iTestStep].iEventID,
	                  iEventStorage->iReceivedEvents[iTestStep].iParam1,
	                  iEventStorage->iReceivedEvents[iTestStep].iParam2 );
		return KErrGeneral;
		}
	}
	
	
/*-----------------------------------------------------------------------------
    Function:	WaitEvents (Gets multiple events in any order)
    
    Parameters:	1. Integer (OPTIONAL), callID of the call from the event is expected, 
                   left out if there is no call associated
    			2. String, the name of the first expected event
    			3. Integer (OPTIONAL), callID of the call from the event is expected,
                   left out if there is no call associated
    			4. String, the name of the second exptected event
    			5.. (OPTIONAL) The rest of events in Integer and String pairs
------------------------------------------------------------------------------*/

TInt CCCETester::WaitEvents(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::WaitEvent called");
	
#ifdef __DISABLE_EVENT_VERIFICATION
	iTestTimer->Start(KDelayLength);
	return KErrNone;
#endif
	
    TEvent singleEvent;
    
    RArray<TEvent> scriptEvents;
        
    TPtrC eventParam;
                  
    /* Parse callID and Event/State from test script line
    *  If callID is not present, event is not call specific (use zero value)
    *  Read as many events as there are, store events to array
    */
    
	while ( aItem.GetNextString(eventParam) == KErrNone )
		{
		
		TInt callID = 0;
		
		if ( IsNumber(eventParam) )
			{
			// The parameter was a number, check range and set callID
			if ( callID <= iCallArray.Count() && callID <= KMaxAmountOfCalls )
				{
				callID = (TInt)GetValue(eventParam);
				
				// Event name
				__ASSERT_ALWAYS( aItem.GetNextString(eventParam) == KErrNone, User::Panic( _L("WaitEvents, Error in parameter: eventParam"), -6) );	
				}
			else
				{
				User::Panic( _L("WaitEvents, Wrong CallID"), KErrGeneral );
				}
			}
			
		RemoveComma( eventParam );
		
		singleEvent.iCallID = callID;
		
		// Convert the keyword in script to event ID
		SetEventName( singleEvent, &eventParam );
		
		/* Conference Call Removed and Added events have referenced call as parameter 1
		*/
		if ( singleEvent.iEventType == CEventStorage::EConferenceCallEvent 
	         && ( singleEvent.iEventID == MCCEConferenceCallObserver::ECCEConferenceCallAdded
	         || singleEvent.iEventID == MCCEConferenceCallObserver::ECCEConferenceCallRemoved )
	   	)
		 	{
		 	TInt referencedCall;
		 	__ASSERT_ALWAYS( aItem.GetNextInt(referencedCall) == KErrNone, User::Panic( _L("WaitEvents, error in parameter: referencedCall"), -6) );
		 	singleEvent.iParam1 = referencedCall;
		 	}
		
		TEvent* pEvent = &singleEvent;
		
		// Add to the array
		scriptEvents.AppendL( *pEvent );	
		}
		
	if ( scriptEvents.Count() == 0 )
		{
		// There were no events
		return KErrGeneral;
		}
		
	/* Verify the events
	*  If no event received yet, wait for it
	*  When event is received, the event/state list is
	*  checked through.
	*  - if event is not found: fail
	*  - if event is found, remove that event from list
	*/
	
    while ( scriptEvents.Count() > 0 )
		{
		TBool eventFound = EFalse;
		
		CCETESTERLOG2("EventsVerified=%d, EventReceived=%d", iTestStep, iEventStorage->iReceivedEvents.Count() );

	      // Wait for event
            iEventStorage->GetEvent();
			
		TInt eventIndex;
			
		// Check array
		for ( eventIndex=0; eventIndex<scriptEvents.Count() && eventFound==EFalse; eventIndex++ )
			{
			if ( iEventStorage->iReceivedEvents[iTestStep] == scriptEvents[eventIndex] )
				{
				// Event was verified, move to the next slot
				CCETESTERLOG5("[CCETEST] Verified event: call=%d type=%d eventid=%d par1=%d par2=%d",iEventStorage->iReceivedEvents[iTestStep].iCallID,
                iEventStorage->iReceivedEvents[iTestStep].iEventType, iEventStorage->iReceivedEvents[iTestStep].iEventID,
                iEventStorage->iReceivedEvents[iTestStep].iParam1, iEventStorage->iReceivedEvents[iTestStep].iParam2 );
				
				scriptEvents.Remove(eventIndex);
				
				if ( scriptEvents.Count() > 0 )
					{
					CCETESTERLOG("[CCETEST] Still waiting for events:");
					for ( TInt i=0; i<scriptEvents.Count(); i++)
						{
						CCETESTERLOG6("[CCETEST] %d --> call=%d type=%d eventid=%d par1=%d par2=%d",
		      	        i+1, scriptEvents[i].iCallID, scriptEvents[i].iEventType, scriptEvents[i].iEventID, scriptEvents[i].iParam1, scriptEvents[i].iParam2 );
						}
					}
				
				iTestStep++;
				
				eventFound = ETrue;
				}
			}
		
		if ( !eventFound )
			{
			CCETESTERLOG("************************************************************************************" );
			CCETESTERLOG1("[CCETEST] CCCETester::Wrong event, test case failed in step %d",iTestStep+1 );
			
			// Print expected events
			CCETESTERLOG("[CCETEST] CCCETester::Expected events");
			
			for ( TInt i=0; i<scriptEvents.Count(); i++)
				{
				CCETESTERLOG6("[CCETEST] %d --> call=%d type=%d eventid=%d par1=%d par2=%d", 
				        i+1,
				        scriptEvents[i].iCallID,
				        scriptEvents[i].iEventType,
				        scriptEvents[i].iEventID,
				        scriptEvents[i].iParam1,
				        scriptEvents[i].iParam2 );
				}
			
			// received event
			CCETESTERLOG5("[CCETEST] CCCETester::Received event: call=%d type=%d eventid=%d par1=%d par2=%d",
			        iEventStorage->iReceivedEvents[iTestStep].iCallID,
			        iEventStorage->iReceivedEvents[iTestStep].iEventType,
			        iEventStorage->iReceivedEvents[iTestStep].iEventID,
			        iEventStorage->iReceivedEvents[iTestStep].iParam1, 
			        iEventStorage->iReceivedEvents[iTestStep].iParam2 );
			
			return KErrGeneral;
			}
		}
		
	scriptEvents.Close();
	
	return KErrNone;
	}

/*-----------------------------------------------------------------------------
    Function:	Delay

    Parameters:	1. Integer, delay time in milliseconds (1/1000)
------------------------------------------------------------------------------*/

TInt CCCETester::Delay( CStifItemParser& aItem )
	{
    CCETESTERLOG("[CCETEST] CCCETester::Delay called");
    
    TInt delay;
    __ASSERT_ALWAYS( aItem.GetNextInt(delay) == KErrNone, User::Panic( _L("Delay, Error in parameter: delayParam"), -6) );
         
    //No more parameters
    TPtrC check;
    __ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Delay, Too many parameters"), -6) );
       
    iTestTimer->Start(delay);
	    
    CCETESTERLOG("[CCETEST] CCCETester::Delay finish");
    return KErrNone;
	}

/*-----------------------------------------------------------------------------
    Function: CCCETester::VerifyTest(CStifItemParser& aItem)

    Verify that there are no extra events
------------------------------------------------------------------------------*/
TInt CCCETester::VerifyTest( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::VerifyTest" );
    
    // No parameters
    TPtrC check;
    __ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Delay, Too many parameters"), -6) );
    
    // Check that test step index is equal to Event count
    
    // Wait for Release to complete
    iTestTimer->Start(200);
#ifdef __DISABLE_EVENT_VERIFICATION
    CCETESTERLOG("[CCETEST] *************************************************************************"));
    CCETESTERLOG("[CCETEST] EVENT VERIFICATION DISABLED !!!" )); 
    CCETESTERLOG("[CCETEST] *************************************************************************"));
    return KErrNone;
#endif
    
    if (iTestStep != iEventStorage->iReceivedEvents.Count() )
        {
        CCETESTERLOG("[CCETEST] Event count does not match, test failed" ); 
        CCETESTERLOG2("[CCETEST] Verified: %d, Received: %d", iTestStep, iEventStorage->iReceivedEvents.Count() );
        
        return KErrGeneral;
        }
    CCETESTERLOG("[CCETEST] CCCETester::VerifyTest done ok" );
    
    return KErrNone;
    }

 /*-----------------------------------------------------------------------------
	Function:   SendPluginRequest

    Parameters:	1. Integer: Command ID
    			2. Integer (OPTIONAL): Parameter 2
------------------------------------------------------------------------------*/

TInt CCCETester::SendPluginRequest( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::SendPluginRequest()");
	
	TInt pluginID = 0;
	
	TBuf<128> commandString;  // Command string to be sent to plugin
		
	TPtrC item;
	__ASSERT_ALWAYS( aItem.GetNextString(item) == KErrNone, User::Panic( _L("SendPluginRequest, Error in parameter: pluginNumber"), -6) );
	
	if ( !IsNumber(item) )
		{
		// There wasn't plugin number, set pluginID to 1 and use pluginNumber as commandName
		pluginID = 1;
		}
	else
		{
		// Parameter was number, convert to integer
		pluginID = (TInt)GetValue(item);
		
		// Read command
		__ASSERT_ALWAYS( aItem.GetNextString(item) == KErrNone, User::Panic( _L("SendPluginRequest, Error in parameter: commandName"), -6) );
		}
		
	if ( CheckPluginCommand( item ) != KErrNone )
		{
		return KErrArgument;
		}
	
	// Set item 1 (Plugin command)
	commandString.Copy(item);
	
	// Read parameter 1 (optional)
	if ( aItem.GetNextString(item) == KErrNone )
		{
		// Add space and append to string
		commandString.Append( KCharSpace );
		commandString.Append( item );	
		
		// Read and parameter 2, optional
		if ( aItem.GetNextString(item) == KErrNone )
			{
			// Add space and append
			commandString.Append( KCharSpace );
			commandString.Append( item );
			
			// Read and parameter 3, optional
			if ( aItem.GetNextString(item) == KErrNone )
				{
				// Add space and append
				commandString.Append( KCharSpace );
				commandString.Append( item );
			
				// No more parameters
				TPtrC check;
				__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("SendPluginRequest, Too many parameters"), -6) );
				}
			}
		}

	TInt result = 0;
	
	switch (pluginID)
		{
		case 1:
	
			result = RProperty::Set( KPSUidEventRequest1, 
            	                     KPropertyKeyEventIdentifier, 
                	                 commandString );
            break;
            
		case 2:
		
			result = RProperty::Set( KPSUidEventRequest2,
            	                     KPropertyKeyEventIdentifier, 
                	                 commandString );
            break;
            
        default:
        
        	User::Panic( _L("SendPluginRequest, error in pluginID"), -6);
        	break;
		}
		
	CCETESTERLOG1("[CCETEST] CCCETester::SendPluginRequest, finished with %d", result );
                                  
	return result;
	}
	
/*-----------------------------------------------------------------------------
    Function:   ConfigurePlugin

    Send configuration information to plug-ins. Used extension interface for the plugin 
    to send the information.
    Usage: 
    1) Select operation and as first parameter take the TCCETesterCfg enum name as string
    2) Check documentation for operation from the header file and give the needed parameters
    
    Parameters: 1. Integer: ServiceId to identify tagret plug-in.
                2. String:  Operation to perform at plug-in side
------------------------------------------------------------------------------*/
TInt CCCETester::ConfigurePlugin( CStifItemParser& aItem )
    {
    TRAPD( e1, CheckExtIfL() );
    if( e1 )
        {
        // Must not return retval, which could be KErrNone but expected something else
        return KErrGeneral;
        }
    
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    // Parameter 1: Service ID
    TInt serviceId;
    __ASSERT_ALWAYS( aItem.GetNextInt(serviceId) == KErrNone, User::Panic( _L("ConfigurePlugin, Error in parameter: serviceId"), -6) );

    // Parameter 2: Operation to perform 
    TPtrC cfgstr;
    __ASSERT_ALWAYS( aItem.Remainder(cfgstr) == KErrNone, User::Panic( _L("ConfigurePlugin, Error in parameter: operation"), -6) );

    HBufC8* str = EscapeUtils::ConvertFromUnicodeToUtf8L( cfgstr );

    // param 2 ignored
    TInt retval = iExtIfTest->RequestCommand( serviceId, 0, *str );

    delete str;

    return KErrNone;
    }


/*-----------------------------------------------------------------------------
	Function:   InitIncomingCall

    Parameters:	Integer: pluginID (Optional, default value 1)
------------------------------------------------------------------------------*/
TInt CCCETester::InitIncomingCall( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::InitIncomingCall");
	
	TInt pluginID;
	
	//TPtrC pluginNumber;
	//TPtrC commandName;
	if ( aItem.GetNextInt(pluginID) != KErrNone )
		{
		// No parameter found
		pluginID = 1;
		}
	else
		{
		// Parameter was given
		
		// No more parameters
		TPtrC check;
		__ASSERT_ALWAYS( aItem.GetNextString(check) == KErrNotFound, User::Panic( _L("SendPluginRequest, Too many parameters"), -6) );
		}
		
	TICMUIDValueType pluginuid = 0;
		
	switch (pluginID)
		{
		
		case 1:
			pluginuid = KImplementationUidPlugin1;
			break;
		
		case 2:
			pluginuid = KImplementationUidPlugin2;
			break;
		
		default:
			User::Panic( _L("InitIncomingCall, Error in parameter: pluginID"), -6);
			break;
		}
		
	TInt result = RProperty::Set( KPSUidICMIncomingCall, 
                                  KPropertyKeyICMPluginUID, 
                                  pluginuid );
		
	return result;
	}

// -----------------------------------------------------------------------------
// CCCETester::ResetCallArray
// -----------------------------------------------------------------------------
TInt CCCETester::ResetCallArray( CStifItemParser& /*aItem*/ )
    {
    iCallArray.Reset();
    iCallObserverArray.ResetAndDestroy();
    
    return KErrNone;
    }

	
// -----------------------------------------------------------------------------
// CCCETester::SetVerificationMode
//
// Parameter: Boolean/Int: - To disable event verification (effective during the 
//                           script where used) use 0
//                         - To enable verification again (in the same script) 
//                           use value 1
// -----------------------------------------------------------------------------
TInt CCCETester::SetVerificationMode(CStifItemParser& aItem)
    {
    TInt value;
    __ASSERT_ALWAYS( aItem.GetNextInt(value) == KErrNone && ( value==0 || value==1 ), User::Panic( _L("SetVerificationMode, Error in parameter: value"), -6) );
    
    if ( value == 0 )
        {
        iSkipVerification = ETrue;
        return KErrNone;
        }
    
    if ( value == 1 )
        {
        iSkipVerification = EFalse;
        return KErrNone;
        }
    
    return KErrGeneral;
    }

	
	
/*-----------------------------------------------------------------------------
    Function:	Create_CCE
    
    Parameters: none
------------------------------------------------------------------------------*/ 

TInt CCCETester::Create_CCE( CStifItemParser& /*aItem*/ )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Create_CCE called");
     
	iCceTest = CCCE::NewL(); //needs Active Scheduler
     
    __ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("CCCE::NewL() failed"), KErrGeneral ) );
    
    iCceTest->SetObserver(*iObserver, *iSsObserver);
    
    return KErrNone;    
    }
    
    
    
/*-----------------------------------------------------------------------------
    Function:	Create_CCE_2
    
    Parameters: none
    
    2nd constructor
------------------------------------------------------------------------------*/ 

TInt CCCETester::Create_CCE_2( CStifItemParser& /*aItem*/ )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Create_CCE called");
     
	iCceTest = CCCE::NewL(*iObserver); //needs Active Scheduler
     
    __ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("CCCE::NewL() failed"), KErrGeneral ) );

	// Still needed because of SSObserver    
    iCceTest->SetObserver(*iObserver, *iSsObserver);
    
    return KErrNone;    
    }


/*-----------------------------------------------------------------------------
    Function:	Delete_CCE
    
    Parameters: none
------------------------------------------------------------------------------*/ 

TInt CCCETester::Delete_CCE( CStifItemParser& /*aItem*/ )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Delete_CCE called");
    
	__ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("CCCE::NewL() failed"), KErrGeneral ) );
    
    delete iCceTest;
    
    return KErrNone;
    }    





/*-----------------------------------------------------------------------------
    Function:	OpenNewCall

    Parameters:	1. Integer, callID, call handle to be used in other operations, must
    			   start from 1 and increment by one each time
    			2. Integer, expected return value of the OpewNewCallL
    			3. String Address
    			4. Integer iServiceId
    			5. String TCCECallType iCallType, literal value of enumeration
    			6. String TCCELineType iLineType, literal value of enumeration
------------------------------------------------------------------------------*/

TInt CCCETester::OpenNewCall( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::OpenNewCall called");
	
	TInt err = KErrNone;

	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
	
	// Parameter 1: Call ID
	
	TPtrC par;
    TPtrC& param = par;
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: Call ID"), -6) );
    	
	// Force new callID in the script
    CheckCallIdL(callID, ETrue);
	
	// Parameter 2: Return value
	
	TInt retval;
    __ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: Return value"), -6) );
	
	// Parameter 3: Address
    
    TPtrC address;
    __ASSERT_ALWAYS( aItem.GetNextString(address) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: address"), -6) );
    
    TBufC<1> empty(KNullDesC);
    
    if ( address.CompareF(KNullID) == 0)
    	{
    	address.Set(empty);
    	}
    	
    // Parameter 4: Service ID
    
    TInt serviceId;
    __ASSERT_ALWAYS( aItem.GetNextInt(serviceId) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: serviceId"), -6) );
    
    iCallParams->SetServiceId( (TUint32)serviceId );
   
    // Parameter 5: Call Type
  	
    TPtrC callType;
    __ASSERT_ALWAYS( aItem.GetNextString(callType) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: callType"), -6) );
    
    if ( callType.CompareF(KCCECallTypePS) == 0 )
    	{
    	CCETESTERLOG("CCCETester::OpenNewCall, ECCECallTypePS");
    	iCallParams->SetCallType( CCPCall::ECallTypePS );    
    	}
    else if ( callType.CompareF(KCCECallTypeCSVoice) == 0 )
    	{
    	CCETESTERLOG("CCCETester::OpenNewCall, ECCECallTypeCSVoice");
    	iCallParams->SetCallType( CCPCall::ECallTypeCSVoice );
    	}
    else if ( callType.CompareF(KCCECallTypeVideo) == 0 )
    	{
    	iCallParams->SetCallType ( CCPCall::ECallTypeVideo );
    	}
    else
    	{
    	return KErrArgument;
    	}

    // Parameter 6: Line Type
    	
   	//TPtrC lineType;
    __ASSERT_ALWAYS( aItem.GetNextString(param) == KErrNone, User::Panic( _L("OpenNewCall, Error in parameter: lineType"), -6) );
    
    // No more parameters
    __ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("OpenNewCall, Too many parameters"), -6) );
    
    if ( param.CompareF(KCCELineTypePrimary) == 0 )
    	{
    	iCallParams->SetLineType( CCCECallParameters::ECCELineTypePrimary );
    	}
    else if ( param.CompareF(KCCELineTypeAux) == 0 )
    	{
    	iCallParams->SetLineType( CCCECallParameters::ECCELineTypeAux );
    	}
    else
    	{
    	return KErrArgument;
    	}
    // Create new Call Observer
    CTestCallObserver* newCallObs = NULL;
    TRAP( err, newCallObs = CTestCallObserver::NewL( iEventStorage, callID ) );

    if (KErrNone != err ) 
    	{
       	return err;
    	}
    
    // Store handle of created Call Observer
    err = iCallObserverArray.Append( newCallObs );

    if (KErrNone != err ) 
    	{
    	// Append failed, clean observer
    	if (newCallObs) delete newCallObs;
    	
    	return err;
    	}
    	
	MCCECall* call = NULL;
	  	
    TRAP( err, call = &iCceTest->OpenNewCallL( address, *iCallParams, *newCallObs ) );
    
    CCETESTERLOG1("CCCETester::OpenNewCallL retval=%d", err );
    
    if (retval != err )
   		{
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
   		}
   	else if( err != KErrNone )
   		{
   		// retval was expected but cannot append so exit with return
   		return KErrNone;
   		}
   		
   	if (call != NULL)
   		{
   		CCETESTERLOG("CCCETester::OpenNewCall, appending call to array");
   		err = iCallArray.Append(call);
    	
    	if (KErrNone != err ) 
    		{
    		//if append fails, we must do release here
    		call->Release();
    		return err;
    		}
    	CCETESTERLOG("CCCETester::OpenNewCall, append done");
   		}
   	else
   		{
    	return err;
   		}

	return KErrNone;
	}
	
/*-----------------------------------------------------------------------------
    Function:	Release

    Parameters:	1. Integer, callID of the call to be released
    			2. Integer, expected return value
------------------------------------------------------------------------------*/

TInt CCCETester::Release(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::Release called");

	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Release, Error in parameter: callID"), -6) );
    	
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Release, Release in parameter: retval"), -6) );
	
	//No more parameters
	TPtrC param;
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("Release, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->Release();
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    // null call pointer in array, next call will use sequential increasing number, 
	// previous ones are void and cannot be used anymore
    iCallArray[callID-1] = NULL;
	
	return KErrNone;
	}

	
/*-----------------------------------------------------------------------------
    Function:	Dial

    Parameters:	1. Integer: CallID for which Dial will be called
    			2. Integer: Expected return value of the Dial
    			3. String: SubAddress
    			4. String: Bearer
-----------------------------------------------------------------------------*/

TInt CCCETester::Dial(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::Dial called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Dial, Error in parameter: callID"), -6) );
	
	TInt retval;	
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Dial, Error in parameter: retval"), -6) );
	
	TPtrC subAddress;
	__ASSERT_ALWAYS( aItem.GetNextString(subAddress) == KErrNone, User::Panic( _L("Dial, Error in parameter: subAddress"), -6) );
	
	TPtrC bearer;
	__ASSERT_ALWAYS( aItem.GetNextString(bearer) == KErrNone, User::Panic( _L("Dial, Error in parameter: bearer"), -6) );
	
	//No more parameters
	TPtrC param;
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("Dial, Too many parameters"), -6) );
		
	RMmCustomAPI::TCustomCallParams dialParams;
    
    dialParams.iSubAddress = subAddress;
    
    TPtrC8 bearerP8(reinterpret_cast<const TUint8*>(bearer.Ptr()), bearer.Length()*2);
    if ( bearer.Length() > 40 ) { return KErrArgument; }
    TBuf8<40> bearerB8(bearerP8);
    
	// TODO: Add conversion functions, from TPtrC to TBufC8
    
    dialParams.iBearer = bearerB8;
    
    // Check Call ID
    CheckCallIdL(callID);
	    
    TInt err = iCallArray[callID-1]->Dial( (TDesC8&)dialParams );
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
	
	}


	
/*-----------------------------------------------------------------------------
    Function:	HangUp

    Parameters:	1. Integer, callID of the call to be HangUp
    			2. Integer, expected return value
------------------------------------------------------------------------------*/

TInt CCCETester::HangUp(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::HangUp called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("HangUp, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("HangUp, Error in parameter: retval"), -6) );
	
	//No more parameters
	TPtrC param;
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("HangUp, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->HangUp();
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
	
	return KErrNone;
	}


/*-----------------------------------------------------------------------------
	Function:   Hold

    Parameters:	1. Integer: CallID for which Hold will be called
    			2. Integer: Expected return value of the Hold
------------------------------------------------------------------------------*/

TInt CCCETester::Hold( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Hold called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Hold, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Hold, Error in parameter: retval"), -6) );
	
	 //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Hold, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->Hold();
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
       
    return KErrNone;
    }


/*-----------------------------------------------------------------------------
	Function:   Resume

    Parameters:	1. Integer: CallID for which Resume will be called
    			2. Integer: Expected return value of the Resume
------------------------------------------------------------------------------*/

TInt CCCETester::Resume( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Resume called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Resume, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Resume, Error in parameter: retval"), -6) );
	
	 //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Resume, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->Resume();
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
       
    return KErrNone;
    }

/*-----------------------------------------------------------------------------
	
	Function:   Swap

    Parameters:	1. Integer: CallID for which Call will be swapped
    			2. Integer: Expected return value of the swap

------------------------------------------------------------------------------*/

TInt CCCETester::Swap( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::Swap called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Swap, Error in parameter: callID"), -6) );
	
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Swap, Error in parameter: retval"), -6) );
	
	 //No more parameters
	 TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Swap, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->Swap();
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
       
    return KErrNone;
    }



/*-----------------------------------------------------------------------------
	Function:   Answer

    Parameters:	1. Integer: CallID of the call to Answer
    			2. Integer: Expected return value of the Answer
------------------------------------------------------------------------------*/	
TInt CCCETester::Answer( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::Answer()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("Answer, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("Answer, Error in parameter: retval"), -6) );

 	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("Answer, Too many parameters"), -6) );		
	
	TInt err = iCallArray[callID -1]->Answer();
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    	
    return KErrNone;
	}
	

/*-----------------------------------------------------------------------------
    Function:	InitializeEmergencyCall

    Parameters:	1. Integer, callID, call handle to be used in other operations, must
    			   start from 1 and increment by one each time
    			2. Integer, expected return value of the InitializeEmergencyCallL
------------------------------------------------------------------------------*/

TInt CCCETester::InitializeEmergencyCall( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::InitializeEmergencyCall called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	// Parameter 1: Call ID	
	TPtrC par;
    TPtrC& param = par;
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("InitializeEmergencyCall, Error in parameter: Call ID"), -6) );
	
	// Check Call ID, new call
	CheckCallIdL(callID, ETrue);

	// Parameter 2: Return value
	TInt retval = 0;
    __ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("InitializeEmergencyCall, Error in parameter: Return value"), -6) );
	
    // No more parameters
    __ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("InitializeEmergencyCall, Too many parameters"), -6) );
    
    // Create new Call Observer
    CTestCallObserver* newCallObs = NULL;
    
    TRAPD( err, newCallObs = CTestCallObserver::NewL( iEventStorage, callID ) );
    
    if ( KErrNone != err )
    	{
    	return err;
    	}
    
	// Store handle of created Call Observer
   	iCallObserverArray.Append( newCallObs );
    
	MCCECall* emergencyCall;
	  	
    emergencyCall = &iCceTest->InitializeEmergencyCall( *newCallObs );
       	
   	if( iCallArray.Find(emergencyCall) == KErrNotFound )
   		{
   		iCallArray.Append(emergencyCall);
   		}
   	else
   		{
   		User::Panic( _L("CCCETester::InitializeEmergencyCall, Error appending Emergency Call"), KErrGeneral );
   		}
   		
	return KErrNone;
	}

/*-----------------------------------------------------------------------------
    Function:	DialEmergencyCall

    Parameters:	1. Integer: CallID for which Dial will be called
    			2. String:  Emergency Call number
------------------------------------------------------------------------------*/

TInt CCCETester::DialEmergencyCall( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::DialEmergencyCall called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;

	//1. parameter: CallID
    TInt callID = 0;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("DialEmergencyCall, Error in parameter: callID"), -6) );
	
	// Check Call ID
    CheckCallIdL(callID);
	
	//2. parameter: Emergency Call number
	TPtrC number;
	__ASSERT_ALWAYS( aItem.GetNextString(number) == KErrNone, User::Panic( _L("DialEmergencyCall, Error in parameter: bearer"), -6) );
	
	//No more parameters
	TPtrC param;
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("DialEmergencyCall, Too many parameters"), -6) );
	    
    const TPtrC numberConst(number);
    
    iCallArray[callID-1]->DialEmergencyCall(numberConst);
    
    return KErrNone;
	}


/*-----------------------------------------------------------------------------
	
	Function:   StartDtmfTone

    Parameters:	1. Integer: Expected return value of the StartDtmfTone
    			2. String:  the DTMF tone to be send

------------------------------------------------------------------------------*/

TInt CCCETester::StartDtmfTone( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::StartDtmfTone called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TPtrC ret;
    TPtrC& param = ret;
    
    CheckDtmf();
    
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("StartDtmfTone, Error in parameter: retval"), -6) );
	
	TChar tone;
    __ASSERT_ALWAYS( aItem.GetNextChar(tone) == KErrNone, User::Panic( _L("StartDtmfTone, Error in parameter: tone"), -6) );
    
	 //No more parameters
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("StartDtmfTone, Too many parameters"), -6) );

	TInt err = iDtmfTest->StartDtmfTone( tone );

    if (retval != err )
   		{
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
   		}
       
    return KErrNone;
    }
    
   
/*-----------------------------------------------------------------------------

	Function:   StopDtmfTone

    Parameters:	1. Integer: Expected return value

------------------------------------------------------------------------------*/

TInt CCCETester::StopDtmfTone( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::StopDtmfTone called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TPtrC ret;
    TPtrC& param = ret;
    
	TInt retval;	
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("StopDtmfTone, Error in parameter: retval"), -6) );
	
	 //No more parameters
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("StopDtmfTone, Too many parameters"), -6) );
	
	TInt err = iDtmfTest->StopDtmfTone();
       
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
       
    return KErrNone;
    }

/*-----------------------------------------------------------------------------

	Function:   SendDtmfToneString

    Parameters:	1. Integer: Expected return value of the stop
    			2. String: the DTMF tone string to be send

------------------------------------------------------------------------------*/

TInt CCCETester::SendDtmfToneString( CStifItemParser& aItem  )
	{
	CCETESTERLOG("[CCETEST] CCCETester::SendDtmfToneString called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TPtrC ret;
    TPtrC& param = ret;
    
    CheckDtmf();
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("SendDtmfToneString, Error in parameter: retval"), -6) );
	
	TPtrC string;
    __ASSERT_ALWAYS( aItem.GetNextString(string) == KErrNone, User::Panic( _L("SendDtmfToneString, Error in parameter: string"), -6) );
    
	 //No more parameters
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("SendDtmfToneString, Too many parameters"), -6) );
	 
    //TInt err = iDtmfTest->SendDtmfToneString( dtmfString );
    TInt err = iDtmfTest->SendDtmfToneString( (TDesC&)string );
    
    if (retval != err )
   		{
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
   		}
       
    return KErrNone;	
	}

/*-----------------------------------------------------------------------------

	Function:   ContinueStringSending

    Parameters:	1. Integer: Expected return value
    			2. Boolean: ETrue if sending should be continued, else EFalse 
    			
------------------------------------------------------------------------------*/

TInt CCCETester::ContinueStringSending( CStifItemParser& aItem  )
	{
	CCETESTERLOG("[CCETEST] CCCETester::ContinueStringSending called");

	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TPtrC ret;
    TPtrC& param = ret;
    
    CheckDtmf();
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("SendDtmfToneString, Error in parameter: retval"), -6) );
    
    TInt continueSending;
	__ASSERT_ALWAYS( aItem.GetNextInt(continueSending) == KErrNone, User::Panic( _L("SendDtmfToneString, Error in parameter: retval"), -6) );
        
	 //No more parameters
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("SendDtmfToneString, Too many parameters"), -6) );
	
	TInt err = KErrNone;
	
	if (continueSending)
		{    
		err = iDtmfTest->ContinueStringSending( ETrue );
		}
	else
		{
		err = iDtmfTest->ContinueStringSending( EFalse );
		}
		
    if ( retval != err )
   		{
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
   		}
       
    return KErrNone;
	}

/*-----------------------------------------------------------------------------

	Function:   CancelDtmfStringSending

    Parameters:	none

------------------------------------------------------------------------------*/

TInt CCCETester::CancelDtmfStringSending( CStifItemParser& aItem  )
	{
	CCETESTERLOG("[CCETEST] CCCETester::CancelDtmfStringSending called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TPtrC ret;
    TPtrC& param = ret;
    
    CheckDtmf();
	   
	 //No parameters
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("CancelDtmfStringSending, Too many parameters"), -6) );
	    
	iDtmfTest->CancelDtmfStringSending( );
       
    return KErrNone;
	}
    

/*-----------------------------------------------------------------------------
	Function:   AddToConference

    Parameters:	1. Integer: CallID of the call that is added to Conference
    			2. Integer: Expected return value of the AddToConference
------------------------------------------------------------------------------*/

TInt CCCETester::AddToConference( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::AddToConference called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    CheckConferenceCall();
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("AddToConference, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("AddToConference, Error in parameter: retval"), -6) );
	
	 //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("AddToConference, Too many parameters"), -6) );
	
	TRAPD( err, iConferenceCall->AddCallL( *iCallArray[callID-1] ) );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    	
    return KErrNone;
    }


/*-----------------------------------------------------------------------------
	Function:   RemoveFromConference

    Parameters:	1. Integer: CallID of the call that is removed from Conference
    			2. Integer: Expected return value of the RemoveFromConference
------------------------------------------------------------------------------*/

TInt CCCETester::RemoveFromConference( CStifItemParser& aItem )
	{
    CCETESTERLOG("[CCETEST] CCCETester::RemoveFromConference called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
	CheckConferenceCall();
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("RemoveFromConference, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("RemoveFromConference, Error in parameter: retval"), -6) );
	
	 //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("RemoveFromConference, Too many parameters"), -6) );
	
	TRAPD( err, iConferenceCall->RemoveCallL( *iCallArray[callID-1] ) );
		
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
	}
    



/*-----------------------------------------------------------------------------
	Function:   SwapConference

    Parameters:	1. Integer: Expected return value of the SwapConference
------------------------------------------------------------------------------*/

TInt CCCETester::SwapConference( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::AddToConference called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("SwapConference, Error in parameter: retval"), -6) );
    
     //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("SwapConference, Too many parameters"), -6) );
    
    CheckConferenceCall();

	TInt err = iConferenceCall->Swap();
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
    }
    

/*-----------------------------------------------------------------------------
	Function:   GoOneToOneL

    Parameters:	1. Integer: CallID with who private call is created
    			2. Integer: Expected return value of the SwapConference
------------------------------------------------------------------------------*/
TInt CCCETester::GoOneToOne( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::GoOneToOneL called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("RemoveFromConference, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("GoOneToOneL, Error in parameter: retval"), -6) );
    
     //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("GoOneToOneL, Too many parameters"), -6) );
    
    CheckConferenceCall();
    
    MCCECall* privatedCall = iCallArray[callID-1];

	TRAPD( err, iConferenceCall->GoOneToOneL(*privatedCall) );
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
	}



    
    
/*-----------------------------------------------------------------------------
	Function:   HangupConference

    Parameters:	1. Integer: Expected return value of the HangupConference
------------------------------------------------------------------------------*/

TInt CCCETester::HangupConference( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::HangupConference called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("HangupConference, Error in parameter: retval"), -6) );
    
     //No parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("HangupConference, Too many parameters"), -6) );
    
    CheckConferenceCall();

	TInt err = iConferenceCall->HangUp();
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
    }



/*-----------------------------------------------------------------------------
	Function:   ReleaseConference

    Parameters:	1. Integer: Expected return value of the ReleaseConference
------------------------------------------------------------------------------*/

TInt CCCETester::ReleaseConference( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::ReleaseConference called");
    
    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("ReleaseConference, Error in parameter: retval"), -6) );
    
     //No parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("ReleaseConference, Too many parameters"), -6) );
    
    CheckConferenceCall();

	TInt err = iConferenceCall->Release();
	
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
    }

   


/*-----------------------------------------------------------------------------
	Function:   UnattendedTransfer

	Parameters:	1. Integer: CallID of transferred call
    			2. Integer: Expected return value
------------------------------------------------------------------------------*/	
TInt CCCETester::UnattendedTransfer( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::UnattendedTransfer()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("UnattendedTransfer, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
		
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("UnattendedTransfer, Error in parameter: retval"), -6) );

 	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("UnattendedTransfer, Too many parameters"), -6) );		
	
	TInt err = iCallArray[callID -1]->UnattendedTransfer( KTransferTarget );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    	
    return KErrNone;
	}
	
	
    
/*-----------------------------------------------------------------------------
	Function:   AttendedTransfer

	Parameters:	1. Integer: CallID of the call to which AttendedTransfer is called
    			2. Integer: Expected return value
------------------------------------------------------------------------------*/	
TInt CCCETester::AttendedTransfer( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::AttendedTransfer()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("AttendedTransfer, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
		
    TInt retval = 0;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("AttendedTransfer, Error in parameter: retval"), -6) );

 	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("AttendedTransfer, Too many parameters"), -6) );		
	
	TInt err = iCallArray[callID -1]->AttendedTransfer( KTransferTarget );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    	
    return KErrNone;
	}
	

/*-----------------------------------------------------------------------------
	Function:   AcceptTransfer

	Parameters:	1. Integer: CallID of the call to which AcceptTransfer is called
    			2. Integer: Expected return value
    			3. Integer: Accept transfer: 0/False, 1/True
------------------------------------------------------------------------------*/	
TInt CCCETester::AcceptTransfer( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::AcceptTransfer()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("AcceptTransfer, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
		
    TInt retval = 0;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("AcceptTransfer, Error in parameter: retval"), -6) );
	
	TInt responseParam;
	__ASSERT_ALWAYS( aItem.GetNextInt(responseParam) == KErrNone, User::Panic( _L("AcceptTransfer, Error in parameter: responseParam"), -6) );
	
	TBool response = ETrue;
	
	if ( responseParam == 0 )
		{
		response = EFalse;
		}
	else if ( responseParam == 1 )
		{
		response = ETrue;
		}
	else
		{
		User::Panic( _L("AcceptTransfer, responseParam should be 0 or 1"), -6);
		}
	
	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("AttendedTransfer, Too many parameters"), -6) );		
	
	TInt err = iCallArray[callID -1]->AcceptTransfer( response );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    	
    return KErrNone;
	}
	
/*-----------------------------------------------------------------------------
	Function:   GetForwardAddressChoices

    Parameters:	1. Call ID
------------------------------------------------------------------------------*/

TInt CCCETester::GetForwardAddressChoices( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::GetForwardAddressChoices()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("GetForwardAddressChoices, Error in parameter: callID"), -6) );
    
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("GetForwardAddressChoices, Error in parameter: retval"), -6) );
	
	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("GetForwardAddressChoices, Too many parameters"), -6) );

	// Check Call ID
    CheckCallIdL(callID);
	    
    const CDesC8Array* addressArray = NULL;
    TRAPD( err, addressArray = &iCallArray[callID-1]->GetForwardAddressChoicesL() );
    
    iForwardAddressArray = const_cast<CDesC8Array*>(addressArray);
    
    if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
    
    return KErrNone;
	}

/*-----------------------------------------------------------------------------
	Function:   ForwardToAddress

    Parameters:	1. Integer: Call ID
    			2. Integer: Forward index
------------------------------------------------------------------------------*/
TInt CCCETester::ForwardToAddress( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::ForwardToAddress()");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("ForwardToAddress, Error in parameter: callID"), -6) );
    
    TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("GetForwardAddressChoices, Error in parameter: retval"), -6) );
    
    TInt forwardIndex;
    __ASSERT_ALWAYS( aItem.GetNextInt(forwardIndex)==KErrNone && forwardIndex>=0, User::Panic( _L("ForwardToAddress, Error in parameter: callID"), -6) );
	
	//No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("ForwardToAddress, Too many parameters"), -6) );

	// Check Call ID
    CheckCallIdL(callID);
   	
	TRAPD( err, iCallArray[callID-1]->ForwardToAddressL(forwardIndex) );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
	
	return KErrNone;
	}
	

TInt CCCETester::TerminateAllButRingingAndQueuedCalls( CStifItemParser& /*aItem*/ )
	{
	CCETESTERLOG("[CCETEST] CCCETester::TerminateAllButRiningAndQueuedCalls()");

	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
	iCceTest->TerminateAllButRingingAndQueuedCalls();
	
	return KErrNone;
	}


TInt CCCETester::TerminateCall( CStifItemParser& aItem )
	{
	CCETESTERLOG("[CCETEST] CCCETester::TerminateCall()");

	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
		
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("TerminateCall, Error in parameter: retval"), -6) );
	
	TInt state = 1;
	TBuf<128> callName(KCallName);
	
	// method 1
	iCceTest->TerminateCall( state, callName );
	
	// method 2
	TInt err = iCceTest->TerminateCall( callName );
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
	
	return KErrNone;
	}
	
TInt CCCETester::TerminateAllCalls( CStifItemParser& /*aItem*/ )
	{
	iCceTest->TerminateAllCalls();
	
	return KErrNone;
	}

	
TInt CCCETester::GetAirTimeDuration( CStifItemParser& /*aItem*/ )
	{
	CCETESTERLOG("[CCETEST] CCCETester::GetAirTimeDuration()");

	TTimeIntervalSeconds csDuration;
	TTimeIntervalSeconds psDuration;
	
	iCceTest->GetAirTimeDuration(csDuration, psDuration);
	
	CCETESTERLOG2("[CCETEST] CCCETester::GetAirTimeDuration, CS= %d, PS= %d", &csDuration, &psDuration );
	
	return KErrNone;
	}
	


/*-----------------------------------------------------------------------------
	Function:   RequestExtIfCommand

    Parameters:	1. Integer: ServiceId Service id used for routing info to find correct plug-in. 
    						Return to observer can be different!
    			2. Integer: RequestId ExtIf internal request id. Observer returns refer to this one.
    			3. String: 	aData Custom formatted data to plug-in.
------------------------------------------------------------------------------*/
TInt CCCETester::RequestExtIfCommand( CStifItemParser& aItem )
	{
    TRAPD( e1, CheckExtIfL() );
    if( e1 )
        {
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
        }

	aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

    // Parameter 1: Service ID
   
    TInt serviceId;
    __ASSERT_ALWAYS( aItem.GetNextInt(serviceId) == KErrNone, User::Panic( _L("RequestExtIfCommand, Error in parameter: serviceId"), -6) );

    // Parameter 1: Request ID
   
    TInt requestId;
    __ASSERT_ALWAYS( aItem.GetNextInt(requestId) == KErrNone, User::Panic( _L("RequestExtIfCommand, Error in parameter: serviceId"), -6) );

	// Parameter 3: Address
    
    TPtrC address;
    __ASSERT_ALWAYS( aItem.GetNextString(address) == KErrNone, User::Panic( _L("RequestExtIfCommand, Error in parameter: address"), -6) );
    _LIT(KNullID, "NULL");
    TBufC<1> empty(KNullDesC);
    
    if ( address.CompareF(KNullID) == 0)
    	{
    	address.Set(empty);
    	}


    TInt expectedResult;
    __ASSERT_ALWAYS( aItem.GetNextInt(expectedResult) == KErrNone, User::Panic( _L("RequestExtIfCommand, Error in parameter: serviceId"), -6) );


    HBufC8* converted( NULL );
	converted = EscapeUtils::ConvertFromUnicodeToUtf8L( address );
	 
	TInt err = iExtIfTest->RequestCommand( serviceId, requestId, *converted );
    
    delete converted;
    
    if (err!=expectedResult )
   		{
   		// Must not return retval, which could be KErrNone but expected something else
    	return KErrGeneral;
   		}
       
    return KErrNone;	
	}


TInt CCCETester::GetMobileCallInfo( CStifItemParser& aItem )
	{
	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("GetMobileCallInfo, Error in parameter: callID"), -6) );
    
    //CheckCallIdL(callID);
	
	RMobileCall::TMobileCallInfoV3 callInfo;
	RMobileCall::TMobileCallInfoV3Pckg pckg(callInfo);
	
	TInt panicCode;
    if ( aItem.GetNextInt(panicCode) != KErrNotFound )
    	{
    	TestModuleIf().SetExitReason( CTestModuleIf::EPanic, panicCode );
    	}
    	
    //No more parameters
	TPtrC check;
	__ASSERT_ALWAYS( aItem.GetNextString(check) != KErrNone, User::Panic( _L("GetMobileCallInfo, Too many parameters"), -6) );
	
	iCallArray[callID -1]->GetMobileCallInfo( reinterpret_cast<TDes8&>(pckg) );
	
	TestModuleIf().SetExitReason( CTestModuleIf::ENormal, KErrNone );
		
	return KErrNone;	
	}
	
TInt CCCETester::MiscTests( CStifItemParser& aItem )
	{
	TBuf<200> buf;
	// todo test this line is working ...		
	iCceTest->GetLifeTime((TDes8&)buf);
	
	CSInfo csinfo;
	iCceTest->GetCSInfo(csinfo);
	
	// dtmf add / remove observer coverage
	if( iDtmfObserver )
		{
		MCCEDtmfInterface* temp = &iCceTest->DtmfInterfaceL(*iDtmfObserver);
		temp->AddObserverL(*iDtmfObserver);
		temp->AddObserverL(*iDtmfObserver);
		temp->RemoveObserver(*iDtmfObserver);
		temp->RemoveObserver(*iDtmfObserver);
		}

	TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("MiscTests, Error in parameter: callID"), -6) );
    CheckCallIdL(callID);
	
	//iCallArray[0]->SetConvergedCallL();
	
	iCallArray[callID -1]->IsSecured();
	iCallArray[callID -1]->IsEmergency();
	iCallArray[callID -1]->RemoteParty();
	iCallArray[callID -1]->RemotePartyName();
	iCallArray[callID -1]->DialledParty();
	iCallArray[callID -1]->Tone();
	iCallArray[callID -1]->CallDuration();
	iCallArray[callID -1]->StartTime();
	iCallArray[callID -1]->Caps();
	
	iCallParams->SetServiceId( TUint32(1) );
	iCallParams->SetCallType( CCPCall::ECallTypeCSVoice );
	iCallParams->SetLineType( CCCECallParameters::ECCELineTypePrimary );
	
	iCallArray[callID -1]->SetParameters( *iCallParams );
	iCallArray[callID -1]->Parameters();
	
	iCallArray[callID -1]->TransferTarget();

	// CS methods	
	TBuf8<10> dataBuf(KDataCaps);
	iCallArray[callID -1]->GetMobileDataCallCaps( dataBuf );
	iCallArray[callID -1]->SwitchAlternatingCall();
	iCallArray[callID -1]->NoFDNCheck();
	iCallArray[callID -1]->LogDialedNumber();
	
	
	// Call here, will return an error, used for coverage purpose
	// Called elsewhere too.
	iCallArray[callID -1]->ReplaceActive();

	return KErrNone;
	}



/*-----------------------------------------------------------------------------
    Function:	ReplaceActive

    Parameters:	1. Integer, callID of the call to be HangUp
    			2. Integer, expected return value
------------------------------------------------------------------------------*/

TInt CCCETester::ReplaceActive(CStifItemParser& aItem)
	{
	CCETESTERLOG("[CCETEST] CCCETester::ReplaceActive called");
	
	if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
	
    TInt callID;
    __ASSERT_ALWAYS( aItem.GetNextInt(callID) == KErrNone, User::Panic( _L("ReplaceActive, Error in parameter: callID"), -6) );
		
	CheckCallIdL(callID);
	
	TInt retval;
	__ASSERT_ALWAYS( aItem.GetNextInt(retval) == KErrNone, User::Panic( _L("ReplaceActive, Error in parameter: retval"), -6) );
	
	//No more parameters
	TPtrC param;
	__ASSERT_ALWAYS( aItem.GetNextString(param) != KErrNone, User::Panic( _L("ReplaceActive, Too many parameters"), -6) );
	
	TInt err = iCallArray[callID-1]->ReplaceActive();
	
	if (err != retval) //Expected result
    	{
    	return KErrGeneral;
    	}
	
	return KErrNone;
	}
/*-----------------------------------------------------------------------------
    Function:	OOM_Tests_1

    Test all the methods that can be tested independently
------------------------------------------------------------------------------*/

TInt CCCETester::OOM_Tests_1( CStifItemParser& /* aItem */ )
	{
	CCETESTERLOG("[CCETEST] CCCETester::OOM_Tests_1 called");
	
	//------------------------------------------------
	// 1) CCCE::NewL()
	//------------------------------------------------
	
	// Delete CCE created in default constructor
	delete iCceTest;
	
	TInt count = 0;
	TInt err = KErrNone;

	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
		TRAP( err, iCceTest = CCCE::NewL() );
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err ) return err;
	__ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("OOM test CCCE::NewL() failed"), -2) );
	CCETESTERLOG1("[CCETEST] CCCE::NewL() completed on attempt %d", count );
	
	//------------------------------------------------
	// 2) CCCE::NewL(MCCEObserver& aObserver)
	//------------------------------------------------
	
	// Delete existing CCE instance
	delete iCceTest;
	
	__UHEAP_RESET;
	count = 0;
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
		TRAP( err, iCceTest = CCCE::NewL(*iObserver) );
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err ) return err;
	__ASSERT_ALWAYS( iCceTest != NULL, User::Panic( _L("OOM test CCCE::NewL(MCCEObserver) failed"), -2) );
	CCETESTERLOG1("[CCETEST] CCCE::NewL(MCCEObserver) completed on attempt %d", count );
	
	//------------------------------------------------
	// 3) CCCE::OpenNewCall PS
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;

    // PS call, primary
	iCallParams->SetServiceId( TUint32(2) );
	iCallParams->SetCallType( CCPCall::ECallTypePS );
	iCallParams->SetLineType( CCCECallParameters::ECCELineTypePrimary );
	
	// Create new Call Observer
	CTestCallObserver* newCallObs = NULL;
    TRAP( err, newCallObs = CTestCallObserver::NewL( iEventStorage, 1 ) );
    if (KErrNone != err ) return err;
    
    // Store handle of created Call Observer
    iCallObserverArray.Append( newCallObs );
    
	MCCECall* call1 = NULL;
	
	_LIT(KCallAddress, "Test Address");
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );	  	
        TRAP( err, call1 = &iCceTest->OpenNewCallL( KCallAddress, *iCallParams, *newCallObs ) );
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err )
		{
		return err;
		}
		
	__ASSERT_ALWAYS( call1 != NULL, User::Panic( _L("OOM test CCCE::OpenNewCall (PS) failed"), -2) );
    
    CCETESTERLOG1("[CCETEST] CCCE::OpenNewCall() PS completed on attempt %d", count );
    
    //------------------------------------------------
	// 4) CCCE::OpenNewCall CS
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;

    // PS call, primary
	iCallParams->SetServiceId( TUint32(1) );
	iCallParams->SetCallType( CCPCall::ECallTypeCSVoice );
	iCallParams->SetLineType( CCCECallParameters::ECCELineTypePrimary );

	// Create new Call Observer
	CTestCallObserver* newCallObs2 = NULL;
    TRAP( err, newCallObs2 = CTestCallObserver::NewL( iEventStorage, 2 ) );
    if (KErrNone != err ) return err;
    
    // Store handle of created Call Observer
    iCallObserverArray.Append( newCallObs2 );
    
	MCCECall* call2 = NULL;
	
	do
		{
        count++;
		__UHEAP_FAILNEXT( count );	  	
        TRAP( err, call2 = &iCceTest->OpenNewCallL( KCallAddress, *iCallParams, *newCallObs2 ) );
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err )
		{
		return err;
		}
		
	__ASSERT_ALWAYS( call2 != NULL, User::Panic( _L("OOM test CCCE::OpenNewCall (CS) failed"), -2) );
    
    CCETESTERLOG1("[CCETEST] CCCE::OpenNewCall() CS completed on attempt %d", count );
        
	//------------------------------------------------
	// 5) CCCE::ConferenceCallL
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;
	
	MCCEConferenceCall* conf( NULL );
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );	  	
    	TRAP(err, conf = &iCceTest->ConferenceCallL( (MCCEConferenceCallObserver&)*iConfCallObserver ));
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err )
		{
		return err;
		}
		
	__ASSERT_ALWAYS( conf != NULL, User::Panic( _L("OOM test CCCE::ConferenceCallL failed"), -2) );
	
	CCETESTERLOG1("[CCETEST] CCCE::ConferenceCallL() completed on attempt %d", count );
	
	//------------------------------------------------
	// 6) CCCE::DtmfInterfaceL
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;
	
	MCCEDtmfInterface* dtmf( NULL );
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
		TRAP(err, dtmf = &iCceTest->DtmfInterfaceL( (MCCEDtmfObserver&) *iDtmfObserver ));
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err )
		{
		return err;
		}
	
	__ASSERT_ALWAYS( dtmf != NULL, User::Panic( _L("OOM test CCCE::DtmfInterfaceL() failed"), -2) );
    
    CCETESTERLOG1("[CCETEST] CCCE::DtmfInterfaceL() completed on attempt %d", count );
    
    //------------------------------------------------
	// 7) CCCE::ExtensionInterfaceL
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;
	
	MCCEExtensionInterface* extIf( NULL );
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
	    TRAP( err, extIf = &iCceTest->ExtensionInterfaceL( *iExtIfObserver ) );
		}
	while ( err == KErrNoMemory );
	
	if ( KErrNone != err )
		{
		return err;
		}
		
	__ASSERT_ALWAYS( extIf != NULL, User::Panic( _L("OOM test CCCE::ExtensionInterfaceL() failed"), -2) );
    
	CCETESTERLOG1("[CCETEST] CCCE::ExtensionInterfaceL() completed on attempt %d", count );
	
	//------------------------------------------------
	// 8) CCCE::GetLifeTime
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;
	
	CCETESTERLOG("[CCETEST] CCCE::GetLifeTime");
	
	_LIT8(KTest, "something");
	HBufC8* heapbuf8 = HBufC8::NewLC(21);
	CleanupStack::Pop( heapbuf8 );
	
	TPtr8 ptr8( heapbuf8->Des() );
	ptr8 = KTest;

	// Conversion for rdebug
	//_LIT(KDebug, "something to hold the results");
	//TPtrC ptrc( KDebug );
	//TDesC debug(ptrc);
	//TPtrC8 newptr = ptr8->PtrZ();
	//CCnvCharacterSetConverter::ConvertFromUnicode( ptr8->Ptr(), debug);
	//RDebug::Print(_L("[CCETEST] debug %S"), &ptr8 );
	
	_LIT8( KReturnValue, "47285710728469827493");
	
/**
	TBool result = iCceTest->GetLifeTime( ptr8 );
	
	if ( ptr8.CompareF( KReturnValue ) != 0 )
		{
		RDebug::Print(_L("[CCETEST] CCCE::GetLifeTime") );
		return KErrGeneral;
		}

	delete heapbuf8;
**/

	TInt res = 0;
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
		TBool result = iCceTest->GetLifeTime( ptr8 );
		res = ptr8.CompareF( KReturnValue );
		}
	while ( res != 0 );
	
	delete heapbuf8;
    
    CCETESTERLOG1("[CCETEST] CCCE::GetLifeTime completed on attempt %d", count );
    
    //------------------------------------------------
	// 9) CCCE::GetCSInfo
	//------------------------------------------------
	
	__UHEAP_RESET;
	count = 0;
    
	_LIT( KSerialNumber, "test serial nbr 123456789" );
	_LIT( KSWVersion, "test sw version nbr 123456789" );
	_LIT( KPhoneModel, "test phone nbr 123456789" );
	_LIT( KManufacturerIdSize, "test manufacturer id size 123456789" );
	
	CSInfo csinfo;
	
/**
	iCceTest->GetCSInfo( csinfo );
	
**/
	
	//roundsLeft = 10;
	
	do
		{
		count++;
		__UHEAP_FAILNEXT( count );
		TBool result = iCceTest->GetCSInfo( csinfo );
		}
	while ( count < 10 );
	
	if ( csinfo.iSerialNumber.CompareF(KSerialNumber) != 0 ) return KErrGeneral;
	if ( csinfo.iSWVersion.CompareF(KSWVersion) != 0 ) return KErrGeneral;
	if ( csinfo.iPhoneModel.CompareF(KPhoneModel) != 0 ) return KErrGeneral;
	if ( csinfo.iManufacturerIdSize.CompareF(KManufacturerIdSize) != 0 ) return KErrGeneral;
	

	///////////////////////////////////////////////////
	// End of OOM tests
	///////////////////////////////////////////////////
	
	__UHEAP_RESET;
	call1->Release();
	
	call2->Release();
    
    return KErrNone;
	
	}

/*-----------------------------------------------------------------------------
    Function:   ExpectMediatorEvent
   

    Parameters: 1. Integer, Call index
                2. String, TCCECallType iCallType, literal value of enumeration
                3. String, Call state
                4. Integer, service id
                5. String Call direction
------------------------------------------------------------------------------*/

TInt CCCETester::ExpectMediatorEvent( CStifItemParser& aItem )
    {
    CCETESTERLOG("[CCETEST] CCCETester::ExpectMediatorEvent called");
    

    if ( UnhandledEvents() != KErrNone ) return KErrNotReady;
    
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    
    // Parameter 1: Expected call index
    TInt expectedCallIndex;
    __ASSERT_ALWAYS( aItem.GetNextInt(expectedCallIndex) == KErrNone, User::Panic( _L("ExpectMediatorEvent, Error in parameter: Call index"), -6) );
        
    // Parameter 2: Call type
    TPtrC expectedCallType;
    __ASSERT_ALWAYS( aItem.GetNextString(expectedCallType) == KErrNone, User::Panic( _L("ExpectMediatorEvent, Error in parameter: Call type"), -6) );
    
    CCPCall::TCallType callType;
    if ( expectedCallType.CompareF( KCCECallTypeCSVoice ) == 0 )
        {
        callType = CCPCall::ECallTypeCSVoice;
        }
    else if ( expectedCallType.CompareF( KCCECallTypeVideo ) == 0 ) 
        {
        callType = CCPCall::ECallTypeVideo;
        }
    else if ( expectedCallType.CompareF( KCCECallTypePS ) == 0 )
        {
        callType = CCPCall::ECallTypePS;
        }
    
    // Parameter 3: Call state
    TPtrC expectedCallState;
    __ASSERT_ALWAYS( aItem.GetNextString(expectedCallState) == KErrNone, User::Panic( _L("ExpectMediatorEvent, Error in parameter: Call state"), -6) );
    
    CCPCall::TCallState callState;
    if ( expectedCallState.CompareF( KStateDialling ) == 0 )
        {
        callState = CCPCall::EStateDialling;
        }
    else if ( expectedCallState.CompareF( KStateAnswering ) == 0 )
        {
        callState = CCPCall::EStateAnswering;
        }
    else if ( expectedCallState.CompareF( KStateConnecting ) == 0 )
        {
        callState = CCPCall::EStateConnecting;
        }
    else if ( expectedCallState.CompareF( KStateConnected ) == 0 )
        {
        callState = CCPCall::EStateConnected;
        }
    else if ( expectedCallState.CompareF( KStateDisconnecting ) == 0 )
        {
        callState = CCPCall::EStateDisconnecting;
        }
    else if ( expectedCallState.CompareF( KStateRinging ) == 0 )
        {
        callState = CCPCall::EStateRinging;
        }
    else if ( expectedCallState.CompareF( KStateQueued ) == 0 )
        {
        callState = CCPCall::EStateQueued;
        }
    else if ( expectedCallState.CompareF( KStateForwarding ) == 0 )
        {
        callState = CCPCall::EStateForwarding;
        }
    else if ( expectedCallState.CompareF( KStateHold ) == 0 )  
        {
        callState = CCPCall::EStateHold;
        }
    
    // Parameter 4: Expected service id
    TInt expectedServiceId;
    __ASSERT_ALWAYS( aItem.GetNextInt(expectedServiceId) == KErrNone, User::Panic( _L("ExpectMediatorEvent, Error in parameter: service id"), -6) );
    
    // Parameter 5: Call state
    TPtrC expectedCallDirection;
    __ASSERT_ALWAYS( aItem.GetNextString(expectedCallDirection) == KErrNone, User::Panic( _L("ExpectMediatorEvent, Error in parameter: Call direction"), -6) );
    
    CCPCall::TCallDirection callDirection;
    if ( expectedCallDirection.CompareF( KDirectionMo ) == 0 )
        {
        callDirection = CCPCall::EMobileOriginated;
        }
    else if ( expectedCallDirection.CompareF( KDirectionMt ) == 0 )
        {
        callDirection = CCPCall::EMobileTerminated;
        }
    
    iMediatorObserver->AddExpectationL( expectedCallIndex, callType, callState, expectedServiceId, callDirection );
    
    return KErrNone;
    }
/*-----------------------------------------------------------------------------
    Function:   VerifyMediatorEvents
   

    Parameters: None
                
------------------------------------------------------------------------------*/

TInt CCCETester::VerifyMediatorEvents( CStifItemParser& /*aItems*/ )
    {
    return iMediatorObserver->Verify();
    }

/*-----------------------------------------------------------------------------
    Function:   CallIndexForCall
    Tests callindex of the call
   

    Parameters: 1. Integer, Call id
                2. Integer, Expected call index
------------------------------------------------------------------------------*/

TInt CCCETester::CallIndexForCall( CStifItemParser& aItem )
    {        
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    
    // Parameter 1: call id
    TInt callId;
    __ASSERT_ALWAYS( aItem.GetNextInt(callId) == KErrNone, 
        User::Panic( _L("CallIndexForCall, Error in parameter: Call id"), -6) );
    
    // Parameter 2: expected call index
    TInt expectedCallIndex;
    __ASSERT_ALWAYS( aItem.GetNextInt(expectedCallIndex) == KErrNone, 
        User::Panic( _L("CallIndexForCall, Error in parameter: Expected call index"), -6) );
    
    if ( iCallArray.Count() >= callId )
        {
        MCCECall* call = iCallArray[callId-1];
        if ( !call )
            {
            CCETESTERLOG1("[CCETEST] CCCETester::CallIndexForCall call with call id %d not found!!!!", callId);
            return KErrNotFound;
            }
        if ( call->CallIndex() != expectedCallIndex )
            {
            CCETESTERLOG("[CCETEST] CCCETester::CallIndexForCall call index does not match!");
            return KErrGeneral;
            }
        }
    else
        {
        CCETESTERLOG1("[CCETEST] CCCETester::CallIndexForCall call with call id %d not found!!!!", callId);
        return KErrNotFound;
        }
    return KErrNone;
    }

// End of file
