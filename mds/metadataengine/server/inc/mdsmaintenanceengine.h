/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class for database import/export and maintenance*
*/

#ifndef __MDSMAINTENANCEENGINE_H__
#define __MDSMAINTENANCEENGINE_H__

#include <e32base.h>

#include "mdccommon.h"

// DEFINITIONS
_LIT( KSchemaImportFile, "C:\\Private\\200009F3\\schema.mde" );
_LIT( KSchemaRomImportFile, "Z:\\Private\\200009F3\\schema.mde" );

// forward declarations
class CMdsSchema;
class CMdSManipulationEngine;
class CMdCSerializationBuffer;
class CMdSSqlDbMaintenance;

/**
* Abstract class for validating and creating tables.
*/
class CMdSMaintenanceEngine : public CBase
    {
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        */
        static CMdSMaintenanceEngine* NewL();

        /**
        * NewL.
        * Two-phased constructor.
        */
        static CMdSMaintenanceEngine* NewLC();

        virtual ~CMdSMaintenanceEngine();

    public: // Operations
        
  	    /**
	    * Imports external schema to database.
        * @param aSchema schema that is on phone currently
        * @param aFileName name of the import file
	    */        
        void ImportSchemaL( CMdsSchema& aSchema, const TDesC16& aFileName, TUint32 aVendorId );

        CMdsSchema* ImportSchemaTestL( const TDesC16& aFileName );


  	    /**
	    * Imports metadata to database.
        * @param aSchema schema that is on phone currently
        * @param aFileName name of the import file
	    */        
        TInt ImportMetadataL( CMdSManipulationEngine& aManipulate, 
        					     CMdsSchema& aSchema, const TDesC16& aFileName );

  	    /**
	    * Exports metadata to file.
        * @param aFileName name of the file where to export
        * @param aSchema current schema
        * @param aHierarchyArray an array of criteria what to export
	    */        
        void ExportMetadataL( CMdsSchema& aSchema, const TDesC16& aFileName, 
                              CMdCSerializationBuffer& aItems );
        
        /**
         * Stores drive media id to the DB.
         */
        void StoreDriveMediaIdsL();

  	    /**
	    * Creates metadata tables to database.
	    */
	    void InstallL( CMdSManipulationEngine& aManipulate, CMdsSchema& aSchema );

        /**
        * Loads schema from database
        */
        void LoadSchemaL( CMdsSchema& aSchema );
        
        /**
         * performs initialization (at server start) for default database
         */
        static void InitConnectionL();

        /**
         * closes default database
         */
        static void CloseDatabase();

        /**
         * close and delete default database
         */
		static void DeleteDatabase();

		TInt FailedImports() const
			{
			return iFailedImports;
			};

    private: // Constructors and destructors

        /**
        * CMdSManipulationEngine.
        * C++ default constructor.
        * @param .
        */
        CMdSMaintenanceEngine();

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

	private: // Member data

        /**
         * database validation and creation
         */
        CMdSSqlDbMaintenance* iMaintenance;

        TInt iFailedImports;
        
        TBool iPrivateSchemaFileInvalid;
    };
    
#endif // __MDSMAINTENANCEENGINE_H__
