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
// UpdateToExternalDrive
// [UpdateToExternalDrive]
// Drive : Update drive
// Update the priority of a drive. 
// [APIs Used]
// CMsvSession::UpdateDrivePriorityL
// 
//


#include "T_UpdateToExternalDrive.h"

_LIT(KDrive, "drive");
_LIT(KPriority, "priority");

/**
  Function 		: CT_UpdateToExternalDrive
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_UpdateToExternalDrive::CT_UpdateToExternalDrive(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KUpdateToExternalDrive);
	}

/**
  Function 		: ~CT_UpdateToExternalDrive
  Description 	: Destructor
  @return 		: N/A
*/
CT_UpdateToExternalDrive::~CT_UpdateToExternalDrive()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_UpdateToExternalDrive
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_UpdateToExternalDrive* CT_UpdateToExternalDrive::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_UpdateToExternalDrive* self = new(ELeave) CT_UpdateToExternalDrive(aSharedDataIMAP);
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
void CT_UpdateToExternalDrive::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_UpdateToExternalDrive::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: Update the priority of a drive. 
  @return 		: TVerdict - Test step result  
*/
TVerdict CT_UpdateToExternalDrive::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step: UpdateToExternalDrive"));	
	TInt extDrive=0;
	GetIntFromConfig(ConfigSection(), KDrive, extDrive);	
	TInt priority = 0;
	GetIntFromConfig(ConfigSection(), KPriority, priority);
	
	// only change if not already using a external drive			
	iSharedDataIMAP.iSession->UpdateDrivePriorityL(TDriveNumber(extDrive), (TUint&)priority);
	return TestStepResult();
	}

