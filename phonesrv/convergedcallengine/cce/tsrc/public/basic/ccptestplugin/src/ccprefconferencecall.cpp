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


//  INCLUDES
#include <e32base.h>
#include <e32debug.h>
#include <mccpconferencecallobserver.h>
#include "ccprefcall.h"
#include "ccprefconferencecall.h"
#include "ccprefconvergedcallprovider.h"
#include "ccptestpluginlogger.h"

// Forward declarations
class CCPRefCall;

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefConferenceCall::CCPRefConferenceCall( CCPRefConvergedCallProvider& aProvider ) : 
    iProvider(&aProvider),
    iObserver(NULL)
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefConferenceCall::~CCPRefConferenceCall() 
    {
    for( TUint i=0; i<iConfCallArray.Count(); i++ )
        {
        iConfCallArray.Remove( i );
        }

    iConfCallArray.Reset();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TUint32 CCPRefConferenceCall::ServiceId() const
    {
    TUint32 value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::ServiceId"),value ) )
    	{
    	return value;
    	}
    
    // uses same service id as for the provider
    return iProvider->ServiceId();
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::HangUp()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConferenceCall::HangUp");

    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::HangUp"),value ) )
    	{
    	return value;
    	}
    	
    if ( !iIsActive )
    	{
    	return KErrNotReady;
    	}
    
    // Send Conf call removed event for each call
    for ( TInt index=0; index<iConfCallArray.Count(); index++)
    	{
    	TInt ccpCall = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[index]);
    	
    	iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, iProvider->iCallArray[ccpCall] );
    	}
    
    // Notify Conference Idle state
    iIsActive = EFalse;
    iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceIdle );
    
    // Set the individual call states to Idle
    while ( iConfCallArray.Count() > 0 )
    	{
    	TInt ccpCall = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[0]);
    	
    	if ( ccpCall != KErrNotFound )
    		{
            // Removing call from conf call array here prevents Hangup to do the same thing
	    	iConfCallArray.Remove(0);
    		
    		switch ( iProvider->iCallArray[ccpCall]->State() )
				{
				case MCCPCallObserver::ECCPStateRinging:
				case MCCPCallObserver::ECCPStateQueued:
				case MCCPCallObserver::ECCPStateConnecting:
				case MCCPCallObserver::ECCPStateForwarding:
					iProvider->iCallArray[ccpCall]->iIsCallActive = EFalse;
					iProvider->iCallArray[ccpCall]->SetNewState( MCCPCallObserver::ECCPStateIdle );
					break;
				
				case MCCPCallObserver::ECCPStateConnected:
				case MCCPCallObserver::ECCPStateHold:
				case MCCPCallObserver::ECCPStateTransferring:		
					iProvider->iCallArray[ccpCall]->HangUp();
					break;
						
				default:	
					break;
				}
    		}
    	else
    		{
    		break;
    		}
    	}   
    
    __ASSERT_ALWAYS( iConfCallArray.Count() == 0, User::Panic( _L("CCPRefConferenceCall::HangUp, call count does not match"), KErrGeneral ) );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::Hold()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConferenceCall::Hold");
    
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::Hold"),value ) )
    	{
    	return value;
    	}

    if ( !iIsOnHold && iIsActive )
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::Hold - put conference to Hold") );

    	iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceHold );
    	
    	iIsOnHold = ETrue;
    	}
    else
    	{
    	return KErrNotReady;
    	}
    
    return KErrNone;
    }
      
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::Resume()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConferenceCall::Resume");
    
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::Resume"),value ) )
    	{
    	return value;
    	}
    
    if ( iIsOnHold && iIsActive )
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::Resume - resume conference from Hold") );

    	iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceActive );
    	
    	iIsOnHold = EFalse;
    	}
    else
    	{
    	return KErrNotReady;
    	}
    
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::Swap()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConferenceCall::Swap");
    
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::Swap"),value ) )
    	{
    	return value;
    	}

    TInt callCount = iConfCallArray.Count();
    
    TInt callIndex;
    
    if ( !iIsOnHold && iIsActive )
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::Swap - changing to ECCPConferenceHold") );
    	
    	/* All the calls in the conference are set to Hold state
    	*  (must be done before setting conference state!!!)
    	*/
    	for ( TUint i=0; i < callCount; i++ )
        	{
        	if (iConfCallArray[i]->State() == MCCPCallObserver::ECCPStateConnected )
        		{
	        	callIndex = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[i] );
	        	
	        	if ( callIndex != KErrNotFound )
	        		{
	        		iProvider->iCallArray[callIndex]->DontNotifyNext();
	        		iProvider->iCallArray[callIndex]->Hold();
	        		}
        		}
        	}

    	iIsOnHold = ETrue;    	
    	iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceHold );
    	}
    else
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::Swap - changing to ECCPConferenceActive") );
    	
    	/* Resume all calls that were in Hold
    	*/
    	for ( TUint i=0; i < callCount; i++ )
    		{
    		if (iConfCallArray[i]->State() == MCCPCallObserver::ECCPStateHold )
    			{    			
    			callIndex = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[i] );
    			
    			if ( callIndex != KErrNotFound )
    				{
    				iProvider->iCallArray[callIndex]->DontNotifyNext();
    				iProvider->iCallArray[callIndex]->Resume();
    				}
    			}
    		}
    	
    	iIsOnHold = EFalse;
    	iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceActive );	
    	}
    
    iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceSwapped );
        
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefConferenceCall::AddCallL( MCCPCall* aCall )
    {
    CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConferenceCall::AddCallL - Calls before = %d", iConfCallArray.Count() );
    
    User::LeaveIfNull(aCall);

    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::AddCallL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::AddCallL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}
    
    if( iConfCallArray.Find(aCall) == KErrNotFound )
        {
        iConfCallArray.AppendL(aCall);
        }
    else
    	{
    	User::Panic( _L("[TESTPLUGIN] CCPRefConferenceCall::AddCallL, call already added"), KErrGeneral );
    	}
    	
    __ASSERT_ALWAYS( iConfCallArray.Find( aCall ) != KErrNotFound, User::Panic( _L("CCPRefConferenceCall::AddCallL fatal error"), KErrGeneral ));
    
	// Find the aCall from CCP array
	TInt callAddedCcp = iProvider->iCallArray.Find( (CCPRefCall*)aCall );
	
	// If conferance active, set the added call to the same state
    if ( !iIsOnHold && iIsActive )
		{
		if ( aCall->State() == MCCPCallObserver::ECCPStateHold )
			{
			if ( callAddedCcp != KErrNotFound )
				{
				CCPTESTPLUGINLOG("[TESTPLUGIN] Resuming the added call" );
				iProvider->iCallArray[callAddedCcp]->DontNotifyNext();
    			iProvider->iCallArray[callAddedCcp]->Resume();
				}
			}
		}
    // If conference is on hold, set the added call to the same state (not if this is the first call)
	else if ( iIsActive && iIsOnHold && iConfCallArray.Count() > 1)
		{
		if ( aCall->State() == MCCPCallObserver::ECCPStateConnected )
			{
		    if ( callAddedCcp != KErrNotFound )
				{
				CCPTESTPLUGINLOG("[TESTPLUGIN] Holding the added call" );
				iProvider->iCallArray[callAddedCcp]->DontNotifyNext();
    			iProvider->iCallArray[callAddedCcp]->Hold();
				}
			}
		}
	
	/* Conference Call Added event
	*/
	iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallAdded, aCall );
	
    /* Conference Active state, notified when there are two calls
	*/
	if ( iConfCallArray.Count() == 2 )
	   	{
	   	ConferenceActive();
	  	}
	else if ( iConfCallArray.Count() > 8 )
		{
		User::Panic( _L("[TESTPLUGIN] CCPRefConferenceCall::AddCallL, too many calls added"), KErrGeneral );
		}
    
    CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConferenceCall::AddCallL - Calls after = %d", iConfCallArray.Count() );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//

void CCPRefConferenceCall::RemoveCallL( MCCPCall* aCall ) 
    {
    User::LeaveIfNull(aCall);
    
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::RemoveCallL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::AddCallL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}

    TInt numberOfCalls = iConfCallArray.Count();
 	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::RemoveCallL() - Before remove: %d"), numberOfCalls );
    __ASSERT_ALWAYS( numberOfCalls >= 0, User::Panic( _L("CCPRefConferenceCall call count under zero"), KErrGeneral ) );
    
    // Remove the given call from array
    TInt callToRemove = iConfCallArray.Find(aCall);
    
    if ( callToRemove < 0 || callToRemove > 8 )
    	{
    	User::Panic( _L("RemoveCallL, call not found "), KErrGeneral );
    	}
    
    /* Remove the call from conference and send event
    */
    iConfCallArray.Remove(callToRemove);
    iConfCallArray.Compress();
    iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, aCall );
    
    /* HangUp the removed call, if it is still found in CCPRefConvergedCallProvider->iCallArray
    *  If it is not found, the call has already been released.
    */
    CCPRefCall* call = (CCPRefCall*)aCall;
    //call->IsCallActive();
    
    if ( iProvider->iCallArray.Find( call ) != KErrNotFound 
         && call->IsCallActive() )
    	{
    	aCall->HangUp();
    	}
          
    /* If there is only one calls left in conference,
    *  Resume the last call in conference
   	*/
   	TInt debugg = iConfCallArray.Count();
   	
   	if ( iConfCallArray.Count() == 1 )
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::RemoveCallL, Not enough calls, conference will be ended") );
    	
    	/* If the there was less than three calls in conference,
	    *  then the conference will be ended:
	    *  - Set state to ConferenceIdle
	    *  - Reset the conference call array (last call is not removed)
	    */
	    
		iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, iConfCallArray[0] );
		
		// Remove (last) call from conf call array
		iConfCallArray.Remove(0);
    	iConfCallArray.Compress();
	    
	    iIsActive = EFalse;
    	iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceIdle );
    	
    	// Set the last call to Connected, is in index zero

    	//iConfCallArray.Reset() not needed since it is done by CCCEConferenceCall
    	
    	// only used for remote side conf end?
    	//iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceTerminated );
   		}
   		   	  		   	   
   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::RemoveCallL() - After remove = %d"), iConfCallArray.Count() );
   	}
    
    


// -----------------------------------------------------------------------------
// RemoveConfCall (internal method)
// 
// Like RemoveCallL, but does not call HangUp
// -----------------------------------------------------------------------------
//

void CCPRefConferenceCall::RemoveConfCall( CCPRefCall& aCall ) 
    {
    User::LeaveIfNull(&aCall);
    
    TInt numberOfCalls = iConfCallArray.Count();
 	
 	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::RemoveConfCall() - Before remove: %d"), numberOfCalls );
   
    __ASSERT_ALWAYS( numberOfCalls >= 0, User::Panic( _L("CCPRefConferenceCall call count under zero"), KErrGeneral ) );
    
    // Remove the given call from array
    
    TInt callToRemove = iConfCallArray.Find(&aCall);
    
    if ( callToRemove >=  numberOfCalls )
    	{
    	User::Panic( _L("RemoveCallL, call index out of bound"), KErrGeneral );
    	}
    
    /* If the call was found remove the call from conference and send event
    */
    if ( callToRemove != KErrNotFound )
    	{
    	RDebug::Print(_L("[TESTPLUGIN] Conference call found, removing") );
    	iConfCallArray.Remove(callToRemove);
    	iConfCallArray.Compress();
    	iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, &aCall );
    	}
   		   	  		   	   
   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::RemoveConfCall() - After remove = %d"), iConfCallArray.Count() );
   	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::CallCount() const
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::CallCount"),value ) )
    	{
    	return value;
    	}

    TInt numberOfCalls = iConfCallArray.Count();
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::CallCount() - Number of calls = %d"), numberOfCalls);
    //return iConfCallArray.Count();
    return numberOfCalls;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefConferenceCall::GoOneToOneL( MCCPCall& aCall ) 
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::GoOneToOneL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::GoOneToOneL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}

    TInt oneToOneCall = iConfCallArray.Find(&aCall);
    if( oneToOneCall==KErrNotFound )
        {
        User::Leave(KErrNotFound);
        }
      
    // remove privated call from conference
    TInt privatedCall = iConfCallArray.Find(&aCall);
    iConfCallArray.Remove(privatedCall);
    iConfCallArray.Compress();
    iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, &aCall );
    
    /* After removing the privated call from the conference, the number of calls in
     * conference determines the next action
     * 1) If there are still two or more calls in conference, put
     *    conference on hold
     * 2) If there is only one call left, put the call on hold and end conference
     */
    
    if ( iConfCallArray.Count() >= 2 )
    	{
		// Hold conference if Active    
	    if ( !iIsOnHold )
	    	{
	    	Swap();
	    	}        	
    	}
    else if ( iConfCallArray.Count() == 1 )
    	{
    	TInt callToHold = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[0] );
    	
    	/* Before setting conference to Idle, we must remove the
    	*  last call from iConfCallArray. Otherwise the ConferenceRelease (caused
    	*  by Idle) hangs up this call
    	*/
    	MCCPCall* notifyCall = iConfCallArray[0];
    	iConfCallArray.Remove(0);
    	iConfCallArray.Compress();
    	// Optional message
    	iObserver->ConferenceCallEventOccurred( MCCPConferenceCallObserver::ECCPConferenceCallRemoved, notifyCall );
    	
    	// Hold the last remaining call if connected
    	if ( iProvider->iCallArray[callToHold]->State() == MCCPCallObserver::ECCPStateConnected )
    		{
  			iProvider->iCallArray[callToHold]->DontNotifyNext();
  			iProvider->iCallArray[callToHold]->Hold();
    		}
  		
  		// End the conference
  		iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceIdle );
    	}
    else
    	{
    	// fatal error
    	User::Panic( _L("CCPRefConferenceCall::GoOneToOneL called when only one call in conference"), KErrGeneral );
    	}
    	
    /* Set privated call Active if on Hold
	*/
    TInt privatedCallIndex = iProvider->iCallArray.Find( (CCPRefCall*)&aCall );
    
    if ( privatedCallIndex != KErrNotFound )
    	{
		if ( iProvider->iCallArray[privatedCallIndex]->State() == MCCPCallObserver::ECCPStateHold )
	    	{   	
	    	iProvider->iCallArray[privatedCallIndex]->SetNewState( MCCPCallObserver::ECCPStateConnected );
	    	}
    	}
    else
    	{
    	// Error? Panic to check
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::GoOneToOneL, Call was not on Hold") );
    	User::Panic( _L("CCPRefConferenceCall::GoOneToOneL, Call was not on Hold"), KErrGeneral );
    	}
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::GoOneToOneL, end") );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefConferenceCall::CurrentCallsToConferenceL()
    {
    RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::CurrentCallsToConferenceL") );
    
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::CurrentCallsToConferenceL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::CurrentCallsToConferenceL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}

    RDebug::Print(_L("[TESTPLUGIN] calls already in conference: %d"), iConfCallArray.Count() );
    
    TInt addedCalls = 0;
    for ( ; addedCalls<iProvider->iCallArray.Count(); addedCalls++ )
        {
        MCCPCallObserver::TCCPCallState state = iProvider->iCallArray[addedCalls]->State();
        RDebug::Print(_L("[TESTPLUGIN] state: %d"), state );
        
        if ( state == MCCPCallObserver::ECCPStateConnected || 
             state == MCCPCallObserver::ECCPStateHold )
            {
            iConfCallArray.AppendL( iProvider->iCallArray[addedCalls] );
            }
        }
    
    if ( addedCalls > 1 ) 
        {
        ConferenceActive();
        }
    
    RDebug::Print(_L("[TESTPLUGIN] calls added to conference: %d"), addedCalls );
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefConferenceCall::AddObserverL( const MCCPConferenceCallObserver& aObserver )
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::AddObserverL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::AddObserverL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}

    // only one observer used at a time, replaces current one
    iObserver = &const_cast<MCCPConferenceCallObserver&>(aObserver);
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefConferenceCall::RemoveObserver(const MCCPConferenceCallObserver& aObserver )
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::RemoveObserver"),value ) )
    	{
    	return value;
    	}

    if( iObserver!=&aObserver )
        {
        return KErrNotFound;
        }

    // only one observer used at a time
    iObserver = NULL;

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// GetCallArray
// -----------------------------------------------------------------------------
TInt CCPRefConferenceCall::GetCallArray( RPointerArray<MCCPCall>& aCallArray )
	{
    aCallArray.Reset();
    
    for ( TInt i=0; i<iConfCallArray.Count(); i++ )
        {
        aCallArray.Append( iConfCallArray[i] );
        }
    
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// Not in API. Has conference been released or notAdd observer.
// -----------------------------------------------------------------------------
//
TBool CCPRefConferenceCall::IsReleased()
    {
    TBool value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPConferenceCall::IsReleased"),value ) )
    	{
    	return value;
    	}

    return iIsReleased;
    }

void CCPRefConferenceCall::ConferenceActive()
    {
    RDebug::Print(_L("[TESTPLUGIN] CCPRefConferenceCall::ConferenceActive") );
        
    if ( iObserver )
        {
        iObserver->ConferenceCallStateChanged( MCCPConferenceCallObserver::ECCPConferenceActive );
        }
    
    iIsActive = ETrue;
    
    // Conference becomes active, so all calls in conference are set to Connected state
    for ( TUint i=0; i < iConfCallArray.Count(); i++ )
        {
        if (iConfCallArray[i]->State() == MCCPCallObserver::ECCPStateHold )
            {
            // Find the aCall from CCP array
            TInt callIndex = iProvider->iCallArray.Find( (CCPRefCall*)iConfCallArray[i] );
            
            if ( callIndex != KErrNotFound )
                {
                iProvider->iCallArray[callIndex]->DontNotifyNext();
                iProvider->iCallArray[callIndex]->Resume();
                }
            }
        }
    }
