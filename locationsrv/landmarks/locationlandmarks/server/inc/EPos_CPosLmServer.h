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
* Description: The main server side class of Landmarks Server.
*
*
*/


#ifndef CPOSLMSERVER_H
#define CPOSLMSERVER_H

#include <e32base.h>
#include <f32file.h>
#include <EPos_Landmarks.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include "EPos_CPosLmOperationManager.h"
#include "EPos_MPosLmServerManagerObserver.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_MPosLmBackupObserver.h"

class CPosLmBackupListener;

// Ranges
const TInt KPosLmRanges[] =
    {
    EPosLmServerInitializeSync,
    // 0th range up to EPosLmServerReadDatabaseSettings inclusive

    EPosLmServerRegisterSession,
    // 1th Range up to EPosLmServerUnregisterSubsession inclusive

    EPosLmServerDbLock,
    // 2nd Range up to EPosLmServerDbLock inclusive

    EPosLmServerCreateDatabase,
    // 3rd Range up to EPosLmServerFreeResources inclusive

    EPosLmServerFreeRange2,
    // 4th Range up to EPosLmServerCustomCheckRange2 exclusive
    
    EPosLmServerCustomCheckRange2,
    // 5th Range up to EPosLmServerReadRange2 exclusive
    
    EPosLmServerReadRange2,
    // 6th Range up to EPosLmServerWriteRange2 exclusive
    
    EPosLmServerWriteRange2, 
    // 7th Range up to EPosLmServerLast exclusive
    
    EPosLmServerLast
    // last Range up to KMaxTInt inclusive
    };

// Number of ranges
const TUint KPosLmRangeCount = sizeof(KPosLmRanges)/sizeof(TInt);


// Elements index
const TUint8 KPosLmElementsIndex[KPosLmRangeCount] =
    {
    0,                              // applies to 0th range
    CPolicyServer::EAlwaysPass,     // applies to 1st range
    CPolicyServer::ECustomCheck,    // applies to 2nd range
    1,                              // applies to 3rd range
    CPolicyServer::EAlwaysPass,     // applies to 4th range
    CPolicyServer::ECustomCheck,    // applies to 5th range
    0,                              // applies to 6th range
    1,                              // applies to 7th range
    CPolicyServer::ENotSupported    // applies to last range
    };

// Elements
const CPolicyServer::TPolicyElement KPosLmElements[] =
    {
    {_INIT_SECURITY_POLICY_C1(ECapabilityReadUserData),
        CPolicyServer::EFailClient}, // Element index 0
    {_INIT_SECURITY_POLICY_C2(ECapabilityReadUserData,
        ECapabilityWriteUserData),
        CPolicyServer::EFailClient} // Element index 1
    };

// Capability policy
const CPolicyServer::TPolicy KPolicy =
    {
    0, //The index into KPosLmElements that is used to check
       // a connection attempt.
    KPosLmRangeCount,
    KPosLmRanges,
    KPosLmElementsIndex,
    KPosLmElements,
    };

class CPosLmServerDelayedShutdown;
class CPosLmServerSettingsHandler;
class CPosLmDbRegistry;
class HPosLmDatabaseInfo;
class CPosLmIndexManager;

/**
*  The main server side class of Landmarks Server.
*/
class CPosLmServer : public CPolicyServer, public MPosLmServerManagerObserver,
    public MPosLmBackupObserver
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPosLmServer* NewL();

        /**
        * Destructor.
        */
        ~CPosLmServer();

    public: // Functions from base classes

        /**
        * From CServer2
        *
        * @param[in] aVersion The server version.
        * @param[in] The connect message from the client.
        * @return A new session.
        */
        CSession2* NewSessionL(
            const TVersion& aVersion,
            const RMessage2& aMessage ) const;

        /**
        * From CPolicyServer
        *
        * @param[in] aMsg The message to check
        * @param[in,out] aAction A reference to the action to take if the security
        *   check fails.
        * @param[in,out] aMissing A reference to the list of security attributs
        *   missing from the checked process.
        * @return A value from TCustomResult.
        */
        TCustomResult CustomSecurityCheckL(
            const RMessage2& aMsg,
            TInt& aAction,
            TSecurityInfo& aMissing );

        /**
        * From CActive
        *
        * @param[in] aError the error code of the server leave
        * @return always KErrNone
        */
        TInt RunError( TInt aError );

        /**
        * From MPosLmServerManagerObserver
        *
        * Called when a manager has finished and has no running server
        * operations.
        */
        void HandleManagerFinished();

        /**
        * From MPosLmBackupObserver
        *
        * Called when a backup/restore operation starts.
        *
        */
        void HandleBackupEvent();

    public: // New functions

        /**
        * Returns a manager that corresponds to the client request function.
        *
        * @param[in] aFunction A client request function.
        * @return An operation manager.
        */
        CPosLmOperationManager* OperationManagerL( TInt aFunction );

        /**
        * Increment number of sessions by one.
        */
        void IncrementSessions();

        /**
        * Decreases the number of sessions by one. If number of sessions are
        * zero, the server will be shutdown (with a delay).
        */
        void DecrementSessions();

        /**
        * Starts the settings listener if it is not started.
        * @return KErrNone if successful, otherwise a system-wide error code.
        */
        TInt StartSettingsListenerIfNeeded();

        /**
        * Aquires a lock on the database with the specified URI.
        *
        * @param[in] aWriteLock ETrue to set a write lock or EFalse to set a read
        *   lock.
        * @param[in] aUri The URI of the database to lock.
        * @return ETrue if the db can be locked, otherwise EFalse.
        */
        TBool CanAcquireLock( TBool aWriteLock, const TDesC& aUri );

        /**
        * Removes messages associated with a specific session from the
        * operation managers.
        *
        * @param[in] aSession A pointer to a session.
        */
        void RemoveSessionMessages( TAny* aSession );

        /**
        * Forwards an operation event from a management database
        * to all sessions.
        *
        * @param[in] aEvent A database management event.
        * @param[in] aUri A database URI.
        */
        void ForwardEventToAllSessionsL( TPosLmDatabaseEvent aEvent, const TDesC& aUri );

        /**
        * Forwards an operation event from a specific database
        * to all sessions.
        *
        * @param[in] aEvent A database event.
        * @param[in] aUri A database URI from which the operation event originates.
        * @param[in] aCheckUri ETrue if the URI should be checked. EFalse otherwise.
        */
        void ForwardEventToAllSessionsL(
            TPosLmEvent aEvent,
            const TDesC& aUri,
            TBool aCheckUri );

        /**
        * Completes all outstanding event requests.
        * @param[in] aError An error code to complete request with.
        */
        void CompleteOutstandingEventRequests( TInt aError );

        /**
        * Returns a handle to an open database registry.
        * @param[in] aDrive The drive.
        * @param[in] aCreateDb ETrue to create a registry, EFalse otherwise.
        */
        CPosLmDbRegistry* GetRegistryL( TChar aDrive, TBool aCreateDb = EFalse );

        /** Returns a file session */
        RFs& FileSession();

        /** Leaves with KErrNotReady if backup is ongoing. */
        void LeaveIfBackupIsRunningL();
        
        /** Returns lm name index manager */
        CPosLmIndexManager& IndexManager();

    private:

        struct TRegistryInfo
            {
            CPosLmDbRegistry* iRegistry;
            TChar iDrive;
            };

        /**
        * C++ constructor.
        * @param[in] aPriority Priority at which to run the server active object.
        */
        CPosLmServer( TInt aPriority );

        void ConstructL();

        // By default, prohibit copy constructor
        CPosLmServer( const CPosLmServer& );
        // Prohibit assigment operator
        CPosLmServer& operator= ( const CPosLmServer& );

        void CloseAllRegistries();

    private:    // Data

        // Managers
        TFixedArray<CPosLmOperationManager*,
            CPosLmOperationManager::EPosMaxNumberOfManagers> iManagers;

        TInt                         iNumSessions;
        CPosLmServerDelayedShutdown* iShutdown;
        TTimeIntervalMicroSeconds32  iSrvShutdownDelay;
        CPosLmServerSettingsHandler* iServerSettingsHandler;
        RFs                          iFs;
        RArray<TRegistryInfo>        iRegistryArray;
        HBufC*                       iDbPath;
        CPosLmBackupListener*        iBackupListener;
        CPosLmIndexManager*        iIndexManager;
    };

#endif      // CPOSLMSERVER_H

