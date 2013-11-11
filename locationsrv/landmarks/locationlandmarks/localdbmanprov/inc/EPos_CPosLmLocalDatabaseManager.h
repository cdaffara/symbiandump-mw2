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
* Description: This class is used to manage local landmark databases.
*
*
*/


#ifndef CPOSLMLOCALDATABASEMANAGER_H
#define CPOSLMLOCALDATABASEMANAGER_H

//  INCLUDES
#include <EPos_CPosLmDatabaseManagerPluginBase.h>
#include <EPos_HPosLmDatabaseInfo.h>
#include "EPos_RPosLmLocalDbManSubsession.h"

// DATA TYPES
struct TMediaInfo
    {
    TChar iDrive;
    TMediaType iMediaType;
    };

// CLASS DECLARATION

/**
*  This class is used to manage local landmark databases.
*
*  For local landmark databases, the URI consists of the drive and the
*  database file name, e.g. "c:landmarks.ldb". The path cannot be specified
*  by the client. The extension of the database file name must be "ldb".
*  If the a path is included or if the file name extension is not .ldb",
*  the client receives the error code @p KErrArgument. For local landmark
*  databases, the client receives the error code @p KErrBadName if the
*  file name is invalid and @p KErrNotReady if the drive specified in the URI
*  does not exist.
*
*  If the client specifies a local database URI and does not specify the drive
*  letter, e.g. "landmarks.ldb", c: drive is assumed.
*/
class CPosLmLocalDatabaseManager : public CPosLmDatabaseManagerPluginBase
    {
     public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aConstructionParams A pointer to a landmarks server
        * session handle,
        * @returns A new instance of this class.
        */
        static CPosLmLocalDatabaseManager* NewL(TAny* aConstructionParams);

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalDatabaseManager();

    public: // Functions from base classes


       /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database to check media for.
        * @return Type of storage media.
        */
        virtual TMediaType DatabaseMedia(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database to check drive letter
        *   for.
        * @return The drive letter, or 0 if drive letter is not applicable.
        */
        virtual TChar DatabaseDrive(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @return Pointer to the protocol descriptor, e.g. "file"
        */
        virtual TPtrC Protocol();

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @return The list of database URIs.
        */
        virtual CDesCArray* ListDatabasesLC();

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseInfoArray On return, contains information about
        *   the landmark databases.
        */
        virtual void ListDatabasesL(
        /* OUT */       RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseInfo Information about the landmark database to
        *   register.
        * @par Leave codes:
        * @p KErrNotSupported Always returned.
        */
        virtual void RegisterDatabaseL(
        /* IN/OUT */       HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database to register.
        * @par Leave codes:
        * @p KErrNotSupported Always returned.
        */
        virtual void UnregisterDatabaseL(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @par Leave codes:
        * @p KErrNotSupported Always returned.
        */
        virtual void UnregisterAllDatabasesL();

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database to modify settings for.
        * @param aDatabaseSettings The new settings for the database.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.\n
        * @p KErrNotFound The specified database is not found.
        */
        virtual void ModifyDatabaseSettingsL(
        /* IN  */       const TDesC& aDatabaseUri,
        /* IN  */       const TPosLmDatabaseSettings& aDatabaseSettings
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        * @par Leave codes:
        * @p KErrNotSupported The protocol specified in the URI is not
        *   supported by this plug-in.\n
        * @p KErrNotFound The specified database is not found.
        */
        virtual void GetDatabaseInfoL(
        /* IN/OUT */    HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database which should be checked
        *   for existence.
        * @return @p ETrue if the database exists, otherwise @p EFalse.
        * @par Leave codes:
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        */
        virtual TBool DatabaseExistsL(
        /* IN  */       const TDesC&  aDatabaseUri
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseInfo Contains the name and drive of the new database.
        * @par Leave codes:
        * @p KErrNotSupported The operation is not supported by the plug-in.\n
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        * @p KErrAlreadyExists There is already a database at this URI.
        */
        virtual void CreateDatabaseL(
        /* IN/OUT */       HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
        * @param aDatabaseUri The URI of the database to delete.
        * @par Leave codes:
        * @p KErrNotSupported The operation is not supported by the plug-in.\n
        * @p KErrArgument The URI is incorrect or the protocol specified in the
        *   URI is not supported by this plug-in.\n
        * @p KErrInUse The database is in use by some client.\n
        * @p KErrAccessDenied The database is read-only.
        */
        virtual void DeleteDatabaseL(
        /* IN  */       const TDesC&  aDatabaseUri
        );

        /**
        * From CPosLocalDatabaseManagerPluginBase.
        *
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
        virtual void CopyDatabaseL(
        /* IN  */       const TDesC&  aSourceUri,
        /* IN  */       const TDesC&  aTargetUri
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalDatabaseManager();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        */
        void ConstructL();

        /**
        * Synchronizes the database registry, i.e. removes all databases
        * in the registry that cannot be found on disk.
        */
        void SynchronizeRegistryL();

        /**
        * Initializes the internal lookup table for media types.
        */
        void CreateDriveInfoListL();

    private:

        // By default, prohibit copy constructor
        CPosLmLocalDatabaseManager(
            const CPosLmLocalDatabaseManager& );
        // Prohibit assigment operator
        CPosLmLocalDatabaseManager& operator= (
            const CPosLmLocalDatabaseManager& );

    private:    // Data

        RPosLmLocalDbManSubsession  iSubsession;
        RArray<TMediaInfo>          iMediaInfoList;
    };

#endif      // CPOSLMLOCALDATABASEMANAGER_H

// End of File
