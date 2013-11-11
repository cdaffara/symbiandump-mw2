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
* Description:  Messenger Request Manager.
*
*/


#ifndef CPHSRVMESSENGERREQUESTMANAGER_H
#define CPHSRVMESSENGERREQUESTMANAGER_H


// INCLUDES
#include <e32base.h>
#include "mphsrvmessengernegotiators.h" 
#include <cphcltcommandhandler.h> 

// FORWARD DECLARATIONS
class CPhSrvMessengerObject;
class CPhSrvServer;


// CONSTANTS

// Type for array used in this class.
typedef RPointerArray< CPhSrvMessengerObject > RMessengerArray;


// CLASS DECLARATION

/**
*  Messenger request manager.
*
*  @since 2.6
*/
class CPhSrvMessengerRequestManager
:   public CBase, 
    public MPhSrvMessengerNegotiator
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        * 
        * @param aServer The server instance.
        */
        CPhSrvMessengerRequestManager( CPhSrvServer& aServer );

        /**
        * Destructor.
        */
        ~CPhSrvMessengerRequestManager();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();


    public: // Functions from base classes

        /**
        * @see MPhSrvMessengerNegotiators
        */
        void NegotiatorRequest( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage );

        /**
        * @see MPhSrvMessengerNegotiators
        */
        void NegotiatorRequestCancel( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerCommand aRequestType );

        /**
        * @see MPhSrvMessengerNegotiators
        */
        void NegotiatorRequestConstructL( 
            CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage );

        /**
        * @see MPhSrvMessengerNegotiators
        */
        void NegotiatorRequestDelete( 
            CPhSrvSubSessionBase& aSubSession );


    private: 

        /**
        * Updates the request objects.
        * 
        * @param aEntry The entry whose information is updated.
        * @param aCommand The command that was performed for aEntry.
        */
        void UpdateMessengerChain(
            CPhSrvMessengerObject* aEntry,
            TPhCltMessengerCommand aCommand );

        /**
        * Handle all Send requests at one time.
        * 
        * @param aEntry The object whose information is updated.
        * @param aSendArray The send array that is handled.
        */
        void HandleAllSendRequests(
            CPhSrvMessengerObject* aEntry,
            RMessengerArray* aSendArray );

        /**
        * Handles the Send Requests.
        * 
        * @param aEntry The entry whose information is updated.
        * @param aSendArray The send array that is handled.
        * @return ETrue if Send was fully handled, EFalse otherwise.
        */
        TBool HandleSendRequest( 
            CPhSrvMessengerObject* aEntry,
            RMessengerArray* aSendArray );

        /**
        * Checks whether there already is active request from this client 
        * (subsession). Checks Send,Receive and Skip requests.
        * If request is not active, then sets it active.
        * 
        * @param aSubSession The client who's requests are searched.
        * @param aParameters Contains the request type that is checked.
        *                    If not active, sets the given request to be active.
        * @param aMessage    The received message.

        * @return ETrue if there already was request from this client, 
        *         otherwise EFalse.
        */
        TBool RequestAlreadyExists( 
            const CPhSrvSubSessionBase& aSubSession,
            const TPhCltMessengerParameters& aParameters,
            const RMessage2& aMessage );


        /**
        * Returns the correct array which contains messenger objects.
        * 
        * @return The array containing the messenger objects.
        */
        RMessengerArray* MessengerArray();


        /**
        * Returns the correct array which contains send chain.
        * 
        * @return The array containing the send chain.
        */
        RMessengerArray* SendChain();


    private:    // Data

        // A list of request objects received from external Messenger interface.
        // Messenger request manager completes them sequentially 
        // after its finished the previous request.
        RMessengerArray*                        iMessengerObjects;

        // The server instance.
        CPhSrvServer&                           iServer;

        // A FIFO chain of Send requests made by the external Messenger 
        // interface clients. They are completed sequentially.
        RMessengerArray*                        iSendChain;

        // The object whose Receive / Skip is waited.
        CPhSrvMessengerObject*                  iRequestObject;

        // The variable for Ids for sent messages.
        TUint                                   iSentDataIdGenerator;

        // The pointer to hold the default size (KPhCltDefMesSize)
        // buffer at startup, exist always if construction valid.
        // This buffer can be changed to bigger one during execution
        // if memory allocation is successful.
        HBufC8*                                 iDefaultSizeBuffer;

        // Tells whether there is Send operation ongoing or not.
        // ETrue if Send request have been processed at least once but not
        // completed yet.
        TBool                                   iSendProcessOngoing;

    };


#endif // CPHSRVMESSENGERREQUESTMANAGER_H


// End of File
