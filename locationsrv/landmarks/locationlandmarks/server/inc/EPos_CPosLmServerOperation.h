/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: provides functionality common to all server operations
*
*
*/


#ifndef CPOSLMSERVEROPERATION_H
#define CPOSLMSERVEROPERATION_H

#include <e32base.h>
#include <d32dbms.h>
#include <epos_cposlmlocaldbaccess.h>
#include <epos_cposlmdiskutilities.h>

const TReal32 KOperationDone = 1; // Operation is done when progress 1

class MPosLmServerOperationObserver;
class CPosLmServer;

/**
* Base class for server operations. Contains functionality common to all
* server operations.
*/
class CPosLmServerOperation : public CActive
    {
    public: // Data types

        struct TPosMessage
            {
            TAny*                   iSession;
            RMessage2               iMsg;
            };

    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmServerOperation();

    public: // From CActive

        /**
        * Handles a leave occurring in the request completion event handler
        * RunL().
        *
        * @param[in] aError The leave code
        * @return Returns KErrNone.
        */
        TInt RunError( TInt aError );

        /**
        * Implements cancellation of an outstanding request.
        */
        void DoCancel();

    public: // New functions

        /*
        * Adds a message to the synchronous wait list. This message will not
        * be completed until the operation is done.
        *
        * @param[in, out] aMessage A message.
        * @param[in] aSession The session associated with the message.
        */
        void AddToSyncWaitListL(
            const RMessage2& aMessage,
            TAny* aSession );

        /*
        * Adds a message to the asynchronous wait list. This message will be
        * completed when a part of the operation is done to send back status
        * and progress to the client. It also completes if the operation is
        * done.
        *
        * @param[in, out] aMessage A message.
        * @param[in] aSession The session associated with the message.
        */
        void AddToAsyncWaitListL(
            const RMessage2& aMessage,
            TAny* aSession );

        /*
        * Cancels and removes a message from the asynchronous wait list. The
        * message sent as parameter is completed with KErrNone.
        *
        * @param[in,out] aMessage A message.
        * @param[in] aSession The session associated with the message.
        */
        void RemoveFromAsyncWaitList(
            const RMessage2& aMessage,
            TAny* aSession );

        /**
        * Removes messages associated with a specific session from the wait
        * lists.
        *
        * @param[in] aSession The session associated with the messages.
        */
        void RemoveSessionMessages( TAny* aSession );
        
        /** Returns status of this operation */
        TInt Status() const;
        
        /** Checks whether given session is a registered client
         *  of this operation */
        TBool IsRegistered( TAny* aSession ) const;        

        /** Checks whether this operation has registered clients */
        TBool HasClients() const;        

    protected:

        /**
        * C++ default constructor.
        *
        * @param[in] aServer Reference to the server
        * @param[in, out] aObserver Server operation observer.
        */
        CPosLmServerOperation(
            CPosLmServer& aServer,
            MPosLmServerOperationObserver* aObserver );

        /**
        * 2nd phase constructor for base class.
        *
        * @param[in] aUri Database URI.
        */
        void BaseConstructL( const TDesC& aUri );

        /**
        * Notifies the clients about the current state. Clients on the
        * asynchronous wait list will always be notified but clients on the
        * synchronous wait list will be notified only if progress is 100%.
        */
        void NotifyClients();

        /**
        * Completes this active object after each incremental step of the
        * operation.
        */
        void CompleteSelf();
        
        /** Checks if given session has requests in this operation */
        TBool HasRequests( TAny* aSession ) const;
        
    private:    // New functions

        /**
        * Writes status and progress to client.
        *
        * @param[in, out] aMessage Message arrived from client.
        */
        void Write( const RMessage2& aMessage );

        /*
        * Completes and removes the messages in the async wait list with
        * the error code of iStatusFlag. Progress is sent to client.
        *
        */
        void CompleteAndRemoveMessagesFromAsyncWaitList();

        /*
        * Completes and removes the messages in the sync wait list with
        * the error code of iStatusFlag.
        *
        */
        void CompleteAndRemoveMessagesFromSyncWaitList();

        /**
        * Removes messages associated with a specific session from a
        * specified wait list.
        *
        * @param[in, out] aWaitList A wait list.
        * @param[in] aSession The session associated with the message.
        */
        void RemoveFromWaitList(
            RArray<TPosMessage>& aWaitList,
            TAny* aSession );

        /**
        * Creates and returns a message struct based on the message and
        * the session associated with the message.
        *
        * @param[in] aMessage A message.
        * @param[in] aSession The session associated with the message.
        * @return A message struct.
        */
        TPosMessage Message(
            const RMessage2& aMessage,
            TAny* aSession );

    protected:    // Data

        CPosLmServer& iServer;
    
        // Server operation observer
        MPosLmServerOperationObserver* iObserver;

        // Wait list for synchronous messages
        RArray<TPosMessage> iSyncWaitList;

        // Wait list for asynchronous messages
        RArray<TPosMessage> iAsyncWaitList;

        // The current progress of the operation in percent.
        TReal32 iProgress;

        // The current status of the operation
        TInt iStatusFlag;

        // Disk utilities
        CPosLmDiskUtilities* iDiskUtilities;

        // The drive the database is on
        TChar iDbDrive;

        // Database
        CPosLmLocalDbAccess* iDb;

        // List of sessions
        RPointerArray<TUint> iClientSessions;
        
        // URI of a database
        HBufC* iDbUri;

    };

#endif      // CPOSLMSERVEROPERATION_H

// End of File

