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
* Description:  Command Handler Negotiators.
*
*/


#ifndef MPHSRVCOMHANDNEGOTIATORS_H
#define MPHSRVCOMHANDNEGOTIATORS_H


// INCLUDES
#include <e32base.h>
#include <cphcltcommandhandler.h> 

// FORWARD DECLARATIONS
class MPhSrvComHandInitiator;
class MPhSrvComHandInitiatorStatus;
class CPhSrvSubSessionBase;


// CLASS DECLARATION


/**
*  Abstract command handler negotiators class.
*
*  @since 2.6
*/
class MPhSrvComHandNegotiator
    {
    public: // New functions

        /**
        * Make a command handler request.
        *
        * @param aMessage Message to be negotiated.
        * @param aParams Request parameters.
        * @param aSubSession Subsession for command handler.
        */
        virtual void NegotiatorRequestL( 
            const RMessage2& aMessage, 
            const TPhCltComHandCommandParameters& aParams,
            CPhSrvSubSessionBase& aSubSession ) = 0;

        /**
        * Cancel's an oustanding command handler request. 
        * If the specified subsession doesn't have an oustanding request, 
        * then the subsession is panicked.
        *
        * @param aSubSession Subsession that has the command handler request.
        */
        virtual void NegotiatorRequestCancel( 
            CPhSrvSubSessionBase& aSubSession ) = 0;

        /**
        * Called by the command handler notifier subsession when it's ready 
        * to negotiate (perform another request) with the external interface.
        *
        * @param aInitiator Command handler initiator instance.
        */
        virtual void SetNegotiatorReadyRequestL( 
            MPhSrvComHandInitiator& aInitiator ) = 0;

        /**
        * Called by the command handler notifier subsession when it isn't going
        * to be available to make further requests (session has closed)
        *
        * @param aInitiator Command handler initiator instance.
        */
        virtual void SetNegotiatorReadyRequestCancel( 
            MPhSrvComHandInitiator& aInitiator ) = 0;

        /**
        * Called by the command handler notifier subsession when it's attempted
        * to perform a request - the command handler negotiator can then inform
        * the external command handler interface of the result.
        *
        * @param aResult Result of the request perfom.
        */
        virtual void SetNegotiatorAttemptedRequestResponseValue( 
            TPhCltPhoneResults aResult ) = 0;
    };

#endif // MPHSRVCOMHANDNEGOTIATORS_H


// End of File
