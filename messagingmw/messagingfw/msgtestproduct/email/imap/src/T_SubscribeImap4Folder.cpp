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
// SubscribeImap4Folder
// [Paramaters]
// ImapAccountName			:Name of the Imap account
// ParentFolderName			:Name of the remote parent folder
// FolderName				:Folder name that needs to be subscribed
// Subscribes to a remote IMAP4 folder name
// 
//

//User includes
#include "T_SubscribeImap4Folder.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>

//Epoc include
#include <imapcmds.h>

// Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KFolderName,"FolderName");

/**
  Function : CT_MsgSubscribeImap4Folder
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgSubscribeImap4Folder::CT_MsgSubscribeImap4Folder(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KSubscribeImap4Folder);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgSubscribeImap4Folder 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgSubscribeImap4Folder
*/
CT_MsgSubscribeImap4Folder* CT_MsgSubscribeImap4Folder::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgSubscribeImap4Folder* self = new(ELeave) CT_MsgSubscribeImap4Folder(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ~CT_MsgSubscribeImap4Folder
  Description : Destructor
  @return : N/A
*/
CT_MsgSubscribeImap4Folder::~CT_MsgSubscribeImap4Folder()
	{
	delete iOperation;
	iOperation=NULL;
	}
	
/**
  Function : doTestStepL
  Description : Subscribes to a remote IMAP4 folder name
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgSubscribeImap4Folder::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : SubscribeImap4Folder"));		
	if(ReadIni())
		{
		// Get the Id of the remote folder
		TMsvId folderId;
		TRAPD(err,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iFolderName, iParentName));
		if(err == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
			
		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		// Appends the folderId onto the end of the array
		selection->AppendL(folderId);

		TBuf8<1> param;
			
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		(iSharedDataIMAP.iMtm)->SwitchCurrentEntryL(imapServiceId);
			
		CT_MsgActive& active = Active();
		delete iOperation;
		iOperation=NULL;
		
		iOperation = (iSharedDataIMAP.iMtm)->InvokeAsyncFunctionL(KIMAP4MTMLocalSubscribe,*selection,param,active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());
		
		CleanupStack::PopAndDestroy(selection);
		}
	
	return TestStepResult();
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgSubscribeImap4Folder::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, iParentName))
		{
		ERR_PRINTF1(_L("Parent Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KFolderName, iFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
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
void CT_MsgSubscribeImap4Folder::ProgressL(TBool bFinal)
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
void CT_MsgSubscribeImap4Folder::CancelStep()
	{
	iOperation->Cancel();
	}
