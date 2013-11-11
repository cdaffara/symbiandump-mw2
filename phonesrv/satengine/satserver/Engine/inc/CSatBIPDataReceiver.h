/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSatBIPDataReceiver is responsible of Receive operations
*
*/



#ifndef CSATBIPDATARECEIVER_H
#define CSATBIPDATARECEIVER_H

//  INCLUDES
#include    <e32base.h>
#include    <in_sock.h>
#include    "MSatBIPUtils.h"

// FORWARD DECLARATIONS
class CSatBIPGPRSDataChannel;

// CLASS DECLARATION

/**
*  CSatBIPDataReceiver is responsible for receiving data using deifned protocol
*  and to notify CSatBIPGPRSDataChannel when data has been sent.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatBIPDataReceiver : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Default constructor.
        * @param aObserver GPRS data channel to whom the received data is passed
        * @param aSocket Socket where the data is received
        * @param aIsTcpSocket Indicates whether to use TCP or UDP
        */
        CSatBIPDataReceiver( CSatBIPGPRSDataChannel& aObserver,
                             RSocket& aSocket,
                             TBool aIsTcpSocket );

        /**
        * Destructor.
        */
        virtual ~CSatBIPDataReceiver();

    public: // Functions from base classes

        /**
        * From CActive. Informs observer that data has been received.
        */
        void RunL();

        /**
        * From CActive. Cancels any outgoing requests
        */
        void DoCancel();

    public: // New functions

        /**
        * This notification is called when SIM has received all the data
        * from ReceiveStore in case of UDP. If there are any pending datagrams,
        * it is stored and receive is available again.
        */
        void ReceiveStoreEmptyNotificationL();

        /**
        * Starts receiving data using predefined protocol.
        * @param aAddr Source address where the data is received.
        */
        void StartReceive( TInetAddr& aAddr );

    private:    // Data

        // Observer the data sent event.
        CSatBIPGPRSDataChannel& iObserver;

        // Socket
        RSocket& iSocket;

        // Defines wether to use UDP or TCP sockets
        TBool iIsTcpSocket;

        // Receive buffer where the data is received
        TBuf8<KSatBIPMinReceiveBufferSize> iRecvBuffer;

        // Contains the number of bytes received
        TInt iReceiveDataLen;

        // Wrapper for bytes received indication
        TSockXfrLength iRecvPckg;
    };

#endif      // CSATBIPDATARECEIVER_H

// End of File
