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
// CopyMsgsToExternalDrive
// [CopyMsgsToExternalDrive]
// Drive : external drive
// Copy the message store to external drive
// [APIs Used]
// CMsvSession::ChangeDriveL
// 
//

#include "T_CopyMsgsToExternalDrive.h"

_LIT(KDrive,"Drive");

/**
  Function 		: CT_CopyMsgsToExternalDrive
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_CopyMsgsToExternalDrive::CT_CopyMsgsToExternalDrive(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KCopyMsgsToExternalDrive);
	}

/**
  Function 		: ~CT_CopyMsgsToExternalDrive
  Description 	: Destructor
  @return 		: N/A
*/
CT_CopyMsgsToExternalDrive::~CT_CopyMsgsToExternalDrive()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_CopyMsgsToExternalDrive
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_CopyMsgsToExternalDrive* CT_CopyMsgsToExternalDrive::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_CopyMsgsToExternalDrive* self = new(ELeave) CT_CopyMsgsToExternalDrive(aSharedDataIMAP);
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
void CT_CopyMsgsToExternalDrive::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_CopyMsgsToExternalDrive::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: 
  @return 		: TVerdict - Test step result
*/
TVerdict CT_CopyMsgsToExternalDrive::doTestStepL()
	{	
	INFO_PRINTF1(_L("Test Step: CopyMsgsToExternalDrive"));
	
	TInt extDrive = EDriveD;
	GetIntFromConfig(ConfigSection(), KDrive, extDrive);

	CT_MsgActive&	active=Active();
	delete iOperation;
	iOperation = NULL;		
	iOperation = iSharedDataIMAP.iSession->CopyStoreL(extDrive, active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	return TestStepResult();
	}
