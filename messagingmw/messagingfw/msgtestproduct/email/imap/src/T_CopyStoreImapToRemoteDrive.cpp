// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CopyStoreImapToRemoteDrive
// [Action Description]
// This copies the message Store to the specified drive..
// 
//

//User includes
#include "T_CopyStoreImapToRemoteDrive.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>

//Literals used
_LIT(KDrive, "drive");

/**
  Function : CT_MsgCopyStoreImapToRemoteDrive
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCopyStoreImapToRemoteDrive::CT_MsgCopyStoreImapToRemoteDrive(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KCopyStoreImapToRemoteDrive);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgCopyStoreImapToRemoteDrive				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgCopyStoreImapToRemoteDrive
*/
CT_MsgCopyStoreImapToRemoteDrive* CT_MsgCopyStoreImapToRemoteDrive::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCopyStoreImapToRemoteDrive* self = new(ELeave) CT_MsgCopyStoreImapToRemoteDrive(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
/**
  Function : ~CT_MsgCopyStoreImapToRemoteDrive
  Description : Destructor
  @return : N/A
*/
CT_MsgCopyStoreImapToRemoteDrive::~CT_MsgCopyStoreImapToRemoteDrive()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : doTestStepL
  Description : This copies the message Store to the specified drive.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCopyStoreImapToRemoteDrive::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep : CopyStoreImapToRemoteDrive"));
	
	if(ReadIni())
		{
		TInt error = KErrGeneral;
		INFO_PRINTF1(_L("Testing: Copy Store -- started"));

		CMsvOperationWait* active = CMsvOperationWait::NewLC();		
		CMsvEntry* cEntry = CMsvEntry::NewL(*iSharedDataIMAP.iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
		CleanupStack::PushL(cEntry);
		
		cEntry->SetEntryL(KMsvRootIndexEntryId);		
		TBuf8<256> progress;
		TBuf8<32> param;
		CMsvOperation* operation = NULL;
		active->Start();
		TRAP(error, operation = iSharedDataIMAP.iSession->CopyStoreL(iDriveId, active->iStatus));
		TEST(error == KErrNone);
		CActiveScheduler::Start();
		TEST(operation->iStatus.Int()==KErrNone);
		delete operation;
		CleanupStack::PopAndDestroy(cEntry);
		CleanupStack::PopAndDestroy(active);	
		INFO_PRINTF1(_L("Testing: Copy Store -- ended"));
		}		
	return TestStepResult();
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCopyStoreImapToRemoteDrive::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetIntFromConfig(ConfigSection(), KDrive, iDriveId))
		{
		ERR_PRINTF1(_L("Cannot find target DriveId in ini file"));
		SetTestStepResult(EFail);
		result = EFalse;
		}	
	return result;
	}
	
/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgCopyStoreImapToRemoteDrive::ProgressL(TBool bFinal)
	{
	//	Stub
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation */
		}
	}

/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : void
*/
void CT_MsgCopyStoreImapToRemoteDrive::CancelStep()
	{
	iOperation->Cancel();
	}
