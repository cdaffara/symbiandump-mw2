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
* Description:  CTestSuplNetworkServer class
*
*/


#include <epos_suplnetwork.h>
#include "testsuplnetworkserver.h"


// DESCRIPTION
// This module contains implementation of CTestSuplNetworkServer 
// class

//
// Constructor.
//
CTestSuplNetworkServer::CTestSuplNetworkServer()
	{
				
	}


//
// Destructor.
//

CTestSuplNetworkServer ::~CTestSuplNetworkServer()
	{

	}

//
// Two phase constructor.
//
CTestSuplNetworkServer* CTestSuplNetworkServer::NewLC()
	{
	CTestSuplNetworkServer* suplNetworkServer=new(ELeave) CTestSuplNetworkServer;
	CleanupStack::PushL(suplNetworkServer);
	return suplNetworkServer;
	}

//
// This function will test Connect method of RSuplNetworkServer class.
//
void CTestSuplNetworkServer::TestConnectL()
	{
	RSuplNetworkServer suplNetworkServer;
	User::LeaveIfError(suplNetworkServer.Connect());
	suplNetworkServer.Close();	
	}
	
//
// This function will test Connect method of RSuplNetworkServer class
// for negative case.
//
void CTestSuplNetworkServer::TestConnectL(TInt /*aValue*/)
	{
	RSuplNetworkServer suplNetworkServer;
	User::LeaveIfError(suplNetworkServer.Connect());
	User::LeaveIfError(suplNetworkServer.Connect());	
	}
	
//
// This function will test ForwardMessage method of RSuplNetworkServer class
//
void CTestSuplNetworkServer::TestForwardMessageL()
	{
	RSuplNetworkServer suplNetworkServer;
	_LIT8(KMessage,"Send");
	TBufC8<10> message(KMessage);    
	TInt error=suplNetworkServer.ForwardMessage(message);
	suplNetworkServer.Close();
	}
	
//
// This function will test Version method of RSuplNetworkServer class
//
void CTestSuplNetworkServer::TestVersionL()
	{
	RSuplNetworkServer suplNetworkServer;
	User::LeaveIfError(suplNetworkServer.Connect());
	TVersion version=suplNetworkServer.Version();
	if(version.iBuild!=0 ||version.iMajor!=1 || version.iMinor!=0)
	    {
		User::Leave(KErrGeneral);
	    }
	
	suplNetworkServer.Close();
	}	