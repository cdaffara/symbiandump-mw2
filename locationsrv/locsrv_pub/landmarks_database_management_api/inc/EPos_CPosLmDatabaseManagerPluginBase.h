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
* Description:   CPosLmDatabaseManagerPluginBase class
*
*/



#ifndef CPOSLMDATABASEMANAGERPLUGINBASE_H
#define CPOSLMDATABASEMANAGERPLUGINBASE_H

#include <e32base.h>
#include <badesca.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include <EPos_TPosLmDatabaseSettings.h>
#include <EPos_HPosLmDatabaseInfo.h>

class CPosLmDbManPluginBaseExtension;
class RPosLandmarkServer;

/**
*  @internal
*
*  <b>This class is not used by client applications. 
*  It is reserved for future extensions.</b>
*/
/*
*  This class is the base class which has to be subclassed in order to write a
*  plug-in for database management for a specific protocol. It is reserved for
*  for future extensions.
*
*  @p CPosLmDatabaseManagerPluginBase contains functions for listing,
*  registering, unregistering, creating, deleting, copying landmark databases,
*  etc.
*
*  @lib eposlmdbmanlib.lib
*  @since S60 3.0
*/
class CPosLmDatabaseManagerPluginBase : public CBase
    {
    public:

        /*
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmDatabaseManagerPluginBase();

    public:

        /*
        * Retrieves the type of media where a landmark database resides.
        *
        * This function does not validate the URI. If the URI is invalid, this
        * function will just return @p EMediaUnknown.
        *
        * @param[in] aDatabaseUri The URI of the database to check media for.
        * @return Type of storage media.
        */
        virtual TMediaType DatabaseMedia( const TDesC& aDatabaseUri ) = 0;

        /*
        * Retrieves the drive letter for the drive where a landmark database
        * resides.
        *
        * If the landmark database is remote or otherwise the drive letter is
        * not applicable, 0 is returned.
        *
        * This function does not validate the URI. If the URI is invalid, this
        * function will just return 0.
        *
        * @param[in] aDatabaseUri The URI of the database to check drive letter
        *   for.
        * @return The drive letter, or 0 if drive letter is not applicable.
        */
        virtual TChar DatabaseDrive( const TDesC& aDatabaseUri ) = 0;

        /*
        * Returns the protocol which the plug-in handles.
        *
        * The returned descriptor pointer is valid until the plug-in is
        * unloaded.
        *
        * @return Pointer to the protocol descriptor, e.g. "file"
        */
        virtual TPtrC Protocol() = 0;

        /*
        * Lists the URIs to all landmark databases handled by this
        * plug-in.
        *
        * The client takes ownership of the returned array.
        *
        * If no databases are found, an empty array is returned.
        *
        * This function requires @p ReadUserData capability.
        *
        * @return The list of database URIs.
        */
        virtual CDesCArray* ListDatabasesLC() = 0;

        /*
        * Lists information about each landmark database handled by this
        * plug-in.
        *
        * The client specifies an array which is populated by this function.
        * The client takes ownership of all information objects in the array.
        *
        * If no databases are found, the input array is not modified.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[out] aDatabaseInfoArray On return, contains information about
        *   the landmark databases.
        */
        virtual void ListDatabasesL(
            RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray
        ) = 0;

        /*
        * Registers a landmark database.
        *
        * The landmark database is then returned when listing landmark
        * databases.
        *
        * Some protocols like "file" does not allow registering of databases
        * and will leave with error code @p KErrNotSupported. To add a
        * database of such protocol, the client must call @ref CreateDatabaseL.
        *
        * The client supplies an information object containing the URI of the
        * database to register. The information object can also contain
        * database settings, e.g. a display name for the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in,out] aDatabaseInfo Information about the landmark database to
        *   register.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in or the protocol does not allow
        *   registering landmark databases.
        * @leave KErrArgument The URI is incorrect.
        * @leave KErrAlreadyExists The database already exists in the registry.
        */
        virtual void RegisterDatabaseL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /*
        * Unregisters a landmark database.
        *
        * After this, the landmark database will not be returned when listing
        * landmark databases.
        *
        * Some protocols like "file" does not allow unregistering of databases
        * and will leave with error code @p KErrNotSupported. To remove a
        * database of such protocol, the client must call @ref DeleteDatabaseL.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aDatabaseUri The URI of the database to register.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in or the protocol does not allow
        *   unregistering landmark databases.
        * @leave KErrArgument The URI is incorrect.
        */
        virtual void UnregisterDatabaseL( const TDesC& aDatabaseUri ) = 0;

        /*
        * Unregisters all landmark database which are accessed through this
        * plug-in.
        *
        * After this, the landmark databases will not be returned when listing
        * landmark databases.
        *
        * Some protocols like "file" does not allow unregistering of databases
        * and will leave with error code @p KErrNotSupported. To remove a
        * "file"-protocol database, the client must call @ref DeleteDatabaseL.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @leave KErrNotSupported This plug-in does not allow unregistering
        *   landmark databases.
        */
        virtual void UnregisterAllDatabasesL() = 0;

        /*
        * Modifies the settings for a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @param[in] aDatabaseUri The URI of the database to modify settings for.
        * @param[in] aDatabaseSettings The new settings for the database.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.
        * @leave KErrNotFound The specified database is not found.
        */
        virtual void ModifyDatabaseSettingsL(
            const TDesC& aDatabaseUri,
            const TPosLmDatabaseSettings& aDatabaseSettings
        ) = 0;

        /*
        * Retrieve information about a landmark database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in,out] aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        *
        * @leave KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.
        * @leave KErrNotFound The specified database is not found.
        */
        virtual void GetDatabaseInfoL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /*
        * Checks if the specified landmark database exists.
        *
        * The database to check is specified by passing a URI to this function.
        * URI construction is described in the class description for
        * @ref CPosLmDatabaseManager.
        *
        * This function requires @p ReadUserData capability. If the database is
        * remote, @p NetworkServices capability is also needed.
        *
        * @param[in] aDatabaseUri The URI of the database which should be checked
        *   for existence.
        * @return @p ETrue if the database exists, otherwise @p EFalse.
        * 
        * @leave KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.
        */
        virtual TBool DatabaseExistsL( const TDesC&  aDatabaseUri ) = 0;

        /*
        * Creates a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @param[in,out] aDatabaseInfo Information about the landmark database to
        *   create.
        *
        * @leave KErrNotSupported The protocol is not supported or the create
        *   operation is not supported by the protocol plug-in.
        * @leave KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.
        * @leave KErrAlreadyExists There is already a database at this URI.
        */
        virtual void CreateDatabaseL( HPosLmDatabaseInfo& aDatabaseInfo ) = 0;

        /*
        * Deletes a landmark database.
        *
        * The database to delete is specified by passing a URI to this
        * function. URI construction is described in the class description for
        * @ref CPosLmDatabaseManager. The URI must specify the protocol which
        * is handled by this database manager plug-in.
        *
        * If the specified database does not exist, the call is ignored.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @param[in] aDatabaseUri The URI of the database to delete.
        *
        * @leave KErrNotSupported The protocol is not supported or the delete
        *   operation is not supported by the plug-in.
        * @leave KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.
        * @leave KErrInUse The database is in use by some client.
        * @leave KErrAccessDenied The database is read-only.
        */
        virtual void DeleteDatabaseL( const TDesC& aDatabaseUri ) = 0;

        /*
        * Copies a landmark database to a new location.
        *
        * Database locations are specified as URIs. URI construction is
        * described in the class description for @ref CPosLmDatabaseManager.
        * The target and source URIs must specify the protocol which is handled
        * by this database manager plug-in.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @param[in] aSourceUri The URI of the database to copy.
        * @param[in] aTargetUri The URI of the new database location.
        * 
        * @leave KErrNotSupported The operation is not supported by the plug-in.
        * @leave KErrArgument A URI is incorrect or the protocol specified in a
        *   URI is not supported by this plug-in.
        * @leave KErrInUse There is a write-lock on the database, e.g. some client
        *   is currently modifying the database.
        * @leave KErrNotFound There is no database at the source URI.
        */
        virtual void CopyDatabaseL(
            const TDesC& aSourceUri,
            const TDesC& aTargetUri
        ) = 0;

    protected:

        /*
        * C++ constructor.
        */
        IMPORT_C CPosLmDatabaseManagerPluginBase();

        /*
        * Creates the internals of the manager.
        *
        * This function must be called first in the manager's @b ConstructL()
        * method.
        *
        * @param[in] aConstructionParameters The construction parameters supplied
        *   in the factory call.
        */
        IMPORT_C void BaseConstructL( TAny* aConstructionParameters );

    protected:

        /*
        * Returns a reference to an open landmark server session.
        *
        * @return The server session.
        */
        IMPORT_C RPosLandmarkServer& Session() const;

    private:

        // Prohibit copy constructor
        CPosLmDatabaseManagerPluginBase(
            const CPosLmDatabaseManagerPluginBase& );
        // Prohibit assigment operator
        CPosLmDatabaseManagerPluginBase& operator=(
            const CPosLmDatabaseManagerPluginBase& );

    private:

        CPosLmDbManPluginBaseExtension* iExtension;
    };

#endif      // CPOSLMDATABASEMANAGERPLUGINBASE_H


