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
* Description:  Call Request.
*
*/


#ifndef CPHSRVCALLREQUEST_H
#define CPHSRVCALLREQUEST_H


// INCLUDES
#include <e32base.h>
#include <phclttypes.h> 


// CONSTANTS

// Default emergency number
_LIT( KPhSrvUsedEmergencyCallNumber , "112" );


// FORWARD DECLARATIONS
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Call request class.
*
*  @since 1.0
*/
class CPhSrvCallRequest : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Tell the external client (who requested that a number was called) the
        * result of their actions. This information has come via the notifier
        * (i.e. the phone app engine).
        *
        * @param aMessage The outstanding client request which will be completed
        *                 when the call initiation outcome is known.
        * @param aArguments The call arguments, such as phone number, contact 
        *                   id, client window group etc.
        * @param aSubSessionHandle The subsession handle.
        * @return Pointer to created CPhSrvCallRequest instance.
        */
        static CPhSrvCallRequest* NewLC(
            const RMessage2& aMessage, 
            const TPhCltCallArgs& aArguments, 
            TInt aSubSessionHandle );

        /**
        * Constructor for Emergency call object only.
        */
        static CPhSrvCallRequest* NewL();


    public: // New functions

        /**
        * Tell the external client (who requested that a number was called) the
        * result of their actions. This information has come via the notifier
        * (i.e. the phone app engine).
        *
        * @param aResultOfAttemptingCall 
        *        The result of initiating an earlier call.
        */
        void InformOfCallAttemptResult( 
            TPhCltPhoneResults aResultOfAttemptingCall );

        /**
        * Cancel the asynchronous dial process. Completes client request with
        * KErrCancel
        */
        void Cancel();

        /**
        * The sub-session handle.
        *
        * @return The unique handle associated with the subsession which 
        *         initiated a call request.
        */
        TInt SubSessionHandle() const;

        /**
        * The arguments for the call.
        *
        * @return The call arguments.
        */
        const TPhCltCallArgs& CallArguments() const;

        /**
        * Update Emergency call status
        * @param aMessage The outstanding client request which will be completed
        *                 when the call initiation outcome is known.
        * @param aArguments The call arguments, such as phone number, 
        *                   contact id, client window group etc.
        * @param aSubSessionHandle Subsession handle to emergency call.
        */
        void UpdateEmercgencyCall( 
            const RMessage2& aMessage, 
            const TPhCltCallArgs& aArguments, 
            TInt aSubSessionHandle );

        /**
        * Clear the Emergency call object data.
        */
        void ClearEmergencyCall();


    private:

        /**
        * C++ constructor
        */
        CPhSrvCallRequest( 
            const RMessage2& aMessage, 
            const TPhCltCallArgs& aArguments, 
            TInt aSubSessionHandle );

        /**
        * C++ constructor
        */
        CPhSrvCallRequest();


    private:  // Data

        // A list of requests made by the external call interface that the phone
        // app should dial. It completes them sequentially after its finished 
        // the previous call.
        RMessage2       iPendingRequestPointer;

        // The object responsible for processing external client call requests. 
        // This is essentially an interface to the phone app engine.
        TPhCltCallArgs  iCallArguments;

        // A handle to the subsession that initiated this request.
        TInt            iSubSessionHandle;
    };


#endif // CPHSRVCALLREQUEST_H


// End of File
