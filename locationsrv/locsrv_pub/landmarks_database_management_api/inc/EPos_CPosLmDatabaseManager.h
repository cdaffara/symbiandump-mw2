/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CPosLmDatabaseManager class
*
*/



#ifndef CPOSLMDATABASEMANAGER_H
#define CPOSLMDATABASEMANAGER_H

#include <e32base.h>
#include <badesca.h>
#include "EPos_TPosLmDatabaseEvent.h"
#include "EPos_TPosLmDatabaseSettings.h"
#include "EPos_HPosLmDatabaseInfo.h"


/**
*  This class is used to manage landmark databases.
*
*  @p CPosLmDatabaseManager contains functions for listing registering,
*  unregistering, creating, deleting, copying landmark databases, etc.
*  It also has functions for managing the default landmark database. The
*  client can listen to events related to database management.
*
*  A database is local if it resides in the phone or in some device which
*  is mapped to the phones file system. If a database is not local, it is
*  remote.
*
*  The client refers to a database by URI. The URI consists of a protocol
*  specifier and the database location: "protocol://location". If the
*  client does not specify a protocol, "file://" is assumed.
*
*  For local landmark databases, the URI consists of the drive and the
*  database file name, e.g. "c:landmarks.ldb". The path cannot be specified
*  by the client. The extension of the database file name must be "ldb".
*  If a path is included or if the file name extension is not .ldb",
*  the client receives the error code @p KErrArgument. For local landmark
*  databases, the client receives the error code @p KErrBadName if the
*  file name is invalid and @p KErrNotReady if the drive specified in the URI
*  does not exist.
*
*  If the client specifies a local database URI and does not specify the drive
*  letter, e.g. "landmarks.ldb", default database drive is assumed.
*
*  Local databases are created by calling @ref CreateDatabaseL. "Bookmarks"
*  to remote databases are created by calling @ref RegisterDatabaseL. After
*  this they are listed by the database manager.
*
*  If @ref CPosLmDatabaseManager is used, the client must call the global
*  function @ref ReleaseLandmarkResources before terminating, in order to
*  release all used landmark resources, otherwise the client may receive
*  an ALLOC panic.
*
*  @lib eposlmdbmanlib.lib
*  @since S60 3.0
*/
class CPosLmDatabaseManager : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * This function requires @p ReadUserData capability.
        */
        IMPORT_C static CPosLmDatabaseManager* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmDatabaseManager();

    public:

        /**
        * Retrieves the URI of the default landmark database.
        *
        * The default landmark database is the one which is opened when calling
        * @p CPosLandmarkDatabase::OpenL without any parameters.
        *
        * Only "file"-protocol databases can be set as default.
        *
        * This function requires @p ReadUserData capability.
        *
        * @return The URI of the default landmark database. The client takes
        *   ownership of the descriptor object.
        */
        virtual HBufC* DefaultDatabaseUriLC() = 0;

        /**
        * Sets a landmark database as default.
        *
        * This database is opened the next time
        * @p CPosLandmarkDatabase::OpenL() called without parameters.
        *
        * To move the current default database to another drive, first use
        * @ref CopyDatabaseL to copy the database to the new drive, then use
        * @ref SetDefaultDatabaseUriL to set the new database as default and
        * finally, use @ref DeleteDatabaseL if the old default database should
        * be deleted.
        *
        * Only "file"-protocol databases can be set as default.
        *
        * This function requires @p WriteDeviceData capability.
        *
        * @param[in] aDatabaseUri The URI of the database which should be set as
        *   default.
        * @leave KErrNotSupported The protocol specified something else
        *   than "file://" as protocol in the URI.
        * @leave KErrArgument The URI is incorrect.
        * @leave KErrNotFound The landmark database which should be set as default
        *   does not exist.
        */
        virtual void SetDefaultDatabaseUriL( const TDesC& aDatabaseUri ) = 0;

        /**
        * Listens for database management events.
        *
        * This function is asynchronous and it completes the request
        * status when an event occurs. At this time, the client can read
        * event information from the retrieved event object.
        *
        * Event listening can be canceled by calling
        * @ref CancelNotifyDatabaseEvent.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[out] aEvent Contains the event information when an event
        *   occurs (request completes).
        * @param[out] aStatus Is completed with @p KErrNone if an event
        *   occurs or an error code if some error is encountered.
        */
        virtual void NotifyDatabaseEvent(
            TPosLmDatabaseEvent& aEvent,
            TRequestStatus& aStatus
        ) = 0;

        /**
        * Cancels a call to @ref NotifyDatabaseEvent.
        *
        * @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code.
        */
        virtual TInt CancelNotifyDatabaseEvent() = 0;

        /**
        * Retrieves the database URI associated with the last event.
        *
        * Some events, e.g. @p EPosLmDbDatabaseRegistered and
        * @p EPosLmDbDatabaseUnregistered, refer to a specific database. The
        * URI of the database can be retrieved through this function.
        *
        * If the URI should be retrieved, this function must be called before
        * @ref NotifyDatabaseEvent is called again to listen for the next
        * event.
        *
        * This function requires @p ReadUserData capability.
        *
        * @return The database URI associated with the event. The client takes
        *   ownership of the descriptor object.
        * @leave KErrNotFound There is no database URI associated with the event
        *   or, the database URI has been discarded because the client has called
        *   @ref NotifyDatabaseEvent again.
        */
        virtual HBufC* DatabaseUriFromLastEventLC() = 0;

        /**
        * Lists the URIs to all landmark databases known by the Landmarks
        * system.
        *
        * The client can specify a protocol string as input to this function.
        * The function then only returns a list of the databases which
        * are accessed through this protocol.
        *
        * The client takes ownership of the returned array.
        *
        * If no databases are found, an empty array is returned.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aProtocol The protocol for which database URIs should be
        *   listed. If no protocol is specified, i.e. if an empty string is
        *   specified, all known database URIs are listed.
        * @return The list of database URIs.
        * @leave KErrNotSupported The protocol is not supported in the platform.
        */
        virtual CDesCArray* ListDatabasesLC( const TDesC& aProtocol = KNullDesC ) = 0;

        /**
        * Lists information about each landmark database known by the Landmarks
        * system.
        *
        * The client can specify a protocol string as input to this function.
        * The function then only returns information about the databases
        * which are accessed through this protocol.
        *
        * The client specifies an array which is populated by this function.
        * The client takes ownership of all information objects in the array.
        *
        * If no databases are found, an empty array is returned.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[out] aDatabaseInfoArray On return, contains information about
        *   the landmark databases. Any objects which are in the array when
        *   it is passed to this function are not removed.
        * @param[in] aProtocol The protocol for which database URIs should be
        *   listed. If no protocol is specified, i.e. if an empty string is
        *   specified, all known database URIs are listed.
        * @leave KErrNotSupported The protocol is not supported in the platform.
        */
        virtual void ListDatabasesL(
            RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
            const TDesC& aProtocol = KNullDesC
        ) = 0;

        /**
        * Registers a landmark database.
        *
        * The landmark database is then returned when listing landmark
        * databases.
        *
        * For local landmark databases, this function leaves with error code
        * @p KErrNotSupported. To add a local database, the client must call
        * @ref CreateDatabaseL.
        *
        * The client supplies an information object containing the URI of the
        * database to register. The information object can also contain
        * database settings, e.g. a display name for the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in,out] aDatabaseInfo Information about the landmark database to
        *   register. The information object will be updated with
        *   media type and drive information.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported in the platform or the protocol does not allow
        *   registering landmark databases.
        * @leave KErrArgument The URI is incorrect.
        * @leave KErrAlreadyExists The database already exists in the registry.
        */
        virtual void RegisterDatabaseL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /**
        * Unregisters a landmark database.
        *
        * After this, the landmark database is not returned when listing
        * landmark databases.
        *
        * For local landmark databases, this function leaves with error code
        * @p KErrNotSupported. To remove a local database, the client must call
        * @ref DeleteDatabaseL.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aDatabaseUri The URI of the database to register.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported in the platform or the protocol does not allow
        *   unregistering landmark databases.
        * @leave KErrArgument The URI is incorrect.
        */
        virtual void UnregisterDatabaseL( const TDesC& aDatabaseUri ) = 0;

        /**
        * Unregisters all landmark database which are accessed through a
        * certain protocol.
        *
        * After this, the landmark databases are not be returned when listing
        * landmark databases.
        *
        * For local landmark databases, this function leaves with error code
        * @p KErrNotSupported. To remove a local database, the client must call
        * @ref DeleteDatabaseL.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aProtocol The protocol to unregister all databases for.
        *
        * @leave KErrNotSupported The protocol is not supported in the platform
        *   or the protocol does not allow unregistering landmark databases.
        * @leave KErrArgument The protocol is an empty string.
        */
        virtual void UnregisterAllDatabasesL( const TDesC& aProtocol ) = 0;

        /**
        * Modifies the settings for a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aDatabaseUri The URI of the database to modify settings for.
        * @param[in] aDatabaseSettings The new settings for the database.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported in the platform.
        * @leave KErrNotFound The specified database is not found.
        * @leave KErrArgument The URI is incorrect.
        */
        virtual void ModifyDatabaseSettingsL(
            const TDesC& aDatabaseUri,
            const TPosLmDatabaseSettings& aDatabaseSettings
        ) = 0;

        /**
        * Retrieve information about a landmark database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in,out] aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported in the platform.
        * @leave KErrNotFound The specified database is not found.
        * @leave KErrArgument The URI is incorrect.
        */
        virtual void GetDatabaseInfoL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /**
        * Checks if the specified landmark database exists.
        *
        * The database to check is specified by passing a URI to this function.
        * URI construction is described in the class description for
        * @ref CPosLmDatabaseManager. The URI must point to a database which is
        * handled by this database manager, i.e. not a remote location.
        *
        * This function requires @p ReadUserData capability. If the database is
        * remote, @p NetworkServices capability is also needed.
        *
        * @param[in] aDatabaseUri The URI of the database which should be checked
        *   for existence.
        * @return @p ETrue if the database exists, otherwise @p EFalse.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported.
        * @leave KErrArgument The URI is incorrect.
        */
        virtual TBool DatabaseExistsL( const TDesC&  aDatabaseUri ) = 0;

        /**
        * Creates a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @param aDatabaseInfo Information about the landmark database to
        *   create. The information object will be updated with
        *   media type and drive information.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported.
        * @leave KErrArgument The URI is incorrect.
        * @leave KErrAlreadyExists There is already a database at this URI.
        */
        virtual void CreateDatabaseL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /**
        * Deletes a landmark database.
        *
        * The database to delete is specified by passing a URI to this
        * function. URI construction is described in the class description for
        * @ref CPosLmDatabaseManager.
        *
        * If the specified database does not exist, the call is ignored.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @param aDatabaseUri The URI of the database to delete.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported.
        * @leave KErrArgument The URI is incorrect.
        * @leave KErrInUse The database is in use by some client.
        * @leave KErrAccessDenied The database is read-only.
        */
        virtual void DeleteDatabaseL( const TDesC& aDatabaseUri ) = 0;

        /**
        * Copies a landmark database to a new location.
        *
        * Database locations are specified as URIs. URI construction is
        * described in the class description for @ref CPosLmDatabaseManager.
        * Target URI protocol must be the same as source URI protocol.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the databases are remote, @p NetworkServices
        * capability is also needed.
        *
        * @param[in] aSourceUri The URI of the database to copy.
        * @param[in] aTargetUri The URI of the new database location.
        *
        * @leave KErrNotSupported The protocol specified in the a URI is not
        *   supported.
        * @leave KErrArgument A URI is incorrect.
        * @leave KErrAlreadyExists There is already a database at the target URI.
        * @leave KErrInUse There is a write-lock on the database, e.g. some client
        *   is currently modifying the database.
        * @leave KErrNotFound There is no database at the source URI.
        */
        virtual void CopyDatabaseL(
            const TDesC& aSourceUri,
            const TDesC& aTargetUri
        ) = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLmDatabaseManager();

    private:

        // Prohibit copy constructor
        CPosLmDatabaseManager( const CPosLmDatabaseManager& );
        // Prohibit assigment operator
        CPosLmDatabaseManager& operator= ( const CPosLmDatabaseManager& );

    private:

        // Implementation Uid
        TUid iDtorIdKey;
    };

#endif      // CPOSLMDATABASEMANAGER_H


