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
* Description: 
*   ?description_line
*
*/


#ifndef CDBMANTESTPLUGINBASE_H
#define CDBMANTESTPLUGINBASE_H

//  INCLUDES
#include <EPos_CPosLmDatabaseManagerPluginBase.h>

// CLASS DECLARATION

class CDbManTestPluginBase : public CPosLmDatabaseManagerPluginBase
    {
    public:
        
        /**
        * Destructor.
        */
        ~CDbManTestPluginBase();
        
        
    static CDbManTestPluginBase* NewL(TAny* aConstructionParams);
    

    public: // Functions from base classes.
        
        /**
        * Lists the URIs to all landmark databases handled by this
        * plug-in.
        *
        * The client takes ownership of the returned array.
        *
        * If no databases are found, an empty array is returned.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since Series 60 3.0
        * @return The list of database URIs.
        */
        CDesCArray* ListDatabasesLC();

        /**
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
        * @since Series 60 3.0
        * @param aDatabaseInfoArray On return, contains information about
        *   the landmark databases.
        */
        void ListDatabasesL(
        /* OUT */       RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray
        );

        /**
        * Registers a landmark database.
        *
        * The landmark database is then returned when listing landmark
        * databases.
        *
        * Some protocols like "file" does not allow registering of databases
        * and will leave with error code @p KErrNotSupported. To add a
        * "file"-protocol database, the client must call @ref CreateDatabaseL.
        *
        * The client supplies an information object containing the URI of the
        * database to register. The information object can also contain
        * database settings, e.g. a display name for the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since Series 60 3.0
        * @param aDatabaseInfo Information about the landmark database to
        *   register.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in or the protocol does not allow
        *   registering landmark databases.\n
        * @p KErrArgument The URI is incorrect.\n
        * @p KErrAlreadyExists The database already exists in the registry.
        */
        void RegisterDatabaseL(
        /* IN/OUT */       HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Unregisters a landmark database.
        *
        * After this, the landmark database will not be returned when listing
        * landmark databases.
        *
        * Some protocols like "file" does not allow unregistering of databases
        * and will leave with error code @p KErrNotSupported. To remove a
        * "file"-protocol database, the client must call @ref DeleteDatabaseL.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the database to register.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in or the protocol does not allow
        *   unregistering landmark databases.\n
        * @p KErrArgument The URI is incorrect.
        */
        void UnregisterDatabaseL(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * Unregisters all landmark database which are accessed through this
        * plug-in
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
        * @since Series 60 3.0
        * @par Leave codes:
        * @p KErrNotSupported This plug-in does not allow unregistering
        *   landmark databases.
        */
        void UnregisterAllDatabasesL();

        /**
        * Modifies the settings for a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the database to modify settings for.
        * @param aDatabaseSettings The new settings for the database.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.\n
        * @p KErrNotFound The specified database is not found.
        */
        void ModifyDatabaseSettingsL(
        /* IN  */       const TDesC& aDatabaseUri,
        /* IN  */       const TPosLmDatabaseSettings& aDatabaseSettings
        );

        /**
        * Retrieve information about a landmark database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since Series 60 3.0
        * @param aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.\n
        * @p KErrNotFound The specified database is not found.
        */
        void GetDatabaseInfoL(
        /* IN/OUT */    HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Checks if the specified landmark database exists.
        *
        * The database to check is specified by passing a URI to this function.
        * URI construction is described in the class description for
        * @ref CPosLmDatabaseManager.
        *
        * This function requires @p ReadUserData capability. If the database is
        * remote, @p NetworkServices capability is also needed.
        *
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the database which should be checked
        *   for existence.
        * @return @p ETrue if the database exists, otherwise @p EFalse.
        * @par Leave codes:
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        */
        TBool DatabaseExistsL( 
        /* IN  */       const TDesC&  aDatabaseUri
        );

        /**
        * Creates a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities. If the database is remote, @p NetworkServices
        * capability is also needed.
        *
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the new database.
        * @par Leave codes:
        * @p KErrNotSupported The operation is not supported by the plug-in.\n
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        * @p KErrAlreadyExists There is already a database at this URI.
        */
        void CreateDatabaseL( 
        /* IN/OUT */       HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
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
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the database to delete.
        * @par Leave codes:
        * @p KErrNotSupported The operation is not supported by the plug-in.\n
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        * @p KErrInUse The database is in use by some client.\n
        * @p KErrAccessDenied The database is read-only.
        */
        void DeleteDatabaseL(
        /* IN  */       const TDesC&  aDatabaseUri
        );

        /**
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
        * @since Series 60 3.0
        * @param aSourceUri The URI of the database to copy.
        * @param aTargetUri The URI of the new database location.
        * @par Leave codes:
        * @p KErrNotSupported The operation is not supported by the plug-in.\n
        * @p KErrArgument A URI is incorrect or the protocol specified in a
        *   URI is not supported by this plug-in.\n
        * @p KErrInUse There is a write-lock on the database, e.g. some client
        *   is currently modifying the database.\n
        * @p KErrNotFound There is no database at the source URI.\n
        */
        void CopyDatabaseL(
        /* IN  */       const TDesC&  aSourceUri,
        /* IN  */       const TDesC&  aTargetUri
        );


        /**
        * Retrieves the type of media where a landmark database resides.
        *
        * This function does not validate the URI. If the URI is invalid, this
        * function will just return @p EMediaUnknown.
        *
        * @since Series 60 3.0
        * @param aDatabaseUri The URI of the database to check media for.
        * @return Type of storage media.
        */
        TMediaType DatabaseMedia(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * Retrieves the drive letter for the drive where a landmark database
        * resides.
        *
        * If the landmark database is remote or otherwise the drive letter is
        * not applicable, 0 is returned.
        *
        * This function does not validate the URI. If the URI is invalid, this
        * function will just return 0.
        *
        * @param aDatabaseUri The URI of the database to check drive letter
        *   for.
        * @return The drive letter, or 0 if drive letter is not applicable.
        */
        TChar DatabaseDrive(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * From CPosLmDatabaseManagerPluginBase
        * 
        * @since Series 60 3.0
        * @return Pointer to the protocol descriptor, e.g. "file"
        */
        TPtrC Protocol();
        
    protected:
    
        CDbManTestPluginBase();
        
    private:
    
        TInt FindUri(const TDesC& aUri);

        // By default, prohibit copy constructor
        CDbManTestPluginBase(
            const CDbManTestPluginBase& );
        // Prohibit assigment operator
        CDbManTestPluginBase& operator=(
            const CDbManTestPluginBase& );

    private:    // Data

        RPointerArray<HPosLmDatabaseInfo> iDatabases;
        
    };

#endif      // CDBMANTESTPLUGINBASE_H
            
// End of File
