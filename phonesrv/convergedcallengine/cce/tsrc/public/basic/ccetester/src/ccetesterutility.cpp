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
* Description:  STIF test scripter utility methods
*
*/


// Includes

#include <mcceconferencecallobserver.h>
#include <mccedtmfinterface.h>
#include "cccetester.h"
#include "ccetesterdefs.h"
#include "e32debug.h"
#include "e32property.h"
#include "ctestconfcallobserver.h"
#include "cppcommands.hrh" // Plugin commands

// Call events

_LIT( KEventLocalHold, "localhold" );
_LIT( KEventRemoteHold, "remotehold" );
_LIT( KEventLocalResume, "localresume" );
_LIT( KEventRemoteResume, "remoteresume" );
_LIT( KEventLocalDeflectCall, "localdeflectcall" );
_LIT( KEventLocalTransfer, "localtransfer" );
_LIT( KEventRemoteTransferring, "remotetransferring" );
_LIT( KEventRemoteTransferAlerting, "remotetransferalerting" );
_LIT( KEventRemoteTerminated, "remoteterminated" );
_LIT( KEventRemoteConferenceCreate, "remoteconferencecreate" );
_LIT( KEventRemoteBarred, "remotebarred" );
_LIT( KEventRemoteForwarding, "remoteforwarding" );
_LIT( KEventRemoteWaiting, "remotewaiting" );
_LIT( KEventLocalBarred, "localbarred" );
_LIT( KEventSecureSession, "securesession" );
_LIT( KEventNotSecureSession, "notsecuresession" );
_LIT( KEventVoIPEventNotSecureSessionWithSips, "voipnotsecuresessionwithsips" );
_LIT( KEventVoIPEventEarlyMediaStarted, "voipearlymediastarted" );
_LIT( KEventVoIPEventMultipleChoices, "voipmultiplechoices" );
_LIT( KEventVoIPEventMovedPermanently, "voipmovedpermanently" );
_LIT( KEventCCECSLocalDeflectCall, "cslocaldeflectcall" );
_LIT( KEventCCECSDataPortLoaned, "csdataportloaned" );
_LIT( KEventCCECSDataPortRecovered, "csdataportrecovered" );
_LIT( KEventErrorOccurred, "erroroccurred" );

// Conference call
_LIT( KEventConferenceIdle, "conferenceidle" );
_LIT( KEventConferenceActive, "conferenceactive" );
_LIT( KEventConferenceHold, "conferencehold" );

_LIT( KEventConferenceCallAdded, "conferencecalladded" );
_LIT( KEventConferenceCallRemoved, "conferencecallremoved" );
_LIT( KEventConferenceBuilt, "conferencecallbuilt" );
_LIT( KEventConferenceTerminated, "conferencecallterminated" );
_LIT( KEventConferenceSwapped, "conferencecallswapped" );
_LIT( KEventConferenceSplit, "conferencecallsplit" );

// Dtmf

_LIT( KEventDtmfUnknown, "dtmfunknown" );
_LIT( KEventDtmfManualStart, "dtmfmanualstart" );
_LIT( KEventDtmfManualStop, "dtmfmanualstop" );
_LIT( KEventDtmfManualAbort, "dtmfmanualabort" );
_LIT( KEventDtmfSequenceStart, "dtmfsequencestart" );
_LIT( KEventDtmfSequenceStop, "dtmfsequencestop" );
_LIT( KEventDtmfSequenceAbort, "dtmfsequenceabort" );
_LIT( KEventDtmfStopInDtmfString, "dtmfstopindtmfstring" );
_LIT( KEventDtmfStringSendingCompleted, "dtmfstringsendcompleted" );

// Incoming call

_LIT( KEventIncomingCallKeyword, "incomingcall" );
_LIT( KEventMOCallCreatedKeyword, "mocallcreated" );
_LIT( KEventConferenceCallCreated, "conferencecallcreated" );
_LIT( KEventHandleTransfer, "handletransfer" );

/*-----------------------------------------------------------------------------
	CCCETester::SetEvent(TEvent& aEvent, const TDesC* aEventName)
	
	Parameters:
	 - aEventName is the received keyword
	 - aEvent is the event given as parameter as set corresponding to the keyword
------------------------------------------------------------------------------*/
void CCCETester::SetEventName( TEvent& aEvent, const TDesC* aEventName )
	{
	
	TBuf<KMaxKeywordLength> convertLC(*aEventName);
	convertLC.LowerCase();
	TPtrC eventName(convertLC);
	
	// Call states
		
	if ( eventName.CompareF(KStateIdle) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateIdle;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateRinging) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateRinging;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateConnecting) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateConnecting;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateConnected) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateConnected;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateDisconnecting) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateDisconnecting;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateHold) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateHold;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateTransferring) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateTransferring;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateForwarding) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateForwarding;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateQueued) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateQueued;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateAnswering) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateAnswering;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KStateDialling) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
		aEvent.iEventID = CCPCall::EStateDialling;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}

	// Call events

	else if ( eventName.CompareF(KEventLocalHold) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ELocalHold;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
		
	else if ( eventName.CompareF(KEventRemoteHold) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteHold;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventLocalResume) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ELocalResume;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteResume) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteResume;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventLocalDeflectCall) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ELocalDeflectCall;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventLocalTransfer) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ELocalTransfer;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteTransferring) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteTransferring;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteTransferAlerting) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteTransferAlerting;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteTerminated) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteTerminated;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteConferenceCreate) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteConferenceCreate;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteBarred) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteBarred;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteForwarding) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteForwarding;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventRemoteWaiting) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ERemoteWaiting;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventLocalBarred) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ELocalBarred;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventSecureSession) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ESecureSession;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventNotSecureSession) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ENotSecureSession;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventVoIPEventNotSecureSessionWithSips) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::EVoIPEventNotSecureSessionWithSips;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventVoIPEventEarlyMediaStarted) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::EVoIPEventEarlyMediaStarted;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventVoIPEventMultipleChoices) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::EVoIPEventMultipleChoices;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventVoIPEventMovedPermanently) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::EVoIPEventMovedPermanently;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventCCECSLocalDeflectCall) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ECCECSLocalDeflectCall;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventCCECSDataPortLoaned) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ECCECSDataPortLoaned;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventCCECSDataPortRecovered) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
		aEvent.iEventID = MCCECallObserver::ECCECSDataPortRecovered;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
		
	// Call ErrorOccurred
	
	else if ( eventName.CompareF(KEventErrorOccurred) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverErrorOccurred;
		aEvent.iEventID = 0;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
		
	// Conference call states
		
	else if ( eventName.CompareF(KEventConferenceIdle) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallStateChanged;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceIdle;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceActive) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallStateChanged;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceActive;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceHold) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallStateChanged;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceHold;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	
	// Conference call events
	
	else if ( eventName.CompareF(KEventConferenceCallAdded) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceCallAdded;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceCallRemoved) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceCallRemoved;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceBuilt) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceBuilt;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceTerminated) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceTerminated;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceSwapped) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceSwapped;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventConferenceSplit) == 0 )
		{
		aEvent.iEventType = CEventStorage::EConferenceCallEvent;
		aEvent.iEventID = MCCEConferenceCallObserver::ECCEConferenceSplit;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	
	// DTMF events
	
	else if ( eventName.CompareF(KEventDtmfUnknown) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfUnknown;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfManualStart) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfManualStart;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfManualStop) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfManualStop;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfManualAbort) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfManualAbort;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfSequenceStart) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfSequenceStart;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfSequenceStop) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfSequenceStop;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfSequenceAbort) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfSequenceAbort;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfStopInDtmfString) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfStopInDtmfString;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventDtmfStringSendingCompleted) == 0 )
		{
		aEvent.iEventType = CEventStorage::EDtmfEvent;
		aEvent.iEventID = MCCEDtmfObserver::ECCEDtmfStringSendingCompleted;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
		
	// Incoming call
	
	else if ( eventName.CompareF(KEventIncomingCallKeyword) == 0 )
		{
		aEvent.iEventType = CEventStorage::EObserverIncomingCall;
		aEvent.iEventID = 0;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else if ( eventName.CompareF(KEventMOCallCreatedKeyword) == 0 )
		{
		aEvent.iEventType = CEventStorage::EObserverMOCallCreated;
		aEvent.iEventID = 0;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
    else if ( eventName.CompareF(KEventConferenceCallCreated) == 0 )
        {
        aEvent.iEventType = CEventStorage::EObserverConferenceCallCreated;
        aEvent.iEventID = 0;
        aEvent.iParam1 = 0;
        aEvent.iParam2 = 0;
        }	
	// HandleTransfer
	else if ( eventName.CompareF(KEventHandleTransfer) == 0 )
		{
		aEvent.iEventType = CEventStorage::ECallObserverHandleTransfer;
		aEvent.iEventID = 0;
		aEvent.iParam1 = 0;
		aEvent.iParam2 = 0;
		}
	else
		{
		RDebug::Print(_L("[CCETEST] SetEventName: Unrecognised Keyword %S"), &eventName );
		User::Panic( _L("SetEventName, Incorrect Keyword"), KErrArgument );
		}
	}

TInt CCCETester::CheckPluginCommand( TPtrC& aItem )
	{
	if ( aItem.CompareF(KPluginCmdCreateCall) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdHangUpCall) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdTransferRequest) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdRemoteHold) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdRemoteResume) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdReportCallCount) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdSendTestEvents) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdErrorOccurred) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdSetState) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdPluginInitCsMoCall) == 0 )
		{
		return KErrNone;
		}
	else if ( aItem.CompareF(KPluginCmdCreateConferenceCall) == 0 )
        {
        return KErrNone;
        }
	else
		{
		User::Panic( _L("CCCETester::SetPluginCommand, Incorrect Keyword"), KErrArgument);
		return KErrNotFound; // compiler warning if no return
		}
	}


                                  
	
// -----------------------------------------------------------------------------
// CCCETester::CheckCallID
// -----------------------------------------------------------------------------
//
void CCCETester::CheckCallIdL( TInt aCallID, TBool aIsNewCallId /* =EFalse */)
	{
	
	if ( aIsNewCallId == EFalse )
		{
		// Check that callID is at least 1, but not over the count
		if ( aCallID <= 0 || aCallID > iCallArray.Count() )
			{
			RDebug::Print(_L("MaxCallID=%d RequestedCallID=%d" ), iCallArray.Count(), aCallID);
			User::Leave(KErrArgument);
			}
			
		// Check that the referred call is not null
		if ( iCallArray[aCallID -1] == NULL )
			{
			RDebug::Print(_L("CCCETester::CheckCallID, The referred call (%d) is NULL" ), aCallID );
			User::Leave(KErrArgument);
			}
		}
	else
		{
		// Check that callID is the next unallocated
		if ( aCallID != iCallArray.Count()+1 )
			{
			RDebug::Print(_L("CallID was %d, should be %d" ), aCallID, iCallArray.Count()+1 );
			User::Leave(KErrArgument);
			}
		}
	}

// -----------------------------------------------------------------------------
// CCCETester::CheckDtmf
// -----------------------------------------------------------------------------	
void CCCETester::CheckDtmf()
	{
    // Check if Dtmf already exists
    if (!iDtmfTest)
        {
        iDtmfTest = &iCceTest->DtmfInterfaceL( *iDtmfObserver );
        }
	}

// -----------------------------------------------------------------------------
// CCCETester::CheckExtIf
// -----------------------------------------------------------------------------	
void CCCETester::CheckExtIfL()
	{
    // Check if Dtmf already exists
    if (!iExtIfTest)
        {
        iExtIfTest = &iCceTest->ExtensionInterfaceL( *iExtIfObserver );
        }
	}


// -----------------------------------------------------------------------------
// CCCETester::CheckConferenceCall
// -----------------------------------------------------------------------------
void CCCETester::CheckConferenceCall()
	{
	// Check if MCCEConferenceCall already exists
	if (!iConferenceCall)
		{
		iConferenceCall = &iCceTest->ConferenceCallL( *iConfCallObserver );
		}
	}
	

// -----------------------------------------------------------------------------
// CCCETester::InsertCall
// -----------------------------------------------------------------------------
TInt CCCETester::InsertCall( const MCCECall& aCall )
	{
	TInt err = iCallArray.Append( &aCall );
	
	return err;
	}
	

// -----------------------------------------------------------------------------
// CCCETester::InsertCallObserver
// -----------------------------------------------------------------------------
TInt CCCETester::InsertCallObserver( const CTestCallObserver& newCallObs )
	{
	TInt err = iCallObserverArray.Append( &newCallObs );
	
	return err;
	}
	

// -----------------------------------------------------------------------------
// CCCETester::IsNumber
// -----------------------------------------------------------------------------
TBool CCCETester::IsNumber(TPtrC& aInput)
	{
	TLex input = TLex(aInput);
	TChar ch;
	
	while ( ch = input.Get() )
		{
		if ( !ch.IsDigit() )
			{
			return EFalse;
			}
		}
	
	return ETrue;	
	}


// -----------------------------------------------------------------------------
// CCCETester::GetValue
// -----------------------------------------------------------------------------
TInt CCCETester::GetValue(TPtrC& aInput)
	{
	TLex input = TLex(aInput);
	TInt number;
	
	input.Val(number);	
	
	return number;
	}


// -----------------------------------------------------------------------------
// CCCETester::UnhandledEvents
// -----------------------------------------------------------------------------
TInt CCCETester::UnhandledEvents()
	{
	
#ifdef __DISABLE_EVENT_VERIFICATION
	return KErrNone;
#endif

	RDebug::Print(_L("[CCETEST] received=%d handled=%d" ), iEventStorage->iReceivedEvents.Count(), iTestStep);
	
	TInt unverifiedEvents = iEventStorage->iReceivedEvents.Count() - iTestStep;
	
	if ( unverifiedEvents > 0 )
		{
		for ( TInt counter=0; unverifiedEvents-counter > 0; counter++ )
			{
			TInt index = iEventStorage->iReceivedEvents.Count() - unverifiedEvents - counter;
					
			RDebug::Print(_L("************************************************************************************" ) );
			RDebug::Print(_L("[CCETEST] Unverified event(s): call=%d type=%d eventid=%d par1=%d par2=%d"),
			iEventStorage->iReceivedEvents[index].iCallID,
		    iEventStorage->iReceivedEvents[index].iEventType,
		    iEventStorage->iReceivedEvents[index].iEventID,
		    iEventStorage->iReceivedEvents[index].iParam1,
		    iEventStorage->iReceivedEvents[index].iParam2 );
			}
		
		return KErrNotReady;
		}
	else if ( unverifiedEvents < 0 )
		{
		// Event count is messed up
		return KErrUnknown;
		}
	else
		{
		return KErrNone;
		}
	}
	

void CCCETester::RemoveComma( TPtrC& aEventParam )
	{
	// Remove last char from keyword
	_LIT(KCharComma, ",");
	TBufC<1> commaPtr(KCharComma);
	
	// Check if there is comma
	TInt commaLocation = aEventParam.Find(commaPtr);

	// Remove comma if found
	if (commaLocation >= 0)
		{
		TPtrC paramFixed = aEventParam.Left( aEventParam.Length()-1 );
		aEventParam.Set(paramFixed);
		}		
	}
	
// end of file
	
