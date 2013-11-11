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

#ifndef CCPREFRANSFERPROVIDER_H
#define CCPREFRANSFERPROVIDER_H

// Includes
#include <e32base.h>
#include <e32cmn.h> // TDesC, HBufC
#include <mccptransferprovider.h>

// Forward declarations

class CCPRefCall;
class MCCPTransferObserver;

// Constants

const TInt KAddressMaxLength = 100;

/**
* Call transfer methods.
*/
class CCPRefTransferProvider : public CBase,
                               public MCCPTransferProvider
	{
	
private:

	/**
    * constructor.
    */
    CCPRefTransferProvider( const CCPRefCall& aMyCall );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

public:
 
    /*
	* @param aMyCall call that owns this object.
	* @return created CCPRefTransferProvider object
    */
    static CCPRefTransferProvider* NewL( const CCPRefCall& aMyCall );

    /*
    * dtor
    */
    ~CCPRefTransferProvider();

	/**
	* Attended transfer to given call recipient.
	* There exist a call between A-B and A-C. A wants to transfer the call to B-C. 
	* A itself will not be inline in case the transfer is made succesfully. 
	* In above case C will be the aTransferTargetCall. B is the recipient of the 
	* current call between A-B - called on A's call object transfer provider. 
	* The A-B call is on hold.
	* @param aTransferTargetCall - target receiver of the transferred call. In this case there exist a call 
    * @return In case of an immediate error returns system error code, if request has been started returns KErrNone.
    * Actual request result indication comes via observer class
	* @since S60 3.2
	*/
	TInt AttendedTransfer( MCCPCall& aTransferTargetCall );

	/**
	* Attended transfer to given address. There does not need to be 
	* a current call between the given transfer target.
	* @param aTransferTarget - transfer target address
    * @return In case of an immediate error returns system error code, if request has been started returns KErrNone.
    * Actual request result indication comes via observer class
	* @since S60 3.2
	*/
	TInt AttendedTransfer( const TDesC& aTransferTarget );

	/**
	* Unattended transfer. Call is requested to be transferred to given address.
	* After ECCPRemoteTransferring event current call will be disconnected and no 
	* transfer status is checked from the operation. 
	* @since S60 3.2
	* @param aTransferTarget Address of the target
    * @return In case of an immediate error returns system error code, if request has been started returns KErrNone.
    * Actual request result indication comes via observer class
	*/
	TInt UnattendedTransfer( const TDesC& aTransferTarget );

	/**
	* Accept incoming call transfer request from the call remote party.
	* @since Series 60 3.2
	* @param aAccept Boolean value is transfer accepted or not.
    * @return In case of an immediate error returns system error code, if request has been started returns KErrNone.
    * Actual request result indication comes via observer class
	*/
	TInt AcceptTransfer( const TBool aAccept );
	  
	/**
    * When the other end of the call has requested call transfer to third party it is notified to 
    * CCPCallCall::EventOccurred(ECCPRemoteTransferring).
    * The new recipient of the call can be fetched via this method.
	* @since Series 60 3.2
	* @param none
	* @return New recipient for the call after transfer
	*/
	const TDesC& TransferTarget() const;

    /**
    * Add an observer for transfer related events.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPTransferObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPTransferObserver& aObserver );
    
    void SetTransferTarget( const TDesC& aTransferTarget );
    
private:	// functions
       
    void Delay( TUint aDelayInMilliseconds );
    
private:	// data
    
    RTimer iTimer;   
    
    CCPRefCall& iMyCall;
      
    MCCPTransferObserver* iObserver;
        
public:		// data

	MCCPCall* iActiveTransferCall;
	
	HBufC* iTransferTarget;

	};

#endif // CCPREFRANSFERPROVIDER_H
