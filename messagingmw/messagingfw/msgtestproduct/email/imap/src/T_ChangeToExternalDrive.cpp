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
// ChangeToExternalDrive
// [ChangeToExternalDrive]
// Drive : external drive
// Changes the current location of the mail server index to reside on the first available external drive.
// Does nothing if that drive is already the one being used.
// If there are no removable drives available, the action leaves with KErrNotFound
// [APIs Used]
// CMsvSession::ChangeDriveL
// 
//


#include "T_ChangeToExternalDrive.h"

_LIT(KDrive,"Drive");

/**
  Function 		: CT_ChangeToExternalDrive
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_ChangeToExternalDrive::CT_ChangeToExternalDrive(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KChangeToExternalDrive);
	}

/**
  Function 		: ~CT_ChangeToExternalDrive
  Description 	: Destructor
  @return 		: N/A
*/
CT_ChangeToExternalDrive::~CT_ChangeToExternalDrive()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_ChangeToExternalDrive
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_ChangeToExternalDrive* CT_ChangeToExternalDrive::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_ChangeToExternalDrive* self = new(ELeave) CT_ChangeToExternalDrive(aSharedDataIMAP);
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
void CT_ChangeToExternalDrive::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_ChangeToExternalDrive::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: 
  @return 		: TVerdict - Test step result
*/
TVerdict CT_ChangeToExternalDrive::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step: ChangeToExternalDrive"));
	
	
	TInt extDrive = EDriveD;
	GetIntFromConfig(ConfigSection(), KDrive, extDrive);

	
	// only change if not already using a external drive	
		
	CT_MsgActive&	active=Active();
	delete iOperation;
	iOperation = NULL;
	iOperation = iSharedDataIMAP.iSession->ChangeDriveL(extDrive, active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());
	return TestStepResult();
	}

