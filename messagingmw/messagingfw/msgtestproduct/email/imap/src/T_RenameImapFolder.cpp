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
// RenameImapFolder
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// SrcFolderName		:	Src Folder Name that needs to be renamed
// SrcParentFolderName	:	Parent of Src Folder Name that needs to be renamed
// NewFolderName		:	New Name to be given to the Folder
// Renames Imap Folder
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//

// user includes
#include "T_RenameImapFolder.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
// epoc includes
#include <imapcmds.h>
#include <imapset.h>

// Literals Used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KSrcParentFolderName,"SrcParentFolderName");
_LIT(KNewFolderName, "NewFolderName");

/**
  Function : CT_MsgRenameImapFolder
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgRenameImapFolder::CT_MsgRenameImapFolder(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KRenameImapFolder);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgRenameImapFolder 				 
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgRenameImapFolder* CT_MsgRenameImapFolder::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgRenameImapFolder* self = new(ELeave) CT_MsgRenameImapFolder(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgRenameImapFolder
  Description : Destructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgRenameImapFolder::~CT_MsgRenameImapFolder()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgRenameImapFolder::ProgressL(TBool bFinal)
	{
	//	TODO
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
  @return : N/A
*/
void CT_MsgRenameImapFolder::CancelStep()
	{
	iOperation->Cancel();
	}	
	
/**
  Function : doTestStepL
  Description :Renames the Imap folder
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgRenameImapFolder::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : RenameImapFolder "));
	
	if(ReadIni())
		{
		TImap4RenameFolder cmd;
		cmd.iNewName = iNewFolderName;
		
		TPckgBuf<TImap4RenameFolder> package(cmd);
		
		// Get the Id of the remote folder
		TMsvId folderId;
		TRAPD(r,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iSrcFolderName, iSrcParentFolderName));
		if(r == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote Source folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		
		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);
		
		selection->Reset();
		// Appends the folderId onto the end of the array
		selection->AppendL(folderId);
		CT_MsgActive& active=Active();
		delete iOperation;
		iOperation=NULL;
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMRenameFolder,*selection,package,active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());
		
		CleanupStack::PopAndDestroy(selection);
		
		TImap4GenericProgress temp;	
		TPckgC<TImap4GenericProgress> paramPack(temp);
		paramPack.Set(iOperation->ProgressL());
		TImap4GenericProgress progress=paramPack();
		TBool err = progress.iErrorCode;
		
		if(err==KErrNone)
			{
			INFO_PRINTF1(_L("iErrorCode equals KErrNone"));
			}
			else
			{
			ERR_PRINTF2(_L("iErrorCode does not equal KErrNone; iErrorCode=%d"),err);
			SetTestStepError(err);
			}		
		}	
	return TestStepResult();
	}
	
/**
  Function : ReadIni
  Description :Reads the Ini and returns EPass or EFail if any of the input is missing
  @internalTechnology
  @param :
  @return : TBool
*/	
TBool CT_MsgRenameImapFolder::ReadIni()
	{
	TBool result = ETrue;
	
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);
	
	TPtrC srcFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcFolderName, srcFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSrcFolderName.Set(srcFolderName);
	
	TPtrC srcParentFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcParentFolderName, srcParentFolderName))
		{
		ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSrcParentFolderName.Set(srcParentFolderName);
	
	TPtrC newFolderName;
	if(!GetStringFromConfig( ConfigSection(), KNewFolderName, newFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iNewFolderName.Set(newFolderName);
	
	return result;
	}
