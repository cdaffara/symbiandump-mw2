/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class for SQL database creation*
*/

#ifndef __MDSSQLDBMAINTENANCE_H__
#define __MDSSQLDBMAINTENANCE_H__

#include <e32base.h>

// FORWARD DECLARATIONS
class CMdSSchema;


_LIT( KMdsSchemaVersionName, "SchemaVersion" );
_LIT( KMdsDBVersionName, "DBVersion" );

// CLASS DECLARATION
/**
* Class for validating and creating tables in database.
*/
class CMdSSqlDbMaintenance : public CBase
    {
    public: // Constructors and destructor
        virtual ~CMdSSqlDbMaintenance();
        
        static CMdSSqlDbMaintenance* NewL();
        static CMdSSqlDbMaintenance* NewLC();

    private: // Private constructors

        CMdSSqlDbMaintenance();
        void ConstructL( );

    public: // Personal methods

        /**
        * Validates the existence of database
        * @return ETrue if the database exists, EFalse if needs to be created.
        */
        TBool ValidateL();

        /**
        * Drops and re-creates tables
        */
        void CreateDatabaseL();

        /**
        * Does quick check to the existing database to detect obvious corruption
        */
        TBool CheckForCorruptionL();
    };

#endif // __MDSSQLDBMAINTENANCE_H__
