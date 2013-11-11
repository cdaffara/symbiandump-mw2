/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*   numbers.
*
*/


#ifndef LANDMARKDATABASESTRUCTURE_H
#define LANDMARKDATABASESTRUCTURE_H

#include <e32std.h>

const TInt KPosLmSqlStatementMaxLen = 256;

// Common SQL queries
_LIT(KPosLmSqlAll, "*");
_LIT(KPosLmSqlSelect, "SELECT %S FROM %S");
_LIT(KPosLmSqlFindString, "SELECT %S FROM %S WHERE %S='%S'");
_LIT(KPosLmSqlFindInt, "SELECT %S FROM %S WHERE %S=%d");
_LIT(KPosLmSqlFindUint, "SELECT %S FROM %S WHERE %S=%u");
_LIT(KPosLmSqlFind2Uint, "SELECT %S FROM %S WHERE %S=%u AND %S=%u");
_LIT(KPosLmSqlDeleteUint,"DELETE FROM %S WHERE %S=%u");
_LIT(KPosLmSqlDeleteBy2Uint,"DELETE FROM %S WHERE %S=%u AND %S=%u");
_LIT(KPosLmSqlSelectOrderByString, "SELECT %S FROM %S ORDER BY %S");
_LIT(KPosLmSqlDescOrder, " DESC");

// Table names
_LIT(KPosLmLandmarkTable, "lmt_landmark");
_LIT(KPosLmLandmarkFieldTable, "lmt_lmfield");
_LIT(KPosLmCategoryTable, "lmt_cat");
_LIT(KPosLmLandmarkCategoryTable, "lmt_lmcat");
_LIT(KPosLmIconTable, "lmt_icon");
_LIT(KPosLmSettingsTable, "lmt_settings");
_LIT(KPosLmIndexTable, "lmt_index");

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
// KPosLmMaskPicNrCol declared above
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

// COLUMN NAMES: Indexes table
// KPosLmLanguageCol declared above
_LIT(KPosLmTimestampCol, "lmc_time");
_LIT(KPosLmIndexDataCol, "lmc_lmnameindex");

// INDEXES: Landmark table
_LIT(KPosLmLandmarkIdIndex, "lmi_lmid");

// INDEXES: Category table
_LIT(KPosLmCategoryIdIndex, "lmi_catid");
_LIT(KPosLmGlobalCategoryIdIndex, "lmi_glcatid");
_LIT(KPosLmCategoryNameIndex, "lmi_catname");
_LIT(KPosLmCategoryLmIdIndex, "lmi_lmcat_lmid");

// INDEXES: Icon table
_LIT(KPosLmIconIdIndex, "lmi_iconid");

// INDEXES: Landmark Fields table
_LIT(KPosLmFieldsLmIdIndex, "lmi_fields_lmid");

// DATA TYPES

// Tables
enum TPosLmDatabaseTables
    {
    EPosLmLandmarkTable = 1,
    EPosLmLandmarkFieldTable,
    EPosLmCategoryTable,
    EPosLmLandmarkCategoryTable,
    EPosLmIconTable,
    EPosLmEventTable,
    EPosLmSettingsTable,
    EPosLmIndexTable
    };

// Landmark columns
enum TPosLmLandmarkCols
    {
    EPosLmLcLandmarkIdCol = 1,
    EPosLmLcLatitudeCol,
    EPosLmLcLongitudeCol,
    EPosLmLcAltitudeCol,
    EPosLmLcHorizAccCol,
    EPosLmLcVertAccCol,
    EPosLmLcCoverageRadiusCol,
    EPosLmLcIconIdCol,
    EPosLmLcIconPicNrCol,
    EPosLmLcMaskPicNrCol,
    EPosLmLcNameCol,
    EPosLmLcDescriptionCol
    };

// Landmark Field columns
enum TPosLmLandmarkFieldCols
    {
    EPosLmLfcLandmarkIdCol = 1,
    EPosLmLfcFieldTypeCol,
    EPosLmLfcFieldStringCol
    };

// Category columns
enum TPosLmCategoryCols
    {
    EPosLmCcCategoryIdCol = 1,
    EPosLmCcGlobalCategoryIdCol,
    EPosLmCcIconIdCol,
    EPosLmCcIconPicNrCol,
    EPosLmCcMaskPicNrCol,
    EPosLmCcCategoryNameCol,
    EPosLmCcCategoryNameModifiedCol
    };

// Landmark-Category columns
enum TPosLmLandmarkCategoryCols
    {
    EPosLmLccLandmarkIdCol = 1,
    EPosLmLccCategoryIdCol
    };

// Icon columns
enum TPosLmIconCols
    {
    EPosLmIcIconIdCol = 1,
    EPosLmIcIconPathCol
    };

// Settings columns
enum TPosLmSettingsCols
    {
    EPosLmScLanguageCol  = 1,
    EPosLmScInitFlagCol
    };

// Indexes columns
enum TPosLmIndexCols
    {
    EPosLmIncLanguageCol  = 1,
    EPosLmIncTimestampCol,
    EPosLmIncIndexDataCol
    };

#endif // LANDMARKDATABASESTRUCTURE_H

