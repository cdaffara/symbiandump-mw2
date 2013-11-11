// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// ChangeToInternalDrive
// [Action Parameters]
// CMsvSession& Session   <input>: Reference to the session.
// [Action Description]
// Changes the current location of the mail server index to reside on the C drive.
// Does nothing if C drive is already the one being used.
// [APIs Used]
// CMsvSession::ChangeDriveL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionChangeToInternalDrive.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "MSVAPI.H"


CMtfTestAction* CMtfTestActionChangeToInternalDrive::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeToInternalDrive* self = new (ELeave) CMtfTestActionChangeToInternalDrive(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionChangeToInternalDrive::CMtfTestActionChangeToInternalDrive(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionChangeToInternalDrive::~CMtfTestActionChangeToInternalDrive()
	{
	delete iOperation;
	}

void CMtfTestActionChangeToInternalDrive::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeToInternalDrive);
	CActiveScheduler::Add(this);

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	TInt currentDrive = MessageServer::CurrentDriveL(fs);
	CleanupStack::PopAndDestroy(&fs);  // fs
	
	// only change if not already using the C drive	
	if (currentDrive != EDriveC)
		{
		iOperation = paramSession->ChangeDriveL(TInt(EDriveC), iStatus);
		SetActive();
		}
	else
		{
		// nothing to do then
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeToInternalDrive);
		TestCase().ActionCompletedL(*this);
		}
	}


void CMtfTestActionChangeToInternalDrive::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionChangeToInternalDrive::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	// unpack the final progress
	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;
	progressPack.Copy(iOperation->ProgressL());
	progress = progressPack();
	
	User::LeaveIfError(progress.iError);

	TestCase().ActionCompletedL(*this);
	}

