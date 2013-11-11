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
* Description:  CSatBIPDataSender is responsible of Send operations
*
*/



#ifndef CSATBIPDATASENDER_H
#define CSATBIPDATASENDER_H

//  INCLUDES
#include    <e32base.h>
#include    <in_sock.h>
#include    "MSatBIPUtils.h"

// FORWARD DECLARATIONS
class MSatSendDataObserver;

// CLASS DECLARATION

/**
*  CSatBIPDataSender is responsible for sending data using deifned protocol and
*  to notify MSatSendDataObserver when data has been sent.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatBIPDataSender : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Default constructor.
        * @param aObserver Observer of successfull send data
        * @param aSocket Socket where the data is sent
        * @param aIsTcpSocket Indicates wheter to use TCP or UDP
        * @param aBufferSize Size of Tx buffer. Sent back to aObserver
        */
        CSatBIPDataSender( MSatSendDataObserver& aObserver,
                           RSocket& aSocket,
                           TBool aIsTcpSocket,
                           TInt aBufferSize );

        /**
        * Destructor.
        */
        virtual ~CSatBIPDataSender();

    public: // Functions from base classes

        /**
        * From CActive. Informs observer that data has been sent.
        */
        void RunL();

        /**
        * From CActive. Cancels any outgoing requests
        */
        void DoCancel();

    public: // New functions

        /**
        * Sends data using predefined protocol.
        * @param aData Data to send.
        * @param aDest Destination address where the data is sent. Used only
        *        with UDP sockets.
        */
        void SendData( const TDesC8& aData, TInetAddr& aDest );

    private:    // Data

        // Observer the data sent event.
        MSatSendDataObserver& iObserver;

        // Socket
        RSocket& iSocket;

        // Defines wether to use UDP or TCP sockets
        TBool iIsTcpSocket;

        // Holds the amount of data sent with same connection
        TUint iUplinkData;

        // Sender buffer
        TBuf8<KSatBIPMaxBufferSize> iSendPtr;

        // Buffer size. Returned to observer when data is sent
        TInt iBufferSize;

    };

#endif      // CSATBIPDATASENDER_H

// End of File
