/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Types used in PhoneServer Messenger related functionality.
*
*/


#ifndef PHSRVMESSENGERTYPES_H
#define PHSRVMESSENGERTYPES_H


// INCLUDES
#include <cphcltmessenger.h> 

// DATA TYPES


/**
* The Messenger parameters defined as in one class.
* The handler of received messenger request must first check the
* performed request, and then read the appropriate parameters.
* 
* @since 2.6
*/
class TPhSrvMessengerParameters
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        * Set the default parameter values. 
        */
        inline TPhSrvMessengerParameters() :
            iMessengerCommand( EPhCltMesCommandUnknown ), 
            iDefaultMessageSize( 0 ),
            iSendMessagePtr( NULL ),
            iSendPayloadLength( 0 ),
            iSendDataValid( EFalse ),
            iSentDataId( 0 ),
            iReceiveBufferMaxSize( 0 ),
            iReceiveDataValid( EFalse ),
            iSkipNextMessage( EFalse ),
            iSkippedSentDataId( 0 )
            {
            iCategoryUid = TUid::Uid( 0 );
            }


    public:    // Data  

        // The Messenger Negotiator command.
        TPhCltMessengerCommand  iMessengerCommand;

        // The message types that are listened.
        TUid                    iCategoryUid;

        // The default message size.
        TInt                    iDefaultMessageSize;


        // The Send message payload.
        const TDesC8*           iSendMessagePtr;

        // The length of the sent message.
        TInt                    iSendPayloadLength;

        // Tells whether send data is valid or not.
        TBool                   iSendDataValid;

        // The Send message status to be completed when ready.
        RMessage2               iSendStatus;

        // The ID of the sent message.
        TUint                   iSentDataId;

        // The maximum size of the receive buffer.
        TInt                    iReceiveBufferMaxSize;
        // The Receive message parameters.
        //TPhCltMessengerParameters iReceiveParameters;

        // Tells whether receive data is valid or not.
        TBool                   iReceiveDataValid;

        // The Receive message status, size and payload 
        // to be completed when ready.
        RMessage2               iReceiveMessage;


        // Is the next Sent message Skipped?
        TBool                   iSkipNextMessage;

        // The ID of the Skipped message.
        TUint                   iSkippedSentDataId;

    };

#endif // PHSRVMESSENGERTYPES_H

// End of File
