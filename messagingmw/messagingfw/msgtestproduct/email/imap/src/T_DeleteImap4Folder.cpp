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
// DeleteImap4Folder
// [Paramaters]
// ParentFolderName			:Name of the remote parent folder of the remote folder to be deleted
// FolderName				:Name of the local/remote folder to be deleted
// Target					:Specify either REMOTE or LOCAL depending on where you want to delete the folder
// AccountName				:Name of the Imap account
// SubFolderTestFlag		:OPTIONAL - Boolean flag parameter to test deleting sub folder operations
// Deletes a IMAP4 folder under a parent remote/local entry
// 
//

//User includes
#include "T_DeleteImap4Folder.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Literals used
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KFolderName,"FolderName");
_LIT(KTarget,"Target");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSubFolderTestFlag,"SubFolderTestFlag");

/**
  Function : CT_MsgDeleteImap4Folder
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDeleteImap4Folder::CT_MsgDeleteImap4Folder(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KDeleteImap4Folder);
	}

/**
  Function : ~CT_MsgDeleteImap4Folder
  Description : Destructor
  @return : N/A
*/
CT_MsgDeleteImap4Folder::~CT_MsgDeleteImap4Folder()
	{
	delete iOperation;
	iOperation=NULL;
	}
	
/**
  Function : NewL
  Description : Creates an object of CT_MsgDeleteImap4Folder 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgDeleteImap4Folder
*/
CT_MsgDeleteImap4Folder* CT_MsgDeleteImap4Folder::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDeleteImap4Folder* self = new(ELeave) CT_MsgDeleteImap4Folder(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgDeleteImap4Folder::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, iParentFolderName))
		{
		ERR_PRINTF1(_L("Parent folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KFolderName, iFolderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KTarget, iTarget))
		{
		ERR_PRINTF1(_L("Target of either REMOTE or LOCAL is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Deletes an IMAP4 folder under the specified parent remote/local entry
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgDeleteImap4Folder::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteImap4Folder"));	
	if(ReadIni())
		{	
		TMsvId folderId;
		// Local folder deletion...
		if(iTarget.Compare(_L("LOCAL"))==0)
			{
			// Retrieve the folder Id based on the local folder name read from the ini file	
			if (iParentFolderName.Compare(_L("KMsvLocalServiceIndexEntryId")) == 0)
				{
				folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iFolderName);
				if(folderId == KErrNotFound)
					{
					ERR_PRINTF1(_L("Invalid local folder name specified"));
					SetTestStepResult(EFail);
					return TestStepResult();
					}
				INFO_PRINTF2(_L("The local folder Id for deletion is %d"),folderId);
				}
			else
				{
				TRAPD(err,folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iFolderName));
				if(err == KErrNotFound)
					{
					ERR_PRINTF1(_L("Invalid local folder name specified"));
					SetTestStepResult(EFail);
					return TestStepResult();
					}
				INFO_PRINTF2(_L("The local folder Id for deletion is %d"),folderId);
				}
			}
		// Remote folder deletion...
		else if(iTarget.Compare(_L("REMOTE"))==0)
			{
			TRAPD(err,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iFolderName, iParentFolderName));
			if(err == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid remote folder name specified"));
				SetTestStepResult(EFail);
				return TestStepResult();
				}	
			INFO_PRINTF2(_L(" The remote folder Id for deletion is %d"),folderId);	
			}
		// Target Local or Remote not specified...
		else
			{
			ERR_PRINTF1(_L("Invalid Target for folder deletion"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
			
		// Folder deletion process...
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
			
		CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
			
		entry->SetEntryL(entry->Entry().Parent());

		if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
			{	
			entry->DeleteL(folderId);
			}
		else
			{
			CT_MsgActive& active = Active();
			delete iOperation;
			iOperation=NULL;
			
			iOperation = entry->DeleteL(folderId,active.iStatus);
			active.Activate();
			CActiveScheduler::Start();
			User::LeaveIfError(active.Result());
			
			TImap4GenericProgress temp;	
			TPckgC<TImap4GenericProgress> paramPack(temp);
			paramPack.Set(iOperation->ProgressL());
			TImap4GenericProgress progress=paramPack();
			
			/* 
		 	 * Read optional boolean flag parameter to test deleting sub folder operations from ini file;
		     * If subFolderTestFlag is ETrue,then test for \noselect attribute, otherwise ignore
		     */	
			TBool subFolderTestFlag = EFalse;
			GetBoolFromConfig(ConfigSection(),KSubFolderTestFlag,subFolderTestFlag);
				
			TBool err = progress.iErrorCode;
			if(err==KErrNone)
				{
				INFO_PRINTF1(_L("iErrorCode equals KErrNone"));
				if(subFolderTestFlag)
					{
					TMsvEmailEntry emailEntry(entry->Entry());
					TBool noSelectFlag = emailEntry.Mailbox();
					if(noSelectFlag)
						{
						SetTestStepResult(EFail);
						}
					}
				}
			else
				{
				INFO_PRINTF2(_L("iErrorCode does not equal KErrNone; iErrorCode=%d"),err);
				if(!subFolderTestFlag)
					{
					SetTestStepError(err);
					}
				}
			}
				
		CleanupStack::PopAndDestroy(entry);
		}
	return TestStepResult();
	}
	
/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgDeleteImap4Folder::ProgressL(TBool bFinal)
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
void CT_MsgDeleteImap4Folder::CancelStep()
	{
	iOperation->Cancel();
	}
