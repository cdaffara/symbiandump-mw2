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
* Description:  CCP Call transfer observer
*
*/


#ifndef MCCPTRANSFEROBSERVER_H
#define MCCPTRANSFEROBSERVER_H

// INCLUDES
#include <e32base.h>


/**
* CCP API call transfer observer class
*
*  @since S60 3.2
*/
class MCCPTransferObserver
    {
public: 
    enum TCCPTransferEvent
       {
       /** Call has been transferred to another remote party as a result of a local action. 
       This is the result of transfer being succesfully completed */
       ECCPLocalTransfer,
       /** Outgoing call is transfering. The transfer request has now been accepted by the current call recipient. */
       ECCPRemoteTransferring,
       /** Outgoing transfer call is alerting. Current call recipinet has sent connection request to transfer target*/
       ECCPRemoteTransferAlerting
       };

protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPTransferObserver() {};
    
public:
    /**
    * A transfer event has occurred concerning a specific call.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @return none
    */
    virtual void TransferEventOccurred( 
                 const MCCPTransferObserver::TCCPTransferEvent aEvent ) = 0;
    };

#endif // MCCPTRANSFEROBSERVER_H

// End of File
