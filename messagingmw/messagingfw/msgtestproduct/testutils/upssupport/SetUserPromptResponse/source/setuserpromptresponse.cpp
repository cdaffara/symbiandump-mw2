// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// Implements a tool to create/modify a file with
// the string supplied from the commandline.
// 
//

// Epoc includes
#include <f32file.h>
#include <bacline.h>

// Literals Used
_LIT(KUserResPonseFile,"c:\\UserResponse.txt");

/**
  MainL()
  Description : Create/Modify the content of the file(UserResponse.txt)
  				with the string supplied from commandline.
  @param : N/A
  @return : N/A
*/
LOCAL_C void MainL(void)
	{
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

 	RFs fileServerSession;
	User::LeaveIfError(fileServerSession.Connect());
	CleanupClosePushL(fileServerSession);
	
	CCommandLineArguments *cmdLineArgs = CCommandLineArguments::NewLC();
	
	TInt argc;
	argc = cmdLineArgs->Count();
	
	//At least operation type and database file path must be given
	if(argc < 2)
		{
		User::Leave(KErrArgument);
		}
	RFile file;
	file.Replace(fileServerSession,KUserResPonseFile,EFileWrite);
	CleanupClosePushL(file);
	TPtrC argv = cmdLineArgs->Arg(1);
	TBuf8<8> inFileBuf8;
	inFileBuf8.Copy(argv);
	file.Write(inFileBuf8);
			
	CleanupStack::PopAndDestroy(4, scheduler);
	}

/**
  E32Main()
  Description :	It is the entry point 
  @return	:	Returns the error code
*/
GLDEF_C TInt E32Main()
	{
	CTrapCleanup *cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
		
	__UHEAP_MARK;
	
	TRAPD(retval,MainL());
	
	__UHEAP_MARKEND;
	
	delete cleanup;

	if(retval == KErrArgument)
		{
		return KErrNone;
		}
	return retval;
	}

