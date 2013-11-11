/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Landmarks Server session class.
*
*
*/


#ifndef CPOSLMSERVERSESSION_H
#define CPOSLMSERVERSESSION_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <EPos_Landmarks.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include "EPos_HPosLmDatabaseInfo.h"

// FORWARD DECLARATIONS
class CPosLmServer;
class CPosLmSubsessionBase;
class CPosLmSubsessionRegistry;

// CLASS DECLARATION

/**
* Landmarks Server session class.
*/
class CPosLmServerSession : public CSession2
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aServer The server creating this session.
        */
        static CPosLmServerSession* NewL(
        /* IN/OUT */    CPosLmServer& aServer
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmServerSession();

    public: // New functions

        /**
        * Checks if the db of this session matches the specified URI and if so
        * if the db has a write lock.
        *
        * @param aUri The URI to compare with.
        * @return ETrue if the URI matches and this session has a write lock,
        *   otherwise EFalse.
        */
        TBool HasWriteLock(
        /* IN */        const TDesC& aUri
        );

        /**
        * Checks if the db of this session matches the specified URI and if so
        * if the db has any lock.
        *
        * @param aUri The URI to compare with.
        * @return ETrue if the URI matches and this session has any lock,
        *   otherwise EFalse.
        */
        TBool HasAnyLock(
        /* IN */        const TDesC& aUri
        );

        /**
        * Handles an incoming operation event from the server. If the URI
        * matches the URI associated with this session, the event is
        * forwarded to the client, if it is listening for events.
        *
        * @param aEvent The event.
        * @param aUri A URI associated with the database from which the event
        *   was generated.
        * @param aCheckUri ETrue if the URI should be checked. EFalse otherwise.
        */
        void HandleOperationEventL(
        /* IN */        TPosLmEvent aEvent,
        /* IN */        const TDesC& aUri,
        /* IN */        TBool aCheckUri
        );

        /**
        * Handles an incoming operation event from the server.
        * This function handles only database management events.
        *
        * @param aEvent The event.
        * @param aUri A database URI.
        */
        void HandleOperationEventL(
        /* IN */        TPosLmDatabaseEvent aEvent,
        /* IN */        const TDesC& aUri
        );

        /**
        * Completes outstanding events in the event handlers.
        *
        * @param aError An error code to complete the request with.
        */
        void CompleteOutstandingEventRequest(
        /* IN */        TInt aError
        );

    public: // From base classes

        /**
        * Handles the servicing of client requests to the server.
        *
        * @param aMessage The message containing the client request.
        */
        void ServiceL(
        /* IN */        const RMessage2& aMessage
        );

   private:

        /**
        * C++ default constructor.
        *
        * @param aServer The server creating this session.
        */
        CPosLmServerSession(
        /* IN/OUT */    CPosLmServer& aServer
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Dispatches the client requests.
        *
        * @param aMessage The message containing the client request.
        */
        void DispatchMessageL(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Adds a subsession to the registry. The subsession ownership is
        * transferred from the caller.
        *
        * @param aSubsession The subsession to add.
        * @param aMessage Message containing the client request.
        */
        void AddSubsessionToRegistryL(
        /* IN */        CPosLmSubsessionBase* aSubsession,
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Closes a subsession.
        *
        * @param aMessage The message containing the client request.
        */
        void CloseSubsession(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Forwards the message to a subsession. Leaves with KErrNotSupported
        * if no subsession exists.
        *
        * @param aMessage Message containing the client request.
        */
        void ForwardToSubsessionL(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Creates a Local Access subsession.
        *
        * @param aMessage Message containing the client request.
        */
        void CreateLocalAccessSubsessionL(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Creates an Event Listener subsession.
        *
        * @param aMessage Message containing the client request.
        */
        void CreateEventListenerSubsessionL(
        /* IN */        const RMessage2& aMessage
        );

        /**
        * Creates a Local DB Management subsession.
        *
        * @param aMessage Message containing the client request.
        */
        void CreateLocalDbManSubsessionL(
        /* IN */        const RMessage2& aMessage
        );

        void RegisterDatabaseL(
        /* IN */        const RMessage2& aMessage
        );

        void UnregisterDatabaseL(
        /* IN */        const RMessage2& aMessage
        );

        void UnregisterAllDatabasesL(
        /* IN */        const RMessage2& aMessage
        );

        void ModifyDatabaseSettingsL(
        /* IN */        const RMessage2& aMessage
        );

        void ReadDatabaseSettingsL(
        /* IN */        const RMessage2& aMessage
        );

        void ListDatabaseUrisL(
        /* IN */        const RMessage2& aMessage
        );

        void RemainingDatabaseUrisL(
        /* IN */        const RMessage2& aMessage
        );

        void ListDatabaseInfoL(
        /* IN */        const RMessage2& aMessage
        );

        void RemainingDatabaseInfoL(
        /* IN */        const RMessage2& aMessage
        );

        void ReportEventL(
        /* IN */        const RMessage2& aMessage
        );

        void FreeResources(
        /* IN */        const RMessage2& aMessage
        );
        
        //we can not use RFs::GetSystemDriveChar because this function is available
        //only from 3.2 plateform. So it will break compatibility in 3.1.
        TChar GetSystemDriveChar();

    private:    // Data

        // Pointer to the server
        CPosLmServer& iLmServer;

        TBool iDecrementSessions;

        CPosLmSubsessionRegistry* iSubsessionRegistry;

        // Stores intermediate results of list operations
        CDesCArray* iDesArray;

        // Stores intermediate results of list operations
        RPointerArray<HPosLmDatabaseInfo> iDbInfoArray;

    };

#endif      // CPOSLMSERVERSESSION_H

// End of File

