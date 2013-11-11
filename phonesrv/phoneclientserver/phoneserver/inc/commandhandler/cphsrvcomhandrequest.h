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
* Description:  Command Handler Request.
*
*/


#ifndef CPHSRVCOMHANDREQUEST_H
#define CPHSRVCOMHANDREQUEST_H


// INCLUDES
#include <e32base.h>
#include <phclttypes.h> 
#include <cphcltcommandhandler.h> 

// CONSTANTS


// FORWARD DECLARATIONS
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Command handler request class.
*
*  @since 2.6
*/
class CPhSrvComHandRequest : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor. For normal requests, i.e. that are reserved when 
        * they arrive.
        *
        * @param aMessage The outstanding client request which will be completed
        *                 when the command handler initiation outcome is known.
        * @param aParams  The command handler request arguments.
        * @param aSubSessionHandle The subsession handle.
        * @return Pointer to created CPhSrvComHandRequest instance.
        */
        static CPhSrvComHandRequest* NewLC(
            const RMessage2& aMessage, 
            const TPhCltComHandCommandParameters& aParams,
            TInt aSubSessionHandle );

        /**
        * Constructor for Emergency request object only.
        *
        * @return Pointer to created CPhSrvComHandRequest instance.
        */
        static CPhSrvComHandRequest* NewL();


    public: // New functions

        /**
        * Tell the external client (the requester) the result of their 
        * actions. This information has come via the notifier
        * (i.e. phone app engine).
        *
        * @param aResultOfAttemptingCall 
        *        The result of initiating an earlier command handler request.
        */
        void InformOfComHandRequestAttemptResult( 
            TPhCltPhoneResults aResultOfAttemptingRequest );

        /**
        * Cancel the asynchronous command handler process. 
        * Completes client request with KErrCancel.
        */
        void Cancel();

        /**
        * The sub-session handle.
        *
        * @return The unique handle associated with the subsession which 
        *         initiated a command handler request.
        */
        TInt SubSessionHandle() const;

        /**
        * The parameters for the command handler request.
        *
        * @return The command handler request parameters.
        */
        const TPhCltComHandCommandParameters& 
            ComHandParameters() const;

        /**
        * Update Emergency request status.
        * 
        * @param aMessage The outstanding client request which will be completed
        *                 when the command handler initiation outcome is known.
        * @param aArguments The command handler request parameters.
        * @param aSubSessionHandle Subsession handle to emergency request.
        */
        void UpdateEmergencyRequest( 
            const RMessage2& aMessage, 
            const TPhCltComHandCommandParameters& aParams,
            TInt aSubSessionHandle );

        /**
        * Clear the Emergency request object data.
        */
        void ClearEmergencyRequest();


    private:

        /**
        * C++ constructor
        */
        CPhSrvComHandRequest( 
            const RMessage2& aMessage, 
            const TPhCltComHandCommandParameters& aParams,
            TInt aSubSessionHandle );

        /**
        * C++ constructor
        */
        CPhSrvComHandRequest();


    private:  // Data

        // A request made by the external command handler interface that the
        // phone app should handle. It completes them sequentially after its 
        // finished the previous request.
        RMessage2                       iPendingRequestPointer;

        // The object responsible for processing external client command 
        // handler requests. 
        // This is essentially an interface to the phone app engine.
        TPhCltComHandCommandParameters  iComHandParams;

        // A handle to the subsession that initiated this request.
        TInt                            iSubSessionHandle;
    };


#endif // CPHSRVCOMHANDREQUEST_H


// End of File
