/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Subsession for Ussd.
*
*/

#ifndef CPHSRVSUBSESSIONUSSD_H
#define CPHSRVSUBSESSIONUSSD_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 
#include "mphsrvussdmessagesentobserver.h" 


// CLASS DECLARATION

/**
*  USSD Subsession.
*
*  @since 1.0
*/
class CPhSrvSubSessionUssd : 
    public CPhSrvSubSessionBase, 
    public MPhSrvUssdMessageSentObserver
    {
    public:

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionUssd( CPhSrvSession& aSession );

        
    private: // Functions from base classes

        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );
        
        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );
        
        /**
        * @see MPhSrvUssdMessageSentObserver
        */
        void UssdMessageSentObserverHandleResult( TInt aError );
        

    private: // SubSession function handlers

        void CmdSubSessionCloseL( const RMessage2& aMessage );

        void CmdSubSessionSendMessageL( const RMessage2& aMessage );

        void CmdSubSessionSendMessageCancelL( const RMessage2& aMessage );

        void CmdSubSessionStartEditingL( const RMessage2& aMessage );

        void CmdSubSessionAppStartingL( const RMessage2& aMessage );

        void CmdSubSessionAppTerminatingL( const RMessage2& aMessage );

        void CmdSubSessionAppToForegroundL( const RMessage2& aMessage );

        void CmdSubSessionAppToBackgroundL( const RMessage2& aMessage );

        void CmdSubSessionStartSATL( const RMessage2& aMessage );

        void CmdSubSessionStopSATL( const RMessage2& aMessage );


    private:    // Data
        
        // Indicates whether we have a pending send request.
        TBool       iHavePendingSendMessagePointer;
        
        // Initialized when the client initiates a send message request.
        RMessage2   iPendingSendMessagePointer;
        
    };


#endif // CPHSRVSUBSESSIONUSSD_H


// End of File
