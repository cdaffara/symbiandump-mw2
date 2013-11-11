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


#ifndef MPHSRVEMERGENCYNEGOTIATORS_H
#define MPHSRVEMERGENCYNEGOTIATORS_H


// INCLUDES
#include <e32base.h>
#include <phclttypes.h> 


// FORWARD DECLARATIONS
class MPhSrvEmergencyCall;
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Base class for call negotiators.
*
*  @since 1.0
*/
class MPhSrvEmergencyCallBase
    {
    public: // New functions

        /**
        * Makes an dial request.
        *
        * @param aMessage Message to be negotiated.
        * @param aArgs Call arguments.
        * @param aSubSession Subsession for dial.
        */
        virtual void EmergencyDialL( 
           const RMessage2& aMessage) = 0;
           
          
        /**
        * Cancel's an oustanding dial request. If the specified subsession 
        * doesn't have an oustanding dial request, then the subsession is 
        * panicked.
        *
        * @param aSubSession Subsession that has the dial request.
        */
        virtual void EmergencyDialCancel() = 0;
    };

/**
*  Abstract call negotiators class.
*
*  @since 1.0
*/
class MPhSrvEmergencyNegotiatorCall : public MPhSrvEmergencyCallBase
    {
    public: // New functions

        /**
        * Called by the call notifier subsession when it's ready to negotiate
        * (make another call) with the external call interface.
        *
        * @param aInitiator Call initiator instance.
        */
        virtual void SetNegotiatorReadyCall( 
            MPhSrvEmergencyCall& aInitiator ) = 0;

        /**
        * Called by the call notifier subsession when it isn't going to be
        * available to make further calls (session has closed)
        *
        * @param aInitiator Call initiator instance.
        */
        virtual void SetNegotiatorReadyCallCancel( 
            MPhSrvEmergencyCall& aInitiator ) = 0;

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


#endif // MPHSRVEMERGENCYNEGOTIATORS_H


// End of File
