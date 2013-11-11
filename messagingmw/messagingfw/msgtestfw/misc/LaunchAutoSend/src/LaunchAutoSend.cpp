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
// LaunchAutoSend.cpp
//

#include <e32std.h>
#include <e32base.h>

_LIT(KMsvAutoSendExe, "Autosend.exe");
const TUid KMsvAutoSendExeUid = {0x1000A402}; //268477442

void DoExecuteL(const TDesC& aCmd)
	{
	CActiveScheduler* activeSch = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(activeSch);
	CActiveScheduler::Install(activeSch);

	RProcess process;
	User::LeaveIfError(process.Create(KMsvAutoSendExe, aCmd,
							TUidType(KNullUid, KNullUid, KMsvAutoSendExeUid)));
	
	TRequestStatus status = KRequestPending;
	process.Logon(status);
	process.Resume();
	User::WaitForRequest(status);

	//Check the process completion status
	TInt err = (process.ExitType() == EExitPanic) ? KErrGeneral: status.Int();
	process.Close();
	User::LeaveIfError(err);
	
	CleanupStack::PopAndDestroy(activeSch);
	}

TInt Execute(const TDesC& aCmd)
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	TRAPD(err, DoExecuteL(aCmd));

	delete cleanup;
	__UHEAP_MARKEND;

	return err;
	}

GLDEF_C TInt E32Main()
	{
	TBuf<0x100> cmd;
	User::CommandLine(cmd);
	return Execute(cmd);
	}
