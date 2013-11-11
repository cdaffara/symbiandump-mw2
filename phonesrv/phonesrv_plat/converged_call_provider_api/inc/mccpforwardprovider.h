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
* Description:  CCP call forward provider
*
*/


#ifndef MCCPFORWARDPROVIDER_H
#define MCCPFORWARDPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h> // CDesC8Array

class MCCPForwardObserver;

/**
* Call forward methods. In case plug-in support call forwarding these methods can be fetched from 
* MCCPCall::ForwardProviderL.
* MCCPForwardObserver is used for forwarding specific events. 
* MCCPCallObserver is used for general call related status, event, error etc notifications.
*
*  @since S60 3.2
*/
class MCCPForwardProvider
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPForwardProvider() {};
    
public:
    /**
    * Returns pointer to array containing addresses to forward the call to.
    * Index zero contains the first address. If there are no addresses available
    * returns an empty array.
    * @since Series 60 3.2
    * @param none
    * @return Reference to array containing addresses
    * @leave KErrNotReady call is not in forwarding state
    * @leave KErrNotSupported if call is not mobile originated
    * @leave system error code 
    * @pre Call state is MCCPCallObserver::ECCPStateForwarding and call type is MO
    * @pre Call MCCPForwardObserver::ECCPMultipleChoices event is received
    */
    virtual const CDesC8Array& GetForwardAddressChoicesL() = 0;

    /**
    * Forward call to address at given index.
    * @since Series 60 3.2
    * @param aIndex Index of address where the call is to be forwarded. Address is 
    * found in the array received from GetForwardAddressChoicesL. Index starts from zero.
    * @return none
    * @leave KErrArgument Index is not in array
    * @leave KErrNotReady Call is not in forwarding state
    * @leave KErrNotSupported If call is not mobile originated
    * @pre Call state is MCCPCallObserver::ECCPStateForwarding and call type is MO
    * @pre Call MCCPForwardObserver::ECCPMultipleChoices event is received
    */
    virtual void ForwardToAddressL( const TInt aIndex ) = 0;
    
    /**
    * Add an observer for forward related events.
    * Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer to add.
    * @return none
    * @leave system error if observer adding fails
    */
    virtual void AddObserverL( const MCCPForwardObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param aObserver Observer to remove.
    * @return KErrNone if removed succesfully. 
    * @return KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCPForwardObserver& aObserver ) = 0;
    };


#endif // MCCPFORWARDPROVIDER_H
