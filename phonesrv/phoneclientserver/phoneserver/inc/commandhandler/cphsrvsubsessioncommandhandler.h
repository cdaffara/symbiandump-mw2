/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Subsession for Command Handler.
*
*/

#ifndef CPHSRVSUBSESSIONCOMMANDHANDLER_H
#define CPHSRVSUBSESSIONCOMMANDHANDLER_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 


// CLASS DECLARATION

/**
*  Command Handler Subsession.
*
*  @since 2.6
*/
class CPhSrvSubSessionCommandHandler 
:   public CPhSrvSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @return CPhSrvSubSessionCommandHandler subsession.
        */
        static CPhSrvSubSessionBase* NewL(
            CPhSrvSession& aSession );


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

        void CmdSubSessionClose( const RMessage2& aMessage );

        void CmdSubSessionComHandAtdL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandAtaL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandChldL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandChupL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandVtsL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandCancelL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandForwardRequestL(
            const RMessage2& aMessage );
        
        void CmdSubSessionComHandMuteMicL( 
            const RMessage2& aMessage );
        
        void CmdSubSessionComHandMuteRingingToneL( 
            const RMessage2& aMessage );
            
        TBool IsCancelAllowed(
            const RMessage2& aMessage );
    private: 

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionCommandHandler( CPhSrvSession& aSession );
        
    };

#endif // CPHSRVSUBSESSIONCOMMANDHANDLER_H


// End of File
