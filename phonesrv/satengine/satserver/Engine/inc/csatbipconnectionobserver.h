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
* Description:  Observes the status of the GPRS connection
*
*/



#ifndef CSATBIPCONNECTIONOBSERVER_H
#define CSATBIPCONNECTIONOBSERVER_H


#include    "CSatBIPUtils.h"


/**
 *  Observes the GPRS network status and stores current value.
 *
 *  @lib SatEngine.lib
 *  @since Series 60 3.0
 */
class CSatBIPConnectionObserver : public CActive
    {
public:

    /**
     * Default constructor.
     * @param aBipUtils Reference to BIP utils class.
     */
    CSatBIPConnectionObserver( CSatBIPUtils& aBipUtils );

    /**
     * Destructor.
     */
    virtual ~CSatBIPConnectionObserver();

    /**
     * From CActive. Connection status changed
     */
    void RunL();

    /**
     * From CActive. Cancels any outgoing requests
     */
    void DoCancel();

    /**
     * Starts observing the connection status
     */
    void StartObserver();

    /**
     * Current status of the GPRS sefvice
     * @return Current status of the GPRS service
     */
    RPacketService::TStatus Status() const;

private:

    /**
     * Reference to CSatBIPUtils class.
     */
    CSatBIPUtils& iBipUtils;

    /**
     * Current status of the GPRS network
     */
    RPacketService::TStatus iConnectionStatus;

    /**
     * Last status of the GPRS network
     */
    RPacketService::TStatus iPrevConnectionStatus;
    };

#endif      // CSATBIPCONNECTIONOBSERVER_H
