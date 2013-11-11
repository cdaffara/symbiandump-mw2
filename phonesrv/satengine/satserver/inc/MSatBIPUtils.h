/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  BIP Utils interface for BIP proactive commands.
*
*/



#ifndef MSATBIPUTILS_H
#define MSATBIPUTILS_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>

// CLASS DECLARATION
class MSatBIPDataChannel;
class MSatApnHandler;

// CONSTANTS
// These constants are used by BIP related objects
// Used in terminal responses and DataAvailable envelope
const TInt KSatBIPMoreThanMaxSize = 0xFF;

// The minimum size of receive buffer is 1500 bytes.
const TInt KSatBIPMinReceiveBufferSize = 0x5DC;

// Maximum buffer size from SIM in OpenChannel command is 1500.
const TInt KSatBIPMaxBufferSize = 0x5DC;

// Maximum size of one ReceiveCommand TerminalResponse
const TInt KSatBIPMaxResponseSize = 0xED;

/**
*  BIP Utils interface. Provides functionality to
*  BIP proactive command handlers.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatBIPUtils
    {
    public: // Enums

        // These values can be used straigth in TerminalResponses
        // additional info with KBearerIndepProtocolError
        enum TSatBIPErrors
            {
            ESatBIPSuccess                     = 0x00,
            ESatBIPNoChannelAvail              = 0x01,
            ESatBIPChannelClosed               = 0x02,
            ESatBIPChannelIdNotValid           = 0x03,
            ESatBIPReqBufSizeNotAvail          = 0x04,
            ESatBIPSecurityError               = 0x05,
            ESatBIPTransportLevelNotAvailable  = 0x06,
            ESatBIPRemoteDeviceNotReachable    = 0x07,
            ESatBIPServiceError                = 0x08,
            ESatBIPServiceIdUnknown            = 0x09
            };

        // This comes with OpenChannel command to identify the type
        // of bearer in the data channel.
        enum TSatBearerType
            {
            ESatDefault,    /* Default bearer type */
            ESatGPRS,       /* GPRS */
            ESatCSD         /* CSD, Not supported */
            };

        // Defines the possible channel IDs. The values should be the same as are in
        // etelsat.h and these values can be used in functions.
        enum TSatChannelId
            {
            ESatNoChannel               = 0x00,
            ESatChannel1                = 0x01,
            ESatChannel2                = 0x02,
            ESatChannel3                = 0x03,
            ESatChannel4                = 0x04,
            ESatChannel5                = 0x05,
            ESatChannel6                = 0x06,
            ESatChannel7                = 0x07
            };

        // Supported status informations
        enum TSatChannelStatus
            {
            ESatNoFurtherInformation = 0x00,
            ESatLinkDropped          = 0x05
            };

    protected:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatBIPUtils() {};

        /**
        * Destructor.
        */
        virtual ~MSatBIPUtils() {};

    public: // New functions

        /**
        * Creates new data channel and returns reference to it.
        * @param aBearerType identifies the type of data channel to create.
        * @param TSatBIPError code indicating the execution status.
        * @return Pointer to created data channel
        */
        virtual MSatBIPDataChannel* CreateChannelL(
            const TSatBearerType aBearerType,
            TInt& aErrCode ) = 0;

        /**
        * Request to close specified data channel.
        * @param aChannelId Identifies the channel to be closed.
        * @return TSatBIPError code indicating the execution status.
        */
        virtual TInt CloseChannel( const TInt aChannelId ) = 0;

        /**
        * Request to open new data channel
        * @param aChannelId Identifies the channel.
        * @param aErrCode TSatBIPError code indicating the execution status.
        * @return Pointer to requested data channel
        */
        virtual MSatBIPDataChannel* DataChannel( const TInt aChannelId,
            TInt& aErrCode ) = 0;

        /**
        * Request to close specified data channel.
        * @return Array containing all available data channels.
        */
        virtual CArrayPtrFlat<MSatBIPDataChannel>* DataChannels() = 0;

        /**
        * Generates channel status object from given parameters to
        * used in terminal responses.
        * @param aChannelId Data channel identifier.
        * @param aStatus Data channel's status.
        * @return channel status object that can be put into terminal responses.
        */
        virtual RSat::TChannelStatus GenerateChannelStatusL(
            const TInt aChannelId, TInt aStatus ) = 0;

        /**
        * Reference to APN handler
        * @return Interface reference to CSatApnHandler
        */
        virtual MSatApnHandler& ApnHandler() = 0;
        
        /**
        * Status of the PDP Context
        * @return ETrue, if there is active PDP Context
        */
        virtual TBool IsContextActive() = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatBIPUtils( const MSatBIPUtils& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatBIPUtils& operator=( const MSatBIPUtils& );
    };

#endif      // MSATBIPUTILS_H

// End of File
