/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Subsession for Command Handler notify.
*
*/

#ifndef CPHSRVSUBSESSIONCOMHANDNOTIFY_H
#define CPHSRVSUBSESSIONCOMHANDNOTIFY_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 
#include "mphsrvcomhandinitiators.h" 

// CLASS DECLARATION

/**
*  Command handler Subsession.
*
*  @since 2.6
*/
class CPhSrvSubSessionCommandHandlerNotify 
:   public CPhSrvSubSessionBase,
    public MPhSrvComHandInitiator
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @return CPhSrvSubSessionCommandHandlerNotify subsession.
        */
        static CPhSrvSubSessionBase* NewL(
            CPhSrvSession& aSession );

        /**
        * Destructor.
        *
        */
        ~CPhSrvSubSessionCommandHandlerNotify();


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
        * @see MPhSrvComHandInitiator
        */
        void InitiatorMakeAsynchronousComHandRequest( 
            const TPhCltComHandCommandParameters& 
                aParams );

        /**
        * @see MPhSrvComHandInitiator
        */
        TBool InitiatorIsReadyToPerformRequest() const;


    private: // SubSession function handlers

        void CmdSubSessionClose( const RMessage2& aMessage );

        void CmdSubSessionComHandNotifyRequestL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandNotifyRequestCancelL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandNotifyReportL( 
            const RMessage2& aMessage );


    private: 

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionCommandHandlerNotify( CPhSrvSession& aSession );
        

        
    private:     // Data

        // Indicates whether we have an pending request
        TBool                       iHavePendingComHandRequestPointer;
    
        // Initialized when the phone app engine is 
        // ready to handle command handler requests.
        RMessage2                   iPendingComHandRequestPointer;

    };


#endif // CPHSRVSUBSESSIONCOMHANDNOTIFY_H


// End of File
