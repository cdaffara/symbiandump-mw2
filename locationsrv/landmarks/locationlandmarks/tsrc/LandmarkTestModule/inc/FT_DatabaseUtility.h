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


#ifndef FT_DATABASEUTILITY_H
#define FT_DATABASEUTILITY_H

//  INCLUDES
#include <e32std.h>

// CONSTANTS
const TInt KPosLmSqlStatementMaxLen = 256;
_LIT(KPosLmSqlAll, "*");
_LIT(KPosLmSqlSelect, "SELECT %S FROM %S");

// Table names
_LIT(KPosLmLandmarkTable, "lmt_landmark");
_LIT(KPosLmLandmarkFieldTable, "lmt_lmfield");
_LIT(KPosLmCategoryTable, "lmt_cat");
_LIT(KPosLmLandmarkCategoryTable, "lmt_lmcat");
_LIT(KPosLmIconTable, "lmt_icon");
_LIT(KPosLmSettingsTable, "lmt_settings");

// COLUMN NAMES: Landmark table
_LIT(KPosLmLandmarkIdCol, "lmc_lmid");
_LIT(KPosLmLatitudeCol, "lmc_lat");
_LIT(KPosLmLongitudeCol, "lmc_long");
_LIT(KPosLmAltitudeCol, "lmc_alt");
_LIT(KPosLmHorizAccCol, "lmc_hacc");
_LIT(KPosLmVertAccCol, "lmc_vacc");
_LIT(KPosLmCoverageRadiusCol, "lmc_covrad");
_LIT(KPosLmIconIdCol, "lmc_iconid"); 
_LIT(KPosLmIconPicNrCol, "lmc_iconnr");
_LIT(KPosLmMaskPicNrCol, "lmc_masknr");
_LIT(KPosLmNameCol, "lmc_name");
_LIT(KPosLmNDescCol, "lmc_desc");

// COLUMN NAMES: Landmark Field
// KPosLmLandmarkIdCol declared above
_LIT(KPosLmFieldTypeCol, "lmc_fieldtype");
_LIT(KPosLmFieldStringCol, "lmc_fieldstr");

// COLUMN NAMES: Category table
_LIT(KPosLmCategoryIdCol, "lmc_catid");
_LIT(KPosLmGlobalCategoryIdCol, "lmc_glcatid");
// KPosLmIconIdCol declared above
// KPosLmIconPicNrCol declared above
_LIT(KPosLmCategoryNameCol, "lmc_catname");
_LIT(KPosLmCategoryNameModifiedCol, "lmc_namemod");

// COLUMN NAMES: Landmark-Category table
// KPosLmLandmarkIdCol declared above
// KPosLmCategoryIdCol declared above

// COLUMN NAMES: Icon table
// KPosLmIconIdCol declared above
_LIT(KPosLmIconPathCol, "lmc_iconpath");

// COLUMN NAMES: Landmark settings table
_LIT(KPosLmLanguageCol, "lmc_language");
_LIT(KPosLmInitFlagCol, "lmc_init");

// INDEXES: Landmark table
_LIT(KPosLmLandmarkIdIndex, "lmi_lmid");

// INDEXES: Category table
_LIT(KPosLmCategoryIdIndex, "lmi_catid");
_LIT(KPosLmGlobalCategoryIdIndex, "lmi_glcatid");
_LIT(KPosLmCategoryNameIndex, "lmi_catname");

// INDEXES: Icon table
_LIT(KPosLmIconIdIndex, "lmi_iconid");

// CLASS DECLARATION
class RDbNamedDatabase;

/**
*  Utility function for landmark database.
*/
class FTDatabaseUtility
    {

    public: // New functions

        /**
        * Removes the protocol (i.e. "file://") from the URI. This function
        * assumes that the protocol is present first in the specified URI.
        *
        * @param aUri A URI. On out it contains the URI without the protocol.
        */
        IMPORT_C static void RemoveProtocolFromUriL(
        /* IN/OUT */    TPtrC& aDbUri
        );

        static void CreateDatabaseL(const TDesC& aDbpath, TBool aReplaceDatabase= EFalse, TBool aWrongId=EFalse, TBool aCreateInvalidIndex=EFalse);
		

    private:

        static void CreateTablesL(RDbNamedDatabase& aDb, TBool aCreateInvalidIndex);

        static void CreateLandmarkTableL(RDbNamedDatabase& aDb, TBool aCreateInvalidIndex);

        static void CreateLandmarkFieldTableL(RDbNamedDatabase& aDb);

        static void CreateCategoryTableL(RDbNamedDatabase& aDb, TBool aCreateInvalidIndex);

        static void CreateLmCategoryTableL(RDbNamedDatabase& aDb);

        static void CreateIconTableL(RDbNamedDatabase& aDb);

        static void CreateSettingsTableL(RDbNamedDatabase& aDb);

        static void CreateIndexesL(RDbNamedDatabase& aDb);

        static void CreateIndexL(RDbNamedDatabase& aDb,const TDesC& aIndex, 
        	const TDesC& aTable,
        	const TDesC& aColumn,
        	TBool aUnique
        );

        static void SetCounterToOneL(RDbNamedDatabase& aDb,const TDesC& aTable);

    };

#endif      // FT_DATABASEUTILITY_H
            
// End of File

