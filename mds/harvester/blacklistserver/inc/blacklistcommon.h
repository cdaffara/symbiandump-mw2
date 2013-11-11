/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/
#ifndef COMMON_H
#define COMMON_H

#include <e32base.h>

_LIT( KBlacklistServerName, "!BlacklistServer" );
_LIT( KBlacklistServerExe,  "!BlacklistServer.exe" ); // 0x0 2001B2EB = server uid
_LIT( KBlacklistChunkName,  "BlacklistServerChunk" );

const TUid KUidKBlacklistServer = { 0x2001B2EB };  

const TUint KBlacklistServerMajorVersion = 0;
const TUint KBlacklistServerMinorVersion = 1;
const TUint KBlacklistServerBuildVersion = 1;

enum TService
{
    EBlacklistAdd,
    EBlacklistRemove,
    EBlacklistRemoveFromDB,
    EGetBlacklistData,
    EBlacklistCloseDB,
    EMaxService     // Not an actual service
};

// SQL clauses for server
_LIT( KDefaultBlacklistDatabaseFile, "C:[200009fe]blacklistdb.sq" );
_LIT( KMdsBlacklistInsert, "INSERT INTO Blacklist(Modified, Uri, MediaId) VALUES(?, ?, ?);" );
_LIT( KMdsBlacklistUpdate, "UPDATE Blacklist SET Modified = ? WHERE Uri = ? AND MediaId = ?;" );
_LIT( KMdsBlacklistDelete, "DELETE FROM Blacklist WHERE Uri = ? AND MediaId = ?;" );
_LIT( KMdsBlacklistCheck, "SELECT Modified FROM Blacklist WHERE Uri = ? AND MediaId = ?;" );
_LIT( KMdsBlacklistCheckTableExistence, "SELECT COUNT(*) FROM Blacklist;" );
_LIT( KDropBlacklistTable, "DROP TABLE IF EXISTS Blacklist;" );
_LIT( KCreateBlacklistTable, "CREATE TABLE Blacklist(Uri TEXT NOT NULL, MediaId INTEGER NOT NULL, Modified INTEGER NOT NULL, PRIMARY KEY(Uri,MediaId));" );
_LIT( KMdsBlacklistCache, "SELECT Uri, MediaId, Modified FROM Blacklist;" );

#endif

// End of file
