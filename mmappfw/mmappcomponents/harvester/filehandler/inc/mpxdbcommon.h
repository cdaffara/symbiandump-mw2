/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Common Literals for the harvester db
*
*/


#ifndef MPXDBCOMMON_H
#define MPXDBCOMMON_H

_LIT( KHarvesterDBPath, "\\private\\101ffca9\\");
// Harvester db increment 9 v1
// Please update the increment number for each increment 
// Please update the version number for each schema change
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
_LIT( KHarvesterDBName, "harvesterdbv10_2.dat" );
_LIT( KHarvesterDBNameEMMC, "harvesterdbv10_2i.dat" );
#else
_LIT( KHarvesterDBName, "harvesterdbv10_2n.dat" );
_LIT( KHarvesterDBNameEMMC, "harvesterdbv10_2in.dat" );
#endif 
_LIT( KHarvesterDBPattern, "harvesterdbv*.dat" );

const TInt KDbMaxTableCreationSQLLength = 1024;

// Files table name
_LIT(KHarvesterMainTable, "Files"); 

// Column names 
_LIT(KHarPathName,    "FilePath" );  // Drive and Folder for an item
_LIT(KHarFileName,    "FileName" );  // File name for an item
_LIT(KHarLastModTime, "ModTime" );   // Last modified time stamp
_LIT(KHarCollectionDB, "DBID"   );   // Collection database ID
_LIT(KHarItemDRM,      "DRM"    );   // DRM file flag

// Column numbers
const TInt KHarPathNameColumn = 1;
const TInt KHarFileNameColumn = 2;
const TInt KHarModTimeColumn  = 3;
const TInt KHarColDBIDColumn  = 4;
const TInt KHarDRMColumn      = 5;

// Column types
_LIT(KHarPathNameType, " LONG VARCHAR");
_LIT(KHarFileNameType, " LONG VARCHAR");
_LIT(KHarLastModTimeType,  " TIME"); 
_LIT(KHarColDBIDType, " INTEGER");
_LIT(KHarItemDRMType, " BIT");

// AUX table name
_LIT(KHarvesterAuxTable, "Aux");

// AUX Column names
_LIT(KAuxVolumeId,           "VolumeId");
_LIT(KAuxVolumeIdType,       " UNSIGNED INTEGER");

// AUX Column number
const TInt KHarAuxVolumeIdColumn = 1;


// SQL query strings
_LIT(KStartCreateTable, "CREATE TABLE ");
_LIT(KSelectAll,        "SELECT * FROM ");
_LIT(KSelect,           "SELECT ");
_LIT(KUpdate,           "UPDATE ");
_LIT(KDelete,           "DELETE ");
_LIT(KAll,              "* ");
_LIT(KFrom,             "FROM ");
_LIT(KWhere,            " WHERE ");
_LIT(KAnd,              " AND ");
_LIT(KSet,              " SET ");
_LIT(KItemBracket,      "'");
_LIT(KItemHash,         "#");
_LIT(KEquals,           " = ");
_LIT(KOpenBracket,      "(");
_LIT(KCloseBracket,     ")");
_LIT(KCommaSign,        ","); 
_LIT(KAuxVolumeIdQuery, "INSERT INTO Aux(VolumeId) VALUES (%u)");
_LIT(KLike,             " LIKE ");
_LIT(KWildcard,         "*");
_LIT(KOn,               "1");
#endif // MPXDBCOMMON_H