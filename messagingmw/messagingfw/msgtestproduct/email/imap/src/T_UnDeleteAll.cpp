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
// UnDeleteAll
// [Paramaters]
// ImapAccountName		:	Name of the IMAP account
// FolderName			:	Name of the remote folder
// Undelete all the deleted message
// 
//


// User includes
#include "T_UnDeleteAll.h"
#include <t_utils.h>

_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSrcFolderName,"SrcFolderName");

/**
  Function 		: CT_MsgUnDeleteAll
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_MsgUnDeleteAll::CT_MsgUnDeleteAll(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KUnDeleteAll);
	}

/**
  Function 		: ~CT_MsgUnDeleteAll
  Description 	: Destructor
  @return 		: N/A
*/
CT_MsgUnDeleteAll::~CT_MsgUnDeleteAll()
	{
	delete iOperation;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_MsgUnDeleteAll
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_MsgUnDeleteAll* CT_MsgUnDeleteAll::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgUnDeleteAll* self = new(ELeave) CT_MsgUnDeleteAll(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 	// Call CT_MsgAsyncStepIMAP::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @param 		: bFinal	Variable of TBool
  @return 		: N/A
*/
void CT_MsgUnDeleteAll::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_MsgUnDeleteAll::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: Performs an undelete all operations to the deleted entries by
  				  using KIMAP4MTMUndeleteAll command.
  @return 		: TVerdict - Test step result
*/
TVerdict CT_MsgUnDeleteAll::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: UnDeleteAll"));

	// Read the name of the source folder from the ini file
	TPtrC srcFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcFolderName, srcFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}
	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	// Create a children selection for a remote folder
	CMsvEntrySelection* selection = CT_MsgUtils::CreateChildrenSelectionRemoteFolderL( iSharedDataIMAP.iSession,
																	   imapAccountName,srcFolderName);
	CleanupStack::PushL(selection);

	// Apply the UnDeleteALL command to the all deleted messages
	CT_MsgActive&	active=Active();
	TBuf8<1> pack;
	TRAPD(err, iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMUndeleteAll,*selection,pack,active.iStatus));
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());
	
	// Check the final progress
	TImap4CompoundProgress tempProgress;	
	TPckgC<TImap4CompoundProgress> actualProgressPack(tempProgress);
	actualProgressPack.Set(iOperation->ProgressL());
	
	// Initialise all non-zero values in the expected progress
	iExpectedProgress.iGenericProgress.iType = EImap4GenericProgressType;
	iExpectedProgress.iGenericProgress.iOperation = TImap4GenericProgress::EOffLineUndelete;
	iExpectedProgress.iGenericProgress.iMsgsToDo = selection->Count();
	iExpectedProgress.iGenericProgress.iMsgsDone = selection->Count();
	TPckgBuf<TImap4CompoundProgress> expectedProgressPack(iExpectedProgress);
	
	if (expectedProgressPack != actualProgressPack)
		{
		ERR_PRINTF1(_L("Unexpected Final Progress"));
		SetTestStepResult(EFail);
		}

	CleanupStack::PopAndDestroy(selection); // selection

	return TestStepResult();
	}
