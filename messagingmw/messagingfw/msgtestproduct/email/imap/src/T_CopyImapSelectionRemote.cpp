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
// CopyImapSelectionRemote
// [Paramaters]
// ImapAccountName				:Name of the Imap account
// ParentSrcFolderName			:Parent remote folder name of the source folder whose entries needs
// to be copied
// SrcFolderName				:Name of the remote source folder whose entries needs to be copied
// ParentTargetFolderName		:Parent remote folder name of the target folder to which the
// entries need to be copied 	
// TargetFolderName				:Name of the target folder to which the entries need to be copied 				
// [Action Description]
// Copies the entries under remote source folder to the remote target folder.
// 
//

//User includes
#include "T_CopyImapSelectionRemote.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentSrcFolderName,"ParentSrcFolderName");
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KParentTargetFolderName,"ParentTargetFolderName");
_LIT(KTargetFolderName,"TargetFolderName");

/**
  Function : CT_MsgCopyImapSelectionRemote
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCopyImapSelectionRemote::CT_MsgCopyImapSelectionRemote(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KCopyImapSelectionRemote);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgCopyImapSelectionRemote				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgCopyImapSelectionRemote
*/
CT_MsgCopyImapSelectionRemote* CT_MsgCopyImapSelectionRemote::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCopyImapSelectionRemote* self = new(ELeave) CT_MsgCopyImapSelectionRemote(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}	
	
/**
  Function : ~CT_MsgCopyImapSelectionRemote
  Description : Destructor
  @return : N/A
*/
CT_MsgCopyImapSelectionRemote::~CT_MsgCopyImapSelectionRemote()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : doTestStepL
  Description : Connection with the IMAP4 server is already established in an earlier test step.
				It obtains a selection of the entires under the remote folder given.
				It copies these entries into the specified target remote folder.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCopyImapSelectionRemote::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep : CopyImapSelectionRemote"));
	
	if(ReadIni())
		{
		// Retrieving the Imap service Id for the given Imap account
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId);
		
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}

		//Get the Id of the remote source folder
		TMsvId srcFolderId;
		TRAPD(err,srcFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,iImapAccountName,iSrcFolderName,iParentSrcFolderName));
		if(err == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote source folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		INFO_PRINTF2(_L(" The remote source folder Id is %d"),srcFolderId);
		
		//Get a selection of the children entries for the remote source folder
		CMsvEntrySelection* selection =  CT_MsgUtils::CreateChildrenSelectionRemoteFolderWithTypeL(iSharedDataIMAP.iSession,iImapAccountName,iSrcFolderName,KUidMsvMessageEntry);
		CleanupStack::PushL(selection);
							
		//Get the Id of the remote target folder
		TMsvId targetFolderId;
		TRAP(err,targetFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL((iSharedDataIMAP.iSession),iImapAccountName,iTargetFolderName,iParentTargetFolderName));
		INFO_PRINTF2(_L(" The target folder Id is %d"),targetFolderId);
		if(err == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote target folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		
		// Check if there are no entries under the remote folder 
		TInt count = selection->Count();
		if (count == 0)
			{
			User::Leave(KErrArgument);
			}
		// Fetch the Id of the first entry
		TMsvId entryId = (*selection)[0];
		
		// Creates a new CMsvEntry for the specified entry ID.
		CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),entryId,TMsvSelectionOrdering());
		CleanupStack::PushL(entry);
							
		// Sets the context to the parent of the specified entry.
		entry->SetEntryL(entry->Entry().Parent());
		
		CT_MsgActive& active = Active();
		delete iOperation;
		iOperation=NULL;
		
		// Copy within service...
		iOperation = entry->CopyL(*selection,targetFolderId,active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());
							
		CleanupStack::PopAndDestroy(2,selection); // entry,selection
		}
	return TestStepResult();
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCopyImapSelectionRemote::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KParentSrcFolderName, iParentSrcFolderName))
		{
		ERR_PRINTF1(_L("Parent Source Folder Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KSrcFolderName, iSrcFolderName))
		{
		ERR_PRINTF1(_L("Source Folder Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KParentTargetFolderName, iParentTargetFolderName))
		{
		ERR_PRINTF1(_L("Parent Target Folder Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KTargetFolderName, iTargetFolderName))
		{
		ERR_PRINTF1(_L("Target Folder Name is not specified"));
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
void CT_MsgCopyImapSelectionRemote::ProgressL(TBool bFinal)
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
void CT_MsgCopyImapSelectionRemote::CancelStep()
	{
	iOperation->Cancel();
	}
