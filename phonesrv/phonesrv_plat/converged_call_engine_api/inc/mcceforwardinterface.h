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
* Description:  CCE call forward interface
*
*/


#ifndef MCCEFORWARDINTERFACE_H
#define MCCEFORWARDINTERFACE_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h> // CDesC8Array

/**
* Call forward methods.
* MCCECallObserver is used for errors, statuses, notifications etc
*
*  @since S60 3.2
*/
class MCCEForwardInterface
    {
 protected: // Constructors and destructor

    /** 
    * Protected destructor. Object cannot be deleted from client.
    * @since S60 3.2
    */
    virtual ~MCCEForwardInterface() {};
    
 public:
    /**
    * Returns reference to array containing addresses of forward address choices.
    * Index starts from zero.
    * @since Series 60 3.2
    * @param none
    * @return array containing forward address choices
    * @leave KErrNotReady Call is not in forwarding state
    * @leave KErrNotSupported If call is not mobile originated
    * @leave system error code 
    * @pre Call state is MCCECallObserver::ECCEStateForwarding and call type is MO
    * @pre Call MCCECallObserver::EVoIPEventMultipleChoices event is received
    */
    virtual const CDesC8Array& GetForwardAddressChoicesL() = 0;

    /**
    * Forward call to selected address at given index in array received from GetForwardAddressChoicesL
    * @since Series 60 3.2
    * @param aIndex index in the address where the call is to be forwarded. 
    * Index starts from zero.
    * @return none 
    * @leave KErrArgument Index is not in array
    * @leave KErrNotReady Call is not in forwarding state
    * @leave KErrNotSupported If call is not mobile originated
    * @pre Call state is MCCECallObserver::ECCEStateForwarding and call type is MO
    */
    virtual void ForwardToAddressL( const TInt aIndex ) = 0;
    };


#endif // MCCEFORWARDINTERFACE_H
