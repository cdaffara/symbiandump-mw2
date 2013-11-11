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
* Description:  Call Request Manager.
*
*/


#ifndef CPHSRVCALLREQUESTMANAGER_H
#define CPHSRVCALLREQUESTMANAGER_H


// INCLUDES
#include <e32base.h>
#include "mphsrvobjectnegotiators.h" 
#include "mphsrvobjectinitiators.h" 
#include <cphcltextphonedialdata.h> 

// FORWARD DECLARATIONS
class CPhSrvCallRequest;


// CLASS DECLARATION

/**
*  Call request manager.
*
*  @since 1.0
*/
class CPhSrvCallRequestManager : 
    public CBase, 
    public MPhSrvNegotiatorCall
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        */
        CPhSrvCallRequestManager();

        /**
        * Destructor.
        */
        ~CPhSrvCallRequestManager();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();


    public: // Functions from base classes

        /**
        * @see MPhSrvNegotiatorCallBase
        */
        void NegotiatorDialL( 
            const RMessage2& aMessage,  
            CPhCltExtPhoneDialData& aArgs,
            CPhSrvSubSessionBase& aSubSession );

        /**
        * @see MPhSrvNegotiatorCallBase
        */
        void NegotiatorDialCancelL( CPhSrvSubSessionBase& aSubSession );

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorReadyCallL( MPhSrvInitiatorCall& aInitiator );

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorReadyCallCancel( MPhSrvInitiatorCall& aInitiator );

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorAttemptedCallResponseValue( 
            TPhCltPhoneResults aResult );


    private:    // Data

        // A list of requests made by the external call interface that the phone
        // app should dial. It completes them sequentially after its finished 
        // the previous call.
        // Index 0 is reserved for the emergency call.
        RPointerArray<CPhSrvCallRequest>*  iOutstandingRequestsCall;

        // The object responsible for processing external client call requests. 
        // This is essentially an interface to the phone app engine.
        MPhSrvInitiatorCall*               iCallInitiator;
    };


#endif // CPHSRVCALLREQUESTMANAGER_H


// End of File
