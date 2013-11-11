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
* Description: Provides static functions which handles database creation
*								and deletion.
*
*
*/


#ifndef POSLMDATABASEMANAGER_H
#define POSLMDATABASEMANAGER_H

#include <e32base.h>
#include <d32dbms.h>

/**
*  This class contains static functions which handles database creation
*  and deletion.
*/
class PosLmDatabaseManager
    {
    public:
        enum TLmDbChangeType
            {
            ELmDbAdded = 0,
            ELmDbRemoved
            };

    public: // New functions

        /**
        * Deletes a database.
        * @param[in] aDatabaseUri A database URI.
        */
        static void DeleteDatabaseL( const TDesC& aUri );

        /**
        * Creates a landmark database at the specified location.
        *
        * @param[in] aUri A database URI.
        * @param[in] aReplaceDatabase If ETrue, the database will be replaced
        *   if it exists. If EFalse, the database will be created.
        */
        static void CreateDatabaseL(
            const TDesC& aUri,
            TBool aDefaultDatabase = EFalse,
            TBool aReplaceDatabase = EFalse );

        /**
        * Copies a database.
        *
        * @param[in] aSourceUri URI for the source database that shall be copied.
        * @param[in] aTargetUri URI for the new database copy.
        */
        static void CopyDatabaseL(
            const TDesC& aSourceUri,
            const TDesC& aTargetUri );

        /** Creates table with info of server-maintained indexes */
        static void CreateIndexTableL( RDbNamedDatabase& aDb );
        
    private: // New functions

        /*
        * Creates the tables in the landmark database.
        * @param[in] aDb Opened database that will contain the created tables.
        */
        static void CreateTablesL( RDbNamedDatabase& aDb );

        /*
        * Creates the landmark table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateLandmarkTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the landmark field table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateLandmarkFieldTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the category table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateCategoryTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the landmark-category table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateLmCategoryTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the icon table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateIconTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the settings table.
        * @param[in] aDb Opened database that will contain the created table.
        */
        static void CreateSettingsTableL( RDbNamedDatabase& aDb );

        /*
        * Creates the indexes in the landmark database.
        * @param[in] aDb Opened database that will contain the created indexes.
        */
        static void CreateIndexesL( RDbNamedDatabase& aDb );

        /*
        * Creates an index in the landmark database.
        *
        * @param[in] aDb Opened database that will contain the created indexes.
        * @param[in] aIndex An index name.
        * @param[in] aTable An table name.
        * @param[in] aColumn An column name.
        * @param[in] aUnique Tells whether this index should be unique or not.
        */
        static void CreateIndexL(
            RDbNamedDatabase& aDb,
            const TDesC& aIndex,
            const TDesC& aTable,
            const TDesC& aColumn,
            TBool aUnique );

        /*
        * Sets a counter to one and not zero for the specified table in the
        * specified database.
        *
        * @param[in] aDb Opened database to change.
        * @param[in] aTable The table to modify.
        */
        static void SetCounterToOneL(
            RDbNamedDatabase& aDb,
            const TDesC& aTable );

    };

#endif      // POSLMDATABASEMANAGER_H

// End of File

