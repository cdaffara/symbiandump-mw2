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
* Description: Forwards client request messages to server operations.
*
*
*/


#ifndef CPOSLMOPERATIONMANAGER_H
#define CPOSLMOPERATIONMANAGER_H

#include <badesca.h>
#include "EPos_MPosLmServerOperationObserver.h"

class CPosLmServer;
class CPosLmServerOperation;
class MPosLmServerManagerObserver;

/**
* Forwards client request messages to server operations.
*/
class CPosLmOperationManager :
    public CBase, public MPosLmServerOperationObserver
    {

    public: // Data types

    /**
    * Operation manager type.
    */
    enum TPosOperationManagerType
        {
        EPosInitializerManager = 0,     /** Manager handles initialization. */
        EPosMaxNumberOfManagers
        };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aType Operation manager type.
        * @param[in,out] aObserver An observer.
        */
        static CPosLmOperationManager* NewL(
            CPosLmServer& aServer,
            TPosOperationManagerType aType,
            MPosLmServerManagerObserver* aObserver );

        /**
        * Destructor.
        */
        virtual ~CPosLmOperationManager();

    public: // From MPosLmServerOperationObserver.

        /**
        * Called to report status of a server operation.
        *
        * @param[in] aServerOperation The object which handles the server operation.
        * @param[in] aStatus The status of the server operation.
        */
        void HandleOperationStatus(
            CPosLmServerOperation* aServerOperation,
            TInt aStatus );

    public: // New functions

        /**
        * Forwards a client request to an operation depending on the message
        * function type and the database URI . If no operation matches the URI
        * a new operation is created.
        *
        * @param[in] aMessage A client request message.
        * @param[in] aUri A database URI.
        * @param[in] aSession The session associated with the message.
        */
        void AddRequestL(
            const RMessage2& aMessage,
            const TDesC& aUri,
            TAny* aSession );

        /**
        * Gets the number of running server operations this manager currently
        * manages.
        *
        * @return The number of running server operations.
        */
        TInt Count();

        /**
        * Removes messages associated with a specific session from the
        * server operations
        *
        * @param[in] aSession A pointer to a session.
        */
        void RemoveSessionMessages( TAny* aSession );

    private:

        /**
        * C++ default constructor.
        *
        * @param[in] aType Operation manager type.
        * @param[in,out] aObserver An observer.
        */
        CPosLmOperationManager(
            CPosLmServer& aServer,
            TPosOperationManagerType aType,
            MPosLmServerManagerObserver* aObserver
            );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Returns the server operation associated with the specified URI. If no
        * operation exists it will be created.
        *
        * @param[in] aUri A database URI.
        * @param[in] aSession Requestor of the operation
        * @return A server operation corresponding the URI.
        */
        CPosLmServerOperation* OperationL( const TDesC& aUri, TAny* aSession );
        CPosLmServerOperation* FindOperationL( const TDesC& aUri );
        CPosLmServerOperation* CreateOperationL( const TDesC& aUri );

    private:    // Data

        CPosLmServer& iServer;
    
        // Array of running operations. Element position matches a URI.
        RPointerArray<CPosLmServerOperation> iOperations;

        // Array of URIs. Element position matches an operation.
        CDesCArrayFlat* iUriArray;

        // The type of this manager.
        TPosOperationManagerType iType;

        // Operation manager observer
        MPosLmServerManagerObserver* iObserver;

    };

#endif      // CPOSLMOPERATIONMANAGER_H


