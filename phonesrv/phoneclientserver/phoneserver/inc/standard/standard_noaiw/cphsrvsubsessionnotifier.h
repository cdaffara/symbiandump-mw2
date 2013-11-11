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
* Description:  Sub Session Notifier.
*
*/


#ifndef CPHSRVSUBSESSIONNOTIFIER_H
#define CPHSRVSUBSESSIONNOTIFIER_H


// INCLUDES
#include <e32base.h>
#include "cphsrvsubsessionbase.h" 
#include "mphsrvobjectinitiators.h" 
#include "cphsrvsubsessionnotifier.h" 
#include "mphsrvcomhandinitiators.h" 

// CLASS DECLARATION

/**
*  Call Notifier Subsession.
*
*  @since 1.0
*/
class CPhSrvSubSessionNotifier :
    public CPhSrvSubSessionBase, 
    public MPhSrvInitiatorCall,
    public MPhSrvComHandInitiator
    {
    private:


    public:  // Constructors and destructor

        /**
        * Constructor
        *
        * @param aSession The session where this subsession belongs. 
        */
        CPhSrvSubSessionNotifier( CPhSrvSession& aSession );

        /**
        * Destructor
        */
        ~CPhSrvSubSessionNotifier();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();


    private:  // Functions from base classes

        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );

        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );

        /**
        * Called by the notification manager when the phone app engine should
        * be informed that it needs to make a call.
        *
        * @see MPhSrvInitiatorCall
        */
        void InitiatorMakeAsynchronousCall( const TPhCltCallArgs& aArgs );

        /**
        * Does this subsession have a pending request?
        *
        * @see MPhSrvInitiatorCall
        */
        TBool InitiatorIsReadyToMakeCall() const;

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

        void CmdSubSessionCloseL( const RMessage2& aMessage );

        void CmdSubSessionNotifyCallRequestL( const RMessage2& aMessage );

        void CmdSubSessionNotifyCallRequestCancelL( const RMessage2& aMessage );

        void CmdSubSessionReportCallResultL( const RMessage2& aMessage );
        
        void CmdSubSessionComHandNotifyRequestL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandNotifyRequestCancelL( 
            const RMessage2& aMessage );

        void CmdSubSessionComHandNotifyReportL( 
            const RMessage2& aMessage );

    private:     // Data

        // Indicates whether we have an pending request
        TBool                            iHavePendingDialRequestPointer;
    
        // Initialized when the phone app engine is 
        // ready to handle dial requests.
        RMessage2                        iPendingDialRequestPointer;
        
        // Indicates whether we have an pending request
        TBool                       iHavePendingComHandRequestPointer;
    
        // Initialized when the phone app engine is 
        // ready to handle command handler requests.
        RMessage2                   iPendingComHandRequestPointer;
        

    };

#endif      // CPHSRVSUBSESSIONNOTIFIER_H


// End of File
