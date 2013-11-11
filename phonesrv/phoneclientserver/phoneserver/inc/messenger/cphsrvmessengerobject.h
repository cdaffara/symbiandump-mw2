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
* Description:  Messenger object. Contains both send and receive request 
*                storages.
*
*/


#ifndef CPHSRVMESSENGEROBJECT_H
#define CPHSRVMESSENGEROBJECT_H


// INCLUDES
#include <e32base.h>

#include "phsrvmessengertypes.h" 
#include <cphcltcommandhandler.h> 

// FORWARD DECLARATIONS
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Messenger object, i.e. request storage class.
*
*  @since 2.6
*/
class CPhSrvMessengerObject : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor. 
        *
        * @param aSubSession The subsession that owns this object.
        * @param aParameters The Construct parameters.
        * @return Pointer to created CPhSrvMessengerObject instance.
        */
        static CPhSrvMessengerObject* NewLC(
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters );


        /**
        * Destructor
        */
        ~CPhSrvMessengerObject();


    public: // New functions

        /**
        * Tell the external client (the requester) the result of their 
        * actions. 
        *
        * @param aResultOfAttemptingCall 
        *        The result of executing the Messenger request.
        * @param aRequest The request which result this is.
        */
        void InformOfMessengerRequest( 
            const TPhCltPhoneResults aResultOfAttemptingRequest,
            const TPhCltMessengerCommand aRequest );


        /**
        * The sub-session handle.
        *
        * @return The unique handle associated with the subsession which 
        *         initiated a command handler request.
        */
        TInt SubSessionHandle() const;


        /**
        * Sets the given request active. Use IsRequestActive() to check
        * whether request can be set active or not.
        *
        * @param aParameters The request and it's data that is set active.
        * @param aMessage The request status container.
        * @param aSentDataId The Id associated for sent message. Valid only
        *                    if the request is Send.
        */
        void SetActive( 
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage,
            const TUint aSentDataId );


        /**
        * Checks if the given request is already active.
        *
        * @param aRequest The request which is checked.
        * @return ETrue if aRequest is active, EFalse otherwise.
        */
        TBool IsRequestActive( const TPhCltMessengerCommand aRequest ) const;


        /**
        * Check whether this object is ready to receive the message or not.
        * 
        * @param aSentMessage The message that should be received.
        * @param aSentDataId The Id of the sent message to be received.
        * @return ETrue if object is ready to receive, EFalse otherwise.
        */
        TBool IsReadyToReceive( 
            const TDesC8& aSentMessage,
            const TUint aSentDataId );


        /**
        * Complete the active receive request. Use IsRequestActive() to find
        * out whether this method can be called.
        * 
        * @param aSentMessage The sent message that is given to receiver.
        * @param aSentDataId The Id of the sent message.
        * @return KErrNone if the completion was successful, 
        *         Symbian error code otherwise.
        */
        TInt CompleteReceive(
            const TDesC8& aSentMessage,
            const TUint aSentDataId );


        /**
        * Return objects Send data length.
        * 
        * @return The object's Send data length.
        */
        TInt GetSendDataLength();


        /**
        * Return objects Send data.
        * 
        * @param aDes The descriptor that will contain the sent data after 
        *             return. Use GetSendDataLength() to see how big buffer is
        *             needed to return whole sent message.
        */
        void GetSendData( TDes8& sDes ) const;


        /**
        * Return objects interested Uid.
        * 
        * @return The Uid of the messages this object is interested.
        */
        const TUid& GetUid() const;


        /**
        * Return the ID of the sent message.
        * 
        * @return The Id of the sent message.
        */
        TUint SentDataId();


        /**
        * Check if the received message is skipped or not.
        * 
        * @return ETrue if the given message is skipped, otherwise EFalse.
        */
        TBool IsMessageSkipped(
            const TUint aSentDataId );


    private:

        /**
        * C++ constructor
        */
        CPhSrvMessengerObject(
            CPhSrvSubSessionBase& aSubSession );


        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TPhCltMessengerParameters& aParameters );


    private:  // Data

        // The Messenger object parameters.
        TPhSrvMessengerParameters      iParameters;

        // A handle to the subsession that initiated this request.
        CPhSrvSubSessionBase&          iSubSession;
    };


#endif // CPHSRVMESSENGEROBJECT_H


// End of File
