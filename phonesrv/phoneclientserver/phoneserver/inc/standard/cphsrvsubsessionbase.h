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
* Description:  Sub Session Base.
*
*/


#ifndef CPHSRVSUBSESSIONBASE_H
#define CPHSRVSUBSESSIONBASE_H


// INCLUDES
#include <e32base.h>
#include "mphsrvmessageprocessor.h" 


// FORWARD DECLARATIONS
class CPhSrvSubSessionNotifier;
class CPhSrvSession;


// CLASS DECLARATION

/**
*  The base class for all subsessions.
*
*  @since 1.0
*/
class CPhSrvSubSessionBase : 
    public CObject, 
    public MPhSrvMessageProcessor
    {
    public:

        /*
        * The subsessions type.
        *
        *   EPhSrvSubSessionTypeUndefined        Undefined subsession, error.
        *   EPhSrvSubSessionTypeExternalCall     Call subsession.
        *   EPhSrvSubSessionTypeCallNotifier     CallNotifiers subsession.
        *   EPhSrvSubSessionTypeUssd             USSD subsession.
        *   EPhSrvSubSessionTypeIhf              IHF subsession.
        *   EPhSrvSubSessionTypeEmergencyNumber  Emergency number subsession.
        *   EPhSrvSubSessionTypeComHand          Command Handler subsession.
        *   EPhSrvSubSessionTypeComHandNotify    ComHand Notify subsession.
        *   EPhSrvSubSessionTypeMessenger        Messenger subsession.
        *   EPhSrvSubSessionTypeImageHandler     Image handler subsession.
        *   EPhSrvSubSessionTypeEmergencyCall    Emergency call subsession.
        */
        enum TPhSrvSubSessionType
            {
            EPhSrvSubSessionTypeUndefined = KErrNotFound,
            EPhSrvSubSessionTypeExternalCall    = 0,
            EPhSrvSubSessionTypeCallNotifier    = 1,
            EPhSrvSubSessionTypeUssd            = 2,
            EPhSrvSubSessionTypeIhf             = 3,
            EPhSrvSubSessionTypeEmergencyNumber = 4,
            EPhSrvSubSessionTypeComHand         = 5,
            EPhSrvSubSessionTypeComHandNotify   = 6,
            EPhSrvSubSessionTypeMessenger       = 7,
            EPhSrvSubSessionTypeImageHandler    = 8,
            EPhSrvSubSessionTypeEmergencyCall   = 9

            };

    public:  // Constructors and destructor

        /**
        * Create an instance of this class
        *
        * @param aSession The session where this subsession belongs.
        * @param aType The type of subsession that this object should represent.
        */
        CPhSrvSubSessionBase( 
            CPhSrvSession& aSession, 
            TPhSrvSubSessionType aType );

        /**
        * Complete construction of the subsession.
        */
        virtual void ConstructL() { }


    public: // Access

        /**
        * Return the owning session.
        *
        * @return A handle to the owning session.
        */
        CPhSrvSession& PhoneSession();

        /**
        * Return the owning session.
        *
        * @return A constant handle to the owning session.
        */
        const CPhSrvSession& PhoneSession() const;

        /**
        * Return the type of this sub-session object.
        *
        * @return The type of this sub-session object.
        */
        TPhSrvSubSessionType Type() const;

        /**
        * Return the unique handle associated with this subsession.
        *
        * @return The unique handle associated with a subsession.
        */
        TInt SubSessionUniqueHandle() const;


    public:

        /**
        * Writes to client address space and
        * catch bad descriptors.
        *
        * @param aMessage The client.
        * @param aPtr The location where the data is written.
        * @param aDes The data that is written.
        * @param aOffset The data location where to start the writing.
        */
        void Write( 
            const RMessage2& aMessage,
            TInt aLocation,
            const TDesC8& aDes,
            TInt aOffset = 0 );

        /**
        * Reads from client address space and
        * catch bad descriptors.
        *
        * @param aMessage The client.
        * @param aPtr The location where the data is read.
        * @param aDes The read data storage.
        * @param aOffset The data location where to start the reading.
        */
        void Read(
            const RMessage2& aMessage,
            TInt aLocation,
            TDes8& aDes,
            TInt aOffset = 0 );


    private: // Data

        // A handle to the session which owns this subsession.
        CPhSrvSession& iSession;

        // The type of sub-session that this object represents.
        TPhSrvSubSessionType iSubSessionType;
    };


#endif      // CPHSRVSUBSESSIONBASE_H


// End of File
