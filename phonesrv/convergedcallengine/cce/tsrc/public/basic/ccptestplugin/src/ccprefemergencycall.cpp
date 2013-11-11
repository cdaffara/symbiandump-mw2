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
* Description:  CCP reference plug-in implementation.
*
*/


//  INCLUDES
#include <e32base.h>
#include <mccpcallobserver.h>
#include "ccprefemergencycall.h"
#include "ccprefconvergedcallprovider.h"
#include "ccptestplugindefs.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefEmergencyCall::CCPRefEmergencyCall(const CCPRefConvergedCallProvider* aProvider) : 
    iProvider(const_cast<CCPRefConvergedCallProvider*>(aProvider)),
    iRecipient(NULL),
    iObserver(NULL)
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefEmergencyCall::~CCPRefEmergencyCall()
    {
    delete iRecipient;
    }



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefEmergencyCall* 
CCPRefEmergencyCall::NewL( const CCPRefConvergedCallProvider* aProvider,
                           const TDesC& aRecipient,
                           MCCPCallObserver* aObserver )
    {
    CCPRefEmergencyCall* self = new( ELeave ) CCPRefEmergencyCall(aProvider);

    CleanupStack::PushL(self);
    self->ConstructL(aRecipient,aObserver);
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefEmergencyCall::ConstructL( const TDesC& aRecipient,
						              MCCPCallObserver* aObserver )
    {
    iRecipient = aRecipient.AllocL();
    iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefEmergencyCall::Dial( const TDesC& /* aRecipient */ )
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured( _L8("MCCPEmergencyCall::Dial"),value ) )
    	{
    	return value;
    	}
    
    if ( iProvider->Uid().iUid == KPluginCsImplementationUid ||
         iProvider->Uid().iUid == KPluginVccImplementationUid )
        {
        iProvider->TerminateAllCalls();
        }
    
    /* Preferred address usage order: 
    *  1) Plug-in own emergency address 
    *  2) Address given in Dial
    *  3) Address given in CConvergedCallProvider::NewEmergencyCallL
    */
    
    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateDialling, NULL );

    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateConnecting, NULL );
    
    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateConnected, NULL );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefEmergencyCall::Cancel()
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured( _L8("MCCPEmergencyCall::Cancel"),value ) )
    	{
    	return value;
    	}

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefEmergencyCall::HangUp()
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured( _L8("MCCPEmergencyCall::HangUp"),value ) )
    	{
    	return value;
    	}

	iObserver->CallStateChanged( MCCPCallObserver::ECCPStateDisconnecting, NULL );
    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateIdle, NULL );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefEmergencyCall::Answer()
    {
    TInt value;
    if( iProvider->ConfigurationHandler().IsActionConfigured(_L8("MCCPEmergencyCall::Answer"),value ) )
    	{
    	return value;
    	}

    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateAnswering, NULL );
    iObserver->CallStateChanged( MCCPCallObserver::ECCPStateConnected, NULL );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefEmergencyCall::SetObserver( MCCPCallObserver* aObserver )
    {
    iObserver = aObserver;
    }
