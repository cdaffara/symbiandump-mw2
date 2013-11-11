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
* Description:  Command Handler Request Manager.
*
*/


#ifndef CPHSRVCOMHANDREQUESTMANAGER_H
#define CPHSRVCOMHANDREQUESTMANAGER_H


// INCLUDES
#include <e32base.h>
#include "mphsrvcomhandnegotiators.h" 
#include "mphsrvcomhandinitiators.h" 




// FORWARD DECLARATIONS
class CPhSrvComHandRequest;
class CPhSrvServer;

// CLASS DECLARATION

/**
*  Command handler request manager.
*
*  @since 2.6
*/
class CPhSrvComHandRequestManager : 
    public CBase, 
    public MPhSrvComHandNegotiator
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        * 
        * @param aServer The server instance.
        */
        CPhSrvComHandRequestManager( CPhSrvServer& aServer );

        /**
        * Destructor.
        */
        ~CPhSrvComHandRequestManager();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();


    public: // Functions from base classes

        /**
        * @see MPhSrvComHandNegotiators
        */
        void NegotiatorRequestL( 
            const RMessage2& aMessage, 
            const TPhCltComHandCommandParameters& aParams,
            CPhSrvSubSessionBase& aSubSession );

        /**
        * @see MPhSrvComHandNegotiators
        */
        void NegotiatorRequestCancel( 
            CPhSrvSubSessionBase& aSubSession );

        /**
        * @see MPhSrvComHandNegotiators
        */
        void SetNegotiatorReadyRequestL( 
            MPhSrvComHandInitiator& aInitiator );

        /**
        * @see MPhSrvComHandNegotiators
        */
        void SetNegotiatorReadyRequestCancel( 
            MPhSrvComHandInitiator& aInitiator );

        /**
        * @see MPhSrvComHandNegotiators
        */
        virtual void SetNegotiatorAttemptedRequestResponseValue( 
            TPhCltPhoneResults aResult );


    private: 

        /**
        * Updates the request queue.
        */
        void UpdateQueue();

        /**
        * Checks whether there already is request from this client (subsession).
        * 
        * @param aSubSession The client who's requests are searched.
        * @return ETrue if there already is request from this client, 
        *         otherwise EFalse.
        */
        TBool RequestAlreadyExists( CPhSrvSubSessionBase& aSubSession );


    private:    // Data

        // A list of requests made by the external command handler interface 
        // that the phone app should handle. It completes them sequentially 
        // after its finished the previous request.
        // Index 0 is reserved for the emergency request.
        RPointerArray< CPhSrvComHandRequest >*  iOutstandingComHandRequests;

        // The object responsible for processing external client command 
        // handler requests. 
        // This is essentially an interface to the phone app engine.
        MPhSrvComHandInitiator*                 iComHandInitiator;

        // The server instance.
        CPhSrvServer&                           iServer;

        // If NULL, then the active request was deleted during execution.
        CPhSrvComHandRequest*                   iActiveRequest;
    };


#endif // CPHSRVCOMHANDREQUESTMANAGER_H


// End of File
