/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer of Subconnection parameters granted event
*
*/


#ifndef CSATBIPSUBCONNEVENTOBSERVER_H
#define CSATBIPSUBCONNEVENTOBSERVER_H

//  INCLUDES
#include    <e32base.h>
#include    <es_sock.h>

class   CSatBIPGPRSDataChannel;

// CLASS DECLARATION

/**
 *  CSatBIPSubConnEventObserver
 *
 *  Observe the SubConnection parameters granted event
 *
 *  @lib SatEngine.lib
 *  @since Series 60 3.2
 */
NONSHARABLE_CLASS( CSatBIPSubConnEventObserver ): public CActive
    {
public:
// Constructor and destructor

    /**
     * Default constructor.
     *
     * @since S60 3.2
     * @param aObserver GPRS data channel, which responsible for fetch negQoS 
     *                  from SubConnection parameters granted event
     */
    CSatBIPSubConnEventObserver( CSatBIPGPRSDataChannel& aObserver );
    
    /**
     * Destructor
     *
     * @since S60 3.2
     */
    virtual ~CSatBIPSubConnEventObserver();
        
// New functions

    /**
     * Starts observing granted event
     */
   void StartObservSubConnEvent();

// From base class CActive

    /**
     * From CActive. Informs observer that granted event has been received.
     *
     * @since S60 3.2
     */
    void RunL();

    /**
     * From CActive. Cancels any outgoing requests
     *
     * @since S60 3.2
     */
    void DoCancel();

private: // Data
         
    /**
     * EventFilter for subconnection
     */
    RSubConnection::TEventFilter iEventFilter;
        
    /**
     * Buffer for Sub-connection event notiifcation
     */
    TNotificationEventBuf iEventBuffer;
        
    /**
     * Reference to class CSatBIPGPRSDataChannel.
     * Not own.
     */
    CSatBIPGPRSDataChannel& iDataChannel;
    };

#endif      // CSATBIPSUBCONNEVENTOBSERVER_H

