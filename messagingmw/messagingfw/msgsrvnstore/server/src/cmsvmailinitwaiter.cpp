// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include "cmsvmailinitwaiter.h"
#include "MSVSERV.H"

CMsvMailinitWaiter::CMsvMailinitWaiter(CMsvServer& aServer) : 
	CActive(EPriorityHigh), iServer(aServer)
	{
	CActiveScheduler::Add(this);	
	}
	
CMsvMailinitWaiter* CMsvMailinitWaiter::NewL(CMsvServer& aServer)
	{
	CMsvMailinitWaiter* self=new (ELeave) CMsvMailinitWaiter(aServer);
	return self;
	}

void CMsvMailinitWaiter::WaitFor(RProcess& aProcess, TDriveNumber aDriveNum)
	{
	iProcess=aProcess;
	iDriveNum = aDriveNum;
	iProcess.Logon(iStatus);
	SetActive();
	}
	
void CMsvMailinitWaiter::DoCancel()
	{
	iProcess.LogonCancel(iStatus);
	}
	
void CMsvMailinitWaiter::RunL()
	{
	iServer.MailinitFinished( (iProcess.ExitType()==EExitPanic ) ? KErrGeneral : iStatus.Int(), iDriveNum);
	iProcess.Close();
	}

CMsvMailinitWaiter::~CMsvMailinitWaiter()
	{
	Cancel();	
	}
