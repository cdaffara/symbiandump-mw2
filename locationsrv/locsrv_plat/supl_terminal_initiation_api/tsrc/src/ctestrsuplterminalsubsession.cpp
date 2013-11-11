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
* Description:  Implementation of CTestRSuplTerminalSubSession
*
*/



// INCLUDE FILES

#include <StifParser.h>
#include <Stiftestinterface.h>
#include <epos_suplterminalqop.h>
#include <epos_suplterminal.h>
#include "ctestrsuplterminalsubsession.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::CTestRSuplTerminalSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestRSuplTerminalSubSession::CTestRSuplTerminalSubSession( CStifLogger* aLog ) :
					CActive(EPriorityStandard), iLog(aLog)
	{
	CActiveScheduler::Add( this );
	}

// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestRSuplTerminalSubSession* CTestRSuplTerminalSubSession::NewL( CStifLogger* aLog )
	{
	CTestRSuplTerminalSubSession* self = new (ELeave) CTestRSuplTerminalSubSession( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//	
void CTestRSuplTerminalSubSession::ConstructL()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::~CTestRSuplTerminalSubSession
// 
// 
// -----------------------------------------------------------------------------
//		
CTestRSuplTerminalSubSession::~CTestRSuplTerminalSubSession()
	{
	Cancel();
	}
	
// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::OpenCloseL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestRSuplTerminalSubSession::OpenCloseL( CStifItemParser& aItem )
	{
	// This value determines, if a subsession has to be open or not before closing subsession
    TInt openSubsession;
	User::LeaveIfError( aItem.GetNextInt( openSubsession ) );
	// Create a connection with  SUPL Server
	RSuplTerminalServer server;
	User::LeaveIfError( server.Connect() );
	CleanupClosePushL( server );
	// Open a sub-session to SUPL server
	RSuplTerminalSubSession subSession;
	if( openSubsession == 1 )
		{
		User::LeaveIfError( subSession.Open( server ) );	
		}
	// Close sub-session
	subSession.Close();
	// Close server connection
	CleanupStack::PopAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::GetPositionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestRSuplTerminalSubSession::GetPositionL( CStifItemParser& /*aItem*/ )
	{
	// Create a connection with  SUPL Server
	RSuplTerminalServer server;
	User::LeaveIfError( server.Connect() );
	// Open a sub-session to SUPL server
	RSuplTerminalSubSession subSession;
	User::LeaveIfError( subSession.Open( server ) );
	// Get Position, will return KErrNotFound, because no RunSession is called before GetPosition
	TPositionInfo positionInfo;
	TInt err = subSession.GetPosition( positionInfo );
	// Close sub-session
	subSession.Close();
	// Close connection
	server.Close();
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestRSuplTerminalSubSession::RunL()
	{
	iLog->Log(_L("RunL Called..."));
	CActiveScheduler::Stop();
	}

// -----------------------------------------------------------------------------
// CTestRSuplTerminalSubSession::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CTestRSuplTerminalSubSession::DoCancel()
	{
	
	}
