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
// CancelOfflineOperations
// [Paramaters]
// ImapAccountName		:	Name of the Imap account
// SrcFolderName		:	Name of the folder/service on which to cancel offline operations(if the srcFolderName is desired as ImapServiceId then Should be specified as "Service" in the ini
// Cancels all offline operations on the folder specified
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// 
//


// user includes
#include "T_CancelOfflineOperations.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>

// epoc includes
#include <miutset.h>
#include <imapcmds.h>


// Literals Used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KCount,"Count");
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KSrcParentFolderName,"SrcParentFolderName");

/**
  Function : CT_MsgConnectImap4Server
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCancelOfflineOperations::CT_MsgCancelOfflineOperations(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KCancelOfflineOperations);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgCancelOfflineOperations 				 
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCancelOfflineOperations* CT_MsgCancelOfflineOperations::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCancelOfflineOperations* self = new(ELeave) CT_MsgCancelOfflineOperations(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgCancelOfflineOperations
  Description : Destructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCancelOfflineOperations::~CT_MsgCancelOfflineOperations()
	{
	delete iOperation;
	}

	
/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgCancelOfflineOperations::ProgressL(TBool bFinal)
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
void CT_MsgCancelOfflineOperations::CancelStep()
	{
	iOperation->Cancel();
	}
	
/**
  Function : doTestStepL
  Description : Cancels all offline operations on the folder specified
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCancelOfflineOperations::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CancelOfflineOperations "));
		
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TBuf8<1> package;

	TPtrC srcFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcFolderName, srcFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}
	
	TPtrC srcParentFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcParentFolderName, srcParentFolderName))
		{
		ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
		SetTestStepResult(EFail);
		}
		
	TMsvId folderId;
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		}

	if(srcFolderName.CompareC(_L("Service")) == 0)
		{
		folderId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		}
	else
		{
			// Get the Id of the remote folder
		folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,srcFolderName, srcParentFolderName);
		}
	// Appends the imapServiceId onto the end of the array
	selection->AppendL(folderId);
	CT_MsgActive&	active=Active();
	delete iOperation;
	iOperation = NULL;
	TRAPD(err,iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMCancelOffLineOperations,*selection,package,active.iStatus));
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());

	CleanupStack::PopAndDestroy(selection); // selection

	return TestStepResult();
	}
