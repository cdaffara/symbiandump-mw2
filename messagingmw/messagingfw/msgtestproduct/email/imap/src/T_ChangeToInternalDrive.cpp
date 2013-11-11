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
// [TestStep Name]
// ChangeToInternalDrive
// [ChangeToInternalDrive]
// Changes the current location of the mail server index to reside on the C drive.
// Does nothing if C drive is already the one being used.
// [APIs Used]
// CMsvSession::ChangeDriveL
// 
//

#include "T_ChangeToInternalDrive.h"

/**
  Function 		: CT_ChangeToInternalDrive
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_ChangeToInternalDrive::CT_ChangeToInternalDrive(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KChangeToInternalDrive);
	}

/**
  Function 		: ~CT_ChangeToInternalDrive
  Description 	: Destructor
  @return 		: N/A
*/
CT_ChangeToInternalDrive::~CT_ChangeToInternalDrive()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_ChangeToInternalDrive
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_ChangeToInternalDrive* CT_ChangeToInternalDrive::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_ChangeToInternalDrive* self = new(ELeave) CT_ChangeToInternalDrive(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();		// Call CT_MsgAsyncStepIMAP::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @param 		: bFinal	Variable of TBool
  @return 		: N/A
*/
void CT_ChangeToInternalDrive::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_ChangeToInternalDrive::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: 
  @return 		: TVerdict - Test step result
*/
TVerdict CT_ChangeToInternalDrive::doTestStepL()
	{	
	INFO_PRINTF1(_L("Test Step: ChangeToInternalDrive"));

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	TInt currentDrive = MessageServer::CurrentDriveL(fs);
	CleanupStack::PopAndDestroy(&fs);  // fs
	
	// only change if not already using the C drive	
	if (currentDrive != EDriveC)
		{
		CT_MsgActive&	active=Active();
		delete iOperation;
		iOperation = NULL;		
		iOperation = iSharedDataIMAP.iSession->ChangeDriveL(TInt(EDriveC), active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());
		}
	
	return TestStepResult();
	}
