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

#include <mccpcall.h>
#include <mccpcallobserver.h>
#include <mccpforwardobserver.h>
#include <cccecallparameters.h>

#include "ccprefcall.h"
#include "ccprefconvergedcallprovider.h"
#include "ccpreftransferprovider.h"
#include "ccprefforwardprovider.h"
#include "cdelay.h"
#include "ccptestplugindefs.h"
#include "ccprefconferencecall.h"
#include "configurationhandler.h"
#include "escapeutils.h"
#include "ccptestpluginlogger.h"

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefCall::Delay( TInt aDelay )
	{    
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefCall::Delay start");
        
    TRAP_IGNORE( 
        CDelay* timer = CDelay::NewL();
	    timer->Start(aDelay * 1000);
	    delete timer;
	    );
    
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefCall::Delay finish");
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefCall::CCPRefCall(CCPRefConvergedCallProvider* aProvider, TUid aType ) :
    iIsOnHold(EFalse),
    iIsForwared(EFalse),
    iIsQueued(EFalse),
    iIsMobileOriginated(ETrue), // MO call as default, see NewL
    iRecipient(NULL),
    iNotifyAction(ETrue),
    iProvider(*aProvider),
    iIsCallActive(ETrue),    
    iCallState(MCCPCallObserver::ECCPStateIdle),
    iCallObserver(NULL),
    iTransferProvider(NULL),
    iForwardProvider(NULL),
    iCallParameters(NULL),
    iType( aType )
    {

    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefCall::~CCPRefCall()
    {
    delete iRecipient;
    delete iForwardProvider;
    delete iTransferProvider;
    delete iCallParameters;
    }



// -----------------------------------------------------------------------------
// Call creating comes here. In MT case observer is null and CCE will call
// SetObserver later on for us
// -----------------------------------------------------------------------------
//
CCPRefCall* CCPRefCall::NewL( CCPRefConvergedCallProvider* aProvider,
                              const TDesC& aRecipient,
	                          MCCPCallObserver* aObserver,
		                      TUid aType )
    {
    CCPRefCall* self = new( ELeave ) CCPRefCall( aProvider, aType );

    CleanupStack::PushL( self );
    self->ConstructL( aRecipient,aObserver );
    CleanupStack::Pop(self);
    
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefCall::ConstructL( const TDesC& aRecipient,
						     MCCPCallObserver* aObserver )
    {
    iRecipient = aRecipient.AllocL();
    iCallObserver = aObserver;

    iForwardProvider = CCPRefForwardProvider::NewL(*this);
    iTransferProvider = CCPRefTransferProvider::NewL(*this);
    
    if( !iCallObserver )
        {
        // MT call has no observer yet
        iIsMobileOriginated = EFalse;
        }
    }





// -----------------------------------------------------------------------------
// User has answered the incoming call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Answer()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPCall::Answer"),value ) )
    	{
    	return value;
    	}
    
    TInt retval = IsActionOK(ECCPRefCallActionAnswer);

    if( retval!=KErrNone )
        {
        return retval;
        }

    iIsQueued = EFalse; // In case we were queueing, flag change
    
    // Set new state and notify to Call Observer
    SetNewState(MCCPCallObserver::ECCPStateAnswering);

    // Set new state and notify to Call Observer
    SetNewState(MCCPCallObserver::ECCPStateConnected);
        
    return retval;
    }
    
// -----------------------------------------------------------------------------
// User has rejected the incoming call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Reject()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Reject"),value ) )
    	{
    	return value;
    	}
    
    TInt retval = IsActionOK(ECCPRefCallActionReject);

    if( retval!=KErrNone )
        {
        return retval;
        }

    iIsQueued = EFalse; // In case we were queueing, flag change
    iIsCallActive = EFalse;
    
    // Disconnecting -> Idle
    SetNewState(MCCPCallObserver::ECCPStateDisconnecting);
    SetNewState(MCCPCallObserver::ECCPStateIdle);

    return retval;
    }
    
// -----------------------------------------------------------------------------
// Queue incoming call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Queue()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Queue"),value ) )
    	{
    	return value;
    	}
    TInt retval = IsActionOK(ECCPRefCallActionQueue);

    if( retval!=KErrNone )
        {
        return retval;
        }

    iIsQueued = ETrue;
    SetNewState(MCCPCallObserver::ECCPStateQueued);

    return retval;
    }
    
 
// -----------------------------------------------------------------------------
// Dial a new call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Dial()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Dial"),value ) )
    	{
    	return value;
    	}

    TInt retval = IsActionOK(ECCPRefCallActionDial);

    if( retval!=KErrNone )
        {
        return retval;
        }
        
    TInt action = CheckForwardRequest(iRecipient);

	// notify dialling state
    SetNewState(MCCPCallObserver::ECCPStateDialling);
            
    SetNewState(MCCPCallObserver::ECCPStateConnecting);
    
	switch (action)
		{
		case 1:
			// Forward, basic
			SetNewState(MCCPCallObserver::ECCPStateForwarding);
            iForwardProvider->iObserver->ForwardEventOccurred( 
                MCCPForwardObserver::ECCPForwarded );	
	    	SetNewState(MCCPCallObserver::ECCPStateConnected);
	    	break;
	    	
		case 2:
			// Forward basic, remote
			SetNewState(MCCPCallObserver::ECCPStateForwarding);
            iForwardProvider->iObserver->ForwardEventOccurred(
                MCCPForwardObserver::ECCPRemoteForwarding );
            iForwardProvider->iObserver->ForwardEventOccurred(
                MCCPForwardObserver::ECCPForwarded );
	    	SetNewState(MCCPCallObserver::ECCPStateConnected);
	    	break;
	    	
		case 3:
			// Forward Multiple Choices, needs waiting for ForwardToAddressL
			// (in response of GetForwardAddressChoicesL)
			SetNewState(MCCPCallObserver::ECCPStateForwarding);
            iForwardProvider->iObserver->ForwardEventOccurred( 
                MCCPForwardObserver::ECCPMultipleChoices );
			break;
			
		case 4:
            iForwardProvider->iObserver->ForwardEventOccurred( 
                MCCPForwardObserver::ECCPMovedTemporarily );
            iForwardProvider->iObserver->ForwardEventOccurred( 
                MCCPForwardObserver::ECCPMovedPermanentlyEvent );
			break;
			
		default:
			// Normal behaviour
			SetNewState(MCCPCallObserver::ECCPStateConnected);
			break;
		}
		
    return retval;
    }



// -----------------------------------------------------------------------------
// End current call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::HangUp()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::HangUp"),value ) )
    	{
    	return value;
    	}

    // hang up allowed in any state
    
    iIsCallActive = EFalse;
    
#ifdef __PLUGIN_ASYNCHRONOUS_ACTION    
    
    DelayedHangUp( this, 1000 );

#else
    
    SetNewState(MCCPCallObserver::ECCPStateDisconnecting);
    SetNewState(MCCPCallObserver::ECCPStateIdle);
    
    if ( IsInConference( *this ) )
    	{
    	TRAP_IGNORE( iProvider.iConferenceCallProvider->RemoveCallL( this ); )
    	}

#endif

    return KErrNone;
    }
    


// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider::IncomingTransfer
// -----------------------------------------------------------------------------
void CCPRefCall::IncomingTransfer()
	{
	}
	
	
// -----------------------------------------------------------------------------
// MT call user ringing now
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Ringing()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Ringing"),value ) )
    	{
    	return value;
    	}

    TInt retval = IsActionOK(ECCPRefCallActionRinging);

    if( retval!=KErrNone )
        {
        return retval;
        }
	
    SetNewState(MCCPCallObserver::ECCPStateRinging);

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// Cancel ongoing request
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Cancel()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Cancel"),value ) )
    	{
    	return value;
    	}

    TInt retval = KErrNone;
    return retval;
    }


// -----------------------------------------------------------------------------
// Hold call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Hold()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Hold"),value ) )
    	{
    	return value;
    	}

    TInt retval = IsActionOK(ECCPRefCallActionHold);

    if( retval!=KErrNone )
        {
        return retval;
        }
        
    // Save option in case Hold/Resume is called due state change
    TBool notify = iNotifyAction;

    iIsOnHold = ETrue;
    SetNewState(MCCPCallObserver::ECCPStateHold);
    
    if ( notify )
    	{
        iCallObserver->CallEventOccurred( 
            MCCPCallObserver::ECCPLocalHold, this );
    	}
    else
    	{
	    // Back to default value
	    iNotifyAction = ETrue;
    	}

    return KErrNone;
    }


// -----------------------------------------------------------------------------
// Resume held call
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Resume()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Resume"),value ) )
    	{
    	return value;
    	}

    TInt retval = IsActionOK(ECCPRefCallActionResume);

    if( retval!=KErrNone )
        {
        return retval;
        }
        
    // Save option in case Hold/Resume is called due state change
    TBool notify = iNotifyAction;
    
    iIsOnHold = EFalse;
    SetNewState(MCCPCallObserver::ECCPStateConnected);
    
    if ( notify )
    	{
    	iCallObserver->CallEventOccurred( MCCPCallObserver::ECCPLocalResume, this );
    	}
    else
    	{
    	// Back to default value
    	iNotifyAction = ETrue;
    	}
    
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// Swap hold/resume states
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Swap()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Swap"),value ) )
    	{
    	return value;
    	}

    if( iIsOnHold )
        {
        Resume();
        }
    else
        {
        Hold();
        }

    return KErrNone;
    }



// -----------------------------------------------------------------------------
// Get call recipient
// -----------------------------------------------------------------------------
//
const TDesC& CCPRefCall::RemoteParty() const
    {
    HBufC8* buf = HBufC8::NewL(MaxStrSize);
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::RemoteParty"), *buf ) )
    	{
    	HBufC* bufc = EscapeUtils::ConvertToUnicodeFromUtf8L(*buf);
		// to do deleting returned value 
		delete buf; buf = NULL;
    	
    	
    	return *bufc;
    	}

    // to do deleting returned value 
	delete buf;
	buf = NULL;

    return *iRecipient;
    }
    
const TDesC& CCPRefCall::RemotePartyName()
    {
    HBufC8* buf = HBufC8::NewL(MaxStrSize);
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::RemotePartyName"), *buf ) )
    	{
    	HBufC* bufc = EscapeUtils::ConvertToUnicodeFromUtf8L(*buf);
		// to do deleting returned value 
		delete buf; buf = NULL;
    	
    	
    	return *bufc;
    	}

    // to do deleting returned value 
	delete buf;
	buf = NULL;

    return KNullDesC;
    }


const TDesC& CCPRefCall::DialledParty() const
	{
    HBufC8* buf = HBufC8::NewL(MaxStrSize);
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::DialledParty"), *buf ) )
    	{
    	HBufC* bufc = EscapeUtils::ConvertToUnicodeFromUtf8L(*buf);
		// to do deleting returned value 
		delete buf; buf = NULL;
    	
    	
    	return *bufc;
    	}

    // to do deleting returned value 
	delete buf; buf = NULL;

	return iProvider.iCalledParty->Des();
	}

// -----------------------------------------------------------------------------
// Is call forwarded or not
// -----------------------------------------------------------------------------
//
TBool CCPRefCall::IsCallForwarded() const
    {
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::IsCallForwarded"),value ) )
    	{
    	return value;
    	}

    return iIsForwared;
    }


// -----------------------------------------------------------------------------
// Is call mobile originated or not
// -----------------------------------------------------------------------------
//
TBool CCPRefCall::IsMobileOriginated() const
    {
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::IsMobileOriginated"),value ) )
    	{
    	return value;
    	}

    return iIsMobileOriginated;
    }


// -----------------------------------------------------------------------------
// Get current call state
// -----------------------------------------------------------------------------
//
MCCPCallObserver::TCCPCallState CCPRefCall::State() const
    {
    MCCPCallObserver::TCCPCallState value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::State"),value ) )
    	{
    	return value;
    	}

    return iCallState;
    }


// -----------------------------------------------------------------------------
// Get plug-in UID
// -----------------------------------------------------------------------------
//
TUid CCPRefCall::Uid() const
    {
    TUid value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Uid"),value ) )
    	{
    	return value;
    	}
    
    return iType;
    }


// -----------------------------------------------------------------------------
// get call capabilities
// -----------------------------------------------------------------------------
//
MCCPCallObserver::TCCPCallControlCaps CCPRefCall::Caps() const
    {
	MCCPCallObserver::TCCPCallControlCaps value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::Caps"),value ) )
    	{
    	return value;
    	}

    return iCaps;
    }


// -----------------------------------------------------------------------------
// Is call secured
// -----------------------------------------------------------------------------
//
TBool CCPRefCall::IsSecured() const
    {
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::IsSecured"),value ) )
    	{
    	return value;
    	}

    return iIsSecured;
    }

// -----------------------------------------------------------------------------
// Is call secure status specified
// -----------------------------------------------------------------------------
//
TBool CCPRefCall::SecureSpecified() const
    {
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::SecureSpecified"),value ) )
        {
        return value;
        }
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// Set paramaters
// -----------------------------------------------------------------------------
//
void CCPRefCall::SetParameters( const CCCPCallParameters& aNewParams )
    {
    delete iCallParameters;
    iCallParameters = NULL;
    iCallParameters = aNewParams.CloneL();
    }

// -----------------------------------------------------------------------------
// Get paramaters
// -----------------------------------------------------------------------------
//
const CCCPCallParameters& CCPRefCall::Parameters() const
    {
    /*
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::IsSecured"),value ) )
    	{
    	return value;
    	}
    */
    
    return *iCallParameters;
    }



// -----------------------------------------------------------------------------
// Get forward call enabling methods for the call
// -----------------------------------------------------------------------------
//
MCCPForwardProvider* CCPRefCall::ForwardProviderL( const MCCPForwardObserver& aObserver )
    {
    TInt value;
    TBool leave = EFalse;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::ForwardProviderL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefCall::ForwardProviderL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}
   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefCall::ForwardProviderL, configured to return=%d", value );
    	return (MCCPForwardProvider*)value;
    	}

	if (iForwardProvider)
        {
        iForwardProvider->AddObserverL(aObserver);
        }
    return iForwardProvider;
    }


// -----------------------------------------------------------------------------
// Get transfer call enabling methods for the call
// -----------------------------------------------------------------------------
//
MCCPTransferProvider* CCPRefCall::TransferProviderL( const MCCPTransferObserver& aObserver )
    {
    TInt value;
    TBool leave = EFalse;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::TransferProviderL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefCall::TransferProviderL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}
   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefCall::TransferProviderL, configured to return=%d", value );
    	return (MCCPTransferProvider*)value;
    	}

	if (iTransferProvider)
        {
    	iTransferProvider->AddObserverL(aObserver);
        }  

    return iTransferProvider;
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefCall::AddObserverL( const MCCPCallObserver& aObserver )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::AddObserverL"),value ) )
    	{
	   	CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefCall::AddObserverL, was configured to LEAVE=%d", value );
    	User::Leave(value);
    	}

    // only one observer used at a time, replaces current one
    iCallObserver = const_cast<MCCPCallObserver*>(&aObserver);
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::RemoveObserver(const MCCPCallObserver& aObserver )
    {
    TBool value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCall::RemoveObserver"),value ) )
    	{
    	return value;
    	}

    if( iCallObserver != &aObserver )
        {
        return KErrNotFound;
        }

    // only one observer used at a time
    iCallObserver = NULL;

    return KErrNone;
    }

// -----------------------------------------------------------------------------
//  Not in API
// -----------------------------------------------------------------------------
//
MCCPCallObserver* CCPRefCall::Observer() const
    {
    return iCallObserver;
    }



// -----------------------------------------------------------------------------
//  Not in API
// -----------------------------------------------------------------------------
//
void CCPRefCall::SetNewState( const MCCPCallObserver::TCCPCallState aNewState )
    {
    if ( !iCallObserver )
    	{
    	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefCall::SetNewState(), no observer");
    	}
    else if ( iCallState != aNewState )
    	{
	    iCallState = aNewState;
	    iCallObserver->CallStateChanged( aNewState, this );
    	}
    else
    	{
    	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefCall::SetNewState(), already in the requested state");
    	}
    }
// -----------------------------------------------------------------------------
// Not in API
// Example checks for different call actions
// -----------------------------------------------------------------------------
//       
TInt CCPRefCall::IsActionOK( const CCPRefCall::TCCPRefCallAction aAction )
    {
    TInt ok = KErrNone;
    
    // By default don't send ErrorOccurred event
    TCCPError error = ECCPErrorNone;
    
    switch( aAction )
        {
        case ECCPRefCallActionDial :
            {
            if( !iIsMobileOriginated ) 
                {
                ok = KErrNotSupported;
                }
            else if( iCallState!=MCCPCallObserver::ECCPStateIdle )
                {
                ok = KErrNotReady;
                }

            break;
            }

        case ECCPRefCallActionAnswer : 
            {
            if( iIsMobileOriginated )
                {
                ok = KErrNotSupported;
                } 
            else if( iCallState!=MCCPCallObserver::ECCPStateRinging && 
                iCallState!=MCCPCallObserver::ECCPStateQueued )
                {
                ok = KErrNotReady;
                }
           break;
           }
            
        case ECCPRefCallActionReject : 
            {
            if( iIsMobileOriginated )
                {
                ok = KErrNotSupported;
                } 
            else if( iCallState!=MCCPCallObserver::ECCPStateRinging && 
                     iCallState!=MCCPCallObserver::ECCPStateQueued &&
                     iCallState!=MCCPCallObserver::ECCPStateIdle )
                {
                ok = KErrNotReady;
                }

           break;
           }
           
        case ECCPRefCallActionHold : 
            {
            // hold from connected
            if ( iCallState == MCCPCallObserver::ECCPStateHold )
            	{
            	ok = KErrAlreadyExists;
                error = ECCPLocalHoldFail;
            	}
            else if( iCallState!=MCCPCallObserver::ECCPStateConnected )
                {
                ok = KErrNotReady;
                error = ECCPLocalHoldFail;
                }
            
            break;
            }
            
        case ECCPRefCallActionResume : 
            {
            // resume from hold
            if ( iCallState == MCCPCallObserver::ECCPStateConnected )
            	{
            	ok = KErrAlreadyExists;
            	error = ECCPLocalResumeFail;
            	}
            else if( !iIsOnHold )
                {
                ok = KErrNotReady;
                error = ECCPLocalResumeFail;
                }
            
            break;
            }
            
        case ECCPRefCallActionQueue : 
            {
            if( iIsMobileOriginated )
                {
                ok = KErrNotSupported;
                }
            else if ( iCallState!=MCCPCallObserver::ECCPStateIdle )
                {
                ok = KErrNotReady;
                }

            break;
            }
            
        case ECCPRefCallActionRinging : 
            {
            if( iIsMobileOriginated )
                {
                ok = KErrNotSupported;
                }
            else if( iCallState!=MCCPCallObserver::ECCPStateIdle )
                {
                ok = KErrNotReady;
                }

            break;
            }


        case ECCPRefCallActionTransfer : 
            {
            // transfer from connected and hold
            
            if( iCallState!=MCCPCallObserver::ECCPStateConnected &&
                iCallState!=MCCPCallObserver::ECCPStateHold )
                {
                ok = KErrNotReady;
                }

            break;
            }
            
        case ECCPRefCallActionForward : 
            {
            if( !iIsMobileOriginated && 
                iCallState!=MCCPCallObserver::ECCPStateConnecting && 
                iCallState!=MCCPCallObserver::ECCPStateRinging &&
                iCallState!=MCCPCallObserver::ECCPStateForwarding )
                {
                ok = KErrNotReady;
                }

            break;
            }

        default : break;       
        }
    
    if( ok != KErrNone && error != ECCPErrorNone)
        {
        if (iCallObserver)
        	{
        	iCallObserver->ErrorOccurred( error, this );
        	}
        }
        
    return ok;
    }


// -----------------------------------------------------------------------------
// Not in API
// Delete itself
// -----------------------------------------------------------------------------
//
void CCPRefCall::DeleteSelf()
    {
    //delete this;
    }

void CCPRefCall::SetMobileOriginated(TBool aIsMobileOriginated)
	{
	iIsMobileOriginated = aIsMobileOriginated;
	}
	
	

// -----------------------------------------------------------------------------
// Check for plugin commands in phone number
// return 0 => no action
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::CheckForwardRequest(HBufC* aInput)
	{	
	TLex cmd = aInput->Des();
	
	_LIT( KRequestTag, "plugincmd" );
	
	_LIT( KRequestForwardBasic, "forwardbasic" );
	_LIT( KRequestForwardRemote, "forwardremote" );
	_LIT( KRequestForwardChoices, "forwardchoices" );
	_LIT( KRequestForwardOther, "forwardother" );
	
	if ( cmd.Eos() ) return 1;
		
	cmd.Mark();
	
	// Parameter 1: Command tag
	TPtrC tokenTag = cmd.NextToken();
	if ( tokenTag.CompareF(KRequestTag) != 0 ) return 0; //continue Dial
	if ( cmd.Eos() ) return 0;
		
	// Parameter 2: Action
	TPtrC actionParam = cmd.NextToken(); //command
	if ( cmd.Eos() ) return 0;
	
	TBuf<KMaxKeywordLength> tokenAction(actionParam);
	tokenAction.LowerCase();
	
	// Parameter 3: NOT USED
	TPtrC tokenParam = cmd.NextToken();
	
	//TInt param = ReadNumber(tokenParam);
	
	TInt val = 0;
	TInt& paramVal = val;
	cmd.Val(paramVal);
	
	if ( tokenAction == KRequestForwardBasic ) 
		{
		return 1;
		}
	else if ( tokenAction == KRequestForwardRemote )
		{
		return 2;
		}
	else if ( tokenAction == KRequestForwardChoices )
		{
		return 3;
		}
	else if ( tokenAction == KRequestForwardOther )
		{
		return 4;
		}
	else 
		{
		// Non-valid command
		User::Panic( _L("CCPRefConferenceCall::CheckForwardRequest, invalid command"), KErrGeneral );
		return 0;	// get rid of compiler warning
		}
	}
		



void CCPRefCall::DontNotifyNext()
	{
	iNotifyAction = EFalse;
	}
	
TBool CCPRefCall::IsInConference( CCPRefCall& aCall )
	{
    TInt result = KErrNotFound;
    
    if( iProvider.iConferenceCallProvider )
        {
        result = iProvider.iConferenceCallProvider->iConfCallArray.Find( (MCCPCall*)&aCall );
        }
	
	if ( result == KErrNotFound )
		{
		return EFalse;
		}
	else
		{
		return ETrue;
		}
	
	}
	
TBool CCPRefCall::IsCallActive()
	{
	return iIsCallActive;
	}

TCCPTone CCPRefCall::Tone() const
	{
	return iTone;
	}


CCPRefConvergedCallProvider& CCPRefCall::Provider()
	{
	return iProvider;
	}


