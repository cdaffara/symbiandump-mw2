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
* Description:Registry for storing landmark database information.
*
*/


#ifndef CPOSLMDBREGISTRY_H
#define CPOSLMDBREGISTRY_H

// INCLUDES
#include <e32base.h>
#include <d32dbms.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CPosLmDatabaseIterator;
class TPosLmDatabaseSettings;

// CLASS DECLARATION

/**
* Registry for storing landmark database information.
*
*  @version $Revision: 1.4 $, $Date: 2005/03/21 11:35:49 $
*/
class CPosLmDbRegistry : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPosLmDbRegistry* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmDbRegistry();

    public:  // New functions

        /**
        * Creates a database registry.
        *
        * @param aFileSession A file server session.
        * @param aFileName The name of the database registry to create.
        */
        IMPORT_C void CreateL(
        /* IN  */       RFs& aFileSession,
        /* IN  */       const TDesC& aFileName
        );

        /**
        * Opens a database registry.
        *
        * @param aFileSession A file server session.
        * @param aFileName The name of the database registry to open.
        * @return KErrNone if successful, otherwise a system wide
        * error code.
        */
        IMPORT_C TInt Open(
        /* IN  */       RFs& aFileSession,
        /* IN  */       const TDesC& aFileName
        );

        /**
        * Registers a database.
        *
        * If the URI exists already, this function leaves with
        * @p KErrAlreadyExists.
        *
        * @param aDatabaseUri The database URI to add to the database registry.
        * @param aSettings The database settings to add to the database
        *   registry.
        */
        IMPORT_C void RegisterDatabaseL(
        /* IN  */       const TDesC& aDatabaseUri,
        /* IN  */       const TPosLmDatabaseSettings& aSettings
        );

        /**
        * Unregisters a database in the database registry.
        *
        * @param aDatabaseUri The URI of the database to unregister.
        */
        IMPORT_C void UnregisterDatabaseL(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * Unregisters all databases in the database registry
        * for a given protocol.
        *
        * @param aProtocol The protocol to unregister all databases for.
        */
        IMPORT_C void UnregisterAllL(
        /* IN  */       const TDesC& aProtocol
        );

        /**
        * Retrieves database settings for the database URI supplied by client.
        * This function leaves with KErrNotFound if the database does not exist.
        *
        * @param aDatabaseUri URI of the database to read settings for.
        * @param aSettings Contains the database settings on return.
        */
        IMPORT_C void ReadDatabaseSettingsL(
        /* IN   */      const TDesC& aDatabaseUri,
        /* OUT  */      TPosLmDatabaseSettings& aSettings
        );

        /**
        * Modifies the settings for a landmark database in the registry.
        * This function leaves with KErrNotFound if the database does not exist.
        *
        * @param aDatabaseUri URI of the database to modify settings for.
        * @param aSettings The new database settings.
        */
        IMPORT_C void ModifyDatabaseSettingsL(
        /* IN   */      const TDesC& aDatabaseUri,
        /* IN   */      const TPosLmDatabaseSettings& aSettings
        );

        /**
        * Lists all databases with a certain protocol in the registry.
        * The client takes ownership of the iterator object.
        *
        * @param aProtocol A protocol.
        * @return An iterator containing the databases.
        */
        IMPORT_C CPosLmDatabaseIterator* ListDatabasesL(
        /* IN  */       const TDesC& aProtocol
        );

        /**
        * Compacts the database synchronously.
        *
        * @return KErrNone if successful,
        * otherwise any of the system-wide error codes.
        */
        IMPORT_C TInt Compact();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmDbRegistry();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPosLmDbRegistry( const CPosLmDbRegistry& );
        // Prohibit assigment operator
        CPosLmDbRegistry& operator= ( const CPosLmDbRegistry& );

        TPtrC ExtractProtocol(
        /* IN   */      const TDesC& aDatabaseUri
        );

        TPtrC ExtractFileName(
        /* IN   */      const TDesC& aDatabaseUri
        );

        TBool FindEntryL(
        /* IN/OUT   */      RDbView& aView,
        /* IN       */      const TDesC& aDatabaseUri
        );

        TBool FindLongEntryL(
        /* IN/OUT   */      RDbView& aView,
        /* IN       */      const TDesC& aDatabaseUri
        );

        void RecoverIfNeededL();

    private:    // Data

        TInt iCompactLevel;
        RDbNamedDatabase iDb;

    };

#endif      // CPOSLMDBREGISTRY_H

// End of File
