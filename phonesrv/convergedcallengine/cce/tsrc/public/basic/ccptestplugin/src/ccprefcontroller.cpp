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
* Description:  CCP test plug-in implementation.
*
*/


// Includes

#include <e32base.h>
#include <e32debug.h>
#include "ccprefcontroller.h"
#include "ccprefcall.h"
#include "cdelay.h"
#include "ccptestplugindefs.h"
#include "mccpobserver.h"
#include "ccpdefs.h"
#include "ccptestpluginlogger.h"
#include "cppcommands.hrh"
#include "ccprefconferencecall.h"

// Forward declarations

class MCCPObserver;

// Constants

_LIT( KRecipient, "recipentaddress" );
_LIT( KRemoteAddress, "RemoteAddress" );

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
CCPRefController::CCPRefController( 
    CCPRefConvergedCallProvider& aProvider,
    TUid aUid,
    CCPCall::TCallType aCallType,
    TInt aRequestValue ) :
        CActive(CActive::EPriorityUserInput),
        iPluginUid( aUid ),
        iProvider( aProvider ),
        iCallType( aCallType ),
        iRequestValue( aRequestValue )
	{
	}

// -----------------------------------------------------------------------------
// NewL
// -----------------------------------------------------------------------------
//
CCPRefController* CCPRefController::NewL( 
    CCPRefConvergedCallProvider& aProvider, 
    TUid aUid,
    CCPCall::TCallType aCallType,
    TInt aRequestValue )
	{
	CCPRefController* self = 
	    new (ELeave) CCPRefController( 
	        aProvider, aUid, aCallType, aRequestValue );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CCPRefController::~CCPRefController()
	{
	Cancel();
	iProperty.Close();

	TUid psUidEventRequest;
	psUidEventRequest.iUid = iRequestValue;
	RProperty::Delete(psUidEventRequest, KPropertyKeyEventIdentifier);

	delete iPluginCommand;
	}
	
// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//	
void CCPRefController::ConstructL()
	{	
	CActiveScheduler::Add(this);

#ifdef __ENABLE_PLUGIN_COMMANDS
	// Subscribe events ---------------------------------------
	
	TUid psUidEventRequest;
	psUidEventRequest.iUid = iRequestValue;
	
	TInt err = RProperty::Define( psUidEventRequest, 
                                  KPropertyKeyEventIdentifier,
                                  RProperty::EByteArray );
	                                 
	if ( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }
        
    User::LeaveIfError( iProperty.Attach( psUidEventRequest,
                        KPropertyKeyEventIdentifier ) );
    
    iProperty.Subscribe( iStatus );
    SetActive();
#endif

	iPluginCommand = HBufC::NewLC(128);
	CleanupStack::Pop(iPluginCommand);

	}

// -----------------------------------------------------------------------------
//   DoCancel()
// -----------------------------------------------------------------------------
//
void CCPRefController::DoCancel()
	{
	iProperty.Cancel();
	}
	

// -----------------------------------------------------------------------------
//   RunL()
// -----------------------------------------------------------------------------
//  
void CCPRefController::RunL()
	{
	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL");
	
	// resubscribe    
    iProperty.Subscribe( iStatus );
	SetActive();
	
    // Property updated, get new value

    TPtr commandData( iPluginCommand->Des() );
    
    if ( iProperty.Get( commandData ) == KErrNotFound )
        {
        // property deleted, do necessary actions here...
        }
    else
        {
        TLex input = TLex(commandData);
        
        TPtrC commandBuf = input.NextToken();
        TPluginCommand commandNumber = SetPluginCommand(commandBuf);
        
        TInt param1 = 0;
        TInt param2 = 0;
        TInt param3 = 0;
        
        TPtrC paramBuf = input.NextToken();
        if (paramBuf.CompareF(KNullDesC) != 0)
        	{
        	param1 = GetValue(paramBuf);
        	}

		paramBuf.Set( input.NextToken() );
		if (paramBuf.CompareF(KNullDesC) != 0)
			{
			param2 = GetValue(paramBuf);
			}

		paramBuf.Set( input.NextToken() );
		if (paramBuf.CompareF(KNullDesC) != 0)
			{
			param3 = GetValue(paramBuf);
			}
			
		TInt serviceID = 0;

        switch (commandNumber)
        	{          	
        	case EPluginCmdCreateCall:
        		// Request for MT call
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 1 Incoming Call");
        		
        		serviceID = param1;
        		
        		iProvider.CreateMtCall(serviceID);
        		break;
        	
        	case EPluginCmdHangupCall:
        		// Request Hangup for a call
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 2 Hangup");
        		
        		TInt callToHangup = param1 - 1;
        		
        		// Store observer before Hangup, because call might be removed during that
        		MCCPCallObserver* callObserver = iProvider.iCallArray[callToHangup]->iCallObserver;
        		
        		// ECCPRemoteTerminated disables ForceResume
        		callObserver->CallEventOccurred(
        		    MCCPCallObserver::ECCPRemoteTerminated, iProvider.iCallArray[callToHangup] );
        		
        		iProvider.iCallArray[callToHangup]->HangUp();
        		break;
        		
        	case EPluginCmdTransferRequest:
        		// Incoming Transfer
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 3 Incoming Transfer");
        		
        		TInt originatorCallID = param1;
        		serviceID = param3;
        		
        		TBool attendedTransfer;
        		
        		if ( param2 == 0 )
        			{
        			attendedTransfer = EFalse;
        			}
        		else if ( param2 == 1 )
        			{
        			attendedTransfer = ETrue;
        			}
        		else
        			{
        			User::Panic( _L("[TESTPLUGIN] CCPRefController::RunL(), error in parameter: attendedTransfer"), -6);
        			}
        		
        		iProvider.IncomingTransfer( originatorCallID, attendedTransfer, serviceID );
        		break;
        		
        	case EPluginCmdRemoteHold:
        		// Remote Hold
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 4 Remote Hold");
        		
        		TInt callToHold = param1 - 1;
        		
        		// call is not put to hold state
        		        		
        		iProvider.iCallArray[callToHold]->iCallObserver->CallEventOccurred(
        		    MCCPCallObserver::ECCPRemoteHold, iProvider.iCallArray[callToHold] );
        		break;	
        	
        	case EPluginCmdRemoteResume:
        		// Remote Resume
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 5 Remote Resume");
        		
        		TInt callToResume = param1 - 1;
        		
        		iProvider.iCallArray[callToResume]->iCallObserver->CallEventOccurred(
        		    MCCPCallObserver::ECCPRemoteResume, iProvider.iCallArray[callToResume] );
        		break;
            		
        	case EPluginCmdReportCallCount:
        		// Count calls
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 6 Count calls");
        		iProvider.CountRemainingCalls();
        		break;
        		
        	case EPluginCmdSendTestEvents:
        		// Send events (coverage test)
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 7, Send events");
        		
	       		switch (param1)
	       			{	       			
		        	case 1: 
		        		SendTestEvents1();
		        		break;
		        		
		        	case 2:
		        	    serviceID = param1;
		        	    SendTestEvents2(serviceID);
		        		break;

		        	default:
		        		break;
	       			}
	       		break;
	       	        		
        	case EPluginCmdErrorOccurred:
        		// ErrorOccurred event
        		CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::RunL: Plugin Command request 8, ErrorOccurred");
        		
        		TCCPError errorCode = (TCCPError)param1;
        		iProvider.iCallArray[0]->iCallObserver->ErrorOccurred( 
        		    errorCode, iProvider.iCallArray[0] );
        		break;
        		
        	case EPluginCmdSetState:
        		// Set state
        		TInt callID = param1 - 1;
        		TInt state = param2;
        		SetPluginState( callID, (MCCPCallObserver::TCCPCallState)state );
        		break;
        		
        	case EPluginInitiatedCSMoCall:
        		TInt serviceId = param2;
       			PluginInitiatedCSMoCall(serviceId);
        		break;
        		
        	case EPluginCmdCreateConferenceCall:
                PluginInitiatedConferenceCall();
        	    break;
        		
        	default:
        		break;
        	}
        }
	}


// ---------------------------------------------------------------
// CCPRefController::Delay(TUint aDelayInMilliseconds)
// ---------------------------------------------------------------
void CCPRefController::Delay( TUint aDelayInMilliseconds )
	{
	CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefController::Delay(), %d millisec", aDelayInMilliseconds );
	    
    CDelay* timer = CDelay::NewL();
    timer->Start( aDelayInMilliseconds );
    
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::Delay() stop");
	}


void CCPRefController::DelayedHangUp( CCPRefCall* aCall, TInt aDelayInHundredths )
	{
	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefController::DelayedHangUp()");
	
	TCallBack delayedHangUpFunction( DoHangUp, aCall );
	
	CPeriodic* delayedTimer = CPeriodic::NewL( CTimer::EPriorityIdle );
	
	delayedTimer->Start( aDelayInHundredths*10000, KMaxTInt32, delayedHangUpFunction );
	}
	
TInt CCPRefController::DoHangUp( TAny* anyPtr )
	{
    CCPRefCall* call = static_cast<CCPRefCall*>( anyPtr );
    call->iCallState = MCCPCallObserver::ECCPStateIdle;
    
    call->iCallObserver->CallStateChanged(
        MCCPCallObserver::ECCPStateDisconnecting, call );
    call->iCallObserver->CallStateChanged(
        MCCPCallObserver::ECCPStateIdle, call );
    
    call->iCallObserver->CallEventOccurred(
        MCCPCallObserver::ECCPRemoteTerminated, call );
    
    return 0;
	}

void CCPRefController::SendTestEvents1()
	{
	// Send bunch of events to 1st call in array
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPLocalBarred, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPRemoteBarred, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPRemoteWaiting, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPQueued, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCCSPEarlyMediaStarted, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPNotSecureCall, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPRemoteConferenceCreate, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPCSDataPortLoaned, iProvider.iCallArray[0] );
	iProvider.iCallArray[0]->iCallObserver->CallEventOccurred(
	    MCCPCallObserver::ECCPCSDataPortRecovered, iProvider.iCallArray[0] );
	
	iProvider.iCallArray[0]->iCallObserver->CallCapsChanged( 
	    (TUint32)0, iProvider.iCallArray[0] );
	
	iProvider.iCallArray[0]->iCallObserver->CallStateChangedWithInband( 
	    MCCPCallObserver::ECCPStateIdle, iProvider.iCallArray[0] );

	}
	
void CCPRefController::SendTestEvents2(TInt aServiceId )
	{
	// Test1
	iProvider.iObserver->IncomingCall( NULL );
	
	// Test2
	
	CCPRefCall* mtcall = CCPRefCall::NewL( &iProvider,
                                           KRecipient,
                                           NULL, 		// There isn't observer yet
                                           iPluginUid );
                                           
	mtcall->SetMobileOriginated(EFalse);
	CCCPCallParameters* callParams;
	TRAP_IGNORE( callParams = CCCPCallParameters::NewL() );
	__ASSERT_ALWAYS( callParams, User::Panic( _L("[TESTPLUGIN] CCPRefController::PluginInitiatedCSMoCall()"), -4) );
	callParams->SetServiceId( aServiceId );
    callParams->SetCallType( iCallType );

	mtcall->SetParameters( *callParams );
	delete callParams;
	
	CleanupStack::PushL(mtcall);
    // Store the new call, in this case we will use a pointer array
    iProvider.iCallArray.AppendL(mtcall);
    CleanupStack::Pop(mtcall);
        
    iProvider.iObserver->IncomingCall( NULL, *mtcall );
    iProvider.iObserver->IncomingCall( mtcall, *mtcall );
    
    iProvider.iSsObserver->BarringEventOccurred( MCCPSsObserver::ECCPSsIncomingCallBarred );
    iProvider.iSsObserver->BarringEventOccurred( MCCPSsObserver::ECCPSsAnonymousCallBarred );
    iProvider.iSsObserver->BarringEventOccurred( MCCPSsObserver::ECCPSsOutgoingCallBarred );
    
    iProvider.iSsObserver->CLIEventOccurred( MCCPSsObserver::ECCPSsTempClirSuppressUnsuccessful );
    iProvider.iSsObserver->CLIEventOccurred( MCCPSsObserver::ECCPSsTempClirActivationUnsuccessful );
    
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsIncCallIsForw, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsIncCallForwToC, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsOutCallForwToC, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsForwardUnconditionalModeActive, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsForwardUnconditionalModeNotActive, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsForwardConditionallyModeActive, KRemoteAddress );
    iProvider.iSsObserver->CallForwardEventOccurred( MCCPSsObserver::ECCPSsForwardConditionallyModeNotActive, KRemoteAddress );
    
    iProvider.iSsObserver->CallCugEventOccurred( MCCPSsObserver::ECCPSsShowIncCallGroupIndex );
    iProvider.iSsObserver->CallCugEventOccurred( MCCPSsObserver::ECCPSsRejectedCause );
    
    MCCPCSObserver* csObserver = static_cast<MCCPCSObserver*>(iProvider.iObserver);
    csObserver->MoCallCreated( *mtcall );
    
	_LIT(KDataPortName, "Data Port Test Name");
	TBuf<128> dataportname(KDataPortName);
    
    csObserver->DataPortName( dataportname );
	}
	
void CCPRefController::PluginInitiatedCSMoCall( TInt aServiceId )
	{
	
	CCPRefCall* pluginCall = CCPRefCall::NewL( &iProvider,
                                           KRecipient,
                                           NULL, 		// There isn't observer yet
                                           iPluginUid );
                                           
	pluginCall->SetMobileOriginated( ETrue );
	
	CCCPCallParameters* callParams;
	TRAP_IGNORE( callParams = CCCPCallParameters::NewL() );
	__ASSERT_ALWAYS( callParams, User::Panic( _L("[TESTPLUGIN] CCPRefController::PluginInitiatedCSMoCall()"), -4) );
	callParams->SetServiceId( aServiceId );
    callParams->SetCallType( CCPCall::ECallTypeCSVoice );
	pluginCall->SetParameters( *callParams );
	delete callParams;
                                           
	MCCPCSObserver* csObserver = static_cast<MCCPCSObserver*>(iProvider.iObserver);
	
	csObserver->MoCallCreated( *pluginCall );	

	delete pluginCall;
	
	}
	
void CCPRefController::PluginInitiatedConferenceCall()
    {
    if( iProvider.iConferenceCallProvider )
        {
        iProvider.iConferenceCallProviderTemp = new (ELeave) CCPRefConferenceCall( iProvider );
        MCCPCSObserver* csObserver = static_cast<MCCPCSObserver*>(iProvider.iObserver);
        csObserver->ConferenceCallCreated( *iProvider.iConferenceCallProviderTemp );
        }
    else
        {
        iProvider.iConferenceCallProvider = new (ELeave) CCPRefConferenceCall( iProvider );
        iProvider.iConferenceCallProvider->CurrentCallsToConferenceL();
        MCCPCSObserver* csObserver = static_cast<MCCPCSObserver*>(iProvider.iObserver);
        csObserver->ConferenceCallCreated( *iProvider.iConferenceCallProvider );
        }

    }
	
	
TInt CCPRefController::GetValue(TPtrC& aInput)
	{
	TLex input = TLex(aInput);
	TInt number;
	
	input.Val(number);	
	
	return number;
	}
	

CCPRefController::TPluginCommand CCPRefController::SetPluginCommand( const TDesC& aPluginCommand )
	{
	
	TBuf<KMaxKeywordLength> commandLowercase(aPluginCommand);
	commandLowercase.LowerCase();
	
	if ( commandLowercase.CompareF(KPluginCmdCreateCall) == 0 )
		{
		return EPluginCmdCreateCall;
		}
	else if ( commandLowercase.CompareF(KPluginCmdHangUpCall) == 0 )
		{
		return EPluginCmdHangupCall;
		}
	else if ( commandLowercase.CompareF(KPluginCmdTransferRequest) == 0 )
		{
		return EPluginCmdTransferRequest;
		}
	else if ( commandLowercase.CompareF(KPluginCmdRemoteHold) == 0 )
		{
		return EPluginCmdRemoteHold;
		}
	else if ( commandLowercase.CompareF(KPluginCmdRemoteResume) == 0 )
		{
		return EPluginCmdRemoteResume;
		}
	else if ( commandLowercase.CompareF(KPluginCmdReportCallCount) == 0 )
		{
		return EPluginCmdReportCallCount;
		}
	else if ( commandLowercase.CompareF(KPluginCmdSendTestEvents) == 0 )
		{
		return EPluginCmdSendTestEvents;
		}
	else if ( commandLowercase.CompareF(KPluginCmdErrorOccurred) == 0 )
		{
		return EPluginCmdErrorOccurred;
		}
	else if ( commandLowercase.CompareF(KPluginCmdSetAsyncMode) == 0 )
		{
		return EPluginCmdSetAsyncMode;
		}
	else if ( commandLowercase.CompareF(KPluginCmdSetState) == 0 )
		{
		return EPluginCmdSetState;
		}
	else if ( commandLowercase.CompareF(KPluginCmdCancel) == 0 )
		{
		return EPluginCmdCancel;
		}
	else if ( commandLowercase.CompareF(KPluginCmdPluginInitCsMoCall) == 0 )
		{
		return EPluginInitiatedCSMoCall;
		}
	else if ( commandLowercase.CompareF(KPluginCmdCreateConferenceCall) == 0 )
        {
        return EPluginCmdCreateConferenceCall;
        }
	else
		{
		User::Panic( _L("CCCETester::SetPluginCommand, Incorrect Keyword"), KErrArgument);
		return (CCPRefController::TPluginCommand)KErrNotFound; // compiler warning if no return
		}
	}
	
void CCPRefController::SetPluginState( TInt aCallID, MCCPCallObserver::TCCPCallState aState )
	{
	iProvider.iCallArray[aCallID]->SetNewState( aState );
	}
