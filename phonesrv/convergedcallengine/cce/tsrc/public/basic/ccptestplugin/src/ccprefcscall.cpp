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

#include <e32debug.h>
#include <mccpcscall.h>
#include <mccpforwardobserver.h>
#include "ccprefforwardprovider.h"
#include "ccprefcall.h"
#include "ccprefconvergedcallprovider.h"

// -----------------------------------------------------------------------------
// CCPRefCall::Dial
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::Dial( const TDesC8& aCallParams )
	{
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCSCall::Dial"),value ) )
    	{
    	return value;
    	}
	
	TInt retval = IsActionOK(ECCPRefCallActionDial);

    if( retval!=KErrNone )
        {
        return retval;
        }
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::Dial, aCallParams = %S"), &aCallParams );
    
    SetNewState(MCCPCallObserver::ECCPStateDialling);
            
    SetNewState(MCCPCallObserver::ECCPStateConnecting);
    
    TInt action = CheckForwardRequest(iRecipient);
    
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
			
		default:
			// Normal behaviour
			SetNewState(MCCPCallObserver::ECCPStateConnected);
			break;
		}
		
    return KErrNone;
	}
    
// -----------------------------------------------------------------------------
// CCPRefCall::GetMobileCallInfo
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::GetMobileCallInfo( TDes8& /*aCallInfo*/ ) const
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::GetMobileCallInfo") );

    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCSCall::GetMobileCallInfo"),value ) )
    	{
    	return value;
    	}
	
	
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CCPRefCall::SwitchAlternatingCall
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::SwitchAlternatingCall()
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::SwitchAlternatingCall") );

    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCSCall::SwitchAlternatingCall"),value ) )
    	{
    	return value;
    	}
	
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CCPRefCall::GetMobileDataCallCaps
// -----------------------------------------------------------------------------
//
TInt CCPRefCall::GetMobileDataCallCaps( TDes8& /*aCaps*/ ) const
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::GetMobileDataCallCaps") );

    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured( _L8("MCCPCSCall::GetMobileDataCallCaps"),value ) )
    	{
    	return value;
    	}
	
	return KErrNone;
	}
	
void CCPRefCall::NoFDNCheck()
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::NoFDNCheck") );
	}
	
TBool CCPRefCall::LogDialedNumber() const
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefCall::LogDialedNumber") );
	// adjust value if needed
	return ETrue;
	}

	
