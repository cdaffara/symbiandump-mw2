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
// ChangeToExternalDrive
// [Action Parameters]
// CMsvSession&  Session   <input>: Reference to the session.
// TInt          Drive     <output-completion>: External drive (between D: and Y:) that we have changed to.
// [Action Description]
// Changes the current location of the mail server index to reside on the first available external drive.
// Does nothing if that drive is already the one being used.
// If there are no removable drives available, the action leaves with KErrNotFound
// [APIs Used]
// CMsvSession::ChangeDriveL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionChangeToExternalDrive.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "MSVAPI.H"


CMtfTestAction* CMtfTestActionChangeToExternalDrive::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeToExternalDrive* self = new (ELeave) CMtfTestActionChangeToExternalDrive(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionChangeToExternalDrive::CMtfTestActionChangeToExternalDrive(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionChangeToExternalDrive::~CMtfTestActionChangeToExternalDrive()
	{
	if(iOperation)
		delete iOperation;
	}

void CMtfTestActionChangeToExternalDrive::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeToExternalDrive);
	CActiveScheduler::Add(this);
	
	TVolumeInfo info;
	TDriveUnit externalDrive;

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Try to find a removable drive
	for (externalDrive = EDriveD; externalDrive < EDriveZ; externalDrive = TInt(externalDrive) + 1)
		{
		if (fs.Volume(info, TInt(externalDrive)) == KErrNone)
			break;
		}
	TInt currentDrive = MessageServer::CurrentDriveL(fs);
	CleanupStack::PopAndDestroy(&fs); // fs
	
	if (externalDrive == EDriveZ)
		{
		// didn't find any removable drive to change to
		User::Leave(KErrNotFound);				
		}
	
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	// only change if not already using a external drive	
	if (currentDrive < TInt(EDriveD))  // no need to check for Z drive, the index can't live there
		{
		iOperation = paramSession->ChangeDriveL(TInt(externalDrive), iStatus);
		SetActive();
		iDrive = TInt(externalDrive);
		}
	else
		{
		// nothing to do, but let RunL run to complete the action and return the drive ID
		SetActive();
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status,KErrNone);
		iDrive = currentDrive;
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeToExternalDrive);
	}


void CMtfTestActionChangeToExternalDrive::DoCancel()
	{
	if (iOperation)
		iOperation->Cancel();
	}


void CMtfTestActionChangeToExternalDrive::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	if(iOperation)
		{
		// unpack the final progress
		TPckgBuf<TMsvIndexLoadProgress> progressPack;
		TMsvIndexLoadProgress progress;
	    progressPack.Copy(iOperation->ProgressL());
		progress = progressPack();
		
		User::LeaveIfError(progress.iError);
		}
	
	StoreParameterL<TInt>(TestCase(), iDrive, ActionParameters().Parameter(1));
	
	TestCase().ActionCompletedL(*this);
	}

