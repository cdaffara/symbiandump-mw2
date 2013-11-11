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
* Description:  Messenger Negotiators.
*
*/


#ifndef MPHSRVMESSENGERNEGOTIATORS_H
#define MPHSRVMESSENGERNEGOTIATORS_H


// INCLUDES
#include <e32base.h>
#include "phsrvmessengertypes.h" 
#include <cphcltmessenger.h> 

// FORWARD DECLARATIONS
class CPhSrvSubSessionBase;


// CLASS DECLARATION


/**
*  Abstract messenger negotiators class.
*
*  @since 2.6
*/
class MPhSrvMessengerNegotiator
    {
    public: // New functions

        /**
        * Make a messenger request.
        *
        * @param aSubSession Subsession for messenger.
        * @param aParameters The request parameters.
        * @param aMessage The received message. Used also in panic situations.
        */
        virtual void NegotiatorRequest( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage ) = 0;

        /**
        * Cancel an oustanding messenger request. 
        * If the specified subsession doesn't have an oustanding request, 
        * then the subsession is paniced.
        *
        * @param aSubSession Subsession that has the messenger request.
        * @param aRequestType The type of the request to be canceled.
        */
        virtual void NegotiatorRequestCancel( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerCommand aRequestType ) = 0;

        /**
        * Construct, i.e. reserve space for the subsession given 
        * as parameter.
        *
        * @param aSubSession Subsession that is constructed.
        * @param aParameters The needed construct parameters.
        * @param aMessage The message that is completed when request is ready.
        */
        virtual void NegotiatorRequestConstructL( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage ) = 0;

        /**
        * Delete, i.e. free all the space reserved for the subsession given 
        * as parameter.
        *
        * @param aSubSession Subsession whose memory is freed.
        */
        virtual void NegotiatorRequestDelete( 
            CPhSrvSubSessionBase& aSubSession ) = 0;
    };

#endif // MPHSRVMESSENGERNEGOTIATORS_H


// End of File
