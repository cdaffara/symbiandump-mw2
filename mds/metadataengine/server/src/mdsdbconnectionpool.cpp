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
* Description:  Implementations of methods for connection pool*
*/

// INCLUDE FILES
#include "mdsdbconnectionpool.h"
#include "mdssqliteconnection.h"
#include "mdsindexer.h"
#include "mdccommon.h"
#include "mdccommon.pan"

void MMdSDbConnectionPool::DisconnectAll()
	{
	iConnection->CloseDb();
	iDBValid = EFalse;
	}
        
void MMdSDbConnectionPool::ConnectAllL()
	{
	if ( iConnection && !iDBValid )
		{
		iConnection->OpenDbL( KMdsSqlDbDefaultName );
		iDBValid = ETrue;

	    MMdSIndexer::StartIndexL();
		}
	else
		{
		User::Leave( KErrBadHandle );
		}
	}

void MMdSDbConnectionPool::SetDefaultDB( CMdSSqLiteConnection* aConnection )
	{
   	iConnection = aConnection;
	}

CMdSSqLiteConnection& MMdSDbConnectionPool::GetDefaultDBL()
	{
	__ASSERT_DEBUG( iConnection, User::Panic( _L("Incorrect DB connection"), KErrArgument ) );
	if ( !iConnection || !iDBValid )
		{
		User::Leave( KErrBadHandle );
		}
   	return *iConnection;
	}

//----------------------------------------------
// INITIALIZE STATIC DATA
//----------------------------------------------

CMdSSqLiteConnection* MMdSDbConnectionPool::iConnection = NULL;
TBool MMdSDbConnectionPool::iDBValid = EFalse;
