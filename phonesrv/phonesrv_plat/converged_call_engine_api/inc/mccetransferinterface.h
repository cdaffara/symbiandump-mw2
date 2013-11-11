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
* Description:  CCE call transfer interface
*
*/


#ifndef MCCETRANSFERINTERFACE_H
#define MCCETRANSFERINTERFACE_H

//  INCLUDES
#include <e32cmn.h> // TDesC

/**
* Call transfer methods.
* MCCECallObserver is used for errors, statuses, notifications etc
*
*  @since S60 3.2
*/
class MCCETransferInterface
    {
protected:

    /** 
    * Protected destructor. Object cannot be deleted from client.
    * @since S60 3.2
    */
    virtual ~MCCETransferInterface() {};
    
public:

    /**
    * Attended transfer to given address. 
    * Actual result like status, events and errors come via MCCECallObserver class.
    * Afterwards both calls will be disconnected from CCE side.
    * @param aTransferTarget Transfer target address. Always takes active call, address is not used
    * at the moment!
    * @return KErrArgument transfer address was illegal.
    * @return KErrNotFound existing active call was not found with given address. 
    * @return KErrNotReady if this call is not on-hold state and the other in connected state
    * @return KErrNotSupported Current plug-in does not support transfer functionality.
    * @pre Two calls exist. This call is in state MCCECallObserver::ECCEStateHold and the other in 
    * MCCECallObserver::ECCEStateConnected
    * @since S60 3.2
    */
    virtual TInt AttendedTransfer( const TDesC& aTransferTarget ) = 0;

    /**
    * Unattended transfer to given address. Call is requested to be transferred to given address.
    * Afterwards call will be disconnected from CCE side. If transfer fails a new incoming call 
    * will be received to reconnect to recipient. Reconnected call is initiated from recipient end.
    * Actual result like status, events and errors come via MCCECallObserver class.
    * This is not used for CS.
    * @since S60 3.2
    * @param aTransferTarget Address of the target
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotSupported Current plug-in does not support transfer functionality.
    * @return KErrNotReady if call is not in connected or on-hold state.
    * @return KErrArgument transfer address was illegal.
    * @pre Call state is MCCECallObserver::ECCPStateConnected or MCCECallObserver::ECCEStateHold
    */
    virtual TInt UnattendedTransfer( const TDesC& aTransferTarget ) = 0;

    /**
    * Accept incoming unattended transfer request from the call remote party.
    * @since Series 60 3.2
    * @param aAccept Boolean value is transfer accepted or not.
    * @return KErrNotSupported Current plug-in does not support transfer functionality.
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in transferring state.
    * @pre Call state is MCCECallObserver::ECCEStateTransferring
    */
    virtual TInt AcceptTransfer( const TBool aAccept ) = 0;
      
    /**
    * NOTE: Will be removed! MCCECallObserver::HandleTransfer contains remote party already.
    * CCE will map the recipient in openNewCallL to pre-existing one
    * CCECallCall::EventOccurred(ECCERemoteTransferring).
    * The new recipient of the call can be fetched via this method.
    * @since Series 60 3.2
    * @param none
    * @return New recipient for the call after transfer
    */
    virtual const TDesC& TransferTarget() const = 0;
      
    };

#endif // MCCETRANSFERINTERFACE_H

