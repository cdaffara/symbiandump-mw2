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
* Description:  Object Negotiators.
*
*/


#ifndef MPHSRVOBJECTNEGOTIATORS_H
#define MPHSRVOBJECTNEGOTIATORS_H


// INCLUDES
#include <e32base.h>
#include <phclttypes.h> 
#include <cphcltextphonedialdata.h> 

// FORWARD DECLARATIONS
class MPhSrvInitiatorCall;
class MPhSrvInitiatorStatus;
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Base class for call negotiators.
*
*  @since 1.0
*/
class MPhSrvNegotiatorCallBase
    {
    public: // New functions

        /**
        * Makes an dial request.
        *
        * @param aMessage Message to be negotiated.
        * @param aArgs Call arguments.
        * @param aSubSession Subsession for dial.
        */
        virtual void NegotiatorDialL( 
            const RMessage2& aMessage, 
            CPhCltExtPhoneDialData& aArgs, 
            CPhSrvSubSessionBase& aSubSession ) = 0;

        /**
        * Cancel's an oustanding dial request. If the specified subsession 
        * doesn't have an oustanding dial request, then the subsession is 
        * panicked.
        *
        * @param aSubSession Subsession that has the dial request.
        */
        virtual void NegotiatorDialCancelL( 
            CPhSrvSubSessionBase& aSubSession ) = 0;
    };


/**
*  Abstract call negotiators class.
*
*  @since 1.0
*/
class MPhSrvNegotiatorCall : public MPhSrvNegotiatorCallBase
    {
    public: // New functions

        /**
        * Called by the call notifier subsession when it's ready to negotiate
        * (make another call) with the external call interface.
        *
        * @param aInitiator Call initiator instance.
        */
        virtual void SetNegotiatorReadyCallL( 
            MPhSrvInitiatorCall& aInitiator ) = 0;

        /**
        * Called by the call notifier subsession when it isn't going to be
        * available to make further calls (session has closed)
        *
        * @param aInitiator Call initiator instance.
        */
        virtual void SetNegotiatorReadyCallCancel( 
            MPhSrvInitiatorCall& aInitiator ) = 0;

        /**
        * Called by the call notifier subsession when it's attempted to make a
        * call - the call negotiator can then inform the external call 
        * interface of the result.
        *
        * @param aResult Result of the call attempt.
        */
        virtual void SetNegotiatorAttemptedCallResponseValue( 
            TPhCltPhoneResults aResult ) = 0;
    };

#endif // MPHSRVOBJECTNEGOTIATORS_H


// End of File
