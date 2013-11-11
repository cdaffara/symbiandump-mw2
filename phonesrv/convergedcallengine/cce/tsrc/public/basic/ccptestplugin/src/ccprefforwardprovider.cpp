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
#include "ccprefforwardprovider.h"
#include "ccprefcall.h"
#include "ccprefconvergedcallprovider.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefForwardProvider::CCPRefForwardProvider( const CCPRefCall& aMyCall ) :
    iMyCall(const_cast<CCPRefCall&>(aMyCall)),
    iMultipleChoicesArrayPtr(NULL),
    iObserver(NULL)
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefForwardProvider::~CCPRefForwardProvider()
    {
    delete iMultipleChoicesArrayPtr;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefForwardProvider* CCPRefForwardProvider::NewL( const CCPRefCall& aMyCall )
    {
    CCPRefForwardProvider* self = new( ELeave ) CCPRefForwardProvider(aMyCall);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefForwardProvider::ConstructL()
    {
	iMultipleChoicesArrayPtr = new ( ELeave ) CDesC8ArrayFlat(1);
    }



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const CDesC8Array& CCPRefForwardProvider::GetForwardAddressChoicesL()
    {
    TInt value;
    TBool leave = EFalse;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPForwardProvider::GetForwardAddressChoicesL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		RDebug::Print(_L("[TESTPLUGIN] CCPRefForwardProvider::GetForwardAddressChoicesL, was configured to LEAVE=%d"), value );
    		User::Leave(value);
    		}
//   		RDebug::Print(_L("[TESTPLUGIN] CCPRefForwardProvider::GetForwardAddressChoicesL, configured to return=%S"), &value );
/// TODO array retval 
    	}

    return *iMultipleChoicesArrayPtr;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefForwardProvider::ForwardToAddressL( const TInt aIndex )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPForwardProvider::ForwardToAddressL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefForwardProvider::ForwardToAddressL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}

    if( iMyCall.IsActionOK(CCPRefCall::ECCPRefCallActionForward) != KErrNone )
        {
        return;
        }
    
    // Check index is in array
    if( aIndex<0 || (aIndex+1)<iMultipleChoicesArrayPtr->MdcaCount() )
        {
        User::Leave(KErrArgument);
        }
    
    if( iMyCall.IsMobileOriginated() )
        {
        // Call was forwared ok in remote end
        iMyCall.Observer()->CallStateChanged( 
            MCCPCallObserver::ECCPStateConnected, &iMyCall );
        }
    }


// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefForwardProvider::AddObserverL( const MCCPForwardObserver& aObserver )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPForwardProvider::AddObserverL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefForwardProvider::AddObserverL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}
    
    // Only one observer used at a time, replaces current one
    iObserver = const_cast<MCCPForwardObserver*>(&aObserver);
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefForwardProvider::RemoveObserver(const MCCPForwardObserver& aObserver )
    {
    TBool value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPForwardProvider::RemoveObserver"),value ) )
    	{
    	return value;
    	}
    
    if( iObserver!=&aObserver )
        {
        return KErrNotFound;
        }

    iObserver = NULL;

    return KErrNone;
    }

