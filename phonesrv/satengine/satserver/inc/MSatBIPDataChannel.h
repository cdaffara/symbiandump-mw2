/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for BIP Data channel.
*
*/



#ifndef MSATBIPDATACHANNEL_H
#define MSATBIPDATACHANNEL_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include <extendedconnpref.h> // TExtendedConnPref

// CLASS DECLARATION
class MSatSendDataObserver;
class MSatConnectionObserver;
/**
*  Interface for BIP Data channel.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatBIPDataChannel
    {

    public: // Structs

        // Contains all the information needed to establish connection.
        // Parameters are common to all bearers
        struct TSatBipConnectionInfo
            {
            RSat::TSimMeInterface iProtocol;     // TCP or UDP, port number
            RSat::TOtherAddress   iDestination;  // Dest. address, IPv4 or IPv6
            TUint16               iBufferSize;   // Size of the receive buffer
            TExtendedConnPref*    iOverrideSet;  // extended connection
            RSat::TBearerParams   iBearerParams; // QoS parameters
            RSat::TOtherAddress   iSource;       // Local addr, IPv4 or IPv6
            TUint32               iCreatedApnId; // ID of the created APN
            };

    public: // New functions

        /**
        * Sets up connection. Does not activate it yet.
        * @param aConnParams includes all needed information to
        *        open the connection.
        */
        virtual void SetupConnectionL(
            const TSatBipConnectionInfo& aConnParams ) = 0;

        /**
        * Activates connection. Connection information must be set before
        * activating connection
        * @param aObserver Observer of the connection
        */
        virtual void ActivateConnectionL( 
            MSatConnectionObserver* aObserver ) = 0;
        
        /**
        * From MSatBIPDataChannel Returns Negotiated bearer parameters
        * @param aResult Output Negotiated bearer parameters
        */
        virtual void GetNegotiatedQoSParams( 
            RSat::TBearerParams& aResult ) const = 0;
        
        /**
        * Sends the data or stores it into a buffer for later send.
        * @param aData Data to be send.
        * @param aSendImmedeately Indicates whether to send immediately or
        *        store data to buffer for later send.
        * @param aFreeBufferSize The number of bytes of empty size in the buffer
        * @param aObserver Observer of the completion of data send.
        * @return Error code indicating the status of the command execution.
        */
        virtual TInt SendDataL( const TDes8& aData,
                                const TBool aSendImmediately,
                                TInt& aFreeBufferSize,
                                MSatSendDataObserver* aObserver ) = 0;

        /**
        * Receives the data from the buffer.
        * @param aData Data to receive.
        * @param aBytesToRead The number of requested bytes to return
        * @param aAvailableBytes Number of unread bytes left in the buffer.
        * @return Error code indicating the status of the command execution.
        */
        virtual TInt ReceiveDataL( TDes8& aData,
                                   const TInt aBytesToRead,
                                   TInt& aAvailableBytes ) = 0;

        /**
        * Returns the ID of this data channel.
        * @return ID of this channel.
        */
        virtual TInt ChannelId() const = 0;

        /**
        * Returns the ID of this data channel to ChannelStatus
        * @return ID that can be placed to ChannelStatus
        */
        virtual TUint8 ChannelStatusChannelId() const = 0;

        /**
        * Returns the status of this channel.
        * @return Status of this data channel
        */
        virtual TInt Status() const = 0;

        /**
        * Closes the connection and empties the buffers.
        */
        virtual void CloseChannel() = 0;

        /**
        * Cancels all actions.
        */
        virtual void CancelAll() = 0;

        /**
        * Indicates whether PDP contex is active or not.
        * @return TBool indicating context activation.
        */
        virtual TBool IsContextActive() const = 0;
        
        /**
        * Returns connection information
        * @return Connection information
        */
        virtual const TSatBipConnectionInfo& ConnInfo() const = 0;
       
        /**
        * Close udp socket.
        */
        virtual void StopUdpSocket() = 0;

        /**
        * Destructor.
        */
        virtual ~MSatBIPDataChannel() {};

    protected:  // Constructors and destructor

        /**
        * Constructor.
        */
        MSatBIPDataChannel() {};

    private:

        // Assign operator
        MSatBIPDataChannel& operator=( const MSatBIPDataChannel& );

        // Prohibit copy constructor if not deriving from CBase.
        MSatBIPDataChannel( const MSatBIPDataChannel& );
    };

#endif      // MSATBIPDATACHANNEL_H

// End of File
