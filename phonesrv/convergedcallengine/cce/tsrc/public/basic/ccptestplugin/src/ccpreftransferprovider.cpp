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

#include <e32base.h>
#include <e32debug.h>
#include <mccptransferobserver.h>
#include "ccpreftransferprovider.h"
#include "ccprefcall.h"
#include "ccprefconvergedcallprovider.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefTransferProvider::CCPRefTransferProvider( const CCPRefCall& aMyCall ) : 
    iMyCall(const_cast<CCPRefCall&>(aMyCall)),
    iObserver(NULL)
    {
   
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefTransferProvider::~CCPRefTransferProvider()
    {
    iTimer.Cancel();
    iTimer.Close();
    delete iTransferTarget;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefTransferProvider* CCPRefTransferProvider::NewL( const CCPRefCall& aMyCall )
    {
    CCPRefTransferProvider* self = new( ELeave ) CCPRefTransferProvider(aMyCall);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefTransferProvider::ConstructL()
    {
    iTimer.CreateLocal();
    iTransferTarget = HBufC::NewL(KAddressMaxLength);
    }

    
void CCPRefTransferProvider::Delay( TUint aDelayInMilliseconds )
	{
	RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::Delay(), %d millisec"), aDelayInMilliseconds );
	
	TRequestStatus waitStatus;
    iTimer.After( waitStatus, aDelayInMilliseconds*1000 );
    User::WaitForRequest(waitStatus);
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::Delay() stop") );
	}
    
    
    
// -----------------------------------------------------------------------------
// MO transfer initiation, attended - wait for other end connection establishment 
// status
// -----------------------------------------------------------------------------
//
TInt CCPRefTransferProvider::AttendedTransfer( MCCPCall& aTransferTargetCall )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::AttendedTransfer_MCCPCall"),value ) )
    	{
    	return value;
    	}
    
    iActiveTransferCall = &aTransferTargetCall;
    
    return AttendedTransfer( aTransferTargetCall.RemoteParty() );
    }
    
// -----------------------------------------------------------------------------
// MO transfer initiation, attended - wait for other end connection establishment 
// status
// -----------------------------------------------------------------------------
//
TInt CCPRefTransferProvider::AttendedTransfer( const TDesC& aTransferTarget )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::AttendedTransfer_TDesC"),value ) )
    	{
    	return value;
    	}

    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer") );
    
    if( aTransferTarget==KNullDesC )
        {
        iMyCall.Observer()->ErrorOccurred(
            ECCPRequestFailure, &iMyCall );
        
        return KErrArgument; 
        }
        
    TInt err = iMyCall.IsActionOK(CCPRefCall::ECCPRefCallActionTransfer);
        
    if( err != KErrNone )
        {
        return err;
        }
        
	RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - set ECCPStateTransferring state") );
    iMyCall.Observer()->CallStateChanged(
        MCCPCallObserver::ECCPStateTransferring, &iMyCall );
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - send event ECCPRemoteTransferring") );
    iObserver->TransferEventOccurred( MCCPTransferObserver::ECCPRemoteTransferring );
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - set ECCPStateDisconnecting state") );
    iMyCall.Observer()->CallStateChanged(
        MCCPCallObserver::ECCPStateDisconnecting, &iMyCall );
      
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - send event ECCPRemoteTransferAlerting") );
    iObserver->TransferEventOccurred( MCCPTransferObserver::ECCPRemoteTransferAlerting );
    
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - send event ECCPLocalTransfer") );
    iObserver->TransferEventOccurred( MCCPTransferObserver::ECCPLocalTransfer );
        
    RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AttendedTransfer - set ECCPStateIdle") );
	iMyCall.Observer()->CallStateChanged(
	    MCCPCallObserver::ECCPStateIdle, &iMyCall );
	
	// Hangup the other call
	iActiveTransferCall->HangUp();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// MO transfer initiation, attended - don't wait for other end connection 
// establishment status after call is accepted.
// -----------------------------------------------------------------------------
//
TInt CCPRefTransferProvider::UnattendedTransfer( const TDesC& aTransferTarget )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::UnattendedTransfer"),value ) )
    	{
    	return value;
    	}

    if( aTransferTarget==KNullDesC )
        {
        iMyCall.Observer()->ErrorOccurred(
            ECCPRequestFailure, &iMyCall );
        
        return KErrArgument;  
        }
        
    TInt err = iMyCall.IsActionOK(CCPRefCall::ECCPRefCallActionTransfer);
    
    if( err != KErrNone )
        {
        return err;
        }

    iMyCall.Observer()->CallStateChanged(
        MCCPCallObserver::ECCPStateTransferring, &iMyCall );
    
    iObserver->TransferEventOccurred( MCCPTransferObserver::ECCPRemoteTransferring );
    
    iMyCall.Observer()->CallStateChanged(
        MCCPCallObserver::ECCPStateDisconnecting, &iMyCall );

	iMyCall.Observer()->CallStateChanged(
	    MCCPCallObserver::ECCPStateIdle, &iMyCall );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Accept or reject incoming transfer request from the other party of this call
// -----------------------------------------------------------------------------
//
TInt CCPRefTransferProvider::AcceptTransfer( const TBool aAccept )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::AcceptTransfer"),value ) )
    	{
    	return value;
    	}

    if( aAccept )
        {
        // Transfer originator is disconnected through Transferring state
        iMyCall.Observer()->CallStateChanged(
            MCCPCallObserver::ECCPStateTransferring, &iMyCall );

        }
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Get transfer target for the incoming transfer request
// -----------------------------------------------------------------------------
//
const TDesC& CCPRefTransferProvider::TransferTarget() const
    {
/*    
    TDesC value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::UnattendedTransfer"),value ) )
    	{
    	return value;
    	}
*/
    if( !iTransferTarget )
        {
        return KNullDesC;
        }

    return *iTransferTarget;
    }
    

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefTransferProvider::AddObserverL( const MCCPTransferObserver& aObserver )
    {
    TInt value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::AddObserverL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefTransferProvider::AddObserverL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}
    
    // Only one observer used at a time, replaces current one
    iObserver = const_cast<MCCPTransferObserver*>(&aObserver);
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefTransferProvider::RemoveObserver(const MCCPTransferObserver& aObserver )
    {
    TBool value;
    if( iMyCall.Provider().ConfigurationHandler().IsActionConfigured( _L8("MCCPTransferProvider::RemoveObserver"),value ) )
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


void CCPRefTransferProvider::SetTransferTarget( const TDesC& aTransferTarget )
	{
	iTransferTarget->Des();
	
	TPtr ptr( iTransferTarget->Des() );
	ptr = aTransferTarget;
	}

