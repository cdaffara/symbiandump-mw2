/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of TestRSuplTerminalServer
*
*/



// INCLUDE FILES

#include <StifParser.h>
#include <Stiftestinterface.h>
#include <epos_suplterminalqop.h>
#include <epos_suplterminal.h>
#include "testrsuplterminalserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TestRSuplTerminalServer::TestRSuplTerminalServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TestRSuplTerminalServer::TestRSuplTerminalServer( CStifLogger* aLog )				
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TestRSuplTerminalServer::~TestRSuplTerminalServer
// 
// 
// -----------------------------------------------------------------------------
//
TestRSuplTerminalServer::~TestRSuplTerminalServer()
	{
	
	}
	
// -----------------------------------------------------------------------------
// TestRSuplTerminalServer::ConnectCloseL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestRSuplTerminalServer::ConnectCloseL( CStifItemParser& /*aItem*/ )
	{
	RSuplTerminalServer server;
	User::LeaveIfError( server.Connect() );
	server.Close();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// TestRSuplTerminalServer::VersionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestRSuplTerminalServer::VersionL( CStifItemParser& /*aItem*/ )
	{
	RSuplTerminalServer server;
	User::LeaveIfError( server.Connect() );
	TVersion version = server.Version();
	server.Close();
	return KErrNone;
	}

