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
// SelectImapFolder
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// SrcFolderName		:	Src Folder Name that needs to be renamed
// ExpectedErr			:	Expected Error
// Select the mentions folder using KIMAP4MTMSelect command
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//


// user includes
#include "T_SelectImapFolder.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>

// epoc includes
#include <miutset.h>
#include <imapcmds.h>
#include <imapset.h>


// Literals Used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KSrcParentFolderName,"SrcParentFolderName");
_LIT(KExpectedErr, "ExpectedErr");

/**
  Function : CT_MsgSelectImapFolder
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgSelectImapFolder::CT_MsgSelectImapFolder(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KSelectImapFolder);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgSelectImapFolder 				 
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgSelectImapFolder* CT_MsgSelectImapFolder::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgSelectImapFolder* self = new(ELeave) CT_MsgSelectImapFolder(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
//	self->iSharedDataIMAP = aSharedDataIMAP; // Set CT_AsyncStepIMAP.iSharedDataIMAP.
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgSelectImapFolder
  Description : Destructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgSelectImapFolder::~CT_MsgSelectImapFolder()
	{
	delete iOperation;
	}
	
	
/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgSelectImapFolder::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation*/
		}
	}

/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : N/A
*/
void CT_MsgSelectImapFolder::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function : doTestStepL
  Description : Selects the mentioned folder using KIMAP4MTMSelect command
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgSelectImapFolder::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : SelectImapFolder "));
	
	if(ReadIni())
		{
		TMsvId folderId;
		TRAPD(r,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iSrcFolderName, iSrcParentFolderName));
		if(r == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote Source folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		TBuf8<1> package;	

		CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(selection);

		// Appends the folderId onto the end of the array
		selection->AppendL(folderId);
		
		CT_MsgActive&	active=Active();
		
		delete iOperation;
		iOperation=NULL;
		
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMSelect,*selection,package,active.iStatus);	
		
		active.Activate();
		CActiveScheduler::Start();
		
		User::LeaveIfError(active.Result());
		CleanupStack::PopAndDestroy(selection); /* selection*/
		
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
  Description : Reads Ini file and fails the teststep if any of the input is missing
  @internalTechnology
  @param :
  @return : TBool
*/
TBool CT_MsgSelectImapFolder::ReadIni()
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
	
	return result;
	}
