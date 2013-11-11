/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Keeps a list of files that should not be harvested.
*
*/


#include <f32file.h>

#include "harvesterlog.h"
#include "harvesterblacklist.h"

// CHarvesterBlacklist

// ---------------------------------------------------------------------------
// Default constructor for first phase construction.
// ---------------------------------------------------------------------------
//
CHarvesterBlacklist::CHarvesterBlacklist() 
    {
    }

// ---------------------------------------------------------------------------
// Second phase construction
// ---------------------------------------------------------------------------
//
void CHarvesterBlacklist::ConstructL() // second-phase constructor
    {
    ConnectToBlackListServerL();
    }

// ---------------------------------------------------------------------------
// ConnectToBlackListServerL()
// ---------------------------------------------------------------------------
//
void CHarvesterBlacklist::ConnectToBlackListServerL()
    {
    User::LeaveIfError( iBlacklistClient.Connect() );
    }

// ---------------------------------------------------------------------------
// The usual NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterBlacklist* CHarvesterBlacklist::NewL()
    {
    CHarvesterBlacklist* self = new (ELeave) CHarvesterBlacklist();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterBlacklist::~CHarvesterBlacklist()
    {
    // close connection
    iBlacklistClient.Close();
    }

// ---------------------------------------------------------------------------
// Adds a file to blacklist with it's URI. Doesn't leave.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CHarvesterBlacklist::AddFile( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime )
    {
    TRAPD( err, iBlacklistClient.AddL( aUri, aMediaId, aLastModifiedTime ) );
    return err;
    }

// ---------------------------------------------------------------------------
// Removes a file from the blacklist. Doesn't leave.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CHarvesterBlacklist::RemoveFile( const TDesC& aUri, TUint32 aMediaId )
    {
    TRAPD( err, iBlacklistClient.RemoveL( aUri, aMediaId ) );
    return err;
    }


// ---------------------------------------------------------------------------
// Checks if a given file is blacklisted. And doesn't leave.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CHarvesterBlacklist::IsBlacklisted( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime )
    {
    TBool isBlacklisted( EFalse );
    TRAP_IGNORE( isBlacklisted = iBlacklistClient.IsBlacklistedL( aUri, aMediaId, aLastModifiedTime ) );
    return isBlacklisted;
    }
 
// ---------------------------------------------------------------------------
// Closes database connection.
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterBlacklist::CloseDatabase()
    {
    TRAP_IGNORE( iBlacklistClient.CloseDBL() );
    }
    
// ---------------------------------------------------------------------------
// Closes database connection.
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterBlacklist::OpenDatabase()
    {
    TRAP_IGNORE( iBlacklistClient.LoadBlacklistL() );
    }

