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
* Description:  Subsession for Messenger.
*
*/

#ifndef CPHSRVSUBSESSIONMESSENGER_H
#define CPHSRVSUBSESSIONMESSENGER_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 
#include <cphcltmessenger.h> 

// FORWARD DECLARATIONS
class MPhSrvMessengerNegotiator;



// CLASS DECLARATION

/**
*  Messenger Subsession.
*
*  @since 2.6
*/
class CPhSrvSubSessionMessenger
:   public CPhSrvSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @param aSession The pre-opened Phone Server session.
        * @return CPhSrvSubSessionMessenger subsession.
        */
        static CPhSrvSubSessionBase* NewL(
            CPhSrvSession& aSession );

        /**
        * Destructor.
        *
        */
        ~CPhSrvSubSessionMessenger();


    private: // Functions from base classes   

        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );

        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );


    private: // SubSession function handlers

        /**
        * Handle the close command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionClose( const RMessage2& aMessage );

        /**
        * Handle the construct command.

        */
        void CmdSubSessionMessengerConstructL( const RMessage2& aMessage );

        /**
        * Handle the receive command.
        */
        void CmdSubSessionMessengerReceive( const RMessage2& aMessage );

        /**
        * Handle the send command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionMessengerSend( const RMessage2& aMessage );

        /**
        * Handle the skip command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionMessengerSkip( const RMessage2& aMessage );

        /**
        * Handle the cancel command.
        * 
        * @param aMessage The received message.
        */
        void CmdSubSessionMessengerCancel( const RMessage2& aMessage );


    private: // Helper functions.

        /**
        * Returns the Messenger negotiator.
        * 
        * @return Messenger negotiator.
        */
        MPhSrvMessengerNegotiator& GetMessengerNegotiator();

        /**
        * Forwards the request to messenger negotiator.
        * 
        * @param aSubSession Subsession for messenger.
        * @param aParameters The request parameters.
        * @param aMessage The received message.
        */
        void ForwardRequest(
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage );

        /**
        * Cleans the negotiator side.
        */
        void Clean();


    private: 

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionMessenger( CPhSrvSession& aSession );

    };

#endif // CPHSRVSUBSESSIONMESSENGER_H


// End of File
